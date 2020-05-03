#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <functional>
using namespace std;

typedef void(*pFun)(GLFWwindow* window, double xpos, double ypos);

class CWindowGL
{
public:
	CWindowGL(int windW, int windH, std::string &windname);
	~CWindowGL();

	virtual void         PrepareCallback() ;

	void                 SetGLViewPort(int x,int y,int w,int h);
	virtual void         ProcessInput();
	virtual void         ProcessSharder();
	void                 RunGL();
	static  void         framebuffer_size_callback(GLFWwindow* window, int width, int height);
	
	void                 RegistMouseCallback(pFun  pfun);
	void                 RegistScollCallback(pFun  pfun);

	void                 SetMouse_CallbackFunction(const std::function<void(GLFWwindow* window, double xpos, double ypos)>
		                 &function);

	void                 SetMouse_Scroll_Callback(const std::function<void(GLFWwindow* window, double xoffset, double yoffset)>
		                 &function);


	GLFWwindow *         GetWindPtr();
	
	pFun mouse_callback;
	pFun scroll_callback;

private:
	GLFWwindow * window;

	
	
	std::function< void (GLFWwindow* window, double xpos, double ypos)> Fun_mouse_callback = 0;
	std::function< void(GLFWwindow* window, double xoffset, double yoffset)> Fun_scroll_callback = 0;
};




