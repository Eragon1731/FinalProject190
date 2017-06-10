#include "Window.h"
#include "Observer.h"
#include "Factory.h"
#include "GameController.h"
#include "CO2Molecule.h"



const char* window_title = "GLFW Starter Project"; 
GLint shaderProgram;
StereoCamera * cam;

// On some systems you need to change this to the absolute path

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 20.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;

using namespace std;
using namespace glm; 

Factory * factoryModel;
GameController * controller;

int lastUsedMolecule = 5;
int tick = 0;
int activeMolecules = 5;
int gameState = 0;

//SERVER CLIENT DATA
rpc::client * client1; 
int clientID;
int moleculeID;
bool resetFlag = false;

///////
Observer * otherController; 
CO2Molecule * moleculeContainer[50]; 

void Window::initialize_objects()
{
	factoryModel = new Factory(); 
	controller = new GameController();
	otherController = new Observer(); 

	shaderProgram = LoadShaders("./shader_1.vert", "./shader_1.frag");
	cam = new StereoCamera(2000.0f, 0.25f, 1.3333f, 45.0f, 0.001f, 10000.0f);

	Model co2M("../objects/co2/co2.obj");
	Model o2M("../objects/o2/o2.obj");
	for (int i = 0; i < 50; i++) {
		moleculeContainer[i] = new CO2Molecule(co2M, o2M, shaderProgram);
	}

	
	//setting up server client
	client1 = new rpc::client("localhost", 8080); 
	clientID = client1->call("assignID", 1).as<int>(); 
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

//Server to client logic
void Window::idle_callback()
{
	//checking if reset
	if (client1->call("gameReset", 1).as<bool>()) {
		Window::resetGame(); 
	}

	//setting and retrieving position of observer
	client1->call("clientPosition", 1, controller->toWorld[3].x, controller->toWorld[3].y, controller->toWorld[3].z);
	float tempProj[3];
	for (int count = 0; count < 3; count++) {
		float tempFloat = client1->call("recievePosition", 1, count).as<float>();
		tempProj[count] = tempFloat * 10.0f;
		cout << "count: "<< count<< endl;
		cout << "tempProj[i]: " << tempProj[count] << endl;
	}

	glm::vec3 otherPose;
	otherPose = glm::make_vec3(tempProj);
	otherController->otherPosition = otherPose;
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the shader of programID
	glUseProgram(shaderProgram);

	//TESTER::ANAGLYPH
	glm::mat4 leftProjection = cam->ApplyLeftFrustum();
	glColorMask(true, false, false, false);

	factoryModel->Render(Window::V, leftProjection); 
	controller->Render(Window::V, leftProjection);
	
	//molecules
	Window::renderMolecules(leftProjection, Window::V);

	//observer from other game
	otherController->Render(Window::V, leftProjection); 

	glClear(GL_DEPTH_BUFFER_BIT);

	glm::mat4 rightProjection = cam->ApplyRightFrustum();
	glColorMask(false, false, true, false);

	factoryModel->Render(Window::V, rightProjection); 
	controller->Render(Window::V, rightProjection);

	//molecules
	Window::renderMolecules(rightProjection, Window::V);

	//observer from other game
	otherController->Render(Window::V, rightProjection);

	glColorMask(true, true, true, true);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		else if (key == GLFW_KEY_R) {
			client1->call("setGameReset", 1, true);
			Window::resetGame();
		}
		//used to fire laser
		else if (key == GLFW_KEY_SPACE) {
			controller->renderLaser = true;
		}
	}
	else {
		controller->renderLaser = false;
	}
}

void Window::resetGame() {
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	for (int i = 0; i < 50; i++) {
		moleculeContainer[i]->active = false;
		moleculeContainer[i]->isCO2 = true;
		moleculeContainer[i]->ChangeToCO2();
		moleculeContainer[i]->spawn_point = glm::vec3(0.0f,0.0f,0.0f);//glm::vec3(0.0f, -0.75f, -1.5f);
		moleculeContainer[i]->model = glm::mat4(1.0f);
		moleculeContainer[i]->position = moleculeContainer[i]->spawn_point;
		moleculeContainer[i]->rotation = 0;
	}
	activeMolecules = 5;
	lastUsedMolecule = 5;
	tick = 0;
	gameState = 0;
	
	//set back 
	client1->call("setGameReset", 1, false);
}

void Window::renderMolecules(glm::mat4 projection, glm::mat4 view){

	// Set a new molecule to active every second (oculus should have 90 fps)

	if (tick == 200) {
		moleculeContainer[lastUsedMolecule]->active = true;
		activeMolecules++;
		lastUsedMolecule++;
		lastUsedMolecule %= 50;
		tick = 0;
	}
	if (gameState == 0) tick++;

	// Check game state for game over or win
	if (activeMolecules > 10 && gameState == 0) {
		// looooose

		for (int i = 0; i < 50; i++) {
			moleculeContainer[i]->active = true;
			moleculeContainer[i]->spawn_point = glm::vec3(rand() % 4 - 2, rand() % 4 - 2, rand() % 4 - 2);
			moleculeContainer[i]->position = moleculeContainer[i]->spawn_point;
		}

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		gameState = 2;
	}
	else if (activeMolecules <= 0 && gameState == 0) {
		// win!
		gameState = 1;
		glClearColor(135.0f / 255.0f, 206.0f / 255.0f, 250.0f / 255.0f, 1.0f);
	}

	// Render initial active molecules
	for (int i = 0; i < 5; ++i) {
		moleculeContainer[i]->active = true;
	}

	// Render all the active molecules
	for (int i = 0; i < 50; ++i) {
		if (moleculeContainer[i]->active) {
			moleculeContainer[i]->Render(view, projection);
		}
	}


	checkMoleculeIntersection();
}

void Window::checkMoleculeIntersection() {

	glm::vec3 red(1, 0, 0);
	glm::vec3 green(0, 0, 1);

	glm::vec3 rayOrigin;
	glm::vec3 rayDir;
	float rayDist;
	glm::vec3 moleculePos;
	float radius = 0.1f;

	float intersectionDist;

	// Cycle through all our molecules
	for (int i = 0; i < 50; ++i) {
		if (moleculeContainer[i]->active) {

			int numSelecting = 0;

			moleculePos = moleculeContainer[i]->position;

			// left controller ray
			rayOrigin = controller->ray.origin;
			rayDir = controller->ray.dir;
			rayDist = controller->ray.dist;
			intersectionDist = Window::intersection(rayOrigin, rayDir, moleculePos, radius);
			if (intersectionDist < rayDist && intersectionDist > 0.0f) {
				numSelecting++;
			}

			if (numSelecting == 1 && moleculeContainer[i]->isCO2 && gameState == 0) {
				moleculeContainer[i]->ChangeToO2();
				moleculeContainer[i]->isCO2 = false;
				activeMolecules--;

				//store molecule shot
				client1->call("setMoleculeShot", 0, i);
			}

			//check if opponent shot any
			int isShot = client1->call("moleculeShot", 0).as<int>();

			if (gameState == 0 && moleculeContainer[isShot]->isCO2 && numSelecting == 1) {
				moleculeContainer[isShot]->ChangeToO2();
				moleculeContainer[isShot]->isCO2 = false;
				activeMolecules--;
			}
			////////////////////////////////
		}
	}
}

float Window::intersection(glm::vec3 rayOrigin, glm::vec3 rayDir, glm::vec3 moleculePos, float radius) {

	glm::vec3 dir = rayDir;
	glm::vec3 eye = rayOrigin;
	glm::vec3 center = moleculePos;


	float dis = pow(dot(dir, eye - center), 2) - dot(dir, dir) * (dot(eye - center, eye - center) - pow(radius, 2));

	if (dis >= 0) {
		float t1 = glm::dot(-dir, eye - center) + glm::sqrt(dis) / glm::dot(dir, dir);
		float t2 = glm::dot(-dir, eye - center) - glm::sqrt(dis) / glm::dot(dir, dir);

		if (t1 == t2) {
			return t1;
		}

		// If this point is reached, there are two intersection points. Pick the
		// smaller t value, because it means it's the one that's closer to the camera.
		// However, make sure the t value isn't negative
		else if (t1 < 0 && t2 < 0) {
			return std::numeric_limits<float>::infinity();
		}
		else if (t1 < 0) return t2;
		else if (t2 < 0) return t1;

		float closer = t1 < t2 ? t1 : t2;
		return closer;

	}

	return std::numeric_limits<float>::infinity();
}


