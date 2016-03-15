
class Shader
{
public:
	Shader(char* vs_path, char* fs_path);

	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);

	void CompileShaders();
	
	char* pVSFilename;
	char* pFSFilename;

	GLint gWorldLocation;
};

Shader::Shader(char* vs_path, char* fs_path)
{
	pVSFilename = vs_path;
	pFSFilename = fs_path;
}

void Shader:: AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0)
	{
		fprintf(stderr, "Error creating shader type %d", ShaderType);			//之所以用fprintf，是当你将标准输出重定向后，该语句仍然会将错误输出到控制台上。
		exit(0);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);

	glShaderSource(ShaderObj, 1, p, Lengths);
	glCompileShader(ShaderObj);

	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success){
		char logInfo[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, logInfo);
		fprintf(stderr, "fail to  compile the shader type %d : %s", ShaderType, logInfo);
		exit(1);
	}
	glAttachShader(ShaderProgram, ShaderObj);
}

void Shader::CompileShaders()
{
	GLuint ShaderProgram = glCreateProgram();

	if (ShaderProgram == 0){
		fprintf(stderr, "Error creating shader program\n");
	}

	string vs, fs;

	if (!ReadFile(pVSFilename, vs)){
		exit(1);
	}

	if (!ReadFile(pFSFilename, fs)){
		exit(1);
	}

	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { ' ' };
	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0){
		glGetProgramInfoLog(ShaderProgram, 1024, NULL, ErrorLog);
		fprintf(stderr, "fail to link shader program : %s", ErrorLog);
		exit(1);
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (Success == 0){
		glGetProgramInfoLog(ShaderProgram, 1024, NULL, ErrorLog);
		fprintf(stderr, "invalidate the shader program : %s", ErrorLog);
		exit(1);
	}

	glUseProgram(ShaderProgram);

	//Color = glGetUniformLocation(ShaderProgram, "colorChange");
	//gampler = glGetUniformLocation(ShaderProgram, "gSampler");
	gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld");

}