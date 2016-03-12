#include <cmath>
#include <glut.h>


//------------------ViewModel parameters-------------------
GLfloat Center[3] = { 0.0f, 0.0f, 0.0f };  //Center posotion
GLfloat Eye[3];							   //Eye's position
GLfloat Up[3] = { 0.0f, 1.0f, 0.0f };      //Up position         used to set LookAt() func

//------------------View Control Variable-----------------
//record state of the mouse
GLboolean Mouse_Leftdown = GL_FALSE;
GLboolean Mouse_Rightdown = GL_FALSE;
GLboolean Mouse_Middledown = GL_FALSE;

//record the passive position of the mouse X , Y
GLfloat MousePosition_x, MousePosition_y;

GLfloat Y_rotate = M_PI / 4; //angle between y-axis and look direction;
GLfloat X_rotate = M_PI / 4; //angle between x-axis and look direction;
GLfloat C2Elength = 10.0f;	 // lenght between center and eye

static GLfloat mSpeed = 0.4f;	//center move speed
static GLfloat rSpeed = 0.02f;  //rotate speed
static GLfloat lSpeed = 0.4f;   //reserved


//-----------------View Control Func-----------------------
void CalEyePosition() //每次修改完相应参数后，都有调用该函数来进行重绘
{
	if (Y_rotate > M_PI / 2.2) Y_rotate = M_PI / 2.2;  //限制看的方向角度 Y-axis
	if (Y_rotate < 0.01f) Y_rotate = 0.01f;

	if (X_rotate > 2 * M_PI) X_rotate = 2 * M_PI;	   //限时看的方向角度 X-axis
	if (X_rotate < 0.01f) X_rotate = 0.01f;

	if (C2Elength > 50.0f) C2Elength = 50.0f;		   //限制Center和Eye之间的距离
	if (C2Elength < 1.0f) C2Elength = 5.0f;

	//下面利用球面坐标系来计算eye的位置
	Eye[0] = Center[0] + C2Elength * sin(Y_rotate) * cos(X_rotate);
	Eye[2] = Center[2] + C2Elength * sin(Y_rotate) * sin(X_rotate);
	Eye[1] = Center[1] + C2Elength * cos(Y_rotate);
}

//Center Move
void MoveBackward()
{
	Center[0] += mSpeed * cos(X_rotate);
	Center[2] += mSpeed * sin(X_rotate);

	CalEyePosition();
}

void MoveForward()
{
	Center[0] -= mSpeed * cos(X_rotate);
	Center[2] -= mSpeed * sin(X_rotate);

	CalEyePosition();
}

//Visual angle rotates
void RotateLeft()
{
	X_rotate -= rSpeed;

	CalEyePosition();
}

void RotateRight()
{
	X_rotate += rSpeed;

	CalEyePosition();
}

void RotateUp()
{
	Y_rotate += rSpeed;

	CalEyePosition();
}

void RotateDown()
{
	Y_rotate -= rSpeed;

	CalEyePosition();
}