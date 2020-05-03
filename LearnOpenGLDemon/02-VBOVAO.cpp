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
	//编译着色器
	InitShaderSource();
	InitSharderProgrem();
	//绑定VBO/VAO
	InItVBO();
}

void RectangleGLwin::InitShaderSource()
{

	//顶点着色器(Vertex Shader)是几个可编程着色器中的一个。如果我们打算做渲染的话，现代OpenGL需要我们至少设置一个顶点和一个片段着色器
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


	//片段着色器(Fragment Shader)是第二个也是最后一个我们打算创建的用于渲染三角形的着色器。片段着色器全是关于计算你的像素最后的颜色输出。为了让事情更简单，我们的片段着色器将会一直输出橘黄色
	//在计算机图形中颜色被表示为有4个元素的数组：红色、绿色、蓝色和alpha(透明度)分量，通常缩写为RGBA。当在OpenGL或GLSL中定义一个颜色的时候，我们把颜色每个分量的强度设置在0.0到1.0之间。比如说我们设置红为1.0f，绿为1.0f，我们会得到两个颜色的混合色，即黄色。这三种颜色分量的不同调配可以生成超过1600万种不同的颜色
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
	//编译着色器
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//glShaderSource函数把要编译的着色器对象作为第一个参数。第二参数指定了传递的源码字符串数量，这里只有一个
	//第三个参数是顶点着色器真正的源码，第四个参数我们先设置为NULL
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


	//两个着色器现在都编译了，剩下的事情是把两个着色器对象链接到一个用来渲染的着色器程序(Shader Program)中

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

	// 创建 2 个 VBO（EBO 实际上跟 VBO 一样，只是按照用途的另一种称呼）
	
	glGenBuffers(2, m_VboIds);

	//// 绑定第一个 VBO，拷贝顶点数组到显存
	glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//// 绑定第二个 VBO（EBO），拷贝图元索引数据到显存
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboIds[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	
	// 创建并绑定 VAO
	glGenVertexArrays(1, &m_VaoId);
	glBindVertexArray(m_VaoId);

	// 在绑定 VAO 之后，操作 VBO ，当前 VAO 会记录 VBO 的操作
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

	//使用VAO绘制
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