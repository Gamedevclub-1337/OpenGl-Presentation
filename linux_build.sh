if [ ! -d build ];
then
	mkdir build;
	chmod 777 build;
fi;
pushd build
clang -lGL -lGLEW -lglfw ../src/main.c -o Demo
clang -lGL -lGLEW -lglfw ../src/triangle.c -o Triangle
popd
