#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>
using namespace std;

//Function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{
	//With these lines of code we initialize and instantiate the GLFW windows.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Creating a new window
	GLFWwindow* window = glfwCreateWindow(800, 600, "Maya But Actually Works V: 0.01 alpha", NULL, NULL);

	//If it couldn't be created, terminate the program.
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// We tell GLFW to make the context of our window the main context on the current thread.
	glfwMakeContextCurrent(window);

	
	//We pass GLAD the function to load the adress of the OpenGL function pointers which is OS - specific.
	//GLFW gives us glfwGetProcAddress that defines the correct function based on which OS we're compiling for.
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//Creating openGl viewport
	glViewport(0, 0, 800, 600);

	//Whenever the window changes in size, GLFW calls this function
	//and fills in the proper arguments for us to process.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	
	//Render loop
	
	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}

//Function that handles the screen being resized.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}