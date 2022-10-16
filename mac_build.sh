if [ ! -d build ];
then
	mkdir build;
	chmod 777 build;
fi;
pushd build
INCLUDE_LIBS="~/goinfre/homebrew/include"
LINKING_LIBS="~/goinfre/homebrew/lib"

clang -Wno-deprecated-declarations -L ~/goinfre/homebrew/lib -I ~/goinfre/homebrew/include ../src/main.c -o Demo -lGLEW -lglfw -framework OpenGL -DGL_SILENCE_DEPRECATION

clang -Wno-deprecated-declarations -L ~/goinfre/homebrew/lib -I ~/goinfre/homebrew/include ../src/triangle.c -o Triangle -lGLEW -lglfw -framework OpenGL -DGL_SILENCE_DEPRECATION
popd
