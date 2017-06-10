#include "Window.h"
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
//CO2Molecule GameScene::moleculeContainer;

using namespace std;
using namespace glm; 

Factory * factoryModel;
GameController * leftController;

int lastUsedMolecule = 5;
int tick = 0;
int activeMolecules = 5;
int gameState = 0;

rpc::client * client1; 

CO2Molecule * moleculeContainer[50]; 

void Window::initialize_objects()
{
	factoryModel = new Factory(); 
	leftController = new GameController(); 

	shaderProgram = LoadShaders("./shader_1.vert", "./shader_1.frag");
	cam = new StereoCamera(2000.0f, 0.25f, 1.3333f, 45.0f, 0.001f, 10000.0f);

	Model co2M("H:/FinalProject/LeapMotion/objects/co2/co2.obj");
	Model o2M("H:/FinalProject/LeapMotion/objects/o2/o2.obj");
	for (int i = 0; i < 50; i++) {
		moleculeContainer[i] = new CO2Molecule(co2M, o2M, shaderProgram);
	}
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

void Window::idle_callback()
{
	// Call the update function the cube
	

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
	leftController->Render(Window::V, leftProjection); 
	
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::mat4 rightProjection = cam->ApplyRightFrustum();
	glColorMask(false, false, true, false);

	factoryModel->Render(Window::V, rightProjection); 
	leftController->Render(Window::V, rightProjection); 

	glColorMask(true, true, true, true);
	///////////////////////////////////////////

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
		else if (key == GLFW_KEY_R){
			Window::resetGame(); 
		}
		else if (key == GLFW_KEY_SPACE) {
			leftController->renderLaser = true;
		}
		leftController->renderLaser = false; 
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
	//checkMoleculeIntersection();
	//if hit with hand 
	//if hit "render laser"

}

