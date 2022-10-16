#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

#define WINDOW_WIDTH	1366
#define WINDOW_HEIGHT	768
#define WINDOW_TITLE	"DEMO"

static const float entity_size = 0.1;
float x = 0.0, y = 0.0, vx = 0.0, vy = 0.0, angle = 0;

char	*demoReadFile(const char* filepath);
GLuint	demoCompileShader(const char* filepath, GLuint shaderType);
GLuint	demoCreateShaderProgram(const char *vertexShaderFilepath,
							   const char *fragmentShaderFilepath);
void	input(GLFWwindow* window, int key, int scancode, int action, int mode);

int	main(int agrc, char **argv)
{
	GLFWwindow*	window;
	
	if (!glfwInit())
	{
		printf("glfwInit Failed\n");
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
		printf("glfwCreateWindow Failed\n");
		glfwTerminate();
		return (1);
	}
	
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		printf("glewInit Failed\n");
		return (1);
	}
	
	GLuint	vao, vbo, ebo;
	
	float	vertices[] = {
		-entity_size ,   -entity_size , 0.0f,
		entity_size ,  -entity_size , 0.0f,
		- entity_size ,   entity_size , 0.0f,
		entity_size ,   entity_size , 0.0f
	};
	int indecies[] = {
		0, 1, 2,
		1, 3, 2
	};
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indecies), indecies, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	
	GLuint	shaderProgram = demoCreateShaderProgram("shaders/simple_vs.glsl",
												   "shaders/simple_fs.glsl");
	glUseProgram(shaderProgram);
	while (!glfwWindowShouldClose(window))
	{
		float time = glfwGetTime();
		y += vy * 0.01;
		x += vx * 0.01;
		printf("%f\n", y);
		if (x > 1 - entity_size)
			vx = -1;
		if (x < -1 + entity_size)
			vx += 1;
		if (y > 1 - entity_size)
			vy = -1;
		if (y < -1 + entity_size)
			vy = 1;
		glUniform1f(glGetUniformLocation(shaderProgram, "itime"), angle);
		glUniform1f(glGetUniformLocation(shaderProgram, "colorr"), time/10.);
		glUniform1f(glGetUniformLocation(shaderProgram, "y"), y);
		glUniform1f(glGetUniformLocation(shaderProgram, "x"), x);
		glfwPollEvents();
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glfwSetKeyCallback(window, input);
		printf("%f\n", y);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
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
	return (program);
}

void	input(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		vy =  1;
	else if (key == GLFW_KEY_S && action == GLFW_PRESS)
		vy = -1;
	else if (key == GLFW_KEY_A && action == GLFW_PRESS)
		vx =  1;
	else if (key == GLFW_KEY_D && action == GLFW_PRESS)
		vx = -1;
	else if (key == GLFW_KEY_R && action == GLFW_PRESS)
		angle += 0.1;
}

