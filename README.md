# integrate-loki
integrate-loki demonstrates how to integrate the Loki library (written by Andrei Alexandrescu) to a modern and scalable CMake/C++ project.  

# Setup integrate-loki
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
  
# Compilers
The default compiler of integrate-loki is g++. Clang is also perfectly fine to compile the codebase of the repo, 
but it comes with a few additional dependency. To install clang on Ubuntu cast the followings:
sudo apt install clang
sudo apt install libstdc++-12-dev
sudo apt-get install libc++-dev
sudo apt install libc++abi-dev
After successful install the project compilers can be switched by changing 
CMAKE_CXX_COMPILER=g++  
to  
CMAKE_CXX_COMPILER=clang++  
in ./source/scripts/build/init.sh
