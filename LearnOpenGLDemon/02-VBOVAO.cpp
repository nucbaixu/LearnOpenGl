#include "TestAllGLDemon.h"
#include "CWindow.h"

class RectangleGLwin :public CWindowGL
{
public:
	using CWindowGL::CWindowGL;

	RectangleGLwin(int windW, int windH, std::string &windname);
	~RectangleGLwin();

	void InitShaderSource();
	void InitSharderProgrem();
	void InItVBO();
	void ProcessSharder() override;

private:
	const GLchar* vertexShaderSource = 0;
	const GLchar* fragmentShaderSource = 0;
	GLuint m_VaoId;
	GLuint m_VboIds[2];
	GLuint shaderProgram;
};

RectangleGLwin::RectangleGLwin(int windW, int windH, std::string &windname)
	: CWindowGL(windW, windH, windname)
{
	//������ɫ��
	InitShaderSource();
	InitSharderProgrem();
	//��VBO/VAO
	InItVBO();
}

void RectangleGLwin::InitShaderSource()
{

	//������ɫ��(Vertex Shader)�Ǽ����ɱ����ɫ���е�һ����������Ǵ�������Ⱦ�Ļ����ִ�OpenGL��Ҫ������������һ�������һ��Ƭ����ɫ��
	// Shaders
	vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec4 a_position;\n"
		"layout (location = 1) in vec3 a_color;\n"
		"out vec3 v_color; \n"
		"void main()\n"
		"{\n"
		"v_color = a_color;\n"
		"gl_Position = a_position;\n"
		"}\0";


	//Ƭ����ɫ��(Fragment Shader)�ǵڶ���Ҳ�����һ�����Ǵ��㴴����������Ⱦ�����ε���ɫ����Ƭ����ɫ��ȫ�ǹ��ڼ����������������ɫ�����Ϊ����������򵥣����ǵ�Ƭ����ɫ������һֱ����ٻ�ɫ
	//�ڼ����ͼ������ɫ����ʾΪ��4��Ԫ�ص����飺��ɫ����ɫ����ɫ��alpha(͸����)������ͨ����дΪRGBA������OpenGL��GLSL�ж���һ����ɫ��ʱ�����ǰ���ɫÿ��������ǿ��������0.0��1.0֮�䡣����˵�������ú�Ϊ1.0f����Ϊ1.0f�����ǻ�õ�������ɫ�Ļ��ɫ������ɫ����������ɫ�����Ĳ�ͬ����������ɳ���1600���ֲ�ͬ����ɫ
	fragmentShaderSource = "#version 330 core\n"
		"precision mediump float; \n"
		"in vec3 v_color;\n"
		"out vec4 color;\n"
		"void main()\n"
		"{\n"
		"color = vec4(v_color, 1.0f);\n"
		"}\n\0";
}

void RectangleGLwin::InitSharderProgrem()
{
	//������ɫ��
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//glShaderSource������Ҫ�������ɫ��������Ϊ��һ���������ڶ�����ָ���˴��ݵ�Դ���ַ�������������ֻ��һ��
	//�����������Ƕ�����ɫ��������Դ�룬���ĸ���������������ΪNULL
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);


	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);


	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);


	//������ɫ�����ڶ������ˣ�ʣ�µ������ǰ�������ɫ���������ӵ�һ��������Ⱦ����ɫ������(Shader Program)��

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}


	glUseProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void RectangleGLwin::InItVBO()
{
	// 4 vertices, with(x,y,z) ,(r, g, b, a) per-vertex
	GLfloat vertices[] =
	{
		-0.5f,  0.5f, 0.0f,       // v0
		1.0f,  0.0f, 0.0f,        // c0
		-0.5f, -0.5f, 0.0f,       // v1
		0.0f,  1.0f, 0.0f,        // c1
		0.5f, -0.5f, 0.0f,        // v2
		0.0f,  0.0f, 1.0f,        // c2
		0.5f,  0.5f, 0.0f,        // v3
		0.5f,  1.0f, 1.0f,        // c3
	};

	// Index buffer data
	GLushort indices[6] = { 0, 1, 2, 0, 2, 3 };

	// ���� 2 �� VBO��EBO ʵ���ϸ� VBO һ����ֻ�ǰ�����;����һ�ֳƺ���
	
	glGenBuffers(2, m_VboIds);

	//// �󶨵�һ�� VBO�������������鵽�Դ�
	glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//// �󶨵ڶ��� VBO��EBO��������ͼԪ�������ݵ��Դ�
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboIds[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	
	// �������� VAO
	glGenVertexArrays(1, &m_VaoId);
	glBindVertexArray(m_VaoId);

	// �ڰ� VAO ֮�󣬲��� VBO ����ǰ VAO ���¼ VBO �Ĳ���
	glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3 + 3) * sizeof(GLfloat), (const void *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (3 + 3) * sizeof(GLfloat), (const void *)(3 * sizeof(GLfloat)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboIds[1]);

	glBindVertexArray(GL_NONE);
}

RectangleGLwin::~RectangleGLwin()
{
	glDeleteVertexArrays(1, &m_VaoId);
	glDeleteBuffers(2, m_VboIds);
}

void RectangleGLwin::ProcessSharder()
{
	// Draw our first triangle
	glUseProgram(shaderProgram);

	//ʹ��VAO����
	glBindVertexArray(m_VaoId);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const void *)0);

}



int VBOVAOTest()
{
	std::string windname("GetStartGL");
	RectangleGLwin *glwind = new RectangleGLwin(600, 800, windname);
	glwind->RunGL();
	delete glwind;
	return 0;
}