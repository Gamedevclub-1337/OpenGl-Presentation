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




