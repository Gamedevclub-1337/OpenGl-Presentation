pushd ~/goinfre/
#brew installation
mkdir homebrew && curl -L https://github.com/Homebrew/brew/tarball/master | tar xz --strip 1 -C homebrew
popd

echo 'export "PATH=$PATH:~/goinfre/homebrew/bin/"' >> ~/.bashrc
source ~/.bashrc

brew update
brew install glew glfw
