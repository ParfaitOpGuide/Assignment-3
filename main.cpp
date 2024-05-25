#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <chrono>

//GLM HEADERS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Light/Light.h"
#include "Light/PointLight.h"
#include "Light/DirectionLight.h"

#include "Object/ModelExtra.h"
#include "Object/Model3D.h"
#include "Object/Model.h"


#include "Camera/MyCamera.h"
#include "Camera/OrthoCamera.h"
#include "Camera/FirstPersonCamera.h"
#include "Camera/ThirdPersonCamera.h"

#include "ShaderClasses/Shader.h"
#include "P6/MyVector.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std::chrono_literals;
constexpr std::chrono::nanoseconds timestep(16ms);



//window size`
float width = 600.0f, height = 600.0f;


int camType = 0;
//create cams
//ThirdPersonCamera* ThirdPerson;
//FirstPersonCamera* FirstPerson;
OrthoCamera* Orthographic;
//camera vector to store both cameras
std::vector<MyCamera*> vecCameras;

//lights
PointLight* Point = new PointLight(glm::vec3(-4, 5, 0));
DirectionLight* Direction = new DirectionLight(glm::vec3(0, 20, 0));



//uo direction, usually just 1 in y
glm::vec3 worldUp = glm::vec3(0, 1.0f, 0);


//for mouse movement
bool mouse_start = true;
bool mouse_pause = false;
double mouse_x_last;
double mouse_sens = 0.01;

//for model drawing
std::vector<Model*> vecModels;

Model3D* MainModel;
//timer function
float start, end;

//bool to close window with a button because im tired of having to press window when my mouse gets locked
bool close = false;

//light state switch
int lightState = 0;

//speed multiplier for movement
float fSpeed = 2.f;
float fOffset = 0.f;

void Key_Callback(GLFWwindow* window, //pointer to window
	int key, //keycode of press
	int scancode, //physical position of the press
	int action,//either press/release
	int mods) //modifier keys
{
	if (action == GLFW_REPEAT || action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			close = true;
			break;
		}
	}
}

int main(void)
{
	GLFWwindow* window;


	/* Initialize the library */
	if (!glfwInit())
		return -1;

	float width = 600.0f, height = 600.0f;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, "Agasen", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}



	//IDENTITY MATRIXES
	glm::mat3 identity_matrix3 = glm::mat3(1.0f);
	glm::mat4 identity_matrix4 = glm::mat4(1.0f);

	//create shader
	Shader* ShaderSkybox = new Shader(window, "Shaders/skyblock.vert", "Shaders/skyblock.frag");
	Shader* Shader1 = new Shader(window, "Shaders/sampleNew.vert", "Shaders/sampleNew.frag");
	Shader* ShaderNormals = new Shader(window, "Shaders/normals.vert", "Shaders/normals.frag");

	std::vector<Shader*> vecShaders;
	vecShaders.push_back(ShaderNormals);
	vecShaders.push_back(Shader1);
	//create objects and pass the path to the things needed for it specifically, obj first then tex, then normal if needed
	
	MainModel = new Model3D(window, { 0,-3, 0 }, Shader1->shaderProg, "3D/sphere.obj", "3D/sphere.jpg", 4.f);


	//ThirdPerson = new ThirdPersonCamera(MainModel, worldUp, height, width);
	//FirstPerson = new FirstPersonCamera(MainModel, worldUp, height, width);
	Orthographic = new OrthoCamera(MainModel, worldUp);
	
	//Load shader file into a strin steam
	std::fstream vertSrc("Shaders/sampleNew.vert");
	std::stringstream vertBuff;
	std::fstream skyboxVertSrc("Shaders/skyblock.vert");
	std::stringstream skyboxVertBuff;

	vertBuff << vertSrc.rdbuf();
	skyboxVertBuff << skyboxVertSrc.rdbuf();

	std::string vertS = vertBuff.str();
	const char* v = vertS.c_str();
	std::string skyboxVertS = skyboxVertBuff.str();
	const char* sky_v = skyboxVertS.c_str();

	//same but for frag
	std::fstream fragSrc("Shaders/sampleNew.frag");
	std::stringstream fragBuff;
	std::fstream skyboxFragSrc("Shaders/skyblock.frag");
	std::stringstream skyboxFragBuff;

	fragBuff << fragSrc.rdbuf();
	skyboxFragBuff << skyboxFragSrc.rdbuf();

	std::string fragS = fragBuff.str();
	const char* f = fragS.c_str();
	std::string skyboxFragS = skyboxFragBuff.str();
	const char* sky_f = skyboxFragS.c_str();
	
	

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	gladLoadGL();


	//set the callback function to the window
	glfwSetKeyCallback(window, Key_Callback);
	

	//blend yo mama
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, //source factor
		GL_ONE_MINUS_SRC_ALPHA);

	//camera stuff gets pushed into the vector
	vecCameras.push_back(Orthographic);
	
	//vecCameras.push_back(ThirdPerson);
	//vecCameras.push_back(FirstPerson);
	

	//init obj
	vecModels.push_back(MainModel);


	P6::MyVector sample(0, 0, 0);
	
MainModel->Acceleration = P6::MyVector (0, -50, 0);
	float velX, velY, velZ;
	std::cout << "Velocity:\nX: ";
	std::cin >> velX;
	std::cout << "Y: ";
	std::cin >> velY;
	std::cout << "Y: ";
	std::cin >> velZ;


	MainModel->Velocity = P6::MyVector (velX, velY, velZ);
	//clock and var
	using clock = std::chrono::high_resolution_clock;
	auto curr_time = clock::now();
	auto prev_time = curr_time;
	std::chrono::nanoseconds curr_ns(0);

	

	

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window) && !close)
	{
		//get current time
		curr_time = clock::now();
		//duration between this and last
		auto dur = std::chrono::duration_cast<std::chrono::nanoseconds> (curr_time - prev_time);
		//prev to current for next iteration
		prev_time = curr_time;

		curr_ns += dur;

		if (curr_ns >= timestep) {
			//nano to millie
			auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(curr_ns);
			//reset
			curr_ns -= curr_ns;

			MainModel->Update((float)ms.count()/1000);
		}
		//end clock

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//processes events depending on camtype
	//	vecCameras[camType]->processEvents({ -2 * vecModels[0]->x,  -2 * vecModels[0]->y, -2*vecModels[0]->z }, MainModel->objDir);

		
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);

		int shaderCount;
		//pass all necessary drawing stuff to the objects draw, call all objects draw in vector
		for (int i = 0; i < vecModels.size(); i++) {

			
			//std::cout << shaderCount << std::endl;
			vecModels[i]->processEvents(identity_matrix4,
				ShaderNormals->shaderProg,
				vecCameras[camType],
				Point, Direction);
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	for (int i = 0; i < vecModels.size(); i++) {
		vecModels[i]->explodeAndDie(); //deletes all vaos since we cant do that from main anymore
	}


	glfwTerminate();
	return 0;
}