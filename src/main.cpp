#include<iostream>
#include<fstream>
#include <string>
#include <sstream>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "GLErrorHandler.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Color.h"

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

	{	
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


		VertexArray va;
		VertexBuffer vb(positions, 4*2*sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		va.AddBuffer(vb,layout);

		IndexBuffer ib(indices, 6);

		Shader shader("../../res/shaders/Basic.shader");
		shader.Bind();

		Color col(0x001E44);
		col.PrintRGB();
		std::cout << col.GetHexString().str() << std::endl;

		RGBColor rgb = col.GetRGB();

		shader.SetUniform4f("u_Color", rgb.red, rgb.green, rgb.blue, 1.0);

		float b = rgb.blue;

		float increment = 0.01f;
		
		Renderer renderer;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			shader.SetUniform4f("u_Color", rgb.red, rgb.green, b, 1.0f);

			renderer.Draw(va, ib, shader);

			if ((b > 1.0f) | (b < 0.0f))
				increment = -increment;
			
			b += increment;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	glfwTerminate();
	return 0;
}
