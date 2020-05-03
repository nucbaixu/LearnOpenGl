#include "CWindow.h"


void CWindowGL::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void CWindowGL::RegistMouseCallback(pFun  pfun)
{
	mouse_callback = pfun;
}

void CWindowGL::RegistScollCallback(pFun  pfun)
{
	scroll_callback = pfun;
}

void CWindowGL::SetMouse_CallbackFunction(const std::function<void(GLFWwindow*window, double xpos, double ypos)>& function)
{
	Fun_mouse_callback = function;
}

void CWindowGL::SetMouse_Scroll_Callback(const std::function<void(GLFWwindow*window, double xoffset, double yoffset)>& function)
{
	Fun_scroll_callback = function;
}


CWindowGL::CWindowGL(int windW, int windH, std::string &windname)
{
	//首先，我们在main函数中调用glfwInit函数来初始化GLFW，
	//然后我们可以使用glfwWindowHint函数来配置GLFW。glfwWindowHint函数的第一个参数代表选项的名称，我们可以从很多以GLFW_开头的枚举值中选择；
	//第二个参数接受一个整型，用来设置这个选项的值。该函数的所有的选项以及对应的值都可以在 GLFW’s window handling 这篇文档中找到。
	//如果你现在编译你的cpp文件会得到大量的 undefined reference(未定义的引用)错误，也就是说你并未顺利地链接GLFW库。

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//创建一个窗口对象，这个窗口对象存放了所有和窗口相关的数据，而且会被GLFW的其他函数频繁地用到
	window = glfwCreateWindow(windW, windH, windname.c_str(), NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);

	//调用任何OpenGL的函数之前我们需要初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	//注册窗口大小变化调用的回掉函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	
}


void CWindowGL::PrepareCallback()
{
	//glfwSetCursorPosCallback(GetWindPtr(), (GLFWcursorposfun(mouse_callback)));
	//glfwSetScrollCallback(GetWindPtr(), (GLFWscrollfun(scroll_callback)));

	if (Fun_mouse_callback)
	{
		pFun f = *Fun_mouse_callback.target<pFun>();
		glfwSetCursorPosCallback(GetWindPtr(), (GLFWcursorposfun(f)));

	}

	if (Fun_scroll_callback)
	{

		pFun f = *Fun_scroll_callback.target<pFun>();
		glfwSetScrollCallback(GetWindPtr(), (GLFWcursorposfun(f)));
	}
}

CWindowGL::~CWindowGL()
{
}

void CWindowGL::SetGLViewPort(int x, int y, int w, int h)
{
	//在我们开始渲染之前还有一件重要的事情要做，我们必须告诉OpenGL渲染窗口的尺寸大小，即视口(Viewport)，
	//这样OpenGL才只能知道怎样根据窗口大小显示数据和坐标。我们可以通过调用glViewport函数来设置窗口的维度
	//glViewport函数前两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）

	//OpenGL幕后使用glViewport中定义的位置和宽高进行2D坐标的转换，将OpenGL中的位置坐标转换为你的屏幕坐标。
	//例如，OpenGL中的坐标(-0.5, 0.5)有可能（最终）被映射为屏幕中的坐标(200, 450)。注意，处理过的OpenGL坐标范围只为 - 1到1，
	//因此我们事实上将(-1到1)范围内的坐标映射到(0, 800)和(0, 600)
	//然而，当用户改变窗口的大小的时候，视口也应该被调整。我们可以对窗口注册一个回调函数(Callback Function)，它会在每次窗口大小被调整的时候被调用。这个回调函数的原型如下：
	glViewport(x, y, w, h);
}

void CWindowGL::ProcessInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void CWindowGL::ProcessSharder()
{

}

void CWindowGL::RunGL()
{
	//我们可不希望只绘制一个图像之后我们的应用程序就立即退出并关闭窗口。
	//我们希望程序在我们主动关闭它之前不断绘制图像并能够接受用户输入。
	//因此，我们需要在程序中添加一个while循环，我们可以把它称之为渲染循环(Render Loop)，它能在我们让GLFW退出前一直保持运行。下面几行的代码就实现了一个简单的渲染循环
	while (!glfwWindowShouldClose(window))
	{
		ProcessInput();

		//我们要把所有的渲染(Rendering)操作放到渲染循环中，因为我们想让这些渲染指令在每次渲染循环迭代的时候都能被执行
		//我们还调用了glClearColor来设置清空屏幕所用的颜色。当调用glClear函数，清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色。在这里，我们将屏幕设置为了类似黑板的深蓝绿色
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);



		ProcessSharder();

		glfwSwapBuffers(window);//glfwSwapBuffers函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕
		glfwPollEvents();//glfwPollEvents函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）
	}

	//当渲染循环结束后我们需要正确释放/删除之前的分配的所有资源。我们可以在main函数的最后调用glfwTerminate函数来完成
	glfwTerminate();
}



GLFWwindow * CWindowGL::GetWindPtr()
{
	return window;
}


