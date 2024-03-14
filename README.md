# integrate-loki
integrate-loki demonstrates how to integrate the Loki library (written by Andrei Alexandrescu) to a modern and scalable CMake/C++ project.  

# Setup qconv
Supported os: Linux  

Known dependencies:  
cmake  
ninja   

### Setup (tested on Ubuntu 22.04):  
sudo snap install cmake --classic  
sudo apt-get -y install ninja-build   

# Configure, build, test
After the setup is done, the repo can be configured, builded and tested with the following commands (casted from the root of the repo):  
./source/scripts/configure.sh release  
./source/scripts/build.sh release  
./source/scripts/test.sh release  
  
# Compiler
The default compiler of qconv is g++. Clang is also perfectly fine to compile , 
the only advantage of clang is that experience shows that the google benchmark results are more consistent with the clang compiled binaries than with the gcc compiled binaries. Compilers can be switched by changing in ./source/scripts/build/init.sh 
CMAKE_CXX_COMPILER=clang++  
to  
CMAKE_CXX_COMPILER=g++  
