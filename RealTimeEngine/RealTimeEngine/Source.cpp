#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

//Function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
string loadShaderSource(const string& filename);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


int main()
{
	//Loading Shaders from txt file.

	string vertexShaderSourceTxt = loadShaderSource("VertexShader.txt");
	const char* vertexShaderSource = vertexShaderSourceTxt.c_str();

	string fragmentShaderSourceTxt = loadShaderSource("FragmentShader.txt");
	const char* fragmentShaderSource = fragmentShaderSourceTxt.c_str();

	
	//With these lines of code we initialize and instantiate the GLFW windows.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Creating a new window
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Maya But Actually Works V: 0.01 alpha", NULL, NULL);

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



	//Vertex Shader -------------------------------------------------------------------------------

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//Check for compiler errors
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	//Fragment Shader -----------------------------------------------------------------------------

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);


	//Check for compiler errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Linking Shaders -----------------------------------------------------------------------------

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//Check for compiler errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//Vertex Input --------------------------------------------------------------------------------

	float vertices[] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	// Initialize Vertex Buffer Object to have array of vertices
	// Initialize Vertex Array Object to have array of VBOs
	// Initialize Elemtn buffer Object to have array of indexes that connect vertices
	unsigned int VBO, VAO, EBO;

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// Bind Vertices to VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Bind Indexes to EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// set our vertex attributes pointers, links to vertex shader code.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);




	//Render loop

	while (!glfwWindowShouldClose(window))
	{
		//input
		processInput(window);

		//Rendering commands
		//...

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Getting the uniform "ourColor" in the fragment shader"

		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

		// Using the shader
		glUseProgram(shaderProgram);

		//Setting the "ourColor" uniform color

		glUniform4f(vertexColorLocation, 1, 1, 1, 1);

		
		
		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 6);		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	//Delete all GLFW resources when we are done.
	glfwTerminate();

	return 0;
}

//Function that handles the screen being resized.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

string loadShaderSource(const string& filename)
{
	ifstream file;
	file.open(filename.c_str());
	
	if (!file) { return NULL; }
	
	stringstream stream;
	stream << file.rdbuf();
	file.close();

	return stream.str();
}

