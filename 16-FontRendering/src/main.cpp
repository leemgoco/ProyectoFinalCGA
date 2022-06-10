#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>
//std includes
#include <string>
#include <iostream>
#include <time.h>
#include <chrono>

// contains new std::shuffle definition
#include <algorithm>
#include <random>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

//Enemy Include
#include "Headers/Enemy.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"
#include "Headers/ThirdPersonCamera.h"

// Font rendering
#include "Headers/FontTypeRendering.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture.h"

// Include loader Model class
#include "Headers/Model.h"

// Include Terrain
#include "Headers/Terrain.h"

#include "Headers/AnimationUtils.h"

// Include Colision headers functions
#include "Headers/Colisiones.h"

// ShadowBox include
#include "Headers/ShadowBox.h"

// OpenAL include
#include <AL/alut.h>

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;

const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

GLFWwindow* window;

Shader shader;
//Shader con skybox
Shader shaderSkybox;
//Shader con multiples luces
Shader shaderMulLighting;
//Shader para el terreno
Shader shaderTerrain;
//Shader para las particulas de fountain
Shader shaderParticlesFountain;
//Shader para las particulas de fuego
Shader shaderParticlesFire;
//Shader para visualizar el buffer de profundidad
Shader shaderViewDepth;
//Shader para dibujar el buffer de profunidad
Shader shaderDepth;
//Shader para dibujar un objeto con solo textura
Shader shaderTexture;

std::shared_ptr<FirstPersonCamera> cameraFP(new FirstPersonCamera());
std::shared_ptr<Camera> camera(new ThirdPersonCamera());
float distanceFromTarget = 14.0;
float lastDistanceFromTarget = 0.0;
float yaw = 0.0;

Sphere skyboxSphere(20, 20);
Box boxCollider;
Sphere sphereCollider(10, 10);
Box boxViewDepth;
Box boxLightViewBox;

Box pivoteCam;

Box muroFondo;
Box muroFrontal;
Box muroIzquierdo;
Box muroDerecho;
Box muroFondo2;
Box muroFrontal2;
Box muroIzquierdo2;
Box muroDerecho2;
Box boxReferencia;
Box boxMenu;

ShadowBox* shadowBox;

// Models complex instances
/*
// Lamps
Model modelLamp1;
Model modelLamp2;
Model modelLampPost2;
// Hierba
Model modelGrass;
*/

// Fountain
Model modelFountain;
// Model animate instance
// Mayow
Model mayowModelAnimate;
Model astroProta;
Model modelBotones;
Model modelGenerador;
Model modelCompuerta;
Model modelEdCompuerta;
Model modelPlataforma;
Model modelPlaCompuerta;
Model modelRokas;
Model modelLuzGenerador;
Model modelLuzBotones;
Model modelEscenario2;
Model modelBidones;
Model modelCaja1;
Model modelCaja2;
Model modelCompu;
Model modelCuerpo;
Model modelEstanteria;



//Enemigos
Enemy enemigo1(glm::vec3(13.0f, 0.05f, -5.0f), "enemy1");
Enemy enemigo2(glm::vec3(13.0f, 0.05f, -5.0f), "enemy2");
Enemy enemigo3(glm::vec3(13.0f, 0.05f, -5.0f), "enemy3");

// Terrain model instance
Terrain terrain(-1, -1, 200, 1, "../Textures/heightmap.png");
Terrain terrain2(-1, -1, 200, 16, "../Textures/heightmap.png");

GLuint textureYellowID, textureBlueID, textureRedID, textureOrangeID,
textureGreenID, texturePurpleID;
GLuint textureTerrainBackgroundID, textureTerrainRID, textureTerrainGID,
textureTerrainBID, textureTerrainBlendMapID, textureTerrainBlendMapID2;
GLuint textureParticleFountainID, textureParticleFireID, texId;
GLuint skyboxTextureID;
GLuint textureMenuID, textureMenu2ID, textureActivaID;



// Modelo para el redener de texto
FontTypeRendering::FontTypeRendering* modelText;
FontTypeRendering::FontTypeRendering* modelText2;

//vairables para medir tiempo
int tiempo = 0;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

std::string fileNames[6] = { "../Textures/mp_bloodvalley/blood-valley_ft.tga",
		"../Textures/mp_bloodvalley/blood-valley_bk.tga",
		"../Textures/mp_bloodvalley/blood-valley_up.tga",
		"../Textures/mp_bloodvalley/blood-valley_dn.tga",
		"../Textures/mp_bloodvalley/blood-valley_rt.tga",
		"../Textures/mp_bloodvalley/blood-valley_lf.tga" };

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

// Model matrix definitions
glm::mat4 modelMatrixPivoteCam = glm::mat4(1.0f);
glm::mat4 modelMatrixMayow = glm::mat4(1.0f);
glm::mat4 modelMatrixFountain = glm::mat4(1.0f);
glm::mat4 modelMatrixAstroProta = glm::mat4(1.0f);
glm::mat4 modelMatrixAstroProta2 = glm::mat4(1.0f);
glm::mat4 modelMatrixMuroFondo = glm::mat4(1.0f);
glm::mat4 modelMatrixMuroFrontal = glm::mat4(1.0f);
glm::mat4 modelMatrixMuroIzquierdo = glm::mat4(1.0f);
glm::mat4 modelMatrixMuroDerecho = glm::mat4(1.0f);
glm::mat4 modelMatrixMuroFondo2 = glm::mat4(1.0f);
glm::mat4 modelMatrixMuroFrontal2 = glm::mat4(1.0f);
glm::mat4 modelMatrixMuroIzquierdo2 = glm::mat4(1.0f);
glm::mat4 modelMatrixMuroDerecho2 = glm::mat4(1.0f);
glm::mat4 defaultMatrix = glm::mat4(1.0f);
glm::mat4 defaultMatrix2 = glm::mat4(1.0f);

//vectors
glm::vec3 astroPosition;
glm::vec3 astroOrigin = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 camPivOrigin = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 astroInitialOrientation = glm::vec3(1.0f, 0.0f, 0.0f);
glm::mat4 modelMatrixCompuerta = glm::mat4(1.0f);
glm::mat4 modelMatrixEdCompuerta = glm::mat4(1.0f);
glm::mat4 modelMatrixPlataforma = glm::mat4(1.0f);
glm::mat4 modelMatrixPlaCompuerta = glm::mat4(1.0f);
glm::mat4 modelMatrixEscenario2 = glm::mat4(1.0f);

float timer;
int banderaCaminar = 0;
bool fontbandera = false;
int animationIndex = 1;
int animationIndexMayow = 0;
int modelSelected = 2;
bool enableCountSelected = true;
int limiteIzquierdo = 132 - 80;
int limiteDerecho = 132 + 85;
int pasado = 0;
int posterior = 0;
int cameraSelected = 0;
int tiempoRespawnProta = 0;
int tiempoOxigeno = 0;
int situacion = 1;
bool enableCameraSelected = true;
bool playerRespawn = false;
bool playerRespawn2 = false;
bool enableAction = true;
bool actionE = false;
bool enableEscotilla1 = false;
bool cambianivel2 = false;
bool cambianivel3 = false;
bool empiezaJuego = false;
bool pressEnter = false;


bool escenario1 = false;
bool escenario2 = true;

//bool escenario2 = false;
glm::vec3 vectorDireccionEnemigo = glm::vec3(0.0f);
float anguloEntreDosVectores;

std::vector<std::vector<bool>> combBotones = {
	{false, false, false},
	{false, false, false},
	{false, false, false},
	{false, false, false} };

std::vector<bool> lucesBotones = { false, false, false, false };

// Lamps positions
std::vector<glm::vec3> lamp1Position = { glm::vec3(-7.03, 0, -19.14), glm::vec3(
		24.41, 0, -34.57), glm::vec3(-10.15, 0, -54.10) };
std::vector<float> lamp1Orientation = { -17.0, -82.67, 23.70 };
std::vector<glm::vec3> lamp2Position = { glm::vec3(-36.52, 0, -23.24),
		glm::vec3(-52.73, 0, -3.90) };
std::vector<float> lamp2Orientation = { 21.37 + 90, -65.0 + 90 };

//Posición botones y plataformas
std::vector<glm::vec3> botonesPos = { glm::vec3(-66.6, 0, -2), glm::vec3(
		-39.5, 0, 14.6), glm::vec3(34.57, 0, -3), glm::vec3(78, 0, 12) };

std::vector<glm::vec3> rokasPos = { glm::vec3(-66.6, 0, -2), glm::vec3(
		-39.5, 0, 14.6), glm::vec3(34.57, 0, -3), glm::vec3(78, 0, 12) };

// Blending model unsorted
std::map<std::string, glm::vec3> blendingUnsorted = { { "aircraft", glm::vec3(
		10.0, 0.0, -17.5) }, { "lambo", glm::vec3(23.0, 0.0, 0.0) }, { "heli",
		glm::vec3(5.0, 10.0, -5.0) },
		{ "fountain", glm::vec3(5.0, 0.0, -40.0) }, { "fire", glm::vec3(0.0,
				0.0, 7.0) } };

double deltaTime;
double currTime, lastTime;

// Jump variables
bool isJump = false;
float GRAVITY = 1.81;
double tmv = 0;
double startTimeJump = 0;

// Definition for the particle system
GLuint initVel, startTime;
GLuint VAOParticles;
GLuint nParticles = 8000;
double currTimeParticlesAnimation, lastTimeParticlesAnimation;

// Definition for the particle system fire
GLuint initVelFire, startTimeFire;
GLuint VAOParticlesFire;
GLuint nParticlesFire = 2000;
GLuint posBuf[2], velBuf[2], age[2];
GLuint particleArray[2];
GLuint feedback[2];
GLuint drawBuf = 1;
float particleSize = 0.5, particleLifetime = 3.0;
double currTimeParticlesAnimationFire, lastTimeParticlesAnimationFire;
bool cambiaVelocidadEnemigo;

// Colliders
std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > collidersOBB;
std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> > collidersSBB;

std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > collidersOBB2;
std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> > collidersSBB2;


//std::map<std::string, bool> collisionDetection;

// Framesbuffers
GLuint depthMap, depthMapFBO;

/**********************
 * OpenAL config
 */

 // OpenAL Defines
#define NUM_BUFFERS 3
#define NUM_SOURCES 3
#define NUM_ENVIRONMENTS 1
// Listener
ALfloat listenerPos[] = { 0.0, 0.0, 4.0 };
ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
ALfloat listenerOri[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 0.0 };
// Source 0
ALfloat source0Pos[] = { -2.0, 0.0, 0.0 };
ALfloat source0Vel[] = { 0.0, 0.0, 0.0 };
// Source 1
ALfloat source1Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source1Vel[] = { 0.0, 0.0, 0.0 };
// Source 2
ALfloat source2Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source2Vel[] = { 0.0, 0.0, 0.0 };
// Buffers
ALuint buffer[NUM_BUFFERS];
ALuint source[NUM_SOURCES];
ALuint environment[NUM_ENVIRONMENTS];
// Configs
ALsizei size, freq;
ALenum format;
ALvoid* data;
int ch;
ALboolean loop;
std::vector<bool> sourcesPlay = { true, true, true };

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow* window, int key, int scancode, int action,
	int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void initParticleBuffers();
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);
void prepareScene();
void prepareDepthScene();
void renderScene(bool renderParticles = true);
void prepareScene2();
void prepareDepthScene2();
void renderScene2(bool renderParticles = true);
void lucesEscenari1(ShadowBox* shadowBox, glm::mat4* view);
void lucesEscenari2(ShadowBox* shadowBox, glm::mat4* view);
void cameraMove();
bool excepCollider(std::string string1, std::string string2);
bool excepCollider2(std::string string1, std::string string2);
void updateBotonCollider(std::map<std::string, bool> collisionDetection);
void updateEscenario1();
void preRender1();
void collidersManagmentEs1();
void soundEscene1();
void inicialMatrixs();
void updateEscenario2();
void preRender2();
void collidersManagmentEs2();
void soundEscene2();


void initParticleBuffers() {
	// Generate the buffers
	glGenBuffers(1, &initVel);   // Initial velocity buffer
	glGenBuffers(1, &startTime); // Start time buffer

	// Allocate space for all buffers
	int size = nParticles * 3 * sizeof(float);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), NULL,
		GL_STATIC_DRAW);

	// Fill the first velocity buffer with random velocities
	glm::vec3 v(0.0f);
	float velocity, theta, phi;
	GLfloat* data = new GLfloat[nParticles * 3];
	for (unsigned int i = 0; i < nParticles; i++) {

		theta = glm::mix(0.0f, glm::pi<float>() / 6.0f,
			((float)rand() / RAND_MAX));
		phi = glm::mix(0.0f, glm::two_pi<float>(), ((float)rand() / RAND_MAX));

		v.x = sinf(theta) * cosf(phi);
		v.y = cosf(theta);
		v.z = sinf(theta) * sinf(phi);

		velocity = glm::mix(0.6f, 0.8f, ((float)rand() / RAND_MAX));
		v = glm::normalize(v) * velocity;

		data[3 * i] = v.x;
		data[3 * i + 1] = v.y;
		data[3 * i + 2] = v.z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

	// Fill the start time buffer
	delete[] data;
	data = new GLfloat[nParticles];
	float time = 0.0f;
	float rate = 0.00075f;
	for (unsigned int i = 0; i < nParticles; i++) {
		data[i] = time;
		time += rate;
	}
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] data;

	glGenVertexArrays(1, &VAOParticles);
	glBindVertexArray(VAOParticles);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void initParticleBuffersFire() {
	// Generate the buffers
	glGenBuffers(2, posBuf);    // position buffers
	glGenBuffers(2, velBuf);    // velocity buffers
	glGenBuffers(2, age);       // age buffers

	// Allocate space for all buffers
	int size = nParticlesFire * sizeof(GLfloat);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, age[1]);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);

	// Fill the first age buffer
	std::vector<GLfloat> initialAge(nParticlesFire);
	float rate = particleLifetime / nParticlesFire;
	for (unsigned int i = 0; i < nParticlesFire; i++) {
		int index = i - nParticlesFire;
		float result = rate * index;
		initialAge[i] = result;
	}
	// Shuffle them for better looking results
	//Random::shuffle(initialAge);
	auto rng = std::default_random_engine{ };
	std::shuffle(initialAge.begin(), initialAge.end(), rng);
	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, initialAge.data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create vertex arrays for each set of buffers
	glGenVertexArrays(2, particleArray);

	// Set up particle array 0
	glBindVertexArray(particleArray[0]);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	// Set up particle array 1
	glBindVertexArray(particleArray[1]);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, age[1]);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	// Setup the feedback objects
	glGenTransformFeedbacks(2, feedback);

	// Transform feedback 0
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[0]);

	// Transform feedback 1
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[1]);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
			glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
			nullptr);

	if (window == nullptr) {
		std::cerr
			<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
			<< std::endl;
		destroy();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Inicialización de los shaders
	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox_fog.fs");
	shaderMulLighting.initialize(
		"../Shaders/iluminacion_textura_animation_shadow.vs",
		"../Shaders/multipleLights_shadow.fs");
	shaderTerrain.initialize("../Shaders/terrain_shadow.vs",
		"../Shaders/terrain_shadow.fs");
	shaderParticlesFountain.initialize("../Shaders/particlesFountain.vs",
		"../Shaders/particlesFountain.fs");
	shaderParticlesFire.initialize("../Shaders/particlesFire.vs",
		"../Shaders/particlesFire.fs", { "Position", "Velocity", "Age" });
	shaderViewDepth.initialize("../Shaders/texturizado.vs",
		"../Shaders/texturizado_depth_view.fs");
	shaderDepth.initialize("../Shaders/shadow_mapping_depth.vs",
		"../Shaders/shadow_mapping_depth.fs");
	shaderTexture.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado.fs");

	// Inicializacion de los objetos.
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(100.0f, 100.0f, 100.0f));

	boxCollider.init();
	boxCollider.setShader(&shader);
	boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	sphereCollider.init();
	sphereCollider.setShader(&shader);
	sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	boxViewDepth.init();
	boxViewDepth.setShader(&shaderViewDepth);

	boxLightViewBox.init();
	boxLightViewBox.setShader(&shaderViewDepth);

	terrain.init();
	terrain.setShader(&shaderTerrain);
	terrain.setPosition(glm::vec3(100, 0, 100));

	pivoteCam.init();
	pivoteCam.setShader(&shaderMulLighting);

	boxReferencia.init();
	boxReferencia.setShader(&shaderMulLighting);

	boxMenu.init();
	boxMenu.setShader(&shaderMulLighting);
	boxMenu.setPosition(glm::vec3(0.0, -6.0, 0.0));
	boxMenu.setScale(glm::vec3(38.0, 33.0, 1.0));
	boxMenu.setOrientation(glm::vec3(-25.0, 0.0, 0.0));

	//Rokas
	modelRokas.loadModel("../models/rokas/Piedras2.obj");
	modelRokas.setShader(&shaderMulLighting);

	//Generador
	modelGenerador.loadModel("../models/generadorG/Generador.obj");
	modelGenerador.setShader(&shaderMulLighting);

	//Botones
	modelBotones.loadModel("../models/botones/Botones.fbx");
	modelBotones.setShader(&shaderMulLighting);

	//LucesBotones
	modelLuzBotones.loadModel("../models/Luces/Luz/LucesGenerador.obj");
	modelLuzBotones.setShader(&shaderMulLighting);

	//LucesGeneradores
	modelLuzGenerador.loadModel("../models/Luces/LuzEspacio/LuzEspacio.obj");
	modelLuzGenerador.setShader(&shaderMulLighting);

	//Compuerta
	modelCompuerta.loadModel("../models/compuerta/Compuerta.fbx");
	modelCompuerta.setShader(&shaderMulLighting);

	//Edificio compuerta
	modelEdCompuerta.loadModel("../models/edCompuerta/EdificioCompuerta.fbx");
	modelEdCompuerta.setShader(&shaderMulLighting);

	//PlatCompuerta
	modelPlaCompuerta.loadModel("../models/compuerta/CompuertaBase.fbx");
	modelPlaCompuerta.setShader(&shaderMulLighting);

	//Plataforma
	modelPlataforma.loadModel("../models/plataforma/Plataforma.fbx");
	modelPlataforma.setShader(&shaderMulLighting);

	//Fountain
	modelFountain.loadModel("../models/fountain/fountain.obj");
	modelFountain.setShader(&shaderMulLighting);

	//Mayow
	mayowModelAnimate.loadModel("../models/mayow/personaje2.fbx");
	mayowModelAnimate.setShader(&shaderMulLighting);

	//astroProta
	astroProta.loadModel("../models/astroProta/astroAnim.fbx");
	astroProta.setShader(&shaderMulLighting);

	//Muros
	muroFondo.init();
	muroFondo.setShader(&shaderMulLighting);
	muroFrontal.init();
	muroFrontal.setShader(&shaderMulLighting);
	muroDerecho.init();
	muroDerecho.setShader(&shaderMulLighting);
	muroIzquierdo.init();
	muroIzquierdo.setShader(&shaderMulLighting);

	//Escenario2
	modelEscenario2.loadModel("../models/SegundoEscenario/Escenario2.obj");
	modelEscenario2.setShader(&shaderMulLighting);

	//Bidones
	modelBidones.loadModel("../models/SegundoEscenario/Bidones.obj");
	modelBidones.setShader(&shaderMulLighting);

	//Caja1
	modelCaja1.loadModel("../models/SegundoEscenario/Caja1.obj");
	modelCaja1.setShader(&shaderMulLighting);

	//Caja2
	modelCaja2.loadModel("../models/SegundoEscenario/Caja2.obj");
	modelCaja2.setShader(&shaderMulLighting);

	//Compu
	modelCompu.loadModel("../models/SegundoEscenario/Compu.obj");
	modelCompu.setShader(&shaderMulLighting);

	//Cuerpo
	modelCuerpo.loadModel("../models/SegundoEscenario/Cuerpo.obj");
	modelCuerpo.setShader(&shaderMulLighting);

	//Estanteria
	modelEstanteria.loadModel("../models/SegundoEscenario/Estanteria.obj");
	modelEstanteria.setShader(&shaderMulLighting);

	cameraFP->setPosition(glm::vec3(0.0, 3.0, 4.0));
	camera->setDistanceFromTarget(distanceFromTarget);
	camera->setSensitivity(1.0);

	// Definimos el tamanio de la imagen
	int imageWidth, imageHeight;
	FIBITMAP* bitmap;
	unsigned char* data;

	// Carga de texturas para el skybox
	Texture skyboxTexture = Texture("");
	glGenTextures(1, &skyboxTextureID);
	// Tipo de textura CUBE MAP
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		skyboxTexture = Texture(fileNames[i]);
		FIBITMAP* bitmap = skyboxTexture.loadImage(true);
		unsigned char* data = skyboxTexture.convertToData(bitmap, imageWidth,
			imageHeight);
		if (data) {
			glTexImage2D(types[i], 0, GL_RGBA, imageWidth, imageHeight, 0,
				GL_BGRA, GL_UNSIGNED_BYTE, data);
		}
		else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage(bitmap);
	}

	// Definiendo la textura a utilizar
	Texture textureTerrainBackground("../Textures/BlendMap/Tierra.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainBackground.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainBackground.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBackgroundID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainBackground.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainR("../Textures/BlendMap/Cables.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainR.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainR.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainRID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainR.freeImage(bitmap);

	// Definiendo la textura a utilizar para el INICIO
	Texture textureMenu1("../Textures/Menu.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureMenu1.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureMenu1.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureMenuID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureMenuID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureMenu1.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainG("../Textures/BlendMap/Rocas.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainG.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainG.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainGID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainG.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainB("../Textures/BlendMap/Plano.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainB.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainB.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainB.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainBlendMap("../Textures/blendMap2.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainBlendMap.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainBlendMap.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBlendMapID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainBlendMap.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainBlendMap2("../Textures/blendMap3.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainBlendMap2.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainBlendMap2.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBlendMapID2);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainBlendMap2.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureYellow("../Textures/Texturas/Amarillo.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureYellow.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureYellow.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureYellowID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureYellowID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureYellow.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureBlue("../Textures/Texturas/Azul.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureBlue.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureBlue.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureBlueID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureBlueID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureBlue.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureOrange("../Textures/Texturas/Naranja.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureOrange.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureOrange.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureOrangeID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureOrangeID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureOrange.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureGreen("../Textures/Texturas/Verde.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureGreen.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureGreen.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureGreenID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureGreenID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureGreen.freeImage(bitmap);

	Texture textureParticlesFountain("../Textures/bluewater.png");
	bitmap = textureParticlesFountain.loadImage();
	data = textureParticlesFountain.convertToData(bitmap, imageWidth,
		imageHeight);
	glGenTextures(1, &textureParticleFountainID);
	glBindTexture(GL_TEXTURE_2D, textureParticleFountainID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureParticlesFountain.freeImage(bitmap);

	Texture textureParticleFire("../Textures/fire.png");
	bitmap = textureParticleFire.loadImage();
	data = textureParticleFire.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureParticleFireID);
	glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureParticleFire.freeImage(bitmap);

	std::uniform_real_distribution<float> distr01 =
		std::uniform_real_distribution<float>(0.0f, 1.0f);
	std::mt19937 generator;
	std::random_device rd;
	generator.seed(rd());
	int size = nParticlesFire * 2;
	std::vector<GLfloat> randData(size);
	for (int i = 0; i < randData.size(); i++) {
		randData[i] = distr01(generator);
	}

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_1D, texId);
	glTexStorage1D(GL_TEXTURE_1D, 1, GL_R32F, size);
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, size, GL_RED, GL_FLOAT,
		randData.data());
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	shaderParticlesFire.setInt("Pass", 1);
	shaderParticlesFire.setInt("ParticleTex", 0);
	shaderParticlesFire.setInt("RandomTex", 1);
	shaderParticlesFire.setFloat("ParticleLifetime", particleLifetime);
	shaderParticlesFire.setFloat("ParticleSize", particleSize);
	shaderParticlesFire.setVectorFloat3("Accel",
		glm::value_ptr(glm::vec3(0.0f, 0.1f, 0.0f)));
	shaderParticlesFire.setVectorFloat3("Emitter",
		glm::value_ptr(glm::vec3(0.0f)));

	glm::mat3 basis;
	glm::vec3 u, v, n;
	v = glm::vec3(0, 1, 0);
	n = glm::cross(glm::vec3(1, 0, 0), v);
	if (glm::length(n) < 0.00001f) {
		n = glm::cross(glm::vec3(0, 1, 0), v);
	}
	u = glm::cross(v, n);
	basis[0] = glm::normalize(u);
	basis[1] = glm::normalize(v);
	basis[2] = glm::normalize(n);
	shaderParticlesFire.setMatrix3("EmitterBasis", 1, false,
		glm::value_ptr(basis));

	/*******************************************
	 * Inicializacion de los buffers de la fuente
	 *******************************************/
	initParticleBuffers();

	/*******************************************
	 * Inicializacion de los buffers del fuego
	 *******************************************/
	initParticleBuffersFire();

	/*******************************************
	 * Inicializacion del framebuffer para
	 * almacenar el buffer de profunidadad
	 *******************************************/
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH,
		SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
		depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/*******************************************
	 * OpenAL init
	 *******************************************/
	 //alutInit(0, nullptr);
	 //alListenerfv(AL_POSITION, listenerPos);
	 //alListenerfv(AL_VELOCITY, listenerVel);
	 //alListenerfv(AL_ORIENTATION, listenerOri);
	 //alGetError(); // clear any error messages
	 //if (alGetError() != AL_NO_ERROR) {
	 //	printf("- Error creating buffers !!\n");
	 //	exit(1);
	 //} else {
	 //	printf("init() - No errors yet.");
	 //}
	 //// Config source 0
	 //// Generate buffers, or else no sound will happen!
	 //alGenBuffers(NUM_BUFFERS, buffer);
	 //buffer[0] = alutCreateBufferFromFile("../sounds/fountain.wav");
	 //buffer[1] = alutCreateBufferFromFile("../sounds/fire.wav");
	 //buffer[2] = alutCreateBufferFromFile("../sounds/darth_vader.wav");
	 //int errorAlut = alutGetError();
	 //if (errorAlut != ALUT_ERROR_NO_ERROR) {
	 //	printf("- Error open files with alut %d !!\n", errorAlut);
	 //	exit(2);
	 //}

	 //alGetError(); /* clear error */
	 //alGenSources(NUM_SOURCES, source);

	 //if (alGetError() != AL_NO_ERROR) {
	 //	printf("- Error creating sources !!\n");
	 //	exit(2);
	 //} else {
	 //	printf("init - no errors after alGenSources\n");
	 //}
	 //alSourcef(source[0], AL_PITCH, 1.0f);
	 //alSourcef(source[0], AL_GAIN, 3.0f);
	 //alSourcefv(source[0], AL_POSITION, source0Pos);
	 //alSourcefv(source[0], AL_VELOCITY, source0Vel);
	 //alSourcei(source[0], AL_BUFFER, buffer[0]);
	 //alSourcei(source[0], AL_LOOPING, AL_TRUE);
	 //alSourcef(source[0], AL_MAX_DISTANCE, 2000);

	 //alSourcef(source[1], AL_PITCH, 1.0f);
	 //alSourcef(source[1], AL_GAIN, 3.0f);
	 //alSourcefv(source[1], AL_POSITION, source1Pos);
	 //alSourcefv(source[1], AL_VELOCITY, source1Vel);
	 //alSourcei(source[1], AL_BUFFER, buffer[1]);
	 //alSourcei(source[1], AL_LOOPING, AL_TRUE);
	 //alSourcef(source[1], AL_MAX_DISTANCE, 2000);

	 //alSourcef(source[2], AL_PITCH, 1.0f);
	 //alSourcef(source[2], AL_GAIN, 0.3f);
	 //alSourcefv(source[2], AL_POSITION, source2Pos);
	 //alSourcefv(source[2], AL_VELOCITY, source2Vel);
	 //alSourcei(source[2], AL_BUFFER, buffer[2]);
	 //alSourcei(source[2], AL_LOOPING, AL_TRUE);
	 //alSourcef(source[2], AL_MAX_DISTANCE, 500);

	 // Se inicializa el modelo de texeles.
	modelText = new FontTypeRendering::FontTypeRendering(screenWidth,
		screenHeight);
	modelText->Initialize();

	modelText2 = new FontTypeRendering::FontTypeRendering(screenWidth,
		screenHeight);
	modelText2->Initialize();
}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Shaders Delete
	shader.destroy();
	shaderMulLighting.destroy();
	shaderSkybox.destroy();
	shaderTerrain.destroy();
	shaderParticlesFountain.destroy();
	shaderParticlesFire.destroy();

	// Basic objects Delete
	skyboxSphere.destroy();
	boxCollider.destroy();
	sphereCollider.destroy();
	boxViewDepth.destroy();
	boxLightViewBox.destroy();

	// Terrains objects Delete
	terrain.destroy();

	// Custom objects Delete
	modelFountain.destroy();
	pivoteCam.destroy();
	astroProta.destroy();
	muroDerecho.destroy();
	muroFondo.destroy();
	muroFrontal.destroy();
	muroIzquierdo.destroy();
	modelBotones.destroy();
	modelCompuerta.destroy();
	modelEdCompuerta.destroy();
	modelGenerador.destroy();
	modelPlataforma.destroy();
	modelPlaCompuerta.destroy();
	modelRokas.destroy();
	modelLuzBotones.destroy();
	modelLuzGenerador.destroy();
	boxReferencia.destroy();
	boxMenu.destroy();

	// Custom objects animate
	mayowModelAnimate.destroy();

	//modelos segundo escenario
	modelEscenario2.destroy();

	// Textures Delete
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textureBlueID);
	glDeleteTextures(1, &textureGreenID);
	glDeleteTextures(1, &textureOrangeID);
	glDeleteTextures(1, &texturePurpleID);
	glDeleteTextures(1, &textureRedID);
	glDeleteTextures(1, &textureYellowID);
	glDeleteTextures(1, &textureTerrainBackgroundID);
	glDeleteTextures(1, &textureTerrainRID);
	glDeleteTextures(1, &textureTerrainGID);
	glDeleteTextures(1, &textureTerrainBID);
	glDeleteTextures(1, &textureTerrainBlendMapID);
	glDeleteTextures(1, &textureTerrainBlendMapID2);
	glDeleteTextures(1, &textureParticleFountainID);
	glDeleteTextures(1, &textureParticleFireID);

	// Cube Maps Delete
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDeleteTextures(1, &skyboxTextureID);

	// Remove the buffer of the fountain particles
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &initVel);
	glDeleteBuffers(1, &startTime);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAOParticles);

	// Remove the buffer of the fire particles
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(2, posBuf);
	glDeleteBuffers(2, velBuf);
	glDeleteBuffers(2, age);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	glDeleteTransformFeedbacks(2, feedback);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAOParticlesFire);
}

void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action,
	int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	distanceFromTarget -= yoffset;
	camera->setDistanceFromTarget(distanceFromTarget);
	lastDistanceFromTarget = distanceFromTarget;
}

void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			std::cout << "distanceFromTarget:" << lastDistanceFromTarget << std::endl;
			std::cout << "angulo rotacion yaw:" << yaw << std::endl;
			std::cout << "pitch:" << camera->getPitch() << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}
	if (!empiezaJuego) {
		bool pressEnter = glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS;
		if (textureActivaID == textureMenuID && pressEnter)
			empiezaJuego = true;
			fontbandera = true;
	}

	enemigo1.setMatrixEnemigo(modelMatrixMayow);


	if (glfwJoystickPresent(GLFW_JOYSTICK_1) == GL_TRUE) {
		std::cout << "Esta presente el joystick" << std::endl;
		int axesCount, buttonCount;
		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
		std::cout << "Número de ejes disponibles :=>" << axesCount << std::endl;
		std::cout << "Left Stick X axis: " << axes[0] << std::endl;
		std::cout << "Left Stick Y axis: " << axes[1] << std::endl;
		std::cout << "Left Trigger/L2: " << axes[2] << std::endl;
		std::cout << "Right Stick X axis: " << axes[3] << std::endl;
		std::cout << "Right Stick Y axis: " << axes[4] << std::endl;
		std::cout << "Right Trigger/R2: " << axes[5] << std::endl;

		if (fabs(axes[1]) > 0.2) {
			modelMatrixMayow = glm::translate(modelMatrixMayow,
				glm::vec3(0, 0, -axes[1] * 0.1));
			animationIndex = 0;
		}
		if (fabs(axes[0]) > 0.2) {
			modelMatrixMayow = glm::rotate(modelMatrixMayow,
				glm::radians(-axes[0] * 0.5f), glm::vec3(0, 1, 0));
			animationIndex = 0;
		}

		if (fabs(axes[3]) > 0.2) {
			camera->mouseMoveCamera(axes[3], 0.0, deltaTime);
		}
		if (fabs(axes[4]) > 0.2) {
			camera->mouseMoveCamera(0.0, axes[4], deltaTime);
		}

		const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1,
			&buttonCount);
		std::cout << "Número de botones disponibles :=>" << buttonCount
			<< std::endl;
		if (buttons[0] == GLFW_PRESS)
			std::cout << "Se presiona x" << std::endl;

		if (!isJump && buttons[0] == GLFW_PRESS) {
			isJump = true;
			startTimeJump = currTime;
			tmv = 0;
		}
	}

	if (cameraSelected == 1) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraFP->moveFrontCamera(true, deltaTime + 0.5);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameraFP->moveFrontCamera(false, deltaTime + 0.5);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameraFP->moveRightCamera(false, deltaTime + 0.5);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraFP->moveRightCamera(true, deltaTime + 0.5);
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			cameraFP->mouseMoveCamera(offsetX, offsetY, deltaTime);
	}
	else {
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			camera->mouseMoveCamera(offsetX, 0.0, deltaTime);
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			camera->mouseMoveCamera(0.0, offsetY, deltaTime);
	}
	offsetX = 0;
	offsetY = 0;


	if (enableCameraSelected && glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS
		&& (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS
			|| glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)) {
		enableCameraSelected = false;
		cameraSelected++;
		if (cameraSelected > 1)
			cameraSelected = 0;
		std::cout << "cameraSelected:" << cameraSelected << std::endl;
	}
	else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE
		|| (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE
			&& glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_RELEASE))
		enableCameraSelected = true;

	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
		modelMatrixPivoteCam = glm::rotate(modelMatrixPivoteCam, glm::radians(1.0f),
			glm::vec3(0, 0, 1));
		yaw += 1;
	}

	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		modelMatrixPivoteCam = glm::rotate(modelMatrixPivoteCam, glm::radians(-1.0f),
			glm::vec3(0, 0, 1));
		yaw -= 1;
	}
	//std::cout << "respaw empieza el juego:" << (playerRespawn == false && empiezaJuego == true) << std::endl;
	//std::cout << "respaw empieza el cambia nivel:" << (playerRespawn == false && cambianivel2 == true) << std::endl;
	//std::cout << "respaw empieza el cambia nivel 2:" << (playerRespawn == false && cambianivel3 == true) << std::endl;
	if ((playerRespawn == false && empiezaJuego == true) || (playerRespawn == false && cambianivel2 == true) || (playerRespawn == false && cambianivel3 == true)) {
		//std::cout << "primer if:" << actionE << std::endl;
		if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			modelMatrixAstroProta = glm::rotate(modelMatrixAstroProta, glm::radians(3.5f), glm::vec3(0, 1, 0));

		if (enableAction && glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			enableAction = false;
			actionE = true;
			animationIndex = 2;
			std::cout << "actionE:" << actionE << std::endl;
		}
		else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE) {
			enableAction = true;
			actionE = false;
		}

		if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			modelMatrixAstroProta = glm::rotate(modelMatrixAstroProta, glm::radians(3.5f),
				glm::vec3(0, 1, 0));
			animationIndex = 0;
			astroPosition = modelMatrixAstroProta[3];
			enemigo1.setDistance(enemigo1.distanciaAProta(modelMatrixMayow[3], modelMatrixAstroProta[3]));
			//anguloEntreDosVectores = enemigo1.anguloEntreVectores(modelMatrixAstroProta[3], modelMatrixMayow[3]);

		}
		else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			modelMatrixAstroProta = glm::rotate(modelMatrixAstroProta, glm::radians(-3.5f),
				glm::vec3(0, 1, 0));
			animationIndex = 0;
			astroPosition = modelMatrixAstroProta[3];
			enemigo1.setDistance(enemigo1.distanciaAProta(modelMatrixMayow[3], modelMatrixAstroProta[3]));
			//anguloEntreDosVectores = enemigo1.anguloEntreVectores(modelMatrixAstroProta[3], modelMatrixMayow[3]);
		}
		if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			pasado = terrain.getXCoordTerrain(modelMatrixAstroProta[3][0]);
			modelMatrixAstroProta = glm::translate(modelMatrixAstroProta,
				glm::vec3(0.0, 0.0, 0.1));
			animationIndex = 0;
			timer += 0.01f;
			std::cout << timer << std::endl;
			if (timer > 0.1f) 
				banderaCaminar = 1;
			if (timer > 5.0f && banderaCaminar == 1)
				banderaCaminar = 2;
			if (timer > 8.0f && banderaCaminar == 2) 
				banderaCaminar = 3;
			if (timer > 12.0f && banderaCaminar == 3)
				banderaCaminar = 4;
			if (timer > 16.0f && banderaCaminar == 4)
				banderaCaminar = 5;
			if (timer > 20.0f && banderaCaminar == 5)
				banderaCaminar = 6;
			if (timer > 23.0f && banderaCaminar == 6)
				banderaCaminar = 7;
			if (timer > 26.0f && banderaCaminar == 7)
				banderaCaminar = 8;
			if (timer > 29.0f && banderaCaminar == 8)
				banderaCaminar = 9;
			if (timer > 32.0f && banderaCaminar == 9)
				banderaCaminar = 10;
			if (timer > 35.0f && banderaCaminar == 10)
				banderaCaminar = 11;
			//std::cout << banderaCaminar << std::endl;
			//std::cout << "modelMatrixPivote.x: " << terrain.getXCoordTerrain(modelMatrixPivoteCam[3][0]) << std::endl;
			//std::cout << "modelMatrixAstro.x: " << terrain.getXCoordTerrain(modelMatrixAstroProta[3][0]) << std::endl;


			cameraMove();
			astroPosition = modelMatrixAstroProta[3];
			enemigo1.setDistance(enemigo1.distanciaAProta(modelMatrixMayow[3], modelMatrixAstroProta[3]));

			animationIndex = 0;

			//std::cout << "modelMatrixPivote.x: " << terrain.getXCoordTerrain(modelMatrixPivoteCam[3][0]) << std::endl;
			//std::cout << "modelMatrixAstro.x: " << terrain.getXCoordTerrain(modelMatrixAstroProta[3][0]) << std::endl;



		}
		else if (modelSelected
			== 2 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			pasado = terrain.getXCoordTerrain(modelMatrixAstroProta[3][0]);
			modelMatrixAstroProta = glm::translate(modelMatrixAstroProta,
				glm::vec3(0.0, 0.0, -0.1));
			animationIndex = 0;
			//std::cout << "modelMatrixPivote.x: " << terrain.getXCoordTerrain(modelMatrixPivoteCam[3][0]) << std::endl;
			//std::cout << "modelMatrixAstro.x: " << terrain.getXCoordTerrain(modelMatrixAstroProta[3][0]) << std::endl;
			cameraMove();
			astroPosition = modelMatrixAstroProta[3];
			enemigo1.setDistance(enemigo1.distanciaAProta(modelMatrixMayow[3], modelMatrixAstroProta[3]));
			//anguloEntreDosVectores = enemigo1.anguloEntreVectores(modelMatrixAstroProta[3], modelMatrixMayow[3]);


		/*std::cout << "modelMatrixPivote.x: " << terrain.getXCoordTerrain(modelMatrixPivoteCam[3][0])<< std::endl;
		std::cout << "modelMatrixAstro.x: " << terrain.getXCoordTerrain(modelMatrixAstroProta[3][0]) << std::endl;*/
			cameraMove();
			astroPosition = modelMatrixAstroProta[3];
			enemigo1.setDistance(enemigo1.distanciaAProta(modelMatrixMayow[3], modelMatrixAstroProta[3]));
			//anguloEntreDosVectores = enemigo1.anguloEntreVectores(modelMatrixAstroProta[3], modelMatrixMayow[3]);

				//std::cout << "modelMatrixPivote: " << modelMatrixPivoteCam[3][0] << std::endl;
				//std::cout << "modelMatrixMayow: " << modelMatrixMayow[3][0] << std::endl;
				//std::cout << "position mayow: " << terrain.getXCoordTerrain(modelMatrixMayow[3][0]) << std::endl;
		}

	}

	//************************INTERACCIONES DE COLLIDERS ENEMIGO JUGADOR ******************************/

	vectorDireccionEnemigo = enemigo1.calcularDireccionDeMovimiento(astroPosition, modelMatrixMayow[3]);



	if (playerRespawn == true) {
		modelMatrixAstroProta = defaultMatrix;
		modelMatrixAstroProta = glm::translate(modelMatrixAstroProta, astroOrigin);
		modelMatrixAstroProta = defaultMatrix2;
		modelMatrixPivoteCam = glm::translate(modelMatrixAstroProta, camPivOrigin);
		modelMatrixPivoteCam = glm::translate(modelMatrixPivoteCam,
			glm::vec3(0.0f, 5.0f, 23.0f));
		modelMatrixPivoteCam = glm::rotate(modelMatrixPivoteCam, glm::radians(-180.0f),
			glm::vec3(0, 0, 1));
		empiezaJuego = false;
		tiempoRespawnProta++;
		if (tiempoRespawnProta > 50) {
			playerRespawn = false;
			tiempoRespawnProta = 0;
		}

	}

	if (enemigo1.cercaDeProta(enemigo1.distanceToPersonaje) == true)
		enemigo1.velocidad = 0.05 * 1.2f;
	else if (enemigo1.cercaDeProta(enemigo1.distanceToPersonaje) == false)
		enemigo1.velocidad = 0.05 * 1.5f;

	if (enemigo1.respawn == true) {
		/*modelMatrixMayow = glm::translate(modelMatrixMayow,
			glm::vec3(0.0f, 0.0f, 0.0f));*/
		tiempo += 1;


		if (tiempo > 150) {
			enemigo1.respawn = false;
			tiempo = 0.0f;
		}

	}
	else if (enemigo1.respawn == false) {
		//modelMatrixMayow = glm::translate(modelMatrixMayow, vectorDireccionEnemigo * enemigo1.velocidad);
		//modelMatrixMayow = glm::rotate(modelMatrixMayow, enemigo1.faceDirection(vectorDireccionEnemigo), glm::vec3(0,1,0));
	}

	/******************************************************************************/

	bool keySpaceStatus = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
	if (!isJump && keySpaceStatus) {
		isJump = true;
		startTimeJump = currTime;
		tmv = 0;
	}


	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {

	bool psi = true;
	glm::mat4 view;
	glm::vec3 axis;
	glm::vec3 target;
	float angleTarget;

	inicialMatrixs();

	lastTime = TimeManager::Instance().GetTime();

	// Time for the particles animation
	currTimeParticlesAnimation = lastTime;
	lastTimeParticlesAnimation = lastTime;

	currTimeParticlesAnimationFire = lastTime;
	lastTimeParticlesAnimationFire = lastTime;

	glm::vec3 lightPos = glm::vec3(10.0, 10.0, 0.0);

	shadowBox = new ShadowBox(-lightPos, camera.get(), 15.0f, 0.1f, 45.0f);

	//Asignacion de valor de textura
	textureActivaID = textureMenuID;

	while (psi) {
		currTime = TimeManager::Instance().GetTime();
		if (currTime - lastTime < 0.016666667) {
			glfwPollEvents();
			continue;
		}
		lastTime = currTime;
		TimeManager::Instance().CalculateFrameRate(true);
		deltaTime = TimeManager::Instance().DeltaTime;
		psi = processInput(true);

		if (cameraSelected == 0) {
			axis = glm::axis(glm::quat_cast(modelMatrixPivoteCam));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixPivoteCam));
			target = modelMatrixPivoteCam[3];
			if (modelSelected == 1)
				angleTarget -= glm::radians(90.0);

			if (std::isnan(angleTarget))
				angleTarget = 0.0;
			if (axis.y < 0)
				angleTarget = -angleTarget;
			camera->setCameraTarget(target);
			camera->setAngleTarget(angleTarget);

			camera->updateCamera();
			view = camera->getViewMatrix();

		}
		else {
			view = cameraFP->getViewMatrix();
		}

		if (escenario1) {
			updateEscenario1();
			lucesEscenari1(shadowBox, &view);
			/*if (!empiezaJuego) {
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glViewport(0, 0, screenWidth, screenHeight);
				shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
				shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureActivaID);
				shaderTexture.setInt("outTexture", 0);
				boxMenu.render();
				glfwSwapBuffers(window);
				continue;
			}*/
			empiezaJuego = true;
			preRender1();
			renderScene();
			collidersManagmentEs1();
			soundEscene1();
		}

		if (escenario2) {
			//updateEscenario2();
			lucesEscenari2(shadowBox, &view);
			/*if (!empiezaJuego) {
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glViewport(0, 0, screenWidth, screenHeight);
				shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
				shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureActivaID);
				shaderTexture.setInt("outTexture", 0);
				boxMenu.render();
				glfwSwapBuffers(window);
				continue;
			}*/
			empiezaJuego = true;
			preRender2();
			renderScene2();
			collidersManagmentEs2();
			soundEscene2();
		}

		/*******************************************
		 * Debug to box light box
		 *******************************************/
		 /*glm::vec3 front = glm::normalize(-lightPos);
		  glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), front));
		  glm::vec3 up = glm::normalize(glm::cross(front, right));
		  glDisable(GL_CULL_FACE);
		  glm::mat4 boxViewTransform = glm::mat4(1.0f);
		  boxViewTransform = glm::translate(boxViewTransform, centerBox);
		  boxViewTransform[0] = glm::vec4(right, 0.0);
		  boxViewTransform[1] = glm::vec4(up, 0.0);
		  boxViewTransform[2] = glm::vec4(front, 0.0);
		  boxViewTransform = glm::scale(boxViewTransform, glm::vec3(shadowBox->getWidth(), shadowBox->getHeight(), shadowBox->getLength()));
		  boxLightViewBox.enableWireMode();
		  boxLightViewBox.render(boxViewTransform);
		  glEnable(GL_CULL_FACE);*/

		animationIndex = 1;

		/*******************************************
		 * State machines
		 *******************************************/

		 // State machine for the lambo car
		 /*switch (stateDoor) {
		 case 0:
			 dorRotCount += 0.5;
			 if (dorRotCount > 75)
				 stateDoor = 1;
			 break;
		 case 1:
			 dorRotCount -= 0.5;
			 if (dorRotCount < 0) {
				 dorRotCount = 0.0;
				 stateDoor = 0;
			 }
			 break;
		 }*/

		if (fontbandera == true) {
			modelText->render("Oxigeno", 0.7, 0.95, 15, 0.5, 1.0, 1.0, 1.0);
			//std::cout << situacion << std::endl;
			switch (situacion)
			{
			case 1:
				if (banderaCaminar == 1)
					modelText2->render("100%", 0.7, 0.9, 12, 0.0, 1.0, 0.0, 1.0);
				if (banderaCaminar == 2)
					modelText2->render("90%", 0.7, 0.9, 12, 0.0, 1.0, 0.0, 1.0);
				if (banderaCaminar == 3)
					modelText2->render("80%", 0.7, 0.9, 12, 0.0, 1.0, 0.0, 1.0);
				if (banderaCaminar == 4)
					modelText2->render("70%", 0.7, 0.9, 12, 1.0, 1.0, 0.0, 1.0);
				if (banderaCaminar == 5)
					modelText2->render("60%", 0.7, 0.9, 12, 1.0, 1.0, 0.0, 1.0);
				if (banderaCaminar == 6)
					modelText2->render("50%", 0.7, 0.9, 12, 1.0, 1.0, 0.0, 1.0);
				if (banderaCaminar == 7)
					modelText2->render("40%", 0.7, 0.9, 12, 1.0, 0.5, 0.0, 1.0);
				if (banderaCaminar == 8)
					modelText2->render("30%", 0.7, 0.9, 12, 1.0, 0.5, 0.0, 1.0);
				if (banderaCaminar == 9)
					modelText2->render("20%", 0.7, 0.9, 12, 1.0, 0.5, 0.0, 1.0);
				if (banderaCaminar == 10)
					modelText2->render("10%", 0.7, 0.9, 12, 1.0, 0.0, 0.0, 1.0);
				if (banderaCaminar == 11) {
					modelText2->render("0%", 0.7, 0.9, 12, 1.0, 0.0, 0.0, 1.0);
					situacion = 2;
				}
				break;
			case 2:
				modelText2->render("Te quedaste sin oxigeno!", -0.5, 0, 30, 1.0, 0.0, 0.0, 1.0);			
				tiempoOxigeno++;
				if (tiempoOxigeno > 250){
					situacion = 3;
				}
				break;
			case 3:	
				playerRespawn2 = true;
				if (playerRespawn2 == true) {
					modelMatrixAstroProta = defaultMatrix;
					modelMatrixAstroProta = glm::translate(modelMatrixAstroProta, astroOrigin);
					modelMatrixPivoteCam = glm::translate(modelMatrixAstroProta, camPivOrigin);
					modelMatrixPivoteCam = glm::translate(modelMatrixPivoteCam,
						glm::vec3(0.0f, 5.0f, 23.0f));
					modelMatrixPivoteCam = glm::rotate(modelMatrixPivoteCam, glm::radians(-180.0f),
						glm::vec3(0, 0, 1));
					//linea que genera problemas
					//empiezaJuego = false; 
					tiempoRespawnProta++;
					if (tiempoRespawnProta > 50) {						
						playerRespawn2 = false;
						tiempoRespawnProta = 0;
						timer = 0.0f;
						banderaCaminar = 0;
						tiempoOxigeno = 0;
						situacion = 1;
					}
				}
				break;
			}
		}
		glfwSwapBuffers(window);
	}
}

void prepareScene() {


	skyboxSphere.setShader(&shaderSkybox);

	terrain.setShader(&shaderTerrain);

	//Mayow
	mayowModelAnimate.setShader(&shaderMulLighting);

	pivoteCam.setShader(&shaderMulLighting);

	astroProta.setShader(&shaderMulLighting);

	muroFondo.setShader(&shaderMulLighting);

	muroFrontal.setShader(&shaderMulLighting);

	muroDerecho.setShader(&shaderMulLighting);

	muroIzquierdo.setShader(&shaderMulLighting);

	modelBotones.setShader(&shaderMulLighting);

	modelCompuerta.setShader(&shaderMulLighting);

	modelEdCompuerta.setShader(&shaderMulLighting);

	modelGenerador.setShader(&shaderMulLighting);

	modelPlataforma.setShader(&shaderMulLighting);

	modelPlaCompuerta.setShader(&shaderMulLighting);

	modelRokas.setShader(&shaderMulLighting);

	boxReferencia.setShader(&shaderMulLighting);

	modelLuzBotones.setShader(&shaderMulLighting);

	modelLuzGenerador.setShader(&shaderMulLighting);
}

void prepareDepthScene() {

	skyboxSphere.setShader(&shaderDepth);

	terrain.setShader(&shaderDepth);

	//Mayow
	mayowModelAnimate.setShader(&shaderDepth);

	pivoteCam.setShader(&shaderDepth);

	astroProta.setShader(&shaderDepth);

	muroFondo.setShader(&shaderDepth);

	muroFrontal.setShader(&shaderDepth);

	muroDerecho.setShader(&shaderDepth);

	muroIzquierdo.setShader(&shaderDepth);

	modelBotones.setShader(&shaderDepth);

	modelCompuerta.setShader(&shaderDepth);

	modelEdCompuerta.setShader(&shaderDepth);

	modelGenerador.setShader(&shaderDepth);

	modelPlataforma.setShader(&shaderDepth);

	modelPlaCompuerta.setShader(&shaderDepth);

	modelRokas.setShader(&shaderDepth);

	boxReferencia.setShader(&shaderDepth);

	modelLuzBotones.setShader(&shaderDepth);

	modelLuzGenerador.setShader(&shaderDepth);
}

void renderScene(bool renderParticles) {
	/*******************************************
	 * Terrain Cesped
	 *******************************************/
	glm::mat4 modelCesped = glm::mat4(1.0);
	modelCesped = glm::translate(modelCesped, glm::vec3(0.0, 0.0, 0.0));
	modelCesped = glm::scale(modelCesped, glm::vec3(200.0, 0.001, 200.0));
	// Se activa la textura del background
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
	shaderTerrain.setInt("backgroundTexture", 0);
	// Se activa la textura de tierra
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
	shaderTerrain.setInt("rTexture", 1);
	// Se activa la textura de hierba
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
	shaderTerrain.setInt("gTexture", 2);
	// Se activa la textura del camino
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
	shaderTerrain.setInt("bTexture", 3);
	// Se activa la textura del blend map
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
	shaderTerrain.setInt("blendMapTexture", 4);
	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(40, 40)));
	terrain.render();
	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
	glBindTexture(GL_TEXTURE_2D, 0);

	//Muros
	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, textureYellowID);
	//muroFondo.render(modelMatrixMuroFondo);

	//muroFrontal.render(modelMatrixMuroFrontal);

	//muroDerecho.render(modelMatrixMuroDerecho);

	//muroIzquierdo.render(modelMatrixMuroIzquierdo);
	glBindTexture(GL_TEXTURE_2D, 0);

	/*******************************************
	 * Custom objects obj
	 *******************************************/
	 // Forze to enable the unit texture to 0 always ----------------- IMPORTANT
	glActiveTexture(GL_TEXTURE0);

	// Pivote cam
	glDisable(GL_CULL_FACE);
	//pivoteCam.render(modelMatrixPivoteCam);
	//muroFondo.render(modelMatrixMuroFondo);
	glEnable(GL_CULL_FACE);

	/*******************************************
	 * Custom Anim objects obj
	 *******************************************/

	/*
	glm::vec3 ejey = glm::normalize(terrain.getNormalTerrain(modelMatrixCompuerta[3][0], modelMatrixCompuerta[3][2]));
	glm::vec3 ejex = glm::normalize(modelMatrixCompuerta[0]);
	glm::vec3 ejez = glm::normalize(glm::cross(ejex, ejey));
	ejex = glm::normalize(glm::cross(ejey, ejez));
	modelMatrixCompuerta[0] = glm::vec4(ejex, 0.0);
	modelMatrixCompuerta[1] = glm::vec4(ejey, 0.0);
	modelMatrixCompuerta[2] = glm::vec4(ejez, 0.0);
	modelMatrixCompuerta[3][1] = terrain.getHeightTerrain(modelMatrixCompuerta[3][0], modelMatrixCompuerta[3][2]);*/

	//glDisable(GL_BLEND);
	modelMatrixCompuerta[3][1] = terrain.getHeightTerrain(modelMatrixCompuerta[3][0],
		modelMatrixCompuerta[3][2]) + 2.0f;
	glm::mat4 modelMatrixCompuertaBody = glm::mat4(modelMatrixCompuerta);
	modelMatrixCompuertaBody = glm::scale(modelMatrixCompuertaBody,
		glm::vec3(0.0021, 0.0021, 0.0021));
	modelCompuerta.setAnimationIndex(1);
	modelCompuerta.render(modelMatrixCompuertaBody);


	//modelMatrixPivoteCam = glm::rotate(modelMatrixPivoteCam, glm::radians(-180.0f),
	//	glm::vec3(0, 0, 1));

	for (int i = 0; i < botonesPos.size(); i++) {
		botonesPos[i].y = terrain.getHeightTerrain(botonesPos[i].x,
			botonesPos[i].z) + 1.25f;
		modelBotones.setPosition(botonesPos[i]);
		modelBotones.setScale(glm::vec3(1.5, 1.0, 1.0));
		modelBotones.setOrientation(glm::vec3(-90.0, -180.0, 0));
		boxReferencia.setPosition(glm::vec3(botonesPos[i].x,
			botonesPos[i].y + 1.5, botonesPos[i].z));
		modelLuzBotones.setPosition(glm::vec3(botonesPos[i].x,
			botonesPos[i].y + 0.6, botonesPos[i].z + 1.1));
		modelLuzBotones.setScale(glm::vec3(1.5, 2.0, 1.0));
		if (i == 0) {
			glBindTexture(GL_TEXTURE_2D, textureYellowID);
			boxReferencia.render();
		}
		if (i == 1) {
			glBindTexture(GL_TEXTURE_2D, textureBlueID);
			boxReferencia.render();
		}
		if (i == 2) {
			glBindTexture(GL_TEXTURE_2D, textureOrangeID);
			boxReferencia.render();
		}
		if (i == 3) {
			glBindTexture(GL_TEXTURE_2D, textureGreenID);
			boxReferencia.render();
		}
		modelLuzBotones.render();
		modelBotones.render();
	}

	glActiveTexture(GL_TEXTURE0);

	for (int i = 0; i < botonesPos.size(); i++) {
		modelGenerador.setPosition(glm::vec3(botonesPos[i].x + 2.65,
			terrain.getHeightTerrain(botonesPos[i].x + 2.65,
				botonesPos[i].z) + 1.0f, botonesPos[i].z));
		modelGenerador.setScale(glm::vec3(1.0, 1.0, 1.0));
		modelGenerador.setOrientation(glm::vec3(0.0, 180.0, 0));
		//modelBotones.setOrientation(glm::vec3(0, lamp2Orientation[i], 0));
		modelGenerador.render();
	}


	for (int i = 0; i < botonesPos.size(); i++) {
		modelLuzGenerador.setPosition(glm::vec3(botonesPos[i].x + 3.9,
			terrain.getHeightTerrain(botonesPos[i].x + 3.9,
				botonesPos[i].z + 1.4) , botonesPos[i].z + 1.4));
		modelLuzGenerador.setScale(glm::vec3(1.0, 1.0, 1.0));
		modelLuzGenerador.setOrientation(glm::vec3(0.0, 140.0, 0.0));
		//modelBotones.setOrientation(glm::vec3(0, lamp2Orientation[i], 0));
		modelLuzGenerador.render();
	}

	for (int i = 0; i < rokasPos.size(); i++) {
		rokasPos[i].y = terrain.getHeightTerrain(rokasPos[i].x,
			rokasPos[i].z) + 1.25f;
		modelRokas.setPosition(rokasPos[i]);
		modelRokas.setScale(glm::vec3(1.5, 1.0, 1.0));
		modelRokas.setOrientation(glm::vec3(-90.0, -180.0, 0));
		//modelBotones.setOrientation(glm::vec3(0, lamp2Orientation[i], 0));
		//modelRokas.render();
	}

	modelPlataforma.render(modelMatrixPlataforma);

	modelEdCompuerta.render(modelMatrixEdCompuerta);

	modelPlaCompuerta.render(modelMatrixPlaCompuerta);

	//astroProta
	glDisable(GL_CULL_FACE);
	modelMatrixAstroProta[3][1] = terrain.getHeightTerrain(modelMatrixAstroProta[3][0],
		modelMatrixAstroProta[3][2]);
	glm::mat4 modelMatrixAstroBody = glm::mat4(modelMatrixAstroProta);
	modelMatrixAstroBody = glm::scale(modelMatrixAstroBody,
		glm::vec3(0.021, 0.021, 0.021));
	astroProta.setAnimationIndex(animationIndex);
	astroProta.render(modelMatrixAstroBody);
	glEnable(GL_CULL_FACE);

	///**********
	// * Sorter with alpha objects
	// */
	//std::map<float, std::pair<std::string, glm::vec3>> blendingSorted;
	//std::map<std::string, glm::vec3>::iterator itblend;
	//for (itblend = blendingUnsorted.begin(); itblend != blendingUnsorted.end();
	//		itblend++) {
	//	float distanceFromView = glm::length(
	//			camera->getPosition() - itblend->second);
	//	blendingSorted[distanceFromView] = std::make_pair(itblend->first,
	//			itblend->second);
	//}

	/**********
	 * Render de las transparencias
	 */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	//for (std::map<float, std::pair<std::string, glm::vec3> >::reverse_iterator it =
	//		blendingSorted.rbegin(); it != blendingSorted.rend(); it++) {
	//	if (it->second.first.compare("aircraft") == 0) {
	//		// Render for the aircraft model
	//		glm::mat4 modelMatrixAircraftBlend = glm::mat4(modelMatrixAircraft);
	//		modelMatrixAircraftBlend[3][1] = terrain.getHeightTerrain(
	//				modelMatrixAircraftBlend[3][0],
	//				modelMatrixAircraftBlend[3][2]) + 2.0;
	//		modelAircraft.render(modelMatrixAircraftBlend);
	//	} else if (it->second.first.compare("lambo") == 0) {
	//		// Lambo car
	//		glm::mat4 modelMatrixLamboBlend = glm::mat4(modelMatrixLambo);
	//		modelMatrixLamboBlend[3][1] = terrain.getHeightTerrain(
	//				modelMatrixLamboBlend[3][0], modelMatrixLamboBlend[3][2]);
	//		modelMatrixLamboBlend = glm::scale(modelMatrixLamboBlend,
	//				glm::vec3(1.3, 1.3, 1.3));
	//		modelLambo.render(modelMatrixLamboBlend);
	//		glActiveTexture(GL_TEXTURE0);
	//		glm::mat4 modelMatrixLamboLeftDor = glm::mat4(
	//				modelMatrixLamboBlend);
	//		modelMatrixLamboLeftDor = glm::translate(modelMatrixLamboLeftDor,
	//				glm::vec3(1.08676, 0.707316, 0.982601));
	//		modelMatrixLamboLeftDor = glm::rotate(modelMatrixLamboLeftDor,
	//				glm::radians(dorRotCount), glm::vec3(1.0, 0, 0));
	//		modelMatrixLamboLeftDor = glm::translate(modelMatrixLamboLeftDor,
	//				glm::vec3(-1.08676, -0.707316, -0.982601));
	//		modelLamboLeftDor.render(modelMatrixLamboLeftDor);
	//		modelLamboRightDor.render(modelMatrixLamboBlend);
	//		modelLamboFrontLeftWheel.render(modelMatrixLamboBlend);
	//		modelLamboFrontRightWheel.render(modelMatrixLamboBlend);
	//		modelLamboRearLeftWheel.render(modelMatrixLamboBlend);
	//		modelLamboRearRightWheel.render(modelMatrixLamboBlend);
	//		// Se regresa el cull faces IMPORTANTE para las puertas
	//	} else if (it->second.first.compare("heli") == 0) {
	//		// Helicopter
	//		glm::mat4 modelMatrixHeliChasis = glm::mat4(modelMatrixHeli);
	//		modelHeliChasis.render(modelMatrixHeliChasis);

	//		glm::mat4 modelMatrixHeliHeli = glm::mat4(modelMatrixHeliChasis);
	//		modelMatrixHeliHeli = glm::translate(modelMatrixHeliHeli,
	//				glm::vec3(0.0, 0.0, -0.249548));
	//		modelMatrixHeliHeli = glm::rotate(modelMatrixHeliHeli, rotHelHelY,
	//				glm::vec3(0, 1, 0));
	//		modelMatrixHeliHeli = glm::translate(modelMatrixHeliHeli,
	//				glm::vec3(0.0, 0.0, 0.249548));
	//		modelHeliHeli.render(modelMatrixHeliHeli);
	//	} else if (renderParticles
	//			&& it->second.first.compare("fountain") == 0) {
	//		/**********
	//		 * Init Render particles systems
	//		 */
	//		glm::mat4 modelMatrixParticlesFountain = glm::mat4(1.0);
	//		modelMatrixParticlesFountain = glm::translate(
	//				modelMatrixParticlesFountain, it->second.second);
	//		modelMatrixParticlesFountain[3][1] = terrain.getHeightTerrain(
	//				modelMatrixParticlesFountain[3][0],
	//				modelMatrixParticlesFountain[3][2]) + 0.36 * 10.0;
	//		modelMatrixParticlesFountain = glm::scale(
	//				modelMatrixParticlesFountain, glm::vec3(3.0, 3.0, 3.0));
	//		currTimeParticlesAnimation = TimeManager::Instance().GetTime();
	//		if (currTimeParticlesAnimation - lastTimeParticlesAnimation > 10.0)
	//			lastTimeParticlesAnimation = currTimeParticlesAnimation;
	//		//glDisable(GL_DEPTH_TEST);
	//		glDepthMask(GL_FALSE);
	//		// Set the point size
	//		glPointSize(10.0f);
	//		glActiveTexture(GL_TEXTURE0);
	//		glBindTexture(GL_TEXTURE_2D, textureParticleFountainID);
	//		shaderParticlesFountain.turnOn();
	//		shaderParticlesFountain.setFloat("Time",
	//				float(
	//						currTimeParticlesAnimation
	//								- lastTimeParticlesAnimation));
	//		shaderParticlesFountain.setFloat("ParticleLifetime", 3.5f);
	//		shaderParticlesFountain.setInt("ParticleTex", 0);
	//		shaderParticlesFountain.setVectorFloat3("Gravity",
	//				glm::value_ptr(glm::vec3(0.0f, -0.3f, 0.0f)));
	//		shaderParticlesFountain.setMatrix4("model", 1, false,
	//				glm::value_ptr(modelMatrixParticlesFountain));
	//		glBindVertexArray(VAOParticles);
	//		glDrawArrays(GL_POINTS, 0, nParticles);
	//		glDepthMask(GL_TRUE);
	//		//glEnable(GL_DEPTH_TEST);
	//		shaderParticlesFountain.turnOff();
	//		/**********
	//		 * End Render particles systems
	//		 */
	//	} else if (renderParticles && it->second.first.compare("fire") == 0) {
	//		/**********
	//		 * Init Render particles systems
	//		 */
	//		lastTimeParticlesAnimationFire = currTimeParticlesAnimationFire;
	//		currTimeParticlesAnimationFire = TimeManager::Instance().GetTime();

	//		shaderParticlesFire.setInt("Pass", 1);
	//		shaderParticlesFire.setFloat("Time",
	//				currTimeParticlesAnimationFire);
	//		shaderParticlesFire.setFloat("DeltaT",
	//				currTimeParticlesAnimationFire
	//						- lastTimeParticlesAnimationFire);

	//		glActiveTexture(GL_TEXTURE1);
	//		glBindTexture(GL_TEXTURE_1D, texId);
	//		glEnable(GL_RASTERIZER_DISCARD);
	//		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
	//		glBeginTransformFeedback(GL_POINTS);
	//		glBindVertexArray(particleArray[1 - drawBuf]);
	//		glVertexAttribDivisor(0, 0);
	//		glVertexAttribDivisor(1, 0);
	//		glVertexAttribDivisor(2, 0);
	//		glDrawArrays(GL_POINTS, 0, nParticlesFire);
	//		glEndTransformFeedback();
	//		glDisable(GL_RASTERIZER_DISCARD);

	//		shaderParticlesFire.setInt("Pass", 2);
	//		glm::mat4 modelFireParticles = glm::mat4(1.0);
	//		modelFireParticles = glm::translate(modelFireParticles,
	//				it->second.second);
	//		modelFireParticles[3][1] = terrain.getHeightTerrain(
	//				modelFireParticles[3][0], modelFireParticles[3][2]);
	//		shaderParticlesFire.setMatrix4("model", 1, false,
	//				glm::value_ptr(modelFireParticles));

	//		shaderParticlesFire.turnOn();
	//		glActiveTexture(GL_TEXTURE0);
	//		glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
	//		glDepthMask(GL_FALSE);
	//		glBindVertexArray(particleArray[drawBuf]);
	//		glVertexAttribDivisor(0, 1);
	//		glVertexAttribDivisor(1, 1);
	//		glVertexAttribDivisor(2, 1);
	//		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticlesFire);
	//		glBindVertexArray(0);
	//		glDepthMask(GL_TRUE);
	//		drawBuf = 1 - drawBuf;
	//		shaderParticlesFire.turnOff();

	//		/****************************+
	//		 * Open AL sound data
	//		 */
	//		source1Pos[0] = modelFireParticles[3].x;
	//		source1Pos[1] = modelFireParticles[3].y;
	//		source1Pos[2] = modelFireParticles[3].z;
	//		alSourcefv(source[1], AL_POSITION, source1Pos);

	//		/**********
	//		 * End Render particles systems
	//		 */
	//	}

	//}
	glEnable(GL_CULL_FACE);
}

void inicialMatrixs() {
	//Pivote de Camara
	modelMatrixPivoteCam = glm::translate(modelMatrixPivoteCam,
		glm::vec3(0.0f, 5.0f, 23.0f));
	modelMatrixPivoteCam = glm::rotate(modelMatrixPivoteCam, glm::radians(-180.0f),
		glm::vec3(0, 0, 1));

	//Enemigo
	modelMatrixMayow = glm::translate(modelMatrixMayow,
		glm::vec3(13.0f, 0.05f, -5.0f));

	//Matriz
	modelMatrixAstroProta = glm::translate(modelMatrixAstroProta,
		glm::vec3(0.0f, 0.0f, 0.0f));
	/*modelMatrixAstroProta = glm::rotate(modelMatrixAstroProta, glm::radians(-90.0f),
		glm::vec3(1, 0, 0));*/

	//Matriz
	modelMatrixAstroProta2 = glm::translate(modelMatrixAstroProta2,
		glm::vec3(0.0f, 0.0f, -20.0f));

	//Posicion de los muros escenario 1
	modelMatrixMuroFondo = glm::translate(modelMatrixMuroFondo,
		glm::vec3(0.0f, 0.0f, -10.0f));
	modelMatrixMuroFondo = glm::scale(modelMatrixMuroFondo,
		glm::vec3(75.0f, 20.0f, 0.0f));
	modelMatrixMuroFrontal = glm::translate(modelMatrixMuroFrontal,
		glm::vec3(0.0f, 0.0f, 28.0f));
	modelMatrixMuroFrontal = glm::scale(modelMatrixMuroFrontal,
		glm::vec3(75.0f, 20.0f, 0.0f));
	modelMatrixMuroDerecho = glm::translate(modelMatrixMuroDerecho,
		glm::vec3(85.0f, 0.0f, 15.0f));
	modelMatrixMuroDerecho = glm::scale(modelMatrixMuroDerecho,
		glm::vec3(0.0f, 20.0f, 70.0f));
	modelMatrixMuroIzquierdo = glm::translate(modelMatrixMuroIzquierdo,
		glm::vec3(-73.0f, 0.0f, 15.0f));
	modelMatrixMuroIzquierdo = glm::scale(modelMatrixMuroIzquierdo,
		glm::vec3(0.0f, 20.0f, 70.0f));

	modelMatrixCompuerta = glm::translate(modelMatrixCompuerta,
		glm::vec3(5.9f, 0.0f, 8.8f));

	//Model escenario 2
	modelMatrixEscenario2 = glm::translate(modelMatrixEscenario2,
		glm::vec3(28.0f, 0.0f, -2.0f));
	modelMatrixEscenario2 = glm::scale(modelMatrixEscenario2,
		glm::vec3(2.9f, 2.0f, 3.1f));
	modelMatrixEscenario2 = glm::rotate(modelMatrixEscenario2, glm::radians(-90.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	//Posicion de los muros escenario 2
	modelMatrixMuroFondo2 = glm::translate(modelMatrixMuroFondo2,
		glm::vec3(0.0f, 0.0f, -35.0f));
	modelMatrixMuroFondo2 = glm::scale(modelMatrixMuroFondo2,
		glm::vec3(75.0f, 20.0f, 0.0f));
	modelMatrixMuroFrontal2 = glm::translate(modelMatrixMuroFrontal2,
		glm::vec3(0.0f, 0.0f, -1.50f));
	modelMatrixMuroFrontal2 = glm::scale(modelMatrixMuroFrontal2,
		glm::vec3(75.0f, 20.0f, 0.0f));
	modelMatrixMuroDerecho2 = glm::translate(modelMatrixMuroDerecho2,
		glm::vec3(56.5f, 0.0f, 15.0f));
	modelMatrixMuroDerecho2 = glm::scale(modelMatrixMuroDerecho2,
		glm::vec3(0.0f, 20.0f, 70.0f));
	modelMatrixMuroIzquierdo2 = glm::translate(modelMatrixMuroIzquierdo2,
		glm::vec3(-60.0f, 0.0f, 15.0f));
	modelMatrixMuroIzquierdo2 = glm::scale(modelMatrixMuroIzquierdo2,
		glm::vec3(0.0f, 20.0f, 70.0f));


	//

}

void lucesEscenari1(ShadowBox* shadowBox, glm::mat4* view) {

	glm::vec3 lightPos = glm::vec3(10.0, 10.0, 0.0);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f),
		(float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

	shadowBox->update(screenWidth, screenHeight);
	glm::vec3 centerBox = shadowBox->getCenter();

	// Projection light shadow mapping
	glm::mat4 lightProjection = glm::mat4(1.0f), lightView = glm::mat4(
		1.0f);
	glm::mat4 lightSpaceMatrix;

	lightProjection[0][0] = 2.0f / shadowBox->getWidth();
	lightProjection[1][1] = 2.0f / shadowBox->getHeight();
	lightProjection[2][2] = -2.0f / shadowBox->getLength();
	lightProjection[3][3] = 1.0f;

	lightView = glm::lookAt(centerBox,
		centerBox + glm::normalize(-lightPos),
		glm::vec3(0.0, 1.0, 0.0));

	lightSpaceMatrix = lightProjection * lightView;
	shaderDepth.setMatrix4("lightSpaceMatrix", 1, false,
		glm::value_ptr(lightSpaceMatrix));

	// Settea la matriz de vista y projection al shader con solo color
	shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
	shader.setMatrix4("view", 1, false, glm::value_ptr(*view));

	// Settea la matriz de vista y projection al shader con skybox
	shaderSkybox.setMatrix4("projection", 1, false,
		glm::value_ptr(projection));
	shaderSkybox.setMatrix4("view", 1, false,
		glm::value_ptr(glm::mat4(glm::mat3(*view))));
	// Settea la matriz de vista y projection al shader con multiples luces
	shaderMulLighting.setMatrix4("projection", 1, false,
		glm::value_ptr(projection));
	shaderMulLighting.setMatrix4("view", 1, false, glm::value_ptr(*view));
	shaderMulLighting.setMatrix4("lightSpaceMatrix", 1, false,
		glm::value_ptr(lightSpaceMatrix));
	// Settea la matriz de vista y projection al shader con multiples luces
	shaderTerrain.setMatrix4("projection", 1, false,
		glm::value_ptr(projection));
	shaderTerrain.setMatrix4("view", 1, false, glm::value_ptr(*view));
	shaderTerrain.setMatrix4("lightSpaceMatrix", 1, false,
		glm::value_ptr(lightSpaceMatrix));
	// Settea la matriz de vista y projection al shader para el fountain
	shaderParticlesFountain.setMatrix4("projection", 1, false,
		glm::value_ptr(projection));
	shaderParticlesFountain.setMatrix4("view", 1, false,
		glm::value_ptr(*view));
	// Settea la matriz de vista y projection al shader para el fuego
	shaderParticlesFire.setInt("Pass", 2);
	shaderParticlesFire.setMatrix4("projection", 1, false,
		glm::value_ptr(projection));
	shaderParticlesFire.setMatrix4("view", 1, false, glm::value_ptr(*view));

	/*******************************************
	 * Propiedades de neblina
	 *******************************************/
	shaderMulLighting.setVectorFloat3("fogColor",
		glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));
	shaderTerrain.setVectorFloat3("fogColor",
		glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));
	shaderSkybox.setVectorFloat3("fogColor",
		glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));

	/*******************************************
	 * Propiedades Luz direccional
	 *******************************************/
	if (cameraSelected == 0)
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
	else
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(cameraFP->getPosition()));
	shaderMulLighting.setVectorFloat3("directionalLight.light.ambient",
		glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
	shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse",
		glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
	shaderMulLighting.setVectorFloat3("directionalLight.light.specular",
		glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
	shaderMulLighting.setVectorFloat3("directionalLight.direction",
		glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

	/*******************************************
	 * Propiedades Luz direccional Terrain
	 *******************************************/
	if (cameraSelected == 0)
		shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
	else
		shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(cameraFP->getPosition()));
	shaderTerrain.setVectorFloat3("directionalLight.light.ambient",
		glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
	shaderTerrain.setVectorFloat3("directionalLight.light.diffuse",
		glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
	shaderTerrain.setVectorFloat3("directionalLight.light.specular",
		glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
	shaderTerrain.setVectorFloat3("directionalLight.direction",
		glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

	/*******************************************
	 * Propiedades SpotLights
	 *******************************************/
	 /*glm::vec3 spotPosition = glm::vec3(
			 modelMatrixHeli * glm::vec4(0.32437, 0.226053, 1.79149, 1.0));
	 shaderMulLighting.setInt("spotLightCount", 1);
	 shaderTerrain.setInt("spotLightCount", 1);
	 shaderMulLighting.setVectorFloat3("spotLights[0].light.ambient",
			 glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
	 shaderMulLighting.setVectorFloat3("spotLights[0].light.diffuse",
			 glm::value_ptr(glm::vec3(0.2, 0.3, 0.2)));
	 shaderMulLighting.setVectorFloat3("spotLights[0].light.specular",
			 glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
	 shaderMulLighting.setVectorFloat3("spotLights[0].position",
			 glm::value_ptr(spotPosition));
	 shaderMulLighting.setVectorFloat3("spotLights[0].direction",
			 glm::value_ptr(glm::vec3(0, -1, 0)));
	 shaderMulLighting.setFloat("spotLights[0].constant", 1.0);
	 shaderMulLighting.setFloat("spotLights[0].linear", 0.074);
	 shaderMulLighting.setFloat("spotLights[0].quadratic", 0.03);
	 shaderMulLighting.setFloat("spotLights[0].cutOff",
			 cos(glm::radians(12.5f)));
	 shaderMulLighting.setFloat("spotLights[0].outerCutOff",
			 cos(glm::radians(15.0f)));
	 shaderTerrain.setVectorFloat3("spotLights[0].light.ambient",
			 glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
	 shaderTerrain.setVectorFloat3("spotLights[0].light.diffuse",
			 glm::value_ptr(glm::vec3(0.2, 0.3, 0.2)));
	 shaderTerrain.setVectorFloat3("spotLights[0].light.specular",
			 glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
	 shaderTerrain.setVectorFloat3("spotLights[0].position",
			 glm::value_ptr(spotPosition));
	 shaderTerrain.setVectorFloat3("spotLights[0].direction",
			 glm::value_ptr(glm::vec3(0, -1, 0)));
	 shaderTerrain.setFloat("spotLights[0].constant", 1.0);
	 shaderTerrain.setFloat("spotLights[0].linear", 0.074);
	 shaderTerrain.setFloat("spotLights[0].quadratic", 0.03);
	 shaderTerrain.setFloat("spotLights[0].cutOff",
			 cos(glm::radians(12.5f)));
	 shaderTerrain.setFloat("spotLights[0].outerCutOff",
			 cos(glm::radians(15.0f)));*/

			 /*******************************************
			  * Propiedades PointLights
			  *******************************************/
	shaderMulLighting.setInt("pointLightCount",
		lamp1Position.size() + lamp2Orientation.size());
	shaderTerrain.setInt("pointLightCount",
		lamp1Position.size() + lamp2Orientation.size());
	for (int i = 0; i < lamp1Position.size(); i++) {
		glm::mat4 matrixAdjustLamp = glm::mat4(1.0f);
		matrixAdjustLamp = glm::translate(matrixAdjustLamp,
			lamp1Position[i]);
		matrixAdjustLamp = glm::rotate(matrixAdjustLamp,
			glm::radians(lamp1Orientation[i]), glm::vec3(0, 1, 0));
		matrixAdjustLamp = glm::scale(matrixAdjustLamp,
			glm::vec3(0.5, 0.5, 0.5));
		matrixAdjustLamp = glm::translate(matrixAdjustLamp,
			glm::vec3(0, 10.3585, 0));
		glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
		shaderMulLighting.setVectorFloat3(
			"pointLights[" + std::to_string(i) + "].light.ambient",
			glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
		shaderMulLighting.setVectorFloat3(
			"pointLights[" + std::to_string(i) + "].light.diffuse",
			glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
		shaderMulLighting.setVectorFloat3(
			"pointLights[" + std::to_string(i) + "].light.specular",
			glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
		shaderMulLighting.setVectorFloat3(
			"pointLights[" + std::to_string(i) + "].position",
			glm::value_ptr(lampPosition));
		shaderMulLighting.setFloat(
			"pointLights[" + std::to_string(i) + "].constant", 1.0);
		shaderMulLighting.setFloat(
			"pointLights[" + std::to_string(i) + "].linear", 0.09);
		shaderMulLighting.setFloat(
			"pointLights[" + std::to_string(i) + "].quadratic", 0.01);
		shaderTerrain.setVectorFloat3(
			"pointLights[" + std::to_string(i) + "].light.ambient",
			glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
		shaderTerrain.setVectorFloat3(
			"pointLights[" + std::to_string(i) + "].light.diffuse",
			glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
		shaderTerrain.setVectorFloat3(
			"pointLights[" + std::to_string(i) + "].light.specular",
			glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
		shaderTerrain.setVectorFloat3(
			"pointLights[" + std::to_string(i) + "].position",
			glm::value_ptr(lampPosition));
		shaderTerrain.setFloat(
			"pointLights[" + std::to_string(i) + "].constant", 1.0);
		shaderTerrain.setFloat(
			"pointLights[" + std::to_string(i) + "].linear", 0.09);
		shaderTerrain.setFloat(
			"pointLights[" + std::to_string(i) + "].quadratic", 0.02);
	}
	for (int i = 0; i < lamp2Position.size(); i++) {
		glm::mat4 matrixAdjustLamp = glm::mat4(1.0f);
		matrixAdjustLamp = glm::translate(matrixAdjustLamp,
			lamp2Position[i]);
		matrixAdjustLamp = glm::rotate(matrixAdjustLamp,
			glm::radians(lamp2Orientation[i]), glm::vec3(0, 1, 0));
		matrixAdjustLamp = glm::scale(matrixAdjustLamp,
			glm::vec3(1.0, 1.0, 1.0));
		matrixAdjustLamp = glm::translate(matrixAdjustLamp,
			glm::vec3(0.759521, 5.00174, 0));
		glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
		shaderMulLighting.setVectorFloat3(
			"pointLights[" + std::to_string(lamp1Position.size() + i)
			+ "].light.ambient",
			glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
		shaderMulLighting.setVectorFloat3(
			"pointLights[" + std::to_string(lamp1Position.size() + i)
			+ "].light.diffuse",
			glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
		shaderMulLighting.setVectorFloat3(
			"pointLights[" + std::to_string(lamp1Position.size() + i)
			+ "].light.specular",
			glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
		shaderMulLighting.setVectorFloat3(
			"pointLights[" + std::to_string(lamp1Position.size() + i)
			+ "].position", glm::value_ptr(lampPosition));
		shaderMulLighting.setFloat(
			"pointLights[" + std::to_string(lamp1Position.size() + i)
			+ "].constant", 1.0);
		shaderMulLighting.setFloat(
			"pointLights[" + std::to_string(lamp1Position.size() + i)
			+ "].linear", 0.09);
		shaderMulLighting.setFloat(
			"pointLights[" + std::to_string(lamp1Position.size() + i)
			+ "].quadratic", 0.01);
		shaderTerrain.setVectorFloat3(
			"pointLights[" + std::to_string(lamp1Position.size() + i)
			+ "].light.ambient",
			glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
		shaderTerrain.setVectorFloat3(
			"pointLights[" + std::to_string(lamp1Position.size() + i)
			+ "].light.diffuse",
			glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
		shaderTerrain.setVectorFloat3(
			"pointLights[" + std::to_string(lamp1Position.size() + i)
			+ "].light.specular",
			glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
		shaderTerrain.setVectorFloat3(
			"pointLights[" + std::to_string(lamp1Position.size() + i)
			+ "].position", glm::value_ptr(lampPosition));
		shaderTerrain.setFloat(
			"pointLights[" + std::to_string(lamp1Position.size() + i)
			+ "].constant", 1.0);
		shaderTerrain.setFloat(
			"pointLights[" + std::to_string(lamp1Position.size() + i)
			+ "].linear", 0.09);
		shaderTerrain.setFloat(
			"pointLights[" + std::to_string(lamp1Position.size() + i)
			+ "].quadratic", 0.02);
	}
}

void preRender1() {
	/*******************************************
		 * 1.- We render the depth buffer
	*******************************************/
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// render scene from light's point of view
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	//glCullFace(GL_FRONT);
	prepareDepthScene();
	renderScene(false);
	//glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/*******************************************
	 * Debug to view the texture view map
	*******************************************/
	 // reset viewport
	 /*glViewport(0, 0, screenWidth, screenHeight);
	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	  // render Depth map to quad for visual debugging
	  shaderViewDepth.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
	  shaderViewDepth.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
	  glActiveTexture(GL_TEXTURE0);
	  glBindTexture(GL_TEXTURE_2D, depthMap);
	  boxViewDepth.setScale(glm::vec3(2.0, 2.0, 1.0));
	  boxViewDepth.render();*/

	 /*******************************************
	   * 2.- We render the normal objects
	 *******************************************/
	glViewport(0, 0, screenWidth, screenHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	prepareScene();
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	shaderMulLighting.setInt("shadowMap", 10);
	shaderTerrain.setInt("shadowMap", 10);
	/*******************************************
	 * Skybox
	 *******************************************/
	GLint oldCullFaceMode;
	GLint oldDepthFuncMode;
	// deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
	glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
	glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
	shaderSkybox.setFloat("skybox", 0);
	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);
	glActiveTexture(GL_TEXTURE0);
	skyboxSphere.render();
	glCullFace(oldCullFaceMode);
	glDepthFunc(oldDepthFuncMode);
}

void collidersManagmentEs1() {
	std::map<std::string, bool> collisionDetection;
	/*******************************************
		 * Creacion de colliders
		 * IMPORTANT do this before interpolations
		 *******************************************/

		 // Lamps1 colliders
		 //for (int i = 0; i < lamp1Position.size(); i++) {
		 //	AbstractModel::OBB lampCollider;
		 //	glm::mat4 modelMatrixColliderLamp = glm::mat4(1.0);
		 //	modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp,
		 //			lamp1Position[i]);
		 //	modelMatrixColliderLamp = glm::rotate(modelMatrixColliderLamp,
		 //			glm::radians(lamp1Orientation[i]), glm::vec3(0, 1, 0));
		 //	addOrUpdateColliders(collidersOBB, "lamp1-" + std::to_string(i),
		 //			lampCollider, modelMatrixColliderLamp);
		 //	// Set the orientation of collider before doing the scale
		 //	lampCollider.u = glm::quat_cast(modelMatrixColliderLamp);
		 //	modelMatrixColliderLamp = glm::scale(modelMatrixColliderLamp,
		 //			glm::vec3(0.5, 0.5, 0.5));
		 //	modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp,
		 //			modelLamp1.getObb().c);
		 //	lampCollider.c = glm::vec3(modelMatrixColliderLamp[3]);
		 //	lampCollider.e = modelLamp1.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		 //	std::get<0>(collidersOBB.find("lamp1-" + std::to_string(i))->second) =
		 //			lampCollider;
		 //}

		 // Lamps2 colliders
		 //for (int i = 0; i < lamp2Position.size(); i++) {
		 //	AbstractModel::OBB lampCollider;
		 //	glm::mat4 modelMatrixColliderLamp = glm::mat4(1.0);
		 //	modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp,
		 //			lamp2Position[i]);
		 //	modelMatrixColliderLamp = glm::rotate(modelMatrixColliderLamp,
		 //			glm::radians(lamp2Orientation[i]), glm::vec3(0, 1, 0));
		 //	addOrUpdateColliders(collidersOBB, "lamp2-" + std::to_string(i),
		 //			lampCollider, modelMatrixColliderLamp);
		 //	// Set the orientation of collider before doing the scale
		 //	lampCollider.u = glm::quat_cast(modelMatrixColliderLamp);
		 //	modelMatrixColliderLamp = glm::scale(modelMatrixColliderLamp,
		 //			glm::vec3(1.0, 1.0, 1.0));
		 //	modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp,
		 //			modelLampPost2.getObb().c);
		 //	lampCollider.c = glm::vec3(modelMatrixColliderLamp[3]);
		 //	lampCollider.e = modelLampPost2.getObb().e
		 //			* glm::vec3(1.0, 1.0, 1.0);
		 //	std::get<0>(collidersOBB.find("lamp2-" + std::to_string(i))->second) =
		 //			lampCollider;
		 //}

		 //Botones colliders
	for (int i = 0; i < botonesPos.size(); i++) {
		AbstractModel::OBB botonCollider;
		AbstractModel::OBB botonColliderY;
		AbstractModel::OBB botonColliderB;
		AbstractModel::OBB botonColliderR;
		glm::mat4 modelMatrixColliderBoton = glm::mat4(1.0);
		glm::mat4 modelMatrixColliderBotonY = glm::mat4(1.0);
		glm::mat4 modelMatrixColliderBotonB = glm::mat4(1.0);
		glm::mat4 modelMatrixColliderBotonR = glm::mat4(1.0);
		modelMatrixColliderBoton = glm::translate(modelMatrixColliderBoton,
			botonesPos[i]);
		//Botones colores
		modelMatrixColliderBotonY = glm::translate(modelMatrixColliderBoton,
			glm::vec3(-1.0, -0.4, 1.2));
		modelMatrixColliderBotonB = glm::translate(modelMatrixColliderBoton,
			glm::vec3(0.0, -0.4, 1.2));
		modelMatrixColliderBotonR = glm::translate(modelMatrixColliderBoton,
			glm::vec3(1.0, -0.4, 1.2));
		//Retomar caja general
		modelMatrixColliderBoton = glm::translate(modelMatrixColliderBoton,
			glm::vec3(0, 0, -0.2));
		modelMatrixColliderBoton = glm::rotate(modelMatrixColliderBoton,
			glm::radians(-90.0f), glm::vec3(1, 0, 0));
		modelMatrixColliderBoton = glm::rotate(modelMatrixColliderBoton,
			glm::radians(-180.0f), glm::vec3(0, 0, 1));
		//adds
		addOrUpdateColliders(collidersOBB, "botonBox-" + std::to_string(i),
			botonCollider, modelMatrixColliderBoton);
		addOrUpdateColliders(collidersOBB, "botonBox-Y" + std::to_string(i),
			botonColliderY, modelMatrixColliderBotonY);
		addOrUpdateColliders(collidersOBB, "botonBox-B" + std::to_string(i),
			botonColliderB, modelMatrixColliderBotonB);
		addOrUpdateColliders(collidersOBB, "botonBox-R" + std::to_string(i),
			botonColliderR, modelMatrixColliderBotonR);
		// Set the orientation of collider before doing the scale
		botonCollider.u = glm::quat_cast(modelMatrixColliderBoton);
		modelMatrixColliderBoton = glm::scale(modelMatrixColliderBoton,
			glm::vec3(1.5, 0.9, 1.0));
		modelMatrixColliderBoton = glm::translate(modelMatrixColliderBoton,
			modelBotones.getObb().c);
		botonCollider.c = glm::vec3(modelMatrixColliderBoton[3]);
		botonCollider.e = modelBotones.getObb().e
			* glm::vec3(1.5, 0.9, 1.0);
		//Y boton
		botonColliderY.u = glm::quat_cast(modelMatrixColliderBotonY);
		modelMatrixColliderBotonY = glm::scale(modelMatrixColliderBotonY,
			glm::vec3(0.3, 0.5, 0.3));
		botonColliderY.c = glm::vec3(modelMatrixColliderBotonY[3]);
		botonColliderY.e = modelBotones.getObb().e * glm::vec3(0.3, 0.5, 0.3);
		//B boton
		botonColliderB.u = glm::quat_cast(modelMatrixColliderBotonB);
		modelMatrixColliderBotonB = glm::scale(modelMatrixColliderBotonB,
			glm::vec3(0.3, 0.5, 0.3));
		botonColliderB.c = glm::vec3(modelMatrixColliderBotonB[3]);
		botonColliderB.e = modelBotones.getObb().e * glm::vec3(0.3, 0.5, 0.3);
		//R boton
		botonColliderR.u = glm::quat_cast(modelMatrixColliderBotonR);
		modelMatrixColliderBotonR = glm::scale(modelMatrixColliderBotonR,
			glm::vec3(0.3, 0.5, 0.3));
		botonColliderR.c = glm::vec3(modelMatrixColliderBotonR[3]);
		botonColliderR.e = modelBotones.getObb().e * glm::vec3(0.3, 0.5, 0.3);
		std::get<0>(collidersOBB.find("botonBox-" + std::to_string(i))->second) =
			botonCollider;
		std::get<0>(collidersOBB.find("botonBox-Y" + std::to_string(i))->second) =
			botonColliderY;
		std::get<0>(collidersOBB.find("botonBox-B" + std::to_string(i))->second) =
			botonColliderB;
		std::get<0>(collidersOBB.find("botonBox-R" + std::to_string(i))->second) =
			botonColliderR;
	}

	//Botones generadores
	for (int i = 0; i < botonesPos.size(); i++) {
		AbstractModel::OBB generadorCollider;
		glm::mat4 modelMatrixColliderGenerador = glm::mat4(1.0);
		modelMatrixColliderGenerador = glm::translate(modelMatrixColliderGenerador,
			glm::vec3(botonesPos[i].x + 2.65, terrain.getHeightTerrain(botonesPos[i].x + 2.65,
					botonesPos[i].z) + 1.0f, botonesPos[i].z - 0.1));
		/*modelMatrixColliderGenerador = glm::translate(modelMatrixColliderGenerador,
			glm::vec3(1.0, 1.0, 1.0));*/
		addOrUpdateColliders(collidersOBB, "gene-" + std::to_string(i),
			generadorCollider, modelMatrixColliderGenerador);
		// Set the orientation of collider before doing the scale
		generadorCollider.u = glm::quat_cast(modelMatrixColliderGenerador);
		modelMatrixColliderGenerador = glm::scale(modelMatrixColliderGenerador,
			glm::vec3(1.0, 1.0, 1.0));
		modelMatrixColliderGenerador = glm::translate(modelMatrixColliderGenerador,
			modelGenerador.getObb().c);
		generadorCollider.c = glm::vec3(modelMatrixColliderGenerador[3]);
		generadorCollider.e = modelGenerador.getObb().e
			* glm::vec3(1.0, 1.0, 1.0);
		std::get<0>(collidersOBB.find("gene-" + std::to_string(i))->second) =
			generadorCollider;
	}

	//Lamparas generadores
	for (int i = 0; i < botonesPos.size(); i++) {
		AbstractModel::OBB generadorLampCollider;
		glm::mat4 modelMatrixColliderLampGenerador = glm::mat4(1.0);
		modelMatrixColliderLampGenerador = glm::translate(modelMatrixColliderLampGenerador,
			glm::vec3(botonesPos[i].x + 3.9, terrain.getHeightTerrain(botonesPos[i].x + 3.9,
					botonesPos[i].z + 1.4), botonesPos[i].z + 1.4));
		modelMatrixColliderLampGenerador = glm::rotate(modelMatrixColliderLampGenerador,
			glm::radians(140.0f), glm::vec3(0, 1, 0));
		addOrUpdateColliders(collidersOBB, "geneLamp-" + std::to_string(i),
			generadorLampCollider, modelMatrixColliderLampGenerador);
		// Set the orientation of collider before doing the scale
		generadorLampCollider.u = glm::quat_cast(modelMatrixColliderLampGenerador);
		modelMatrixColliderLampGenerador = glm::scale(modelMatrixColliderLampGenerador,
			glm::vec3(0.45, 1.0, 1.0));
		modelMatrixColliderLampGenerador = glm::translate(modelMatrixColliderLampGenerador,
			modelLuzGenerador.getObb().c);
		generadorLampCollider.c = glm::vec3(modelMatrixColliderLampGenerador[3]);
		generadorLampCollider.e = modelLuzGenerador.getObb().e
			* glm::vec3(0.45, 1.0, 1.0);
		std::get<0>(collidersOBB.find("geneLamp-" + std::to_string(i))->second) =
			generadorLampCollider;
	}

	// Collider de compuerta
	AbstractModel::OBB compuertaCollider;
	glm::mat4 modelmatrixColliderCompuerta = glm::mat4(modelMatrixCompuerta);
	modelmatrixColliderCompuerta = glm::rotate(modelmatrixColliderCompuerta,
		glm::radians(-90.0f), glm::vec3(1, 0, 0));
	// Set the orientation of collider before doing the scale
	compuertaCollider.u = glm::quat_cast(modelmatrixColliderCompuerta);
	modelmatrixColliderCompuerta = glm::scale(modelmatrixColliderCompuerta,
		glm::vec3(0.021, 0.021, 0.021));
	modelmatrixColliderCompuerta = glm::translate(modelmatrixColliderCompuerta,
		glm::vec3(modelCompuerta.getObb().c.x,
			modelCompuerta.getObb().c.y,
			modelCompuerta.getObb().c.z));
	compuertaCollider.e = modelCompuerta.getObb().e
		* glm::vec3(0.021, 0.021, 0.021);
	compuertaCollider.c = glm::vec3(modelmatrixColliderCompuerta[3]);
	addOrUpdateColliders(collidersOBB, "compuerta", compuertaCollider,
		modelMatrixCompuerta);

	// Collider de edificio compuerta
	AbstractModel::OBB edCompuertaCollider;
	glm::mat4 modelmatrixColliderEdCompuerta = glm::mat4(modelMatrixEdCompuerta);
	modelmatrixColliderEdCompuerta = glm::rotate(modelmatrixColliderEdCompuerta,
		glm::radians(-90.0f), glm::vec3(1, 0, 0));
	// Set the orientation of collider before doing the scale
	edCompuertaCollider.u = glm::quat_cast(modelmatrixColliderEdCompuerta);
	modelmatrixColliderEdCompuerta = glm::scale(modelmatrixColliderEdCompuerta,
		glm::vec3(0.021, 0.021, 0.021));
	modelmatrixColliderEdCompuerta = glm::translate(modelmatrixColliderEdCompuerta,
		glm::vec3(modelEdCompuerta.getObb().c.x,
			modelEdCompuerta.getObb().c.y,
			modelEdCompuerta.getObb().c.z));
	edCompuertaCollider.e = modelEdCompuerta.getObb().e
		* glm::vec3(0.021, 0.021, 0.021);
	edCompuertaCollider.c = glm::vec3(modelmatrixColliderEdCompuerta[3]);
	addOrUpdateColliders(collidersOBB, "edCompuerta", edCompuertaCollider,
		modelMatrixEdCompuerta);

	// Collider de mayow
	AbstractModel::OBB mayowCollider;
	glm::mat4 modelmatrixColliderMayow = glm::mat4(modelMatrixMayow);
	modelmatrixColliderMayow = glm::rotate(modelmatrixColliderMayow,
		glm::radians(-90.0f), glm::vec3(1, 0, 0));
	// Set the orientation of collider before doing the scale
	mayowCollider.u = glm::quat_cast(modelmatrixColliderMayow);
	modelmatrixColliderMayow = glm::scale(modelmatrixColliderMayow,
		glm::vec3(0.021, 0.021, 0.021));
	modelmatrixColliderMayow = glm::translate(modelmatrixColliderMayow,
		glm::vec3(mayowModelAnimate.getObb().c.x,
			mayowModelAnimate.getObb().c.y,
			mayowModelAnimate.getObb().c.z));
	mayowCollider.e = mayowModelAnimate.getObb().e
		* glm::vec3(0.021, 0.021, 0.021)
		* glm::vec3(0.787401574, 0.787401574, 0.787401574);
	mayowCollider.c = glm::vec3(modelmatrixColliderMayow[3]);
	addOrUpdateColliders(collidersOBB, "mayow", mayowCollider,
		modelMatrixMayow);

	//Collider de astroProta
	AbstractModel::OBB astroProtaCollider;
	glm::mat4 modelmatrixColliderAstroProta = glm::mat4(modelMatrixAstroProta);
	// Set the orientation of collider before doing the scale
	astroProtaCollider.u = glm::quat_cast(modelmatrixColliderAstroProta);
	modelmatrixColliderAstroProta = glm::scale(modelmatrixColliderAstroProta,
		glm::vec3(1.5, 4.0, 1.4));
	modelmatrixColliderAstroProta = glm::translate(modelmatrixColliderAstroProta,
		glm::vec3(astroProta.getObb().c.x,
			astroProta.getObb().c.y + 0.28,
			astroProta.getObb().c.z + 0.28));
	astroProtaCollider.e = astroProta.getObb().e
		* glm::vec3(1.5, 4.0, 1.4);
	astroProtaCollider.c = glm::vec3(modelmatrixColliderAstroProta[3]);
	addOrUpdateColliders(collidersOBB, "astroProta", astroProtaCollider,
		modelMatrixAstroProta);

	//Collider muro fondo
	AbstractModel::OBB muroFondoCollider;
	glm::mat4 modelmatrixColliderMuroFondo = glm::mat4(modelMatrixMuroFondo);
	// Set the orientation of collider before doing the scale
	muroFondoCollider.u = glm::quat_cast(modelmatrixColliderMuroFondo);
	modelmatrixColliderMuroFondo = glm::translate(modelmatrixColliderMuroFondo,
		glm::vec3(muroFondo.getObb().c.x,
			muroFondo.getObb().c.y,
			muroFondo.getObb().c.z));
	muroFondoCollider.e = glm::vec3(100.50f, 10.0f, 0.0f);
	muroFondoCollider.c = glm::vec3(modelmatrixColliderMuroFondo[3]);
	addOrUpdateColliders(collidersOBB, "muroFondo", muroFondoCollider,
		modelMatrixMuroFondo);

	//Collider muro frontal
	AbstractModel::OBB muroFrontalCollider;
	glm::mat4 modelmatrixColliderMuroFrontal = glm::mat4(modelMatrixMuroFrontal);
	// Set the orientation of collider before doing the scale
	muroFrontalCollider.u = glm::quat_cast(modelmatrixColliderMuroFrontal);
	modelmatrixColliderMuroFrontal = glm::translate(modelmatrixColliderMuroFrontal,
		glm::vec3(muroFrontal.getObb().c.x,
			muroFrontal.getObb().c.y,
			muroFrontal.getObb().c.z));
	muroFrontalCollider.e = glm::vec3(100.50f, 10.0f, 0.0f);
	muroFrontalCollider.c = glm::vec3(modelmatrixColliderMuroFrontal[3]);
	addOrUpdateColliders(collidersOBB, "muroFrontal", muroFrontalCollider,
		modelMatrixMuroFrontal);

	//Collider muro derecho
	AbstractModel::OBB muroDerechoCollider;
	glm::mat4 modelmatrixColliderMuroDerecho = glm::mat4(modelMatrixMuroDerecho);
	// Set the orientation of collider before doing the scale
	muroDerechoCollider.u = glm::quat_cast(modelmatrixColliderMuroDerecho);
	modelmatrixColliderMuroDerecho = glm::translate(modelmatrixColliderMuroDerecho,
		glm::vec3(muroDerecho.getObb().c.x,
			muroDerecho.getObb().c.y,
			muroDerecho.getObb().c.z));
	muroDerechoCollider.e = glm::vec3(0.0f, 10.0f, 35.0f);
	muroDerechoCollider.c = glm::vec3(modelmatrixColliderMuroDerecho[3]);
	addOrUpdateColliders(collidersOBB, "muroDerecho", muroDerechoCollider,
		modelMatrixMuroDerecho);

	//Collider muro izquierdo
	AbstractModel::OBB muroIzquierdoCollider;
	glm::mat4 modelmatrixColliderMuroIzquierdo = glm::mat4(modelMatrixMuroIzquierdo);
	// Set the orientation of collider before doing the scale
	muroIzquierdoCollider.u = glm::quat_cast(modelmatrixColliderMuroIzquierdo);
	modelmatrixColliderMuroIzquierdo = glm::translate(modelmatrixColliderMuroIzquierdo,
		glm::vec3(muroIzquierdo.getObb().c.x,
			muroIzquierdo.getObb().c.y,
			muroIzquierdo.getObb().c.z));
	muroIzquierdoCollider.e = glm::vec3(0.0f, 10.0f, 35.0f);
	muroIzquierdoCollider.c = glm::vec3(modelmatrixColliderMuroIzquierdo[3]);
	addOrUpdateColliders(collidersOBB, "muroIzquierdo", muroIzquierdoCollider,
		modelMatrixMuroIzquierdo);

	/*******************************************
	 * Render de colliders
	 *******************************************/
	for (std::map<std::string,
		std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
		collidersOBB.begin(); it != collidersOBB.end(); it++) {
		glm::mat4 matrixCollider = glm::mat4(1.0);
		matrixCollider = glm::translate(matrixCollider,
			std::get<0>(it->second).c);
		matrixCollider = matrixCollider
			* glm::mat4(std::get<0>(it->second).u);
		matrixCollider = glm::scale(matrixCollider,
			std::get<0>(it->second).e * 2.0f);
		boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
		boxCollider.enableWireMode();
		boxCollider.render(matrixCollider);
	}

	for (std::map<std::string,
		std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
		collidersSBB.begin(); it != collidersSBB.end(); it++) {
		glm::mat4 matrixCollider = glm::mat4(1.0);
		matrixCollider = glm::translate(matrixCollider,
			std::get<0>(it->second).c);
		matrixCollider = glm::scale(matrixCollider,
			glm::vec3(std::get<0>(it->second).ratio * 2.0f));
		sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
		sphereCollider.enableWireMode();
		sphereCollider.render(matrixCollider);
	}

	/*******************************************
	 * Test Colisions
	 *******************************************/
	bool isCollisionG = false;
	bool isCollisionEnemy = false;

	for (std::map<std::string,
		std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
		collidersOBB.begin(); it != collidersOBB.end(); it++) {
		bool isCollision = false;
		for (std::map<std::string,
			std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
			collidersOBB.begin(); jt != collidersOBB.end(); jt++) {
			if (it != jt
				&& testOBBOBB(std::get<0>(it->second),
					std::get<0>(jt->second))) {
				if (!(excepCollider(it->first, jt->first)))
				{
					std::cout << "Colision " << it->first << " with "
						<< jt->first << std::endl;

					if ((it->first.compare("mayow") == 0 || it->first.compare("astroProta") == 0)
						&& (jt->first.compare("mayow") == 0 || jt->first.compare("astroProta") == 0))
						isCollisionEnemy = true;

					isCollision = true;
					isCollisionG = true;
				}
			}
		}
		addOrUpdateCollisionDetection(collisionDetection, it->first,
			isCollision);
	}

	if (isCollisionG && actionE && !enableEscotilla1) {
		std::cout << "EntraISC " << std::endl;
		updateBotonCollider(collisionDetection);
		actionE = false;
	}

	for (std::map<std::string,
		std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
		collidersSBB.begin(); it != collidersSBB.end(); it++) {
		bool isCollision = false;
		for (std::map<std::string,
			std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator jt =
			collidersSBB.begin(); jt != collidersSBB.end(); jt++) {
			if (it != jt
				&& testSphereSphereIntersection(std::get<0>(it->second),
					std::get<0>(jt->second))) {
				//std::cout << "Colision " << it->first << " with "
				//		<< jt->first << std::endl;
				isCollision = true;
			}
		}
		addOrUpdateCollisionDetection(collisionDetection, it->first,
			isCollision);
	}

	for (std::map<std::string,
		std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
		collidersSBB.begin(); it != collidersSBB.end(); it++) {
		bool isCollision = false;
		std::map<std::string,
			std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
			collidersOBB.begin();
		for (; jt != collidersOBB.end(); jt++) {
			if (testSphereOBox(std::get<0>(it->second), std::get<0>(jt->second))) {
				std::cout << "Colision " << it->first << " with "
					<< jt->first << std::endl;

				isCollision = true;
				addOrUpdateCollisionDetection(collisionDetection, jt->first,
					isCollision);
			}
		}
		addOrUpdateCollisionDetection(collisionDetection, it->first,
			isCollision);
	}

	std::map<std::string, bool>::iterator colIt;
	for (colIt = collisionDetection.begin();
		colIt != collisionDetection.end(); colIt++) {
		std::map<std::string,
			std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
			collidersSBB.find(colIt->first);
		std::map<std::string,
			std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
			collidersOBB.find(colIt->first);
		if (it != collidersSBB.end()) {
			if (!colIt->second)
				addOrUpdateColliders(collidersSBB, it->first);
		}
		if (jt != collidersOBB.end()) {
			if (!colIt->second) {
				addOrUpdateColliders(collidersOBB, jt->first);
			}
			else {
				if (jt->first.compare("mayow") == 0) {
					if (isCollisionEnemy == true) {
						enemigo1.respawn = true;
						modelMatrixMayow = glm::translate(modelMatrixMayow, enemigo1.calculaReaparicion(enemigo1.origen, modelMatrixMayow[3]));
						playerRespawn = true;
						isCollisionEnemy = false;
					}
				}
				if (jt->first.compare("astroProta") == 0) {
					modelMatrixAstroProta = std::get<1>(jt->second);
				}
			}
		}
	}
}

void soundEscene1() {
	/****************************+
		 * Open AL sound data
		 */
		 // Listener for the Thris person camera
	listenerPos[0] = modelMatrixMayow[3].x;
	listenerPos[1] = modelMatrixMayow[3].y;
	listenerPos[2] = modelMatrixMayow[3].z;
	alListenerfv(AL_POSITION, listenerPos);

	glm::vec3 upModel = glm::normalize(modelMatrixMayow[1]);
	glm::vec3 frontModel = glm::normalize(modelMatrixMayow[2]);

	listenerOri[0] = frontModel.x;
	listenerOri[1] = frontModel.y;
	listenerOri[2] = frontModel.z;
	listenerOri[3] = upModel.x;
	listenerOri[4] = upModel.y;
	listenerOri[5] = upModel.z;

	// Listener for the First person camera
	/*listenerPos[0] = camera->getPosition().x;
	 listenerPos[1] = camera->getPosition().y;
	 listenerPos[2] = camera->getPosition().z;
	 alListenerfv(AL_POSITION, listenerPos);
	 listenerOri[0] = camera->getFront().x;
	 listenerOri[1] = camera->getFront().y;
	 listenerOri[2] = camera->getFront().z;
	 listenerOri[3] = camera->getUp().x;
	 listenerOri[4] = camera->getUp().y;
	 listenerOri[5] = camera->getUp().z;*/
	alListenerfv(AL_ORIENTATION, listenerOri);

	for (unsigned int i = 0; i < sourcesPlay.size(); i++) {
		if (sourcesPlay[i]) {
			sourcesPlay[i] = false;
			alSourcePlay(source[i]);
		}
	}
}

void prepareScene2() {
	skyboxSphere.setShader(&shaderSkybox);

	terrain.setShader(&shaderTerrain);

	//Mayow
	mayowModelAnimate.setShader(&shaderMulLighting);

	pivoteCam.setShader(&shaderMulLighting);

	astroProta.setShader(&shaderMulLighting);

	muroFondo.setShader(&shaderMulLighting);

	muroFrontal.setShader(&shaderMulLighting);

	muroDerecho.setShader(&shaderMulLighting);

	muroIzquierdo.setShader(&shaderMulLighting);

	modelBotones.setShader(&shaderMulLighting);

	modelCompuerta.setShader(&shaderMulLighting);

	modelEdCompuerta.setShader(&shaderMulLighting);

	modelGenerador.setShader(&shaderMulLighting);

	modelPlataforma.setShader(&shaderMulLighting);

	modelPlaCompuerta.setShader(&shaderMulLighting);

	modelRokas.setShader(&shaderMulLighting);

	boxReferencia.setShader(&shaderMulLighting);

	modelLuzBotones.setShader(&shaderMulLighting);

	modelLuzGenerador.setShader(&shaderMulLighting);

	modelEscenario2.setShader(&shaderMulLighting);

	modelBidones.setShader(&shaderMulLighting);

	modelCaja1.setShader(&shaderMulLighting);

	modelCaja2.setShader(&shaderMulLighting);

	modelCompu.setShader(&shaderMulLighting);

	modelCuerpo.setShader(&shaderMulLighting);

	modelEstanteria.setShader(&shaderMulLighting);
}

void prepareDepthScene2() {
	skyboxSphere.setShader(&shaderDepth);

	terrain.setShader(&shaderDepth);

	//Mayow
	mayowModelAnimate.setShader(&shaderDepth);

	pivoteCam.setShader(&shaderDepth);

	astroProta.setShader(&shaderDepth);

	muroFondo.setShader(&shaderDepth);

	muroFrontal.setShader(&shaderDepth);

	muroDerecho.setShader(&shaderDepth);

	muroIzquierdo.setShader(&shaderDepth);

	modelBotones.setShader(&shaderDepth);

	modelCompuerta.setShader(&shaderDepth);

	modelEdCompuerta.setShader(&shaderDepth);

	modelGenerador.setShader(&shaderDepth);

	modelPlataforma.setShader(&shaderDepth);

	modelPlaCompuerta.setShader(&shaderDepth);

	modelRokas.setShader(&shaderDepth);

	boxReferencia.setShader(&shaderDepth);

	modelLuzBotones.setShader(&shaderDepth);

	modelLuzGenerador.setShader(&shaderDepth);

	modelEscenario2.setShader(&shaderDepth);

	modelBidones.setShader(&shaderDepth);

	modelCaja1.setShader(&shaderDepth);

	modelCaja2.setShader(&shaderDepth);

	modelCompu.setShader(&shaderDepth);

	modelCuerpo.setShader(&shaderDepth);

	modelEstanteria.setShader(&shaderDepth);
}

void renderScene2(bool renderParticles) {
	// Se activa la textura del background
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
	shaderTerrain.setInt("backgroundTexture", 0);
	// Se activa la textura de tierra
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
	shaderTerrain.setInt("rTexture", 1);
	// Se activa la textura de hierba
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
	shaderTerrain.setInt("gTexture", 2);
	// Se activa la textura del camino
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
	shaderTerrain.setInt("bTexture", 3);
	// Se activa la textura del blend map
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID2);
	shaderTerrain.setInt("blendMapTexture", 4);
	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(40, 40)));
	terrain.render();
	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
	glBindTexture(GL_TEXTURE_2D, 0);

	//Muros
	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, textureYellowID);
	//muroFondo.render(modelMatrixMuroFondo);

	//muroFrontal.render(modelMatrixMuroFrontal);

	//muroDerecho.render(modelMatrixMuroDerecho);

	//muroIzquierdo.render(modelMatrixMuroIzquierdo);
	glBindTexture(GL_TEXTURE_2D, 0);

	/*******************************************
	 * Custom objects obj
	 *******************************************/
	 // Forze to enable the unit texture to 0 always ----------------- IMPORTANT
	glActiveTexture(GL_TEXTURE0);

	// Pivote cam
	glDisable(GL_CULL_FACE);
	//pivoteCam.render(modelMatrixPivoteCam);
	//muroFondo.render(modelMatrixMuroFondo);
	glEnable(GL_CULL_FACE);

	//astroProta
	glDisable(GL_CULL_FACE);
	modelMatrixAstroProta[3][1] = terrain.getHeightTerrain(modelMatrixAstroProta[3][0],
		modelMatrixAstroProta[3][2]-20.0);
	glm::mat4 modelMatrixAstroBody = glm::mat4(modelMatrixAstroProta);
	modelMatrixAstroBody[3].z = modelMatrixAstroBody[3].z - 20.0;
	modelMatrixAstroBody = glm::scale(modelMatrixAstroBody,
		glm::vec3(0.021, 0.021, 0.021));
	astroProta.setAnimationIndex(animationIndex);
	astroProta.render(modelMatrixAstroBody);
	glEnable(GL_CULL_FACE);

	modelMatrixMayow[3][1] = -GRAVITY * tmv * tmv + 3.5 * tmv
		+ terrain.getHeightTerrain(modelMatrixMayow[3][0],
			modelMatrixMayow[3][2]);
	tmv = currTime - startTimeJump;
	if (modelMatrixMayow[3][1]
		< terrain.getHeightTerrain(modelMatrixMayow[3][0],
			modelMatrixMayow[3][2])) {
		isJump = false;
		modelMatrixMayow[3][1] = terrain.getHeightTerrain(
			modelMatrixMayow[3][0], modelMatrixMayow[3][2]);
	}
	//modelMatrixMayow[3][1] = terrain.getHeightTerrain(modelMatrixMayow[3][0], modelMatrixMayow[3][2]);
	glm::mat4 modelMatrixMayowBody = glm::mat4(modelMatrixMayow);
	modelMatrixMayowBody = glm::scale(modelMatrixMayowBody,
		glm::vec3(0.021, 0.021, 0.021));
	mayowModelAnimate.setAnimationIndex(animationIndexMayow);
	mayowModelAnimate.render(modelMatrixMayowBody);

	//escenario2
	glDisable(GL_CULL_FACE);
	modelEscenario2.render(modelMatrixEscenario2);

	//Bidones
	modelBidones.setPosition(glm::vec3(-10.0, 1.0, -20.0));
	modelBidones.render();

	//Caja1
	modelCaja1.setPosition(glm::vec3(-5.0, 1.0, -20.0));
	modelCaja1.render();

	//Caja2
	modelCaja2.setPosition(glm::vec3(0.0, 1.0, -20.0));
	modelCaja2.render();

	//Compu
	modelCompu.setPosition(glm::vec3(5.0, 1.0, -20.0));
	modelCompu.render();

	//Cuerpo
	modelCuerpo.setPosition(glm::vec3(10.0, 1.0, -20.0));
	modelCuerpo.render();

	//Estanteria
	modelEstanteria.setPosition(glm::vec3(15.0, 1.0, -20.0));
	modelEstanteria.render();

	glEnable(GL_CULL_FACE);

}

void lucesEscenari2(ShadowBox* shadowBox, glm::mat4* view) {
	glm::vec3 lightPos = glm::vec3(10.0, 10.0, 0.0);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f),
		(float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

	shadowBox->update(screenWidth, screenHeight);
	glm::vec3 centerBox = shadowBox->getCenter();

	// Projection light shadow mapping
	glm::mat4 lightProjection = glm::mat4(1.0f), lightView = glm::mat4(
		1.0f);
	glm::mat4 lightSpaceMatrix;

	lightProjection[0][0] = 2.0f / shadowBox->getWidth();
	lightProjection[1][1] = 2.0f / shadowBox->getHeight();
	lightProjection[2][2] = -2.0f / shadowBox->getLength();
	lightProjection[3][3] = 1.0f;

	lightView = glm::lookAt(centerBox,
		centerBox + glm::normalize(-lightPos),
		glm::vec3(0.0, 1.0, 0.0));

	lightSpaceMatrix = lightProjection * lightView;
	shaderDepth.setMatrix4("lightSpaceMatrix", 1, false,
		glm::value_ptr(lightSpaceMatrix));

	// Settea la matriz de vista y projection al shader con solo color
	shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
	shader.setMatrix4("view", 1, false, glm::value_ptr(*view));

	// Settea la matriz de vista y projection al shader con skybox
	shaderSkybox.setMatrix4("projection", 1, false,
		glm::value_ptr(projection));
	shaderSkybox.setMatrix4("view", 1, false,
		glm::value_ptr(glm::mat4(glm::mat3(*view))));
	// Settea la matriz de vista y projection al shader con multiples luces
	shaderMulLighting.setMatrix4("projection", 1, false,
		glm::value_ptr(projection));
	shaderMulLighting.setMatrix4("view", 1, false, glm::value_ptr(*view));
	shaderMulLighting.setMatrix4("lightSpaceMatrix", 1, false,
		glm::value_ptr(lightSpaceMatrix));
	// Settea la matriz de vista y projection al shader con multiples luces
	shaderTerrain.setMatrix4("projection", 1, false,
		glm::value_ptr(projection));
	shaderTerrain.setMatrix4("view", 1, false, glm::value_ptr(*view));
	shaderTerrain.setMatrix4("lightSpaceMatrix", 1, false,
		glm::value_ptr(lightSpaceMatrix));
	// Settea la matriz de vista y projection al shader para el fountain
	shaderParticlesFountain.setMatrix4("projection", 1, false,
		glm::value_ptr(projection));
	shaderParticlesFountain.setMatrix4("view", 1, false,
		glm::value_ptr(*view));
	// Settea la matriz de vista y projection al shader para el fuego
	shaderParticlesFire.setInt("Pass", 2);
	shaderParticlesFire.setMatrix4("projection", 1, false,
		glm::value_ptr(projection));
	shaderParticlesFire.setMatrix4("view", 1, false, glm::value_ptr(*view));

	/*******************************************
	 * Propiedades de neblina
	 *******************************************/
	shaderMulLighting.setVectorFloat3("fogColor",
		glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));
	shaderTerrain.setVectorFloat3("fogColor",
		glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));
	shaderSkybox.setVectorFloat3("fogColor",
		glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));

	/*******************************************
	 * Propiedades Luz direccional
	 *******************************************/
	if (cameraSelected == 0)
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
	else
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(cameraFP->getPosition()));
	shaderMulLighting.setVectorFloat3("directionalLight.light.ambient",
		glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
	shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse",
		glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
	shaderMulLighting.setVectorFloat3("directionalLight.light.specular",
		glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
	shaderMulLighting.setVectorFloat3("directionalLight.direction",
		glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

	/*******************************************
	 * Propiedades Luz direccional Terrain
	 *******************************************/
	if (cameraSelected == 0)
		shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
	else
		shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(cameraFP->getPosition()));
	shaderTerrain.setVectorFloat3("directionalLight.light.ambient",
		glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
	shaderTerrain.setVectorFloat3("directionalLight.light.diffuse",
		glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
	shaderTerrain.setVectorFloat3("directionalLight.light.specular",
		glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
	shaderTerrain.setVectorFloat3("directionalLight.direction",
		glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

}

void preRender2() {
	/*******************************************
		 * 1.- We render the depth buffer
	*******************************************/
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// render scene from light's point of view
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	//glCullFace(GL_FRONT);
	prepareDepthScene2();
	renderScene2(false);
	//glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/*******************************************
	 * Debug to view the texture view map
	*******************************************/
	// reset viewport
	/*glViewport(0, 0, screenWidth, screenHeight);
	 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	 // render Depth map to quad for visual debugging
	 shaderViewDepth.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
	 shaderViewDepth.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, depthMap);
	 boxViewDepth.setScale(glm::vec3(2.0, 2.0, 1.0));
	 boxViewDepth.render();*/

	 /*******************************************
	   * 2.- We render the normal objects
	 *******************************************/
	glViewport(0, 0, screenWidth, screenHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	prepareScene2();
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	shaderMulLighting.setInt("shadowMap", 10);
	shaderTerrain.setInt("shadowMap", 10);
	/*******************************************
	 * Skybox
	 *******************************************/
	GLint oldCullFaceMode;
	GLint oldDepthFuncMode;
	// deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
	glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
	glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
	shaderSkybox.setFloat("skybox", 0);
	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);
	glActiveTexture(GL_TEXTURE0);
	skyboxSphere.render();
	glCullFace(oldCullFaceMode);
	glDepthFunc(oldDepthFuncMode);
}

void collidersManagmentEs2() {
	std::map<std::string, bool> collisionDetection;
	//Collider de astroProta
	AbstractModel::OBB astroProtaCollider;
	glm::mat4 modelmatrixColliderAstroProta = glm::mat4(modelMatrixAstroProta);
	// Set the orientation of collider before doing the scale
	astroProtaCollider.u = glm::quat_cast(modelmatrixColliderAstroProta);
	modelmatrixColliderAstroProta[3].z += -20.0;
	modelmatrixColliderAstroProta = glm::scale(modelmatrixColliderAstroProta,
		glm::vec3(1.5, 4.0, 1.4));
	modelmatrixColliderAstroProta = glm::translate(modelmatrixColliderAstroProta,
		glm::vec3(astroProta.getObb().c.x,
			astroProta.getObb().c.y + 0.28,
			astroProta.getObb().c.z + 0.28));
	astroProtaCollider.e = astroProta.getObb().e
		* glm::vec3(1.5, 4.0, 1.4);
	astroProtaCollider.c = glm::vec3(modelmatrixColliderAstroProta[3]);
	addOrUpdateColliders(collidersOBB2, "astroProta", astroProtaCollider,
		modelMatrixAstroProta);

	// Collider de mayow
	AbstractModel::OBB mayowCollider;
	glm::mat4 modelmatrixColliderMayow = glm::mat4(modelMatrixMayow);
	modelmatrixColliderMayow = glm::rotate(modelmatrixColliderMayow,
		glm::radians(-90.0f), glm::vec3(1, 0, 0));
	// Set the orientation of collider before doing the scale
	mayowCollider.u = glm::quat_cast(modelmatrixColliderMayow);
	modelmatrixColliderMayow = glm::scale(modelmatrixColliderMayow,
		glm::vec3(0.021, 0.021, 0.021));
	modelmatrixColliderMayow = glm::translate(modelmatrixColliderMayow,
		glm::vec3(mayowModelAnimate.getObb().c.x,
			mayowModelAnimate.getObb().c.y,
			mayowModelAnimate.getObb().c.z));
	mayowCollider.e = mayowModelAnimate.getObb().e
		* glm::vec3(0.021, 0.021, 0.021)
		* glm::vec3(0.787401574, 0.787401574, 0.787401574);
	mayowCollider.c = glm::vec3(modelmatrixColliderMayow[3]);
	addOrUpdateColliders(collidersOBB2, "mayow", mayowCollider,
		modelMatrixMayow);

	//Collider muro fondo
	AbstractModel::OBB muroFondoCollider;
	glm::mat4 modelmatrixColliderMuroFondo = glm::mat4(modelMatrixMuroFondo2);
	// Set the orientation of collider before doing the scale
	muroFondoCollider.u = glm::quat_cast(modelmatrixColliderMuroFondo);
	modelmatrixColliderMuroFondo = glm::translate(modelmatrixColliderMuroFondo,
		glm::vec3(muroFondo2.getObb().c.x,
			muroFondo2.getObb().c.y,
			muroFondo2.getObb().c.z));
	muroFondoCollider.e = glm::vec3(100.50f, 10.0f, 0.0f);
	muroFondoCollider.c = glm::vec3(modelmatrixColliderMuroFondo[3]);
	addOrUpdateColliders(collidersOBB2, "muroFondo2", muroFondoCollider,
		modelMatrixMuroFondo2);

	//Collider muro frontal
	AbstractModel::OBB muroFrontalCollider;
	glm::mat4 modelmatrixColliderMuroFrontal = glm::mat4(modelMatrixMuroFrontal2);
	// Set the orientation of collider before doing the scale
	muroFrontalCollider.u = glm::quat_cast(modelmatrixColliderMuroFrontal);
	modelmatrixColliderMuroFrontal = glm::translate(modelmatrixColliderMuroFrontal,
		glm::vec3(muroFrontal2.getObb().c.x,
			muroFrontal2.getObb().c.y,
			muroFrontal2.getObb().c.z));
	muroFrontalCollider.e = glm::vec3(100.50f, 10.0f, 0.0f);
	muroFrontalCollider.c = glm::vec3(modelmatrixColliderMuroFrontal[3]);
	addOrUpdateColliders(collidersOBB2, "muroFrontal2", muroFrontalCollider,
		modelMatrixMuroFrontal2);

	//Collider muro derecho
	AbstractModel::OBB muroDerechoCollider;
	glm::mat4 modelmatrixColliderMuroDerecho = glm::mat4(modelMatrixMuroDerecho2);
	// Set the orientation of collider before doing the scale
	muroDerechoCollider.u = glm::quat_cast(modelmatrixColliderMuroDerecho);
	modelmatrixColliderMuroDerecho = glm::translate(modelmatrixColliderMuroDerecho,
		glm::vec3(muroDerecho2.getObb().c.x,
			muroDerecho2.getObb().c.y,
			muroDerecho2.getObb().c.z));
	muroDerechoCollider.e = glm::vec3(0.0f, 10.0f, 35.0f);
	muroDerechoCollider.c = glm::vec3(modelmatrixColliderMuroDerecho[3]);
	addOrUpdateColliders(collidersOBB2, "muroDerecho2", muroDerechoCollider,
		modelMatrixMuroDerecho2);

	//Collider muro izquierdo
	AbstractModel::OBB muroIzquierdoCollider;
	glm::mat4 modelmatrixColliderMuroIzquierdo = glm::mat4(modelMatrixMuroIzquierdo2);
	// Set the orientation of collider before doing the scale
	muroIzquierdoCollider.u = glm::quat_cast(modelmatrixColliderMuroIzquierdo);
	modelmatrixColliderMuroIzquierdo = glm::translate(modelmatrixColliderMuroIzquierdo,
		glm::vec3(muroIzquierdo2.getObb().c.x,
			muroIzquierdo2.getObb().c.y,
			muroIzquierdo2.getObb().c.z));
	muroIzquierdoCollider.e = glm::vec3(0.0f, 10.0f, 35.0f);
	muroIzquierdoCollider.c = glm::vec3(modelmatrixColliderMuroIzquierdo[3]);
	addOrUpdateColliders(collidersOBB2, "muroIzquierdo2", muroIzquierdoCollider,
		modelMatrixMuroIzquierdo2);

	//Render colliders
	for (std::map<std::string,
		std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
		collidersOBB2.begin(); it != collidersOBB2.end(); it++) {
		glm::mat4 matrixCollider = glm::mat4(1.0);
		matrixCollider = glm::translate(matrixCollider,
			std::get<0>(it->second).c);
		matrixCollider = matrixCollider
			* glm::mat4(std::get<0>(it->second).u);
		matrixCollider = glm::scale(matrixCollider,
			std::get<0>(it->second).e * 2.0f);
		boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
		boxCollider.enableWireMode();
		boxCollider.render(matrixCollider);
	}

	/*******************************************
	 * Test Colisions
	 *******************************************/
	bool isCollisionG = false;
	bool isCollisionEnemy = false;

	for (std::map<std::string,
		std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
		collidersOBB2.begin(); it != collidersOBB2.end(); it++) {
		bool isCollision = false;
		for (std::map<std::string,
			std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
			collidersOBB2.begin(); jt != collidersOBB2.end(); jt++) {
			if (it != jt
				&& testOBBOBB(std::get<0>(it->second),
					std::get<0>(jt->second))) {
				if (!(excepCollider2(it->first, jt->first)))
				{
					std::cout << "Colision " << it->first << " with "
						<< jt->first << std::endl;

					if ((it->first.compare("mayow") == 0 || it->first.compare("astroProta") == 0)
						&& (jt->first.compare("mayow") == 0 || jt->first.compare("astroProta") == 0))
						isCollisionEnemy = true;

					isCollision = true;
					isCollisionG = true;
				}
			}
		}
		addOrUpdateCollisionDetection(collisionDetection, it->first,
			isCollision);
	}

	if (isCollisionG && actionE && !enableEscotilla1) {
		std::cout << "EntraISC " << std::endl;
		updateBotonCollider(collisionDetection);
		actionE = false;
	}

	for (std::map<std::string,
		std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
		collidersSBB2.begin(); it != collidersSBB2.end(); it++) {
		bool isCollision = false;
		for (std::map<std::string,
			std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator jt =
			collidersSBB2.begin(); jt != collidersSBB2.end(); jt++) {
			if (it != jt
				&& testSphereSphereIntersection(std::get<0>(it->second),
					std::get<0>(jt->second))) {
				//std::cout << "Colision " << it->first << " with "
				//		<< jt->first << std::endl;
				isCollision = true;
			}
		}
		addOrUpdateCollisionDetection(collisionDetection, it->first,
			isCollision);
	}

	for (std::map<std::string,
		std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
		collidersSBB2.begin(); it != collidersSBB2.end(); it++) {
		bool isCollision = false;
		std::map<std::string,
			std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
			collidersOBB2.begin();
		for (; jt != collidersOBB2.end(); jt++) {
			if (testSphereOBox(std::get<0>(it->second), std::get<0>(jt->second))) {
				std::cout << "Colision " << it->first << " with "
					<< jt->first << std::endl;

				isCollision = true;
				addOrUpdateCollisionDetection(collisionDetection, jt->first,
					isCollision);
			}
		}
		addOrUpdateCollisionDetection(collisionDetection, it->first,
			isCollision);
	}

	std::map<std::string, bool>::iterator colIt;
	for (colIt = collisionDetection.begin();
		colIt != collisionDetection.end(); colIt++) {
		std::map<std::string,
			std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
			collidersSBB2.find(colIt->first);
		std::map<std::string,
			std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
			collidersOBB2.find(colIt->first);
		if (it != collidersSBB2.end()) {
			if (!colIt->second)
				addOrUpdateColliders(collidersSBB2, it->first);
		}
		if (jt != collidersOBB2.end()) {
			if (!colIt->second) {
				addOrUpdateColliders(collidersOBB2, jt->first);
			}
			else {
				if (jt->first.compare("mayow") == 0) {
					if (isCollisionEnemy == true) {
						enemigo1.respawn = true;
						modelMatrixMayow = glm::translate(modelMatrixMayow, enemigo1.calculaReaparicion(enemigo1.origen, modelMatrixMayow[3]));
						playerRespawn = true;
						isCollisionEnemy = false;
					}
				}
				if (jt->first.compare("astroProta") == 0) {
					modelMatrixAstroProta = std::get<1>(jt->second);
				}
			}
		}
	}

}

void soundEscene2() {
	/****************************+
		 * Open AL sound data
		 */
		 // Listener for the Thris person camera
	listenerPos[0] = modelMatrixMayow[3].x;
	listenerPos[1] = modelMatrixMayow[3].y;
	listenerPos[2] = modelMatrixMayow[3].z;
	alListenerfv(AL_POSITION, listenerPos);

	glm::vec3 upModel = glm::normalize(modelMatrixMayow[1]);
	glm::vec3 frontModel = glm::normalize(modelMatrixMayow[2]);

	listenerOri[0] = frontModel.x;
	listenerOri[1] = frontModel.y;
	listenerOri[2] = frontModel.z;
	listenerOri[3] = upModel.x;
	listenerOri[4] = upModel.y;
	listenerOri[5] = upModel.z;

	// Listener for the First person camera
	/*listenerPos[0] = camera->getPosition().x;
	 listenerPos[1] = camera->getPosition().y;
	 listenerPos[2] = camera->getPosition().z;
	 alListenerfv(AL_POSITION, listenerPos);
	 listenerOri[0] = camera->getFront().x;
	 listenerOri[1] = camera->getFront().y;
	 listenerOri[2] = camera->getFront().z;
	 listenerOri[3] = camera->getUp().x;
	 listenerOri[4] = camera->getUp().y;
	 listenerOri[5] = camera->getUp().z;*/
	alListenerfv(AL_ORIENTATION, listenerOri);

	for (unsigned int i = 0; i < sourcesPlay.size(); i++) {
		if (sourcesPlay[i]) {
			sourcesPlay[i] = false;
			alSourcePlay(source[i]);
		}
	}
}

void cameraMove() {
	posterior = terrain.getXCoordTerrain(modelMatrixAstroProta[3][0]);
	int camaraXcoord = terrain.getXCoordTerrain(modelMatrixPivoteCam[3][0]);
	if (camaraXcoord < posterior) {
		if (terrain.getXCoordTerrain(modelMatrixPivoteCam[3][0]) < limiteDerecho && terrain.getXCoordTerrain(modelMatrixAstroProta[3][0]) > limiteIzquierdo)
			modelMatrixPivoteCam = glm::translate(modelMatrixPivoteCam,
				glm::vec3(-0.1, 0, 0.0));
	}
	if (camaraXcoord > posterior) {
		if (terrain.getXCoordTerrain(modelMatrixPivoteCam[3][0]) > limiteIzquierdo && terrain.getXCoordTerrain(modelMatrixAstroProta[3][0]) < limiteDerecho)
			modelMatrixPivoteCam = glm::translate(modelMatrixPivoteCam,
				glm::vec3(0.1, 0, 0.0));
	}
}

bool excepCollider(std::string string1, std::string string2) {
	if ((string1.compare("muroFondo") == 0 || string1.compare("muroFrontal") == 0 ||
		string1.compare("muroDerecho") == 0 || string1.compare("muroIzquierdo") == 0) &&
		(string2.compare("muroFondo") == 0 || string2.compare("muroFrontal") == 0 ||
			string2.compare("muroDerecho") == 0 || string2.compare("muroIzquierdo") == 0)) {
		return true;
	}
	for (int i = 0; i < botonesPos.size(); i++) {
		if ((string1.compare("botonBox-" + std::to_string(i)) == 0 || string1.compare("botonBox-Y" + std::to_string(i)) == 0 ||
			string1.compare("botonBox-B" + std::to_string(i)) == 0 || string1.compare("botonBox-R" + std::to_string(i)) == 0) &&
			(string2.compare("botonBox-" + std::to_string(i)) == 0 || string2.compare("botonBox-Y" + std::to_string(i)) == 0 ||
				string2.compare("botonBox-B" + std::to_string(i)) == 0 || string2.compare("botonBox-R" + std::to_string(i)) == 0)) {
			return true;
		}
	}
	return false;
}

bool excepCollider2(std::string string1, std::string string2) {
	if ((string1.compare("muroFondo2") == 0 || string1.compare("muroFrontal2") == 0 ||
		string1.compare("muroDerecho2") == 0 || string1.compare("muroIzquierdo2") == 0) &&
		(string2.compare("muroFondo2") == 0 || string2.compare("muroFrontal2") == 0 ||
			string2.compare("muroDerecho2") == 0 || string2.compare("muroIzquierdo2") == 0)) {
		return true;
	}
	for (int i = 0; i < botonesPos.size(); i++) {
		if ((string1.compare("botonBox-" + std::to_string(i)) == 0 || string1.compare("botonBox-Y" + std::to_string(i)) == 0 ||
			string1.compare("botonBox-B" + std::to_string(i)) == 0 || string1.compare("botonBox-R" + std::to_string(i)) == 0) &&
			(string2.compare("botonBox-" + std::to_string(i)) == 0 || string2.compare("botonBox-Y" + std::to_string(i)) == 0 ||
				string2.compare("botonBox-B" + std::to_string(i)) == 0 || string2.compare("botonBox-R" + std::to_string(i)) == 0)) {
			return true;
		}
	}
	return false;
}

void updateBotonCollider(std::map<std::string, bool> collisionDetection) {
	for (int i = 0; i < botonesPos.size(); i++) {
		if (collisionDetection.find("botonBox-Y" + std::to_string(i))->second) {
			std::cout << "collisionDetection " << collisionDetection.find("botonBox-Y" + std::to_string(i))->second << " de "
				<< "botonBox-Y" + std::to_string(i) << std::endl;
			//std::cout << "previo a update " << combBotones[i][0] << std::endl;
			combBotones[i][0] = !combBotones[i][0];
			//std::cout << "despues de update " << combBotones[i][0] << std::endl;
		}
		if (collisionDetection.find("botonBox-B" + std::to_string(i))->second) {
			std::cout << "collisionDetection " << collisionDetection.find("botonBox-B" + std::to_string(i))->second << " de "
				<< "botonBox-B" + std::to_string(i) << std::endl;
			combBotones[i][1] = !combBotones[i][1];
		}
		if (collisionDetection.find("botonBox-R" + std::to_string(i))->second) {
			std::cout << "collisionDetection " << collisionDetection.find("botonBox-R" + std::to_string(i))->second << " de "
				<< "botonBox-R" + std::to_string(i) << std::endl;
			combBotones[i][2] = !combBotones[i][2];
		}
	}
}

void updateEscenario1() {
	if (!enableEscotilla1) {
		if (combBotones[0][0] && !combBotones[0][1] && !combBotones[0][2]) {
			lucesBotones[0] = true;
			std::cout << "combBotones[0] " << lucesBotones[0] << std::endl;
		}
		else {
			lucesBotones[0] = false;
		}
		if (combBotones[2][0] && !combBotones[2][1] && combBotones[2][2]) {
			lucesBotones[2] = true;
			std::cout << "combBotones[2] " << lucesBotones[2] << std::endl;
		}
		else {
			lucesBotones[2] = false;
		}
		if (combBotones[3][0] && combBotones[3][1] && !combBotones[3][2]) {
			lucesBotones[3] = true;
			std::cout << "combBotones[3] " << lucesBotones[3] << std::endl;
		}
		else {
			lucesBotones[3] = false;
		}
		if (lucesBotones[0] && lucesBotones[2] && lucesBotones[3]) {
			enableEscotilla1 = true;
			std::cout << "enableEscotilla1 " << enableEscotilla1 << std::endl;
		}
	}
}

void updateEscenario2() {

}

int main(int argc, char** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}