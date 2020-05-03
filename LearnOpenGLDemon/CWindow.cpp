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
	//���ȣ�������main�����е���glfwInit��������ʼ��GLFW��
	//Ȼ�����ǿ���ʹ��glfwWindowHint����������GLFW��glfwWindowHint�����ĵ�һ����������ѡ������ƣ����ǿ��ԴӺܶ���GLFW_��ͷ��ö��ֵ��ѡ��
	//�ڶ�����������һ�����ͣ������������ѡ���ֵ���ú��������е�ѡ���Լ���Ӧ��ֵ�������� GLFW��s window handling ��ƪ�ĵ����ҵ���
	//��������ڱ������cpp�ļ���õ������� undefined reference(δ���������)����Ҳ����˵�㲢δ˳��������GLFW�⡣

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//����һ�����ڶ���������ڶ����������кʹ�����ص����ݣ����һᱻGLFW����������Ƶ�����õ�
	window = glfwCreateWindow(windW, windH, windname.c_str(), NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);

	//�����κ�OpenGL�ĺ���֮ǰ������Ҫ��ʼ��GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	//ע�ᴰ�ڴ�С�仯���õĻص�����
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
	//�����ǿ�ʼ��Ⱦ֮ǰ����һ����Ҫ������Ҫ�������Ǳ������OpenGL��Ⱦ���ڵĳߴ��С�����ӿ�(Viewport)��
	//����OpenGL��ֻ��֪���������ݴ��ڴ�С��ʾ���ݺ����ꡣ���ǿ���ͨ������glViewport���������ô��ڵ�ά��
	//glViewport����ǰ�����������ƴ������½ǵ�λ�á��������͵��ĸ�����������Ⱦ���ڵĿ�Ⱥ͸߶ȣ����أ�

	//OpenGLĻ��ʹ��glViewport�ж����λ�úͿ�߽���2D�����ת������OpenGL�е�λ������ת��Ϊ�����Ļ���ꡣ
	//���磬OpenGL�е�����(-0.5, 0.5)�п��ܣ����գ���ӳ��Ϊ��Ļ�е�����(200, 450)��ע�⣬�������OpenGL���귶ΧֻΪ - 1��1��
	//���������ʵ�Ͻ�(-1��1)��Χ�ڵ�����ӳ�䵽(0, 800)��(0, 600)
	//Ȼ�������û��ı䴰�ڵĴ�С��ʱ���ӿ�ҲӦ�ñ����������ǿ��ԶԴ���ע��һ���ص�����(Callback Function)��������ÿ�δ��ڴ�С��������ʱ�򱻵��á�����ص�������ԭ�����£�
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
	//���ǿɲ�ϣ��ֻ����һ��ͼ��֮�����ǵ�Ӧ�ó���������˳����رմ��ڡ�
	//����ϣ�����������������ر���֮ǰ���ϻ���ͼ���ܹ������û����롣
	//��ˣ�������Ҫ�ڳ��������һ��whileѭ�������ǿ��԰�����֮Ϊ��Ⱦѭ��(Render Loop)��������������GLFW�˳�ǰһֱ�������С����漸�еĴ����ʵ����һ���򵥵���Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		ProcessInput();

		//����Ҫ�����е���Ⱦ(Rendering)�����ŵ���Ⱦѭ���У���Ϊ����������Щ��Ⱦָ����ÿ����Ⱦѭ��������ʱ���ܱ�ִ��
		//���ǻ�������glClearColor�����������Ļ���õ���ɫ��������glClear�����������ɫ����֮��������ɫ���嶼�ᱻ���ΪglClearColor�������õ���ɫ����������ǽ���Ļ����Ϊ�����ƺڰ��������ɫ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);



		ProcessSharder();

		glfwSwapBuffers(window);//glfwSwapBuffers�����ύ����ɫ���壨����һ��������GLFW����ÿһ��������ɫֵ�Ĵ󻺳壩��������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ
		glfwPollEvents();//glfwPollEvents���������û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã�
	}

	//����Ⱦѭ��������������Ҫ��ȷ�ͷ�/ɾ��֮ǰ�ķ����������Դ�����ǿ�����main������������glfwTerminate���������
	glfwTerminate();
}



GLFWwindow * CWindowGL::GetWindPtr()
{
	return window;
}


