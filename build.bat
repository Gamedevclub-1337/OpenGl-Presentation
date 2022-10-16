@echo off

set INCLUDE_GLFW3=/I .\..\dependencies\glfw3\include\
set INCLUDE_GLEW=/I .\..\dependencies\glew\include\
set LIBS=opengl32.lib user32.lib kernel32.lib gdi32.lib .\..\dependencies\glfw3\lib-vc2022\glfw3dll.lib .\..\dependencies\glew\glew32s.lib

IF NOT EXIST build (
	mkdir build
	copy dependencies\glfw3\lib-vc2022\glfw3.dll build\.
	copy dependencies\glew\lib-vc2022\glfw3.dll build\.
)

pushd build

cl  %INCLUDE_GLFW3% %INCLUDE_GLEW% ..\src\main.c %LIBS% /FeDemo
cl  %INCLUDE_GLFW3% %INCLUDE_GLEW% ..\src\triangle.c %LIBS% /FeTriangle

popd
