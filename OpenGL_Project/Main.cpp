#include <SDL.h>
#include <glew.h>
#include "glm\glm.hpp"
#include <iostream>
#include "Vertex.hpp"

int main()
{

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

	auto window = SDL_CreateWindow("Hi", 800, 600, SDL_WINDOW_OPENGL);
	auto context = SDL_GL_CreateContext(window);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

	/*possible error handling*/
	glewInit();

	int success{};
	char infolog[512]{};

	const char* vertexShaderCode = "#version 460 compatibility \n in vec3 vertexPos; void main(){gl_Position = vec4(vertexPos, 1.0);}";

	GLuint vertexID = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexID, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexID);

	glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexID, 512, NULL, infolog);
		std::cout << "Shader Compilation failed: " << std::endl << infolog << std::endl;
	}

	const char* fragmentShaderCode = "#version 460 compatibility \n out vec4 fragColor; void main(){fragColor = vec4(1.0,0.5, 0.25, 1.0);}";

	GLuint fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentID, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentID);

	glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentID, 512, NULL, infolog);
		std::cout << "Fragment Shader Compilation failed: " << std::endl 
			<< infolog << std::endl;
	}

	GLuint shaderProgID = glCreateProgram();
	glAttachShader(shaderProgID, vertexID);
	glAttachShader(shaderProgID, fragmentID);

	glLinkProgram(shaderProgID);

	glGetProgramiv(shaderProgID, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(shaderProgID, 512, NULL, infolog);
		std::cout << "Linking failed: " << std::endl << infolog << std::endl;
	}

	Vertex vertices[3] = {
		
		glm::vec3(-0.5f, -0.5f, 0.0f),
		glm::vec3(0.5f, -0.5f, 0.0f),
		glm::vec3(0.0f, 0.5f, 0.0f)
	};

	GLuint VertexBufferObject{};
	glGenBuffers(1, &VertexBufferObject);

	//eigentlich noch EBO

	GLuint VertexArrayObject{};
	glGenVertexArrays(1, &VertexArrayObject);
	glBindVertexArray(VertexArrayObject);

	glBindBuffer(GL_ARRAY_BUFFER,VertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glUseProgram(shaderProgID);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glClearColor(1.0f, 0.0f, 0.0f, 0.0f); // setzt Farbe, die Buffer annimmt

	bool closeWindow { false };
	while (!closeWindow)
	{

		glClear(GL_COLOR_BUFFER_BIT); // überschreibt alle Werte mit der Clear Color

		glDrawArrays(GL_TRIANGLES, 0, 3);

		SDL_Event event{};

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_EVENT_QUIT:
				closeWindow = true;
				break;

			default:
				break;
			}
		}
		SDL_GL_SwapWindow(window);
	}
}