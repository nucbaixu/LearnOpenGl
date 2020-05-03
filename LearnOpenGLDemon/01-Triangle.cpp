#include "TestAllGLDemon.h"
#include "CWindow.h"


//������ɫ��(Vertex Shader)�Ǽ����ɱ����ɫ���е�һ����������Ǵ�������Ⱦ�Ļ����ִ�OpenGL��Ҫ������������һ�������һ��Ƭ����ɫ��
//������Ҫ���ĵ�һ����������ɫ������GLSL(OpenGL Shading Language)��д������ɫ����Ȼ����������ɫ�����������ǾͿ����ڳ�����ʹ������
// Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";


//Ƭ����ɫ��(Fragment Shader)�ǵڶ���Ҳ�����һ�����Ǵ��㴴����������Ⱦ�����ε���ɫ����Ƭ����ɫ��ȫ�ǹ��ڼ����������������ɫ�����Ϊ����������򵥣����ǵ�Ƭ����ɫ������һֱ����ٻ�ɫ
//�ڼ����ͼ������ɫ����ʾΪ��4��Ԫ�ص����飺��ɫ����ɫ����ɫ��alpha(͸����)������ͨ����дΪRGBA������OpenGL��GLSL�ж���һ����ɫ��ʱ�����ǰ���ɫÿ��������ǿ��������0.0��1.0֮�䡣����˵�������ú�Ϊ1.0f����Ϊ1.0f�����ǻ�õ�������ɫ�Ļ��ɫ������ɫ����������ɫ�����Ĳ�ͬ����������ɳ���1600���ֲ�ͬ����ɫ
const GLchar* fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";



class TriangleGLwin :public CWindowGL
{
public:
	using CWindowGL::CWindowGL;

	TriangleGLwin(int windW, int windH, std::string &windname);
	~TriangleGLwin();

	void InitSharderProgrem();
	void InItVBO();
	void ProcessSharder() override;

private:
	GLuint VBO;
	GLuint VAO;
	GLuint shaderProgram;
};

TriangleGLwin::TriangleGLwin(int windW, int windH, std::string &windname)
	: CWindowGL(windW, windH, windname)
{
	//������ɫ��
	InitSharderProgrem();
	//��VBO/VAO
	InItVBO();	
}

void TriangleGLwin::InitSharderProgrem()
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

void TriangleGLwin::InItVBO()
{
	GLfloat vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};

	glGenBuffers(1, &VBO);

	//OpenGL�кܶ໺��������ͣ����㻺�����Ļ���������GL_ARRAY_BUFFER��OpenGL��������ͬʱ�󶨶�����壬
	//ֻҪ�����ǲ�ͬ�Ļ������͡����ǿ���ʹ��glBindBuffer�������´����Ļ���󶨵�GL_ARRAY_BUFFERĿ����
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//����һ��������ʹ�õ��κΣ���GL_ARRAY_BUFFERĿ���ϵģ�������ö����������õ�ǰ�󶨵Ļ���(VBO)��
	//Ȼ�����ǿ��Ե���glBufferData�����������֮ǰ����Ķ������ݸ��Ƶ�������ڴ���
	//glBufferData��һ��ר���������û���������ݸ��Ƶ���ǰ�󶨻���ĺ�����
	//���ĵ�һ��������Ŀ�껺������ͣ����㻺�����ǰ�󶨵�GL_ARRAY_BUFFERĿ���ϡ�
	//�ڶ�������ָ���������ݵĴ�С(���ֽ�Ϊ��λ)����һ���򵥵�sizeof������������ݴ�С���С�����������������ϣ�����͵�ʵ������
	//  GL_STATIC_DRAW �����ݲ���򼸺�����ı䡣
	//	GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶࡣ
	//	GL_STREAM_DRAW ������ÿ�λ���ʱ����ı䡣
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// 1. ���ö�������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);


	glGenVertexArrays(1, &VAO);


	glBindVertexArray(VAO);
	// 2. �Ѷ������鸴�Ƶ������й�OpenGLʹ��
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. ���ö�������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//4. ���VAO
	glBindVertexArray(0);
}

TriangleGLwin::~TriangleGLwin()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void TriangleGLwin::ProcessSharder()
{
	// Draw our first triangle
	glUseProgram(shaderProgram);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}



int TriangeleTest()
{
	std::string windname("GetStartGL");
	TriangleGLwin *glwind = new TriangleGLwin(300,400, windname);
	glwind->RunGL();
	delete glwind;
	return 0;
}