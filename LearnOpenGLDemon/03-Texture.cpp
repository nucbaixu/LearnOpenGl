#include "TestAllGLDemon.h"
#include "CWindow.h"
#include "CShader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



class TextureExampleGLwin :public CWindowGL
{
public:
	using CWindowGL::CWindowGL;

	TextureExampleGLwin(int windW, int windH, std::string &windname);
	~TextureExampleGLwin();

	void InitShaderSource();
	void InitSharderProgrem();
	void InItVBO();
	void InitTextureData();
	void ProcessSharder() override;

private:
	Shader *sd = 0;
	unsigned int texture;
	unsigned int VBO, VAO, EBO;
};

TextureExampleGLwin::TextureExampleGLwin(int windW, int windH, std::string &windname)
	: CWindowGL(windW, windH, windname)
{
	//±àÒë×ÅÉ«Æ÷
	InitShaderSource();
	InitSharderProgrem();
	//°ó¶¨VBO/VAO
	InItVBO();

	InitTextureData();
}

void TextureExampleGLwin::InitShaderSource()
{
	// build and compile our shader zprogram
	// ------------------------------------
	sd = new Shader("./TestResource/shaderSource/texture.vs"
		, "././TestResource/shaderSource/texture.fs");
}

void TextureExampleGLwin::InitSharderProgrem()
{
	
}

void TextureExampleGLwin::InItVBO()
{
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void TextureExampleGLwin::InitTextureData()
{
	// load and create a texture 
	// -------------------------
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char *data = stbi_load("./TestResource/img/wall.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

TextureExampleGLwin::~TextureExampleGLwin()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1,&EBO);

	delete sd;
}

void TextureExampleGLwin::ProcessSharder()
{
	// bind Texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// render container
	sd->use();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}



int TextureTest()
{
	std::string windname("Texture");
	TextureExampleGLwin *glwind = new TextureExampleGLwin(600, 800, windname);
	glwind->RunGL();
	delete glwind;
	return 0;
}