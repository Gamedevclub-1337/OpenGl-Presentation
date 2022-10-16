#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

#define WINDOW_WIDTH	1366
#define WINDOW_HEIGHT	768
#define WINDOW_TITLE	"TRIANGLE"


char*	demoReadFile(const char* filepath);
GLuint	demoCompileShader(const char* filepath, GLuint shaderType);
GLuint	demoCreateShaderProgram(const char* vertexShaderFilepath,
							   const char* fragmentShaderFilepath);


int	main(int argc, char **argv)
{
	GLFWwindow*	window;
	
	if (!glfwInit())
	{
		fprintf(stderr, "glfwInit() Failed\n");
		return (1);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	
	if (!window)
	{
		fprintf(stderr, "glfwCreateWindow() Failed\n");
		return (1);
	}
	
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		fprintf(stderr, "glewInit() Failed\n");
		return (1);
	}
	
	float	vertices[] = {
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.0f, 0.5f
	};
	
	
	GLuint	VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	GLuint	VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);
	
	GLuint	shaderProgram = demoCreateShaderProgram("shaders/triangle_vs.glsl",
												   "shaders/triangle_fs.glsl");
	glUseProgram(shaderProgram);
	
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
	}
	
	glfwTerminate();
	return (0);
}


char	*demoReadFile(const char* filepath)
{
	char	*fileBuffer = NULL;
	
	FILE*	fileStream = fopen(filepath, "rb");
	
	if (!fileStream)
	{
		printf("fopen Failed : %s\n", filepath);
		return (NULL);
	}
	
	if (fseek(fileStream, 0, SEEK_END) == -1)
	{
		printf("fseek() SEEK_END Failed : %s\n", filepath);
		fclose(fileStream);
		return (NULL);
	}
	
	long int fileSize = ftell(fileStream);
	
	if (fseek(fileStream, 0, SEEK_SET) == -1)
	{
		printf("fseek() Failed SEEK_SET: %s\n", filepath);
		fclose(fileStream);
		return (NULL);
	}
	
	fileBuffer = (char*)malloc(sizeof(char) * (fileSize + 1));
	
	size_t bytesRead;
	
	if ((bytesRead = fread(fileBuffer, 1, fileSize, fileStream)) != fileSize)
	{
		printf("fread() Failed: bytes Read = %zd of %s\n", bytesRead, filepath);
		free(fileBuffer);
		fclose(fileStream);
		return (NULL);
	}
	fileBuffer[fileSize] = '\0';
	
	return (fileBuffer);
}

GLuint	demoCompileShader(const char* filepath, GLuint shaderType)
{
	char*	shaderFileBuffer = demoReadFile(filepath);
	
	if (!shaderFileBuffer)
	{
		return (0);
	}
	
	GLint	shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, (const GLchar * const *)&shaderFileBuffer, NULL);
	glCompileShader(shaderId);
	
	int	compilationStatus;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compilationStatus);
	if (compilationStatus != GL_TRUE)
	{
		int	logLength = 0;
		char	errorMessage[2048];
		glGetShaderInfoLog(shaderId, 2048, &logLength, errorMessage);
		
		printf("file : %s\n", filepath);
		printf("%s\n", errorMessage);
		glDeleteShader(shaderId);
		shaderId = 0;
	}
	free(shaderFileBuffer);
	return (shaderId);
}

GLuint	demoCreateShaderProgram(const char *vertexShaderFilepath,
							   const char *fragmentShaderFilepath)
{
	GLuint	vs = demoCompileShader(vertexShaderFilepath, GL_VERTEX_SHADER);
	if (vs == 0)
		return (0);
	GLuint	fs = demoCompileShader(fragmentShaderFilepath, GL_FRAGMENT_SHADER);
	if (fs == 0)
	{
		glDeleteShader(vs);
		return (0);
	}
	
	GLuint	program = glCreateProgram();
	
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	
	glLinkProgram(program);
	
	glDeleteShader(vs);
	glDeleteShader(fs);
	return (program);
}
