#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

// contains new std::shuffle definition
#include <algorithm>
#include <random>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

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

GLFWwindow *window;

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

ShadowBox *shadowBox;

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

// Terrain model instance
Terrain terrain(-1, -1, 200, 16, "../Textures/heightmap.png");

GLuint textureCespedID, textureWallID, textureWindowID, textureHighwayID,
		textureLandingPadID;
GLuint textureTerrainBackgroundID, textureTerrainRID, textureTerrainGID,
		textureTerrainBID, textureTerrainBlendMapID;
GLuint textureParticleFountainID, textureParticleFireID, texId;
GLuint skyboxTextureID;

// Modelo para el redener de texto
FontTypeRendering::FontTypeRendering *modelText;

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
glm::mat4 modelMatrixMuroFondo = glm::mat4(1.0f);
glm::mat4 modelMatrixMuroFrontal = glm::mat4(1.0f);
glm::mat4 modelMatrixMuroIzquierdo = glm::mat4(1.0f);
glm::mat4 modelMatrixMuroDerecho = glm::mat4(1.0f);
glm::mat4 modelMatrixCompuerta = glm::mat4(1.0f);
glm::mat4 modelMatrixEdCompuerta = glm::mat4(1.0f);
glm::mat4 modelMatrixPlataforma = glm::mat4(1.0f);

int animationIndex = 1;
int modelSelected = 2;
bool enableCountSelected = true;
int limiteIzquierdo = 132 - 20;
int limiteDerecho = 132 + 20;
int pasado = 0;
int posterior = 0;
int cameraSelected = 0;
bool enableCameraSelected = true;

// Lamps positions
std::vector<glm::vec3> lamp1Position = { glm::vec3(-7.03, 0, -19.14), glm::vec3(
		24.41, 0, -34.57), glm::vec3(-10.15, 0, -54.10) };
std::vector<float> lamp1Orientation = { -17.0, -82.67, 23.70 };
std::vector<glm::vec3> lamp2Position = { glm::vec3(-36.52, 0, -23.24),
		glm::vec3(-52.73, 0, -3.90) };
std::vector<float> lamp2Orientation = { 21.37 + 90, -65.0 + 90 };

//Posición botones y plataformas
std::vector<glm::vec3> botonesPos = { glm::vec3(10, 0, 10), glm::vec3(
		20, 0, 10), glm::vec3(30, 0, 10), glm::vec3(40, 0, 10)};

std::vector<glm::vec3> generadorPos = { glm::vec3(10, 0, 10), glm::vec3(
		20, 0, 10), glm::vec3(30, 0, 10), glm::vec3(40, 0, 10) };

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

// Colliders
std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > collidersOBB;
std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> > collidersSBB;

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
ALvoid *data;
int ch;
ALboolean loop;
std::vector<bool> sourcesPlay = { true, true, true };

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void initParticleBuffers();
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);
void prepareScene();
void prepareDepthScene();
void renderScene(bool renderParticles = true);
void cameraMove();

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
	GLfloat *data = new GLfloat[nParticles * 3];
	for (unsigned int i = 0; i < nParticles; i++) {

		theta = glm::mix(0.0f, glm::pi<float>() / 6.0f,
				((float) rand() / RAND_MAX));
		phi = glm::mix(0.0f, glm::two_pi<float>(), ((float) rand() / RAND_MAX));

		v.x = sinf(theta) * cosf(phi);
		v.y = cosf(theta);
		v.z = sinf(theta) * sinf(phi);

		velocity = glm::mix(0.6f, 0.8f, ((float) rand() / RAND_MAX));
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
	auto rng = std::default_random_engine { };
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

	////Lamp models
	//modelLamp1.loadModel("../models/Street-Lamp-Black/objLamp.obj");
	//modelLamp1.setShader(&shaderMulLighting);
	//modelLamp2.loadModel("../models/Street_Light/Lamp.obj");
	//modelLamp2.setShader(&shaderMulLighting);
	//modelLampPost2.loadModel("../models/Street_Light/LampPost.obj");
	//modelLampPost2.setShader(&shaderMulLighting);

	////Grass
	//modelGrass.loadModel("../models/grass/grassModel.obj");
	//modelGrass.setShader(&shaderMulLighting);

	//Botones
	modelBotones.loadModel("../models/botones/Botones.fbx");
	modelBotones.setShader(&shaderMulLighting);

	//Compuerta
	modelCompuerta.loadModel("../models/compuerta/Compuerta.fbx");
	modelCompuerta.setShader(&shaderMulLighting);

	//Edificio compuerta
	modelEdCompuerta.loadModel("../models/edCompuerta/EdificioCompuerta.fbx");
	modelEdCompuerta.setShader(&shaderMulLighting);

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

	cameraFP->setPosition(glm::vec3(0.0, 3.0, 4.0));
	camera->setDistanceFromTarget(distanceFromTarget);
	camera->setSensitivity(1.0);

	// Definimos el tamanio de la imagen
	int imageWidth, imageHeight;
	FIBITMAP *bitmap;
	unsigned char *data;

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
		FIBITMAP *bitmap = skyboxTexture.loadImage(true);
		unsigned char *data = skyboxTexture.convertToData(bitmap, imageWidth,
				imageHeight);
		if (data) {
			glTexImage2D(types[i], 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		} else
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
	} else
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
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainR.freeImage(bitmap);

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
	} else
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
	} else
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
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainBlendMap.freeImage(bitmap);

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
	} else
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
	} else
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
	/*modelLamp1.destroy();
	modelLamp2.destroy();
	modelLampPost2.destroy();
	modelGrass.destroy();*/
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

	// Custom objects animate
	mayowModelAnimate.destroy();

	// Textures Delete
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textureCespedID);
	glDeleteTextures(1, &textureWallID);
	glDeleteTextures(1, &textureWindowID);
	glDeleteTextures(1, &textureHighwayID);
	glDeleteTextures(1, &textureLandingPadID);
	glDeleteTextures(1, &textureTerrainBackgroundID);
	glDeleteTextures(1, &textureTerrainRID);
	glDeleteTextures(1, &textureTerrainGID);
	glDeleteTextures(1, &textureTerrainBID);
	glDeleteTextures(1, &textureTerrainBlendMapID);
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

void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
	distanceFromTarget -= yoffset;
	camera->setDistanceFromTarget(distanceFromTarget);
	lastDistanceFromTarget = distanceFromTarget;
}

void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod) {
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

	if (glfwJoystickPresent(GLFW_JOYSTICK_1) == GL_TRUE) {
		std::cout << "Esta presente el joystick" << std::endl;
		int axesCount, buttonCount;
		const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
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

		const unsigned char *buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1,
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

	// Seleccionar modelo
	/*if (enableCountSelected && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
		enableCountSelected = false;
		modelSelected++;
		if (modelSelected > 2)
			modelSelected = 0;
		std::cout << "modelSelected:" << modelSelected << std::endl;
	} else if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
		enableCountSelected = true;*/

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

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		animationIndex = 2;
	}

	if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		modelMatrixAstroProta = glm::rotate(modelMatrixAstroProta, glm::radians(3.5f),
				glm::vec3(0, 1, 0));
		animationIndex = 0;
	} else if (modelSelected
			== 2&& glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		modelMatrixAstroProta = glm::rotate(modelMatrixAstroProta, glm::radians(-3.5f),
				glm::vec3(0, 1, 0));
		animationIndex = 0;
	}
	if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		pasado = terrain.getXCoordTerrain(modelMatrixAstroProta[3][0]);
		modelMatrixAstroProta = glm::translate(modelMatrixAstroProta,
				glm::vec3(0.0, 0.0, 0.1));
		animationIndex = 0;
		
		std::cout << "modelMatrixPivote.x: " << terrain.getXCoordTerrain(modelMatrixPivoteCam[3][0]) << std::endl;
		std::cout << "modelMatrixAstro.x: " << terrain.getXCoordTerrain(modelMatrixAstroProta[3][0]) << std::endl;
		

		cameraMove();
	} else if (modelSelected
			== 2&& glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		pasado = terrain.getXCoordTerrain(modelMatrixAstroProta[3][0]);
		modelMatrixAstroProta = glm::translate(modelMatrixAstroProta,
				glm::vec3(0.0, 0.0, -0.1));
		animationIndex = 0;

		std::cout << "modelMatrixPivote.x: " << terrain.getXCoordTerrain(modelMatrixPivoteCam[3][0])<< std::endl;
		std::cout << "modelMatrixAstro.x: " << terrain.getXCoordTerrain(modelMatrixAstroProta[3][0]) << std::endl;
		cameraMove();
	}



	bool keySpaceStatus = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
	if(!isJump && keySpaceStatus){
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

	modelMatrixPivoteCam = glm::translate(modelMatrixPivoteCam,
		glm::vec3(0.0f, 5.0f, 0.0f));

	modelMatrixMayow = glm::translate(modelMatrixMayow,
			glm::vec3(13.0f, 0.05f, -5.0f));
	modelMatrixMayow = glm::rotate(modelMatrixMayow, glm::radians(-90.0f),
			glm::vec3(0, 1, 0));

	modelMatrixAstroProta = glm::translate(modelMatrixAstroProta,
		glm::vec3(0.0f, 0.0f, 0.0f));
	/*modelMatrixAstroProta = glm::rotate(modelMatrixAstroProta, glm::radians(-90.0f),
		glm::vec3(1, 0, 0));*/

	modelMatrixFountain = glm::translate(modelMatrixFountain,
			glm::vec3(5.0, 0.0, -40.0));
	modelMatrixFountain[3][1] = terrain.getHeightTerrain(
			modelMatrixFountain[3][0], modelMatrixFountain[3][2]) + 0.2;
	modelMatrixFountain = glm::scale(modelMatrixFountain,
			glm::vec3(10.0f, 10.0f, 10.0f));

	//Posicion de los muros
	modelMatrixMuroFondo = glm::translate(modelMatrixMuroFondo, 
		glm::vec3(0.0f, 0.0f, 50.0f));
	modelMatrixMuroFondo = glm::scale(modelMatrixMuroFondo,
		glm::vec3(75.0f, 20.0f, 0.0f));

	modelMatrixMuroFrontal = glm::translate(modelMatrixMuroFrontal,
		glm::vec3(0.0f, 0.0f, -20.0f));
	modelMatrixMuroFrontal = glm::scale(modelMatrixMuroFrontal,
		glm::vec3(75.0f, 20.0f, 0.0f));

	modelMatrixMuroDerecho = glm::translate(modelMatrixMuroDerecho,
		glm::vec3(35.0f, 0.0f, 15.0f));
	modelMatrixMuroDerecho = glm::scale(modelMatrixMuroDerecho,
		glm::vec3(0.0f, 20.0f, 70.0f));

	modelMatrixMuroIzquierdo = glm::translate(modelMatrixMuroIzquierdo,
		glm::vec3(-35.0f, 0.0f, 15.0f));
	modelMatrixMuroIzquierdo = glm::scale(modelMatrixMuroIzquierdo,
		glm::vec3(0.0f, 20.0f, 70.0f));

	lastTime = TimeManager::Instance().GetTime();

	// Time for the particles animation
	currTimeParticlesAnimation = lastTime;
	lastTimeParticlesAnimation = lastTime;

	currTimeParticlesAnimationFire = lastTime;
	lastTimeParticlesAnimationFire = lastTime;

	glm::vec3 lightPos = glm::vec3(10.0, 10.0, 0.0);

	shadowBox = new ShadowBox(-lightPos, camera.get(), 15.0f, 0.1f, 45.0f);

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

		std::map<std::string, bool> collisionDetection;

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
				(float) screenWidth / (float) screenHeight, 0.1f, 100.0f);

		/*if (modelSelected == 1) {
			axis = glm::axis(glm::quat_cast(modelMatrixPivoteCam));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixPivoteCam));
			target = modelMatrixPivoteCam[3];
		} else {
			axis = glm::axis(glm::quat_cast(modelMatrixMayow));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixMayow));
			target = modelMatrixMayow[3];
		}*/

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
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con skybox
		shaderSkybox.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false,
				glm::value_ptr(glm::mat4(glm::mat3(view))));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false, glm::value_ptr(view));
		shaderMulLighting.setMatrix4("lightSpaceMatrix", 1, false,
				glm::value_ptr(lightSpaceMatrix));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderTerrain.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderTerrain.setMatrix4("view", 1, false, glm::value_ptr(view));
		shaderTerrain.setMatrix4("lightSpaceMatrix", 1, false,
				glm::value_ptr(lightSpaceMatrix));
		// Settea la matriz de vista y projection al shader para el fountain
		shaderParticlesFountain.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderParticlesFountain.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		// Settea la matriz de vista y projection al shader para el fuego
		shaderParticlesFire.setInt("Pass", 2);
		shaderParticlesFire.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderParticlesFire.setMatrix4("view", 1, false, glm::value_ptr(view));

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
		renderScene();
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
			glm::mat4 modelMatrixColliderBoton = glm::mat4(1.0);
			modelMatrixColliderBoton = glm::translate(modelMatrixColliderBoton,
				botonesPos[i]);
			//modelMatrixColliderBoton = glm::rotate(modelMatrixColliderBoton,
			//		glm::radians(lamp2Orientation[i]), glm::vec3(0, 1, 0));
			addOrUpdateColliders(collidersOBB, "botonBox-" + std::to_string(i),
				botonCollider, modelMatrixColliderBoton);
			// Set the orientation of collider before doing the scale
			botonCollider.u = glm::quat_cast(modelMatrixColliderBoton);
			modelMatrixColliderBoton = glm::scale(modelMatrixColliderBoton,
					glm::vec3(1.0, 1.0, 1.0));
			modelMatrixColliderBoton = glm::translate(modelMatrixColliderBoton,
					modelBotones.getObb().c);
			botonCollider.c = glm::vec3(modelMatrixColliderBoton[3]);
			botonCollider.e = modelBotones.getObb().e
					* glm::vec3(1.0, 1.0, 1.0);
			std::get<0>(collidersOBB.find("botonBox-" + std::to_string(i))->second) =
				botonCollider;
		}

		//Botones generadores
		for (int i = 0; i < generadorPos.size(); i++) {
			AbstractModel::OBB generadorCollider;
			glm::mat4 modelMatrixColliderGenerador = glm::mat4(1.0);
			modelMatrixColliderGenerador = glm::translate(modelMatrixColliderGenerador,
				generadorPos[i]);
			//modelMatrixColliderBoton = glm::rotate(modelMatrixColliderBoton,
			//		glm::radians(lamp2Orientation[i]), glm::vec3(0, 1, 0));
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
		muroFondoCollider.e = glm::vec3(37.50f, 10.0f, 0.0f);
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
		muroFrontalCollider.e = glm::vec3(37.50f, 10.0f, 0.0f);
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
					if (!((it->first.compare("muroFondo") == 0 || it->first.compare("muroFrontal") == 0 ||
						it->first.compare("muroDerecho") == 0 || it->first.compare("muroIzquierdo") == 0) &&
						(jt->first.compare("muroFondo") == 0 || jt->first.compare("muroFrontal") == 0 ||
							jt->first.compare("muroDerecho") == 0 || jt->first.compare("muroIzquierdo") == 0)))
					{
						std::cout << "Colision " << it->first << " with "
							<< jt->first << std::endl;
						isCollision = true;
					}
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first,
					isCollision);
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
					std::cout << "Colision " << it->first << " with "
							<< jt->first << std::endl;
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
				if (testSphereOBox(std::get<0>(it->second),
						std::get<0>(jt->second))) {
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
				if (!colIt->second)
					addOrUpdateColliders(collidersOBB, jt->first);
				else {
					if (jt->first.compare("mayow") == 0)
						modelMatrixMayow = std::get<1>(jt->second);
					if (jt->first.compare("astroProta") == 0)
						modelMatrixAstroProta = std::get<1>(jt->second);
					/*if (jt->first.compare("dart") == 0)
						modelMatrixDart = std::get<1>(jt->second);*/
				}
			}
		}

		// Constantes de animaciones
		/*rotHelHelY += 0.5;*/
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

		modelText->render("Texto en openGL", -1, 0);
		glfwSwapBuffers(window);

		/****************************+
		 * Open AL sound data
		 */
		source0Pos[0] = modelMatrixFountain[3].x;
		source0Pos[1] = modelMatrixFountain[3].y;
		source0Pos[2] = modelMatrixFountain[3].z;
		alSourcefv(source[0], AL_POSITION, source0Pos);

		/*source2Pos[0] = modelMatrixDart[3].x;
		source2Pos[1] = modelMatrixDart[3].y;
		source2Pos[2] = modelMatrixDart[3].z;
		alSourcefv(source[2], AL_POSITION, source2Pos);*/

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
}

void prepareScene() {

	skyboxSphere.setShader(&shaderSkybox);

	terrain.setShader(&shaderTerrain);

	////Lamp models
	//modelLamp1.setShader(&shaderMulLighting);
	//modelLamp2.setShader(&shaderMulLighting);
	//modelLampPost2.setShader(&shaderMulLighting);

	////Grass
	//modelGrass.setShader(&shaderMulLighting);

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

}

void prepareDepthScene() {

	skyboxSphere.setShader(&shaderDepth);

	terrain.setShader(&shaderDepth);

	////Lamp models
	//modelLamp1.setShader(&shaderDepth);
	//modelLamp2.setShader(&shaderDepth);
	//modelLampPost2.setShader(&shaderDepth);

	////Grass
	//modelGrass.setShader(&shaderDepth);

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
	glBindTexture(GL_TEXTURE_2D, textureWallID);
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

	//// Render the lamps
	//for (int i = 0; i < lamp1Position.size(); i++) {
	//	lamp1Position[i].y = terrain.getHeightTerrain(lamp1Position[i].x,
	//			lamp1Position[i].z);
	//	modelLamp1.setPosition(lamp1Position[i]);
	//	modelLamp1.setScale(glm::vec3(0.5, 0.5, 0.5));
	//	modelLamp1.setOrientation(glm::vec3(0, lamp1Orientation[i], 0));
	//	modelLamp1.render();
	//}

	/*for (int i = 0; i < lamp2Position.size(); i++) {
		lamp2Position[i].y = terrain.getHeightTerrain(lamp2Position[i].x,
				lamp2Position[i].z);
		modelLamp2.setPosition(lamp2Position[i]);
		modelLamp2.setScale(glm::vec3(1.0, 1.0, 1.0));
		modelLamp2.setOrientation(glm::vec3(0, lamp2Orientation[i], 0));
		modelLamp2.render();
		modelLampPost2.setPosition(lamp2Position[i]);
		modelLampPost2.setScale(glm::vec3(1.0, 1.0, 1.0));
		modelLampPost2.setOrientation(glm::vec3(0, lamp2Orientation[i], 0));
		modelLampPost2.render();
	}*/

	//// Grass
	//glDisable(GL_CULL_FACE);
	//glm::vec3 grassPosition = glm::vec3(0.0, 0.0, 0.0);
	//grassPosition.y = terrain.getHeightTerrain(grassPosition.x,
	//		grassPosition.z);
	//modelGrass.setPosition(grassPosition);
	//modelGrass.render();
	//glEnable(GL_CULL_FACE);

	// Fountain
	glDisable(GL_CULL_FACE);
	modelFountain.render(modelMatrixFountain);
	glEnable(GL_CULL_FACE);

	// Pivote cam
	glDisable(GL_CULL_FACE);
	//pivoteCam.render(modelMatrixPivoteCam);
	//muroFondo.render(modelMatrixMuroFondo);
	glEnable(GL_CULL_FACE);

	/*******************************************
	 * Custom Anim objects obj
	 *******************************************/
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
	mayowModelAnimate.setAnimationIndex(animationIndex);
	mayowModelAnimate.render(modelMatrixMayowBody);

	modelMatrixCompuerta[3][1] = terrain.getHeightTerrain(modelMatrixCompuerta[3][0],
		modelMatrixCompuerta[3][2]);
	glm::mat4 modelMatrixCompuertaBody = glm::mat4(modelMatrixCompuerta);
	modelMatrixCompuertaBody = glm::scale(modelMatrixCompuertaBody,
		glm::vec3(0.021, 0.021, 0.021));
	modelCompuerta.setAnimationIndex(animationIndex);
	modelCompuerta.render(modelMatrixCompuertaBody);

	for (int i = 0; i < botonesPos.size(); i++) {
		botonesPos[i].y = terrain.getHeightTerrain(botonesPos[i].x,
			botonesPos[i].z);
		modelBotones.setPosition(botonesPos[i]);
		modelBotones.setScale(glm::vec3(1.0, 1.0, 1.0));
		//modelBotones.setOrientation(glm::vec3(0, lamp2Orientation[i], 0));
		//modelBotones.render();
	}

	for (int i = 0; i < generadorPos.size(); i++) {
		generadorPos[i].y = terrain.getHeightTerrain(generadorPos[i].x,
			generadorPos[i].z);
		modelGenerador.setPosition(generadorPos[i]);
		modelGenerador.setScale(glm::vec3(1.0, 1.0, 1.0));
		//modelBotones.setOrientation(glm::vec3(0, lamp2Orientation[i], 0));
		modelGenerador.render();
	}

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
	// * Update the position with alpha objects
	// */
	//// Update the aircraft
	//blendingUnsorted.find("aircraft")->second = glm::vec3(
	//		modelMatrixAircraft[3]);
	//// Update the lambo
	//blendingUnsorted.find("lambo")->second = glm::vec3(modelMatrixLambo[3]);
	//// Update the helicopter
	//blendingUnsorted.find("heli")->second = glm::vec3(modelMatrixHeli[3]);

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

void cameraMove() {
	posterior = terrain.getXCoordTerrain(modelMatrixAstroProta[3][0]);
	int camaraXcoord = terrain.getXCoordTerrain(modelMatrixPivoteCam[3][0]);
	if (camaraXcoord < posterior) {
		if (terrain.getXCoordTerrain(modelMatrixPivoteCam[3][0]) < limiteDerecho && terrain.getXCoordTerrain(modelMatrixAstroProta[3][0]) > limiteIzquierdo)
			modelMatrixPivoteCam = glm::translate(modelMatrixPivoteCam,
				glm::vec3(0.1, 0, 0.0));
	}
	if (camaraXcoord > posterior) {
		if (terrain.getXCoordTerrain(modelMatrixPivoteCam[3][0]) > limiteIzquierdo && terrain.getXCoordTerrain(modelMatrixAstroProta[3][0]) < limiteDerecho)
			modelMatrixPivoteCam = glm::translate(modelMatrixPivoteCam,
				glm::vec3(-0.1, 0, 0.0));
	}
}

int main(int argc, char **argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
