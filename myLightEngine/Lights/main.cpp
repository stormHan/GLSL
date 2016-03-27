#include <math.h>
#include <GL\glew.h>
#include <GL\freeglut.h>

#include "Util_han.h"
#include "Pipeline.h"
#include "Camera.h"
#include "Texture.h"
#include "LightTechnique.h"
#include "Glut_backend.h"

#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1200

#pragma comment(lib, "glew32.lib")

struct Vertex
{
	Vector3f m_pos;
	Vector2f m_tex;
	Vector3f m_normal;

	Vertex() {}

	Vertex(const Vector3f& pos, const Vector2f& tex, const Vector3f& normal)
	{
		m_pos = pos;
		m_tex = tex;
		m_normal = normal;
	}
};

static const float FieldDepth = 20.0f;
static const float FieldWidth = 10.0f;

class Lights : public ICallbacks
{
public:

	Lights()
	{
		m_pGameCamera = NULL;
		m_pTexture = NULL;
		m_pEffect = NULL;
		m_scale = 0.0f;
		m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
		m_directionalLight.AmbientIntensity = 0.0f;
		m_directionalLight.DiffuseIntensity = 0.01f;
		m_directionalLight.Direction = Vector3f(1.0f, -1.0, 0.0);

		m_persProjInfo.FOV = 60.0f;
		m_persProjInfo.Height = WINDOW_HEIGHT;
		m_persProjInfo.Width = WINDOW_WIDTH;
		m_persProjInfo.zNear = 1.0f;
		m_persProjInfo.zFar = 50.0f;
	}

	~Lights()
	{
		delete m_pEffect;
		delete m_pGameCamera;
		delete m_pTexture;
	}

	bool Init()
	{
		Vector3f Pos(5.0f, 1.0f, -3.0f);
		Vector3f Target(0.0f, 0.0f, 1.0f);
		Vector3f Up(0.0, 1.0f, 0.0f);

		m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

		CreateVertexBuffer();

		m_pEffect = new LightingTechnique();

		if (!m_pEffect->Init())
		{
			printf("Error initializing the lighting technique\n");
			return false;
		}

		m_pEffect->Enable();

		m_pEffect->SetTextureUnit(0);

		m_pTexture = new Texture(GL_TEXTURE_2D);

		if (m_pTexture->loadBMP_custom( "../Resource/test.bmp") == 0) {
			return false;
		}

		return true;
	}

	void Run()
	{
		GLUTBackendRun(this);
	}

	virtual void RenderSceneCB()
	{
		m_scale += 0.0057f;

		m_pGameCamera->OnRender();

		glClear(GL_COLOR_BUFFER_BIT);

		PointLight pl[2];
		pl[0].DiffuseIntensity = 0.25f;
		pl[0].Color = Vector3f(1.0f, 0.5f, 0.0f);
		pl[0].Position = Vector3f(3.0f, 1.0f, FieldDepth * (cosf(m_scale) + 1.0f) / 2.0f);
		pl[0].Attenuation.Linear = 0.1f;
		pl[1].DiffuseIntensity = 0.25f;
		pl[1].Color = Vector3f(0.0f, 0.5f, 1.0f);
		pl[1].Position = Vector3f(7.0f, 1.0f, FieldDepth * (sinf(m_scale) + 1.0f) / 2.0f);
		pl[1].Attenuation.Linear = 0.1f;
		m_pEffect->SetPointLights(2, pl);

		SpotLight sl[2];
		sl[0].DiffuseIntensity = 0.9f;
		sl[0].Color = Vector3f(0.0f, 1.0f, 1.0f);
		sl[0].Position = m_pGameCamera->GetPos();
		sl[0].Direction = m_pGameCamera->GetTarget();
		sl[0].Attenuation.Linear = 0.1f;
		sl[0].Cutoff = 10.0f;

		sl[1].DiffuseIntensity = 0.9f;
		sl[1].Color = Vector3f(1.0f, 1.0f, 1.0f);
		sl[1].Position = Vector3f(5.0f, 3.0f, 10.0f);
		sl[1].Direction = Vector3f(0.0f, -1.0f, 0.0f);
		sl[1].Attenuation.Linear = 0.1f;
		sl[1].Cutoff = 20.0f;
		m_pEffect->SetSpotLights(2, sl);

		Pipeline p;
		p.WorldPos(0.0f, 0.0f, 1.0f);
		p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
		p.SetPerspectiveProj(m_persProjInfo);
		m_pEffect->SetWVP(p.GetWVPTrans());
		const Matrix4f& WorldTransformation = p.GetWorldTrans();
		m_pEffect->SetWorldMatrix(WorldTransformation);
		m_pEffect->SetDirectionalLight(m_directionalLight);
		m_pEffect->SetEyeWorldPos(m_pGameCamera->GetPos());
		m_pEffect->SetMatSpecularIntensity(0.0f);
		m_pEffect->SetMatSpecularPower(0);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);
		m_pTexture->Bind(GL_TEXTURE0);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		glutSwapBuffers();
	}



	virtual void KeyboardCB(HAN_KEY HANKey)
	{
		switch (HANKey) {
		case HAN_KEY_ESCAPE:
		case HAN_KEY_q:
			GLUTBackendLeaveMainLoop();
			break;
		case HAN_KEY_a:
			m_directionalLight.AmbientIntensity += 0.05f;
			break;
		case HAN_KEY_s:
			m_directionalLight.AmbientIntensity -= 0.05f;
			break;
		case HAN_KEY_z:
			m_directionalLight.DiffuseIntensity += 0.05f;
			break;
		case HAN_KEY_x:
			m_directionalLight.DiffuseIntensity -= 0.05f;
			break;
		default:
			m_pGameCamera->OnKeyboard(HANKey);
		}
	}


	virtual void PassiveMouseCB(int x, int y)
	{
		m_pGameCamera->OnMouse(x, y);
	}

private:

	void CreateVertexBuffer()
	{
		const Vector3f Normal = Vector3f(0.0, 1.0f, 0.0f);

		Vertex Vertices[6] = {
			Vertex(Vector3f(0.0f, 0.0f, 0.0f), Vector2f(0.0f, 0.0f), Normal),
			Vertex(Vector3f(0.0f, 0.0f, FieldDepth), Vector2f(0.0f, 1.0f), Normal),
			Vertex(Vector3f(FieldWidth, 0.0f, 0.0f), Vector2f(1.0f, 0.0f), Normal),

			Vertex(Vector3f(FieldWidth, 0.0f, 0.0f), Vector2f(1.0f, 0.0f), Normal),
			Vertex(Vector3f(0.0f, 0.0f, FieldDepth), Vector2f(0.0f, 1.0f), Normal),
			Vertex(Vector3f(FieldWidth, 0.0f, FieldDepth), Vector2f(1.0f, 1.0f), Normal)
		};

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	}

	GLuint m_VBO;
	LightingTechnique* m_pEffect;
	Texture* m_pTexture;
	Camera* m_pGameCamera;
	float m_scale;
	DirectionalLight m_directionalLight;
	PersProjInfo m_persProjInfo;
};


int main(int argc, char** argv)
{
	GLUTBackendInit(argc, argv, false, false);

	if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "Lights")) {
		return 1;
	}

	Lights* pApp = new Lights();

	if (!pApp->Init()) {
		return 1;
	}

	pApp->Run();

	delete pApp;

	return 0;
}