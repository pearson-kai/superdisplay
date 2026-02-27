CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -Iinclude/SDL3

SRC = \
	src/main.cpp \
	src/Colour.cpp \
	src/Vec3.cpp \
	src/Triangle.cpp \
	src/Mesh.cpp \
	src/World.cpp \
	src/Camera.cpp \
	src/Screen.cpp

OUT = program

SDL_LIB_PATH = -Llib/SDL3
SDL_LIBS = -lSDL3

all:
	$(CXX) $(CXXFLAGS) $(SRC) $(SDL_LIB_PATH) $(SDL_LIBS) -o $(OUT)