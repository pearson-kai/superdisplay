#include <iostream>
#include <filesystem>
#include <algorithm>

#include "Mesh.hpp"
#include "World.hpp"
#include "Camera.hpp"
#include "Screen.hpp"

namespace fs = std::filesystem;

Mesh* meshMaker(fs::path _meshfile)
{
	// make mesh from meshfolder/mesh#.txt
	
	// return &mesh;
}

int main()
{
	// instantiate screen
	// present splash screen
	
	// instantiate world
	
    fs::path directory = "./../meshfolder/";
    for (const auto& entry : fs::directory_iterator(directory)) 
    {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") 
        {
            // world.add(false, meshMaker(entry));
        }
    }
	
	// instantiate camera
	
	//camera.setWorld(& world);
	
	bool running = true;
	while(running)
	{
        /*
		screen.clearScene();
		screen.buildScene(camera);
		screen.dispayScene();
		*/

		// user action to affect world
		// trigger to check running
	}
	
	//world.explode();
	//screen.explode();
	
	return 0;
}