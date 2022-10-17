# About:
This is a demo provided with the presentation "Introduction into graphics programming using OpenGL"

# What is OpenGL ?:

![opengl-logo](https://user-images.githubusercontent.com/54768823/196055990-85a49460-b865-4acd-8783-39d67f4d790d.png)

OpenGL is one of the graphics API that is aimed to let the programmer have a contact with GPU but indirectly as that the communication between the programmer and the graphics hardware directly is impossible due to the OS limitation, that is why we need a graphics API that do this job for us, it is just an abstraction of the calculations and vertex manipulation and rasterizing between vertecies that need to be done in order to have something put on screen and GPU accelerate those calculation.

# OpenGL pipeline:

Pixels that are rendered on screen via OpenGl walks through a pipeline , the term pipeline simply means that the ouput of a stage is the input of the next one.

![The-graphics-pipeline-in-OpenGL-consists-of-these-5-steps-in-the-new-generation-of-cards](https://user-images.githubusercontent.com/54768823/196056113-cb7d6c20-2863-4f1c-b4fe-0579905daeac.png)

When OpenGL API was developed, it gave the programmer the freedom to manipulate 3 stages of this pipeline, the 2 main stages that needs to be present in order to witness pixels rendered on screen are the Vertex Program and the Fragement Program, Vertex Program is responsible for defining where the coordinates of the vertices of a shape are, while the Fragement is responsible of drawing between those points, and third one which is not necessary is the Geometry Program that gives us the ability to manipulate the vertecies more freely.


# Tools used in this presentation to ease the process of learning:

Creating a window context and handling events and determeining which OpenGL extension is used in the target platform can be a combursome task, and is not the main objective of this presentation, for that we are going to use some libraries that handles these things for us.

-GLFW:

<img width="197" alt="Screen Shot 2022-10-16 at 9 22 17 PM" src="https://user-images.githubusercontent.com/54768823/196056442-a96db558-7812-4670-8f3f-aa4b1ffdab9a.png">

-GLAD.


# Our Triangle !! Step by Step !!:

-First we need to iniatilize glfw to link our program to the functionalities of this library, then creating a window context where OpenGL is going to draw.

```c
GLFWwindow*	window;
	
	if (!glfwInit())          // Check that the initialization is succefull otherwise return an error
	{
		fprintf(stderr, "glfwInit() Failed\n");
		return (1);
	}
  // Specifying some additional parameters about the window creation
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  
  // Creating a window an the buffer
  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	
	if (!window)
	{
		fprintf(stderr, "glfwCreateWindow() Failed\n");
		return (1);
	}
	
  // Tell OpenGl to render in this context
	glfwMakeContextCurrent(window);
```

Then we fill in an array of coordinates of our vertecies and we generate a vertex array object and a vertex buffer object and bind them, next we put the data in the buffer using the size of the whole array so that the data is aligned correctly in memory.

```c
	// The vertecies
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
	
	// By default the vertex attributes are disabled, what we mean by vertex attributes is the input variables that're going to be used in the 
	// shader  program , using glEnableVertexAttribArray with 0 means that we are enabling the first vertex where we going to start the drawing from
	glEnableVertexAttribArray(0);
	// Defining an array of generic vertex data, first parameter is index of the vertex , second is the size of the data, third is the type, fourth 
	// is whether it's normilized or not we'll set it to false cause we don't need that for a triangle, fifth is the stride of the data meaning how
	// the data is going to be aligned in memory, and last is the offset of the first component of array.
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);
```

The shader programs for vertex and fragement shader.

Vertex Shader:
```glsl
	#version 330 core

	// Here we specify the location of our vertex which we will input, which is enabled using glEnableVertexAttribArray(0) in our C code where we passed
	// our vertex data the buffer for the shader program
	layout(location = 0) in vec2 Position;

	void	main()
	{
		// A special OpenGL variable which holds the position of vertecies.
		gl_Position = vec4(Position, 0.0f, 1.0f);
	}
```
Fragement Shader:
```glsl
	#version 330 core

	// The color output that will rasterize between the coordinates of the vertecies
	out	vec4	color;

	void	main()
	{
		color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
```

Compiling and linking the shader program in the C code :
First we need to read our shader code and store it in a string from a file, then we create the vertex and fragement shader using glCreateShader(shadertype), after that we link both shaders to a shader program that we will use for rendering. The code is straightforward.

```C
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
```


# Installation:

Follow this instructions:

## School Macs

- sh installation.sh
- sh build.sh
- sh run.sh

## Windows
you will need to install Visual Studio 2013 or above.

- run the build.bat from the cmd provided by Visual Studio

## Linux

you will need to install glew & glwf
and just run the linux_build.sh




