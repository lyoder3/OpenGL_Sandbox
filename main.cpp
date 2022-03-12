#include<iostream>
#include<fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


struct ShaderPorgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

struct RGBColor
{
	float red;
	float green;
	float blue;
};

static void printColor(RGBColor color) 
{
	std::cout << "Red: " << color.red << ", ";
	std::cout << "Green: " << color.green << ", ";
	std::cout << "Blue: " << color.blue << "\n";
}

static RGBColor convertHexToRGBColor(unsigned int hexCode) 
{
	float red, green, blue;

	unsigned int redCode = (hexCode >> 16) & 0xFF;
	unsigned int greenCode = (hexCode >> 8) & 0xFF;
	unsigned int blueCode = hexCode & 0xFF;

	red = (float) redCode / 255.0f;
	green = (float) greenCode / 255.0f;
	blue = (float) blueCode / 255.0f;

	return {red, green, blue};
}

static void glClearError()
{
	while (glGetError()!= GL_NO_ERROR);
}

static bool glLogCall()
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Erorr] (" << error << ")" << std::endl;
		return false;
	}
	return true;
}

static ShaderPorgramSource ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line)) 
	{
		if (line.find("#shader") != std::string::npos) 
		{
			if (line.find("vertex") != std::string::npos) 
			{
				type = ShaderType::VERTEX;	
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else 
		{
			ss[(int)type] << line << "\n";
		}
	}
	return { ss[0].str(), ss[1].str() };
}

static unsigned int compileShader(unsigned int type, const std::string& source) 
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*) alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);

		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex":"fragment") << " shader" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main() {
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(800, 800, "Main", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	GLenum err = glewInit();

	if (GLEW_OK != err){
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;
	std::cout << glGetString(GL_RENDERER) << std::endl;

	// Setting up the data to draw
	float positions[] = {
		-0.5f, -0.5f, 	// 0
		0.5f, -0.5f,  	// 1
		0.5f, 0.5f,		// 2
		-0.5f, 0.5f,	// 3
	};

	unsigned int indices[] = 
	{
		0,1,2,
		2,3,0
	};


	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Binding vertex buffer and filling it with data
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 4*2*sizeof(float), positions, GL_STATIC_DRAW);
	
	// Tell OpenGL how buffer is laid out
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2 * sizeof(float), 0);

	// Binding index buffer and filling it with data
	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2*3*sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// Read in shaders and create them
	ShaderPorgramSource source = ParseShader("/home/logan/orbit_visualizer/res/shaders/Basic.shader");
	unsigned int shader = createShader(source.VertexSource, source.FragmentSource);
	glUseProgram(shader);

	glBindVertexArray(0);
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	RGBColor col = convertHexToRGBColor(0x001E44);
	int location = glGetUniformLocation(shader,"u_Color");
	glUniform4f(location, col.red, col.green, col.blue, 1.0f);

	float b = col.blue;

	float increment = 0.01f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);
		glUniform4f(location, col.red, col.green, b, 1.0f);

		glBindVertexArray(vao);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		if (b > 1.0f | b < 0.0f)
			increment = -increment;
		
		b += increment;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}
