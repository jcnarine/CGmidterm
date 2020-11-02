
#define GLM_ENABLE_EXPERIMENTAL

#include <Logging.h>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#include <json.hpp>
#include <fstream>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/VertexArrayObject.h"
#include "Graphics/Shader.h"
#include "Gameplay/Camera.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Gameplay/Transform.h"
#include "Graphics/Texture2D.h"
#include "Graphics/Texture2DData.h"
#include "Utilities/InputHelpers.h"
#include "Utilities/MeshBuilder.h"
#include "Utilities/MeshFactory.h"
#include "Utilities/NotObjLoader.h"
#include "Utilities/ObjLoader.h"
#include "Utilities/VertexTypes.h"
#include "CollisionDetection.h"
#include "GameObject.h"
<<<<<<< Updated upstream
=======
#include "Player.h"
#include <glm/gtx/io.hpp>
>>>>>>> Stashed changes

#define LOG_GL_NOTIFICATIONS

Player P1, P2;
/*
	Handles debug messages from OpenGL
	https://www.khronos.org/opengl/wiki/Debug_Output#Message_Components
	@param source    Which part of OpenGL dispatched the message
	@param type      The type of message (ex: error, performance issues, deprecated behavior)
	@param id        The ID of the error or message (to distinguish between different types of errors, like nullref or index out of range)
	@param severity  The severity of the message (from High to Notification)
	@param length    The length of the message
	@param message   The human readable message from OpenGL
	@param userParam The pointer we set with glDebugMessageCallback (should be the game pointer)
*/
void GlDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	std::string sourceTxt;
	switch (source) {
	case GL_DEBUG_SOURCE_API: sourceTxt = "DEBUG"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: sourceTxt = "WINDOW"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceTxt = "SHADER"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY: sourceTxt = "THIRD PARTY"; break;
	case GL_DEBUG_SOURCE_APPLICATION: sourceTxt = "APP"; break;
	case GL_DEBUG_SOURCE_OTHER: default: sourceTxt = "OTHER"; break;
	}
	switch (severity) {
	case GL_DEBUG_SEVERITY_LOW:          LOG_INFO("[{}] {}", sourceTxt, message); break;
	case GL_DEBUG_SEVERITY_MEDIUM:       LOG_WARN("[{}] {}", sourceTxt, message); break;
	case GL_DEBUG_SEVERITY_HIGH:         LOG_ERROR("[{}] {}", sourceTxt, message); break;
		#ifdef LOG_GL_NOTIFICATIONS
	case GL_DEBUG_SEVERITY_NOTIFICATION: LOG_INFO("[{}] {}", sourceTxt, message); break;
		#endif
	default: break;
	}
}

GLFWwindow* window;
Camera::sptr camera = nullptr;

void GlfwWindowResizedCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	camera->ResizeWindow(width, height);
}

bool initGLFW() {
	if (glfwInit() == GLFW_FALSE) {
		LOG_ERROR("Failed to initialize GLFW");
		return false;
	}

#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
	
	//Create a new GLFW window
	window = glfwCreateWindow(800, 800, "INFR1350U", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set our window resized callback
	glfwSetWindowSizeCallback(window, GlfwWindowResizedCallback);

	return true;
}

bool initGLAD() {
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
		LOG_ERROR("Failed to initialize Glad");
		return false;
	}
	return true;
}

void InitImGui() {
	// Creates a new ImGUI context
	ImGui::CreateContext();
	// Gets our ImGUI input/output 
	ImGuiIO& io = ImGui::GetIO();
	// Enable keyboard navigation
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	// Allow docking to our window
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	// Allow multiple viewports (so we can drag ImGui off our window)
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	// Allow our viewports to use transparent backbuffers
	io.ConfigFlags |= ImGuiConfigFlags_TransparentBackbuffers;

	// Set up the ImGui implementation for OpenGL
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 410");

	// Dark mode FTW
	ImGui::StyleColorsDark();

	// Get our imgui style
	ImGuiStyle& style = ImGui::GetStyle();
	//style.Alpha = 1.0f;
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 0.8f;
	}
}

void ShutdownImGui()
{
	// Cleanup the ImGui implementation
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	// Destroy our ImGui context
	ImGui::DestroyContext();
}

std::vector<std::function<void()>> imGuiCallbacks;
void RenderImGui() {
	// Implementation new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	// ImGui context new frame
	ImGui::NewFrame();

	if (ImGui::Begin("Debug")) {
		// Render our GUI stuff
		for (auto& func : imGuiCallbacks) {
			func();
		}
		ImGui::End();
	}
	
	// Make sure ImGui knows how big our window is
	ImGuiIO& io = ImGui::GetIO();
	int width{ 0 }, height{ 0 };
	glfwGetWindowSize(window, &width, &height);
	io.DisplaySize = ImVec2((float)width, (float)height);

	// Render all of our ImGui elements
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// If we have multiple viewports enabled (can drag into a new window)
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		// Update the windows that ImGui is using
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		// Restore our gl context
		glfwMakeContextCurrent(window);
	}
}


void RenderVAO(
	const Shader::sptr& shader,
	const VertexArrayObject::sptr& vao,
	const Camera::sptr& camera,
	const Transform::sptr& transform)
{
	shader->SetUniformMatrix("u_ModelViewProjection", camera->GetViewProjection() * transform->LocalTransform());
	shader->SetUniformMatrix("u_Model", transform->LocalTransform());
	shader->SetUniformMatrix("u_NormalMatrix", transform->NormalMatrix());
	vao->Render();
}

void Player1Input(float dt)
{
	P1.setKeyPressed(false);
	P1.setDt(dt);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		P1.setKeyPressed(true);
		P1.applyForce(glm::vec3(15.0f, 0.0f, 0.0f));
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		P1.setKeyPressed(true);
		P1.applyForce(glm::vec3(-15.0f, 0.0f, 0.0f));
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		P1.setKeyPressed(true);
		P1.applyForce(glm::vec3(0.0f, -15.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		P1.setKeyPressed(true);
		P1.applyForce(glm::vec3(0.0f, 15.0f, 0.0f));
	}

	P1.movePlayer();
	
}

void Player2Input(float dt)
{
	P2.setKeyPressed(false);
	P2.setDt(dt);

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		P2.setKeyPressed(true);
		P2.applyForce(glm::vec3(15.0f, 0.0f, 0.0f));
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		P2.setKeyPressed(true);
		P2.applyForce(glm::vec3(-15.0f, 0.0f, 0.0f));
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		P2.setKeyPressed(true);
		P2.applyForce(glm::vec3(0.0f, -15.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		P2.setKeyPressed(true);
		P2.applyForce(glm::vec3(0.0f, 15.0f, 0.0f));
	}

	P2.movePlayer();

}

void ManipulateTransformWithInput(const Transform::sptr& transform, float dt) {
<<<<<<< Updated upstream
	std::cout <<(transform->GetLocalPosition()) << std::endl;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		transform->MoveLocal( 5.0f * dt, 0.0f,  0.0f); 
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { 
		transform->MoveLocal(-5.0f * dt, 0.0f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		transform->MoveLocal(0.0f, -5.0f * dt, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		transform->MoveLocal(0.0f,  5.0f * dt, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		transform->MoveLocal(0.0f, 0.0f,  5.0f * dt);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		transform->MoveLocal(0.0f, 0.0f, -5.0f * dt);
	}
=======
	/*std::cout <<(transform->GetLocalPosition()) << std::endl;
	system("CLS");*/
>>>>>>> Stashed changes

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		transform->MoveLocal( 5.0f * dt, 0.0f,  0.0f); 
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { 
		transform->MoveLocal(-5.0f * dt, 0.0f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		transform->MoveLocal(0.0f, 0.0f,  5.0f * dt);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		transform->MoveLocal(0.0f, 0.0f, -5.0f * dt);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		transform->MoveLocal(0.0f,  5.0f * dt, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		transform->MoveLocal(0.0f, -5.0f * dt, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) { 
		transform->RotateLocal(0.0f, -45.0f * dt, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		transform->RotateLocal(0.0f,  45.0f * dt,0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		transform->RotateLocal( 45.0f * dt, 0.0f,0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		transform->RotateLocal(-45.0f * dt, 0.0f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		transform->RotateLocal(0.0f, 0.0f, 45.0f * dt);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		transform->RotateLocal(0.0f, 0.0f, -45.0f * dt);
	}
}

<<<<<<< Updated upstream
=======
void puckCollisionWithWall(objectTag t )
{
	std::cout<<("Collision");
	system("CLS");

	if (t== objectTag::BM_WALL){

	}
	else if (t== objectTag::T_WALL){


	}
	else if (t== objectTag::LS_WALL){


	}
	else if(t== objectTag::RS_WALL){


	}

}

>>>>>>> Stashed changes
struct Material
{
	Texture2D::sptr Albedo;
	Texture2D::sptr Specular;
	Texture2D::sptr DiffuseTexture;
	float			TextureMix;
	float           Shininess;
};

int main() {
	Logger::Init(); // We'll borrow the logger from the toolkit, but we need to initialize it

	//Initialize GLFW
	if (!initGLFW())
		return 1;


	//Initialize GLAD
	if (!initGLAD())
		return 1;

	// Let OpenGL know that we want debug output, and route it to our handler function
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(GlDebugMessage, nullptr);

	// Enable texturing
	glEnable(GL_TEXTURE_2D);
	
	VertexArrayObject::sptr table = ObjLoader::LoadFromFile("Objects/AirHockeyTable.obj");
	VertexArrayObject::sptr puck = ObjLoader::LoadFromFile("Objects/AirHockeyPuck.obj");
	VertexArrayObject::sptr player1 = ObjLoader::LoadFromFile("Objects/AirHockeyPad.obj");
	VertexArrayObject::sptr player2 = ObjLoader::LoadFromFile("Objects/AirHockeyPad.obj");
		
	// Load our shaders
	Shader::sptr shader = Shader::Create();
	shader->LoadShaderPartFromFile("Shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
	shader->LoadShaderPartFromFile("Shaders/frag_blinn_phong_textured.glsl", GL_FRAGMENT_SHADER);  
	shader->Link();  

	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 5.0f);
	glm::vec3 lightCol = glm::vec3(0.0f, 0.45f, 0.94f);
	float     lightAmbientPow = 0.789f;
	float     lightSpecularPow = 1.0f;
	glm::vec3 ambientCol = glm::vec3(1.0f);
	float     ambientPow = 0.606f;
	float     shininess = 0.0f;
	float     lightLinearFalloff = 0.260f;
	float     lightQuadraticFalloff = 0.070f;
	
	// TODO: load textures

#pragma region Texture
// Load our texture data from a file
	Texture2DData::sptr paddlediffuseMap = Texture2DData::LoadFromFile("Images/Paddle_Diffuse.png");
	Texture2DData::sptr paddlespecularMap = Texture2DData::LoadFromFile("Images/Paddle_Specular.png");
	// Create a texture from the data
	Texture2D::sptr paddleDiffuse = Texture2D::Create();
	paddleDiffuse->LoadData(paddlediffuseMap);
	Texture2D::sptr paddleSpecular = Texture2D::Create();
	paddleSpecular->LoadData(paddlespecularMap);
	// Creating an empty texture
	Texture2DDescription PaddleDesc = Texture2DDescription();
	PaddleDesc.Width = 1;
	PaddleDesc.Height = 1;
	PaddleDesc.Format = InternalFormat::RGB8;
	Texture2D::sptr paddleTexture = Texture2D::Create(PaddleDesc);
	paddleTexture->Clear();

#pragma endregion TableTexture

#pragma region Texture
// Load our texture data from a file
	Texture2DData::sptr diffuseMap = Texture2DData::LoadFromFile("Images/airHockey.png");
	Texture2DData::sptr specularMap = Texture2DData::LoadFromFile("Images/Table_Specular.png");
	// Create a texture from the data
	Texture2D::sptr tableDiffuse = Texture2D::Create();
	tableDiffuse->LoadData(diffuseMap);
	Texture2D::sptr tableSpecular = Texture2D::Create();
	tableSpecular->LoadData(specularMap);
	// Creating an empty texture
	Texture2DDescription tableDesc = Texture2DDescription();
	tableDesc.Width = 1;
	tableDesc.Height = 1;
	tableDesc.Format = InternalFormat::RGB8;
	Texture2D::sptr tableTexture = Texture2D::Create(tableDesc);
	tableTexture->Clear();

#pragma endregion TableTexture

#pragma region Texture
// Load our texture data from a file
	Texture2DData::sptr plasticDiffuseMap = Texture2DData::LoadFromFile("Images/Plastic_Diffuse.png");
	Texture2DData::sptr specularDiffuseMap = Texture2DData::LoadFromFile("Images/Plastic_Specular.png");
	// Create a texture from the data
	Texture2D::sptr plasticDiffuse = Texture2D::Create();
	plasticDiffuse->LoadData(plasticDiffuseMap);
	Texture2D::sptr plasticSpecular = Texture2D::Create();
	plasticSpecular->LoadData(specularDiffuseMap);
	// Creating an empty texture
	Texture2DDescription plasticDesc = Texture2DDescription();
	plasticDesc.Width = 1;
	plasticDesc.Height = 1;
	plasticDesc.Format = InternalFormat::RGB8;
	Texture2D::sptr plasticTexture = Texture2D::Create(plasticDesc);
	plasticTexture->Clear();

#pragma endregion PlasticTexture

#pragma region Texture

	Texture2DData::sptr boxDiffuseMap = Texture2DData::LoadFromFile("Images/box.bmp");
	Texture2D::sptr boxDiffuse = Texture2D::Create();
	boxDiffuse->LoadData(boxDiffuseMap);
	Texture2DDescription boxDesc = Texture2DDescription();
	boxDesc.Width = 1;
	boxDesc.Height = 1;
	boxDesc.Format = InternalFormat::RGB8;
	Texture2D::sptr texture3 = Texture2D::Create(boxDesc);
	texture3->Clear();

#pragma endregion BoxTexture

	// TODO: store some info about our materials for each object

	// We'll use a temporary lil structure to store some info about our material (we'll expand this later)
	Material materials[4];

	materials[0].Albedo = tableDiffuse;
	materials[0].Specular = tableSpecular;
	materials[0].DiffuseTexture = boxDiffuse;
	materials[0].Shininess = 4.0f;
	materials[0].TextureMix = 0.0;

	materials[1].Albedo = plasticDiffuse;
	materials[1].Specular = plasticSpecular;
	materials[1].DiffuseTexture = boxDiffuse;
	materials[1].Shininess = 32.0f;
	materials[1].TextureMix = 0.0f;

	materials[2].Albedo = paddleDiffuse;
	materials[2].Specular = paddleSpecular;
	materials[2].DiffuseTexture = boxDiffuse;
	materials[2].Shininess = 32.0f;
	materials[2].TextureMix = 0.0f;

<<<<<<< Updated upstream
	materials[3].Albedo = plasticDiffuse;
	materials[3].Specular = plasticSpecular;
=======
	materials[3].Albedo = paddleDiffuse;
	materials[3].Specular = paddleSpecular;
>>>>>>> Stashed changes
	materials[3].DiffuseTexture = boxDiffuse;
	materials[3].Shininess = 32.0f;
	materials[3].TextureMix = 0.0f;


	camera = Camera::Create();
	camera->SetPosition(glm::vec3(0, 3, 18)); // Set initial position
	camera->SetUp(glm::vec3(0, 0, 1)); // Use a z-up coordinate system
	camera->LookAt(glm::vec3(0.0f)); // Look at center of the screen
	camera->SetFovDegrees(100.0f); // Set an initial FOV
	camera->SetOrthoHeight(20.0f);
	// These are our application / scene level uniforms that don't necessarily update
	// every frame
	shader->SetUniform("u_LightPos", lightPos);
	shader->SetUniform("u_LightCol", lightCol);
	shader->SetUniform("u_AmbientLightStrength", lightAmbientPow);
	shader->SetUniform("u_SpecularLightStrength", lightSpecularPow);
	shader->SetUniform("u_AmbientCol", ambientCol);
	shader->SetUniform("u_AmbientStrength", ambientPow);
	shader->SetUniform("u_Shininess", shininess);
	shader->SetUniform("u_LightAttenuationConstant", 1.0f);
	shader->SetUniform("u_LightAttenuationLinear", lightLinearFalloff);
	shader->SetUniform("u_LightAttenuationQuadratic", lightQuadraticFalloff);

	// We'll add some ImGui controls to control our shader
	imGuiCallbacks.push_back([&]() {
		if (ImGui::CollapsingHeader("Scene Level Lighting Settings"))
		{
			if (ImGui::ColorPicker3("Ambient Color", glm::value_ptr(ambientCol))) {
				shader->SetUniform("u_AmbientCol", ambientCol);
			}
			if (ImGui::SliderFloat("Fixed Ambient Power", &ambientPow, 0.01f, 1.0f)) {
				shader->SetUniform("u_AmbientStrength", ambientPow); 
			}
		}
		if (ImGui::CollapsingHeader("Light Level Lighting Settings")) 
		{
			if (ImGui::DragFloat3("Light Pos", glm::value_ptr(lightPos), 0.01f, -10.0f, 10.0f)) {
				shader->SetUniform("u_LightPos", lightPos);
			}
			if (ImGui::ColorPicker3("Light Col", glm::value_ptr(lightCol))) {
				shader->SetUniform("u_LightCol", lightCol);
			}
			if (ImGui::SliderFloat("Light Ambient Power", &lightAmbientPow, 0.0f, 10.0f)) {
				shader->SetUniform("u_AmbientLightStrength", lightAmbientPow);
			}
			if (ImGui::SliderFloat("Light Specular Power", &lightSpecularPow, 0.0f, 1.0f)) {
				shader->SetUniform("u_SpecularLightStrength", lightSpecularPow);
			}
			if (ImGui::DragFloat("Light Linear Falloff", &lightLinearFalloff, 0.01f, 0.0f, 1.0f)) {
				shader->SetUniform("u_LightAttenuationLinear", lightLinearFalloff);
			}
			if (ImGui::DragFloat("Light Quadratic Falloff", &lightQuadraticFalloff, 0.01f, 0.0f, 1.0f)) {
				shader->SetUniform("u_LightAttenuationQuadratic", lightQuadraticFalloff);
			}
		}
		if (ImGui::CollapsingHeader("Material Level Lighting Settings"))
		{
			if (ImGui::SliderFloat("Material One", &materials[0].Shininess, 0.1f, 128.0f)) {
				shader->SetUniform("u_Shininess", materials[0].Shininess);
			}
			if (ImGui::SliderFloat("Material Two", &materials[1].Shininess, 0.1f, 128.0f)) {
				shader->SetUniform("u_Shininess", materials[1].Shininess);
			}
		}
	});

	// GL states
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// NEW STUFF

	// Create some transforms and initialize them
	Transform::sptr transforms[4];
	transforms[0] = Transform::Create();
	transforms[1] = Transform::Create();
	transforms[2] = Transform::Create();
	transforms[3] = Transform::Create();

	// We can use operator chaining, since our Set* methods return a pointer to the instance, neat!
	transforms[0]->SetLocalPosition(0.0f, 0.0f, 0.0f)->SetLocalRotation(90.0, 0.0f, 0.0f);
	transforms[1]->SetLocalPosition(0.0f, 0.0f, 4.45f)->SetLocalRotation(90.0f, 0.0f, 0.0f);
	transforms[2]->SetLocalPosition(3.0f, 0.0f, 4.45f)->SetLocalRotation(90.0f, 0.0f, 0.0f);
	transforms[3]->SetLocalPosition(-3.0f, 0.0f, 4.45f)->SetLocalRotation(90.0f, 0.0f, 0.0f);

	// We'll store all our VAOs into a nice array for easy access
	VertexArrayObject::sptr vaos[4];
	vaos[0] = table;
	vaos[1] = puck;
	vaos[2] = player1;
	vaos[3] = player2;

	P1.setTransform(transforms[2]);
	P2.setTransform(transforms[3]);

	P1.setTag(playerTag::PLAYER_ONE);
	P2.setTag(playerTag::PLAYER_TWO);

	// We'll use a vector to store all our key press events for now
	std::vector<KeyPressWatcher> keyToggles;
	// This is an example of a key press handling helper. Look at InputHelpers.h an .cpp to see
	// how this is implemented. Note that the ampersand here is capturing the variables within
	// the scope. If you wanted to do some method on the class, your best bet would be to give it a method and
	// use std::bind
	keyToggles.emplace_back(GLFW_KEY_T, [&](){ camera->ToggleOrtho(); });

	int selectedVao = 2; // select cube by default
	keyToggles.emplace_back(GLFW_KEY_KP_ADD, [&]() {
		selectedVao++;
		if (selectedVao >= 4)
			selectedVao = 1;
	});
	keyToggles.emplace_back(GLFW_KEY_KP_SUBTRACT, [&]() {
		selectedVao--;
		if (selectedVao <= 0)
			selectedVao = 3;
	});

	InitImGui();
		
	std::vector<GameObject> boxes;
	std::vector<GameObject> circles;

	#pragma region GameObject Creation

	boxes.push_back(GameObject(glm::vec3(0, 4.9, 0), objectTag::T_WALL));
	boxes.push_back(GameObject(glm::vec3(0, -4.9, 0), objectTag::BM_WALL));
	boxes.push_back(GameObject(glm::vec3(7, 4.9, 0), objectTag::LS_WALL));
	boxes.push_back(GameObject(glm::vec3(-7, 4.9, 0), objectTag::RS_WALL));

	circles.push_back(GameObject(glm::vec3(0.0f, 0.0f, 4.45f), objectTag::PUCK));
	circles.push_back(GameObject(glm::vec3(3.0f, 0.0f, 4.45f), objectTag::P1));
	circles.push_back(GameObject(glm::vec3(-3.0f, 0.0f, 4.45f), objectTag::P2));

	#pragma endregion  

	// Our high-precision timer
	double lastFrame = glfwGetTime();
	
	///// Game loop /////
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		// Calculate the time since our last frame (dt)
		double thisFrame = glfwGetTime();
		float dt = static_cast<float>(thisFrame - lastFrame);

		// We'll make sure our UI isn't focused before we start handling input for our game
		if (!ImGui::IsAnyWindowFocused()) {
			// We need to poll our key watchers so they can do their logic with the GLFW state
			// Note that since we want to make sure we don't copy our key handlers, we need a const
			// reference!
			for (const KeyPressWatcher& watcher : keyToggles) {
				watcher.Poll(window);
			}
			/*Player1Input(dt);
			Player2Input(dt);*/
			// We'll run some basic input to move our transform around
			ManipulateTransformWithInput(transforms[selectedVao], dt);
		}

		glClearColor(0.08f, 0.17f, 0.31f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		circles[0].UpdatePosition(transforms[1]->GetLocalPosition());
		circles[1].UpdatePosition(transforms[2]->GetLocalPosition());
		circles[2].UpdatePosition(transforms[3]->GetLocalPosition());

		objectTag boxCol = CollisionDetection::CheckWallCollision(circles[0]);
		if (!(boxCol== objectTag::NONE)){
		puckCollisionWithWall(boxCol);
		}
		
		for (int ix = 1; ix <= 2; ix++) {
			bool cirCol = CollisionDetection::CheckSphereCollision(circles[ix], circles[0]);
			objectTag cirtemp = circles[ix].GetTag();
			if (cirCol) {
			puckCollisionWithWall(cirtemp);
			}
		}

		shader->Bind();
		// These are the uniforms that update only once per frame
		shader->SetUniformMatrix("u_View", camera->GetView());
		shader->SetUniform("u_CamPos", camera->GetPosition());

		// Tell OpenGL that slot 0 will hold the diffuse, and slot 1 will hold the specular
		shader->SetUniform("s_Diffuse",  0);
		shader->SetUniform("s_Specular", 1); 
		shader->SetUniform("s_Diffuse2", 2);
		
		// Render all VAOs in our scene
		for(int ix = 0; ix < 4; ix++) {
			// TODO: Apply materials
			// Apply material properties for each instance
			materials[ix].Albedo->Bind(0);
			materials[ix].Specular->Bind(1);
			materials[ix].DiffuseTexture->Bind(2);
			shader->SetUniform("u_Shininess", materials[ix].Shininess);
			shader->SetUniform("u_TextureMix", materials[ix].TextureMix);
			RenderVAO(shader, vaos[ix], camera, transforms[ix]);			
		}

		RenderImGui();

		glfwSwapBuffers(window);
		lastFrame = thisFrame;
	}

	ShutdownImGui();

	// Clean up the toolkit logger so we don't leak memory
	Logger::Uninitialize();
	return 0;
}