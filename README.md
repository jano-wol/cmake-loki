# cmake-loki
cmake-loki demonstrates how to integrate the Loki library (written by Andrei Alexandrescu) to a modern and scalable CMake/C++ project.  

# Setup cmake-loki
Supported os: Linux  

Known dependencies:  
cmake  
ninja   

### Setup commands (tested on Ubuntu 22.04):  
sudo snap install cmake --classic  
sudo apt-get -y install ninja-build   

# Configure, build, test
After the setup is done, the repo can be configured, builded and tested with the following commands (casted from the root of the repo):  
./source/scripts/configure.sh release  
./source/scripts/build.sh release  
./source/scripts/test.sh release  
If all tests of test.sh succeed, the repo installation is considered successful!

# Repo structure
The client code, which is relying on Loki library, can be found in ./source/test  
After the build step, the compiled executables can be found in ./build/release/test/bin

# Compilers
The default compiler of cmake-loki is g++. Clang is also perfectly fine to compile the codebase of the repo, 
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
