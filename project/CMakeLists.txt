cmake_minimum_required(VERSION 3.10)

# Set project name
project(MyProject)

# Add source files
file(GLOB SOURCES "src/*.cpp")

# Add executable target
add_executable(MyExecutable ${SOURCES})
