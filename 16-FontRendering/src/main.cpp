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
Box boxCD;
Box boxCIS;
Box boxCII;
Box boxCS;

ShadowBox* shadowBox;

// Fountain
Model modelFountain;
// Model animate instance
Model enemyModelAnimate;
Model astroProta;
Model modelBotones;
Model modelGenerador;
Model modelCompuerta;
Model modelEdCompuerta;
Model modelPlataforma;
Model modelRokas;
Model modelRokas2;
Model modelLuzGenerador;
Model modelLuzBotones;

//Scene 2
Model modelEscenario2;
Model modelCuerpo;
Model modelCompu;
Model modelEstanteria;
//Model modelEstanteria2;
Model modelCajaCuadrada;
Model modelCajaLowPoly;
Model modelBidones;
Model modelPalanca;
Model modelPuertaIzq;
Model modelPuertaDer;
Model modelMarcoPuerta;

//Enemigos
Enemy enemigo1(glm::vec3(13.0f, 0.05f, -5.0f), "enemy1");
Enemy enemigo2(glm::vec3(13.0f, 0.05f, -5.0f), "enemy2");
Enemy enemigo3(glm::vec3(13.0f, 0.05f, -5.0f), "enemy3");

// Terrain model instance
Terrain terrain(-1, -1, 200, 14, "../Textures/AlturasMapa1.png");
Terrain terrain2(-1, -1, 200, 1, "../Textures/AlturasMapa2.png");

GLuint textureYellowID, textureBlueID, textureRedID, textureOrangeID,
textureGreenID, texturePurpleID;
GLuint textureTerrainBackgroundID, textureTerrainRID, textureTerrainGID,
textureTerrainBID, textureTerrainBlendMapID, textureTerrainBlendMapID2,
textureTerrainBackgroundID2, textureTerrainRID2, textureTerrainGID2,
textureTerrainBID2;
GLuint texId;
GLuint skyboxTextureID;
GLuint textureMenuID, textureMenu2ID, textureActivaID, textureTranID, textureEndID;

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

std::string fileNames[6] = { "../Textures/CubeMap/CubeMapUp.png",
		"../Textures/CubeMap/CubeMapUp.png",
		"../Textures/CubeMap/CubeMapUp.png",
		"../Textures/CubeMap/CubeMapDown.png",
		"../Textures/CubeMap/CubeMapUp.png",
		"../Textures/CubeMap/CubeMapUp.png" };

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

// Model matrix definitions
glm::mat4 modelMatrixPivoteCam = glm::mat4(1.0f);
glm::mat4 modelMatrixPivoteCam2 = glm::mat4(1.0f);
glm::mat4 modelMatrixEnemigo = glm::mat4(1.0f);
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
glm::mat4 modelMatrixBoxCD = glm::mat4(1.0f);
glm::mat4 modelMatrixBoxCIS = glm::mat4(1.0f);
glm::mat4 modelMatrixBoxCII = glm::mat4(1.0f);
glm::mat4 modelMatrixBoxCS = glm::mat4(1.0f);
glm::mat4 defaultMatrix = glm::mat4(1.0f);
glm::mat4 defaultMatrix2 = glm::mat4(1.0f);
glm::mat4 modelMatrixPipe = glm::mat4(1.0f);
glm::mat4 modelMatrixMarcoPuerta = glm::mat4(1.0f);
glm::mat4 modelMatrixPuertaDer = glm::mat4(1.0f);
glm::mat4 modelMatrixPuertaIzq = glm::mat4(1.0f);

//Matrix Definitios for Scene 2

glm::mat4 modelMatrixEscenario2 = glm::mat4(1.0f);
glm::mat4 modelMatrixEstanteria = glm::mat4(1.0f);

//Matrix Definitios for Scene 2
glm::mat4 modelMatrixCuerpoAmongus = glm::mat4(1.0f);
glm::mat4 modelMatrixCuerpoAmongus2 = glm::mat4(1.0f);
glm::mat4 modelMatrixCompu1 = glm::mat4(1.0f);
glm::mat4 modelMatrixCompu2 = glm::mat4(1.0f);
glm::mat4 modelMatrixEstanteria1 = glm::mat4(1.0f);
glm::mat4 modelMatrixEstanteria2 = glm::mat4(1.0f);
glm::mat4 modelMatrixCajaCuadrada = glm::mat4(1.0f);
glm::mat4 modelMatrixCajaCuadrada2 = glm::mat4(1.0f);
glm::mat4 modelMatrixCajaCuadrada3 = glm::mat4(1.0f);
glm::mat4 modelMatrixCajaCuadrada4 = glm::mat4(1.0f);
glm::mat4 modelMatrixCajaCuadrada5 = glm::mat4(1.0f);
glm::mat4 modelMatrixCajaLowPoly = glm::mat4(1.0f);
glm::mat4 modelMatrixCajaLowPoly2 = glm::mat4(1.0f);
glm::mat4 modelMatrixCajaLowPoly3 = glm::mat4(1.0f);
glm::mat4 modelMatrixCajaLowPoly4 = glm::mat4(1.0f);
glm::mat4 modelMatrixCajaLowPoly5 = glm::mat4(1.0f);
glm::mat4 modelMatrixCajaLowPoly6 = glm::mat4(1.0f);
glm::mat4 modelMatrixBidones = glm::mat4(1.0f);
glm::mat4 modelMatrixBidones2 = glm::mat4(1.0f);
glm::mat4 modelMatrixBidones3 = glm::mat4(1.0f);
glm::mat4 modelMatrixBidones4 = glm::mat4(1.0f);

//vectors
glm::vec3 astroPosition;
glm::vec3 astroOrigin = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 camPivOrigin = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 astroInitialOrientation = glm::vec3(1.0f, 0.0f, 0.0f);

float timer;
int banderaCaminar = 0;
bool fontbandera = false;
int animationIndex = 1;
int animationIndexEnemy = 0;
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
bool musicaIntro = true;
bool deControl = false;
int animationIndexEscotilla = 0;
bool primera = false;

bool escenario1 = true;
bool escenario2 = false;

glm::vec3 vectorDireccionEnemigo = glm::vec3(0.0f);
float anguloEntreDosVectores;

//Lógica primer escenario
std::vector<std::vector<bool>> combBotones = {
	{false, false, false},
	{false, false, false},
	{false, false, false},
	{false, false, false} };

std::vector<bool> lucesBotones = { false, false, false, false };

//Lógica segundo escenario
int indPalanca = 0;
std::vector<int> animaPalancas = { 0, 0, 0, 0 };
std::vector<bool> lucesPalancas = { false, false, false, false };
bool enablePuerta = false;
float movimientoPuerta = 0.0f;

//Posición botones y plataformas
std::vector<glm::vec3> botonesPos = { glm::vec3(-66.6, 0, -2), glm::vec3(
		-39.5, 0, 14.6), glm::vec3(34.57, 0, -3), glm::vec3(78, 0, 12) };

std::vector<glm::vec3> rokasPos = { glm::vec3(-64, 0, 18.75), glm::vec3(
		-11.32, 0, -5.06), glm::vec3(23.43, 0, -20.50), glm::vec3(59.76, 0, 1.367) };

// Palancas positions
std::vector<glm::vec3> palancaPos = { glm::vec3(-62.9, 1.5, -26.8), glm::vec3(
		-35.4, 1.5, -38.3), glm::vec3(27.92, 1.5, -27.25), glm::vec3(54.5, 1.5, -18.9) };
std::vector<float> palancaOrientation = { -90.0, 180.0, 180.0, 90.0};

std::vector<glm::vec3> lucesPuzzleEsc2 = { glm::vec3(-10.8, 1.4, -36.0), glm::vec3(
		-18.0, 1.4, -36.0), glm::vec3(-5.66, 1.4, -36.0)};

std::vector<glm::vec3> rokas2Pos = { glm::vec3(89.25, 0, -0.39), glm::vec3(
		73.242, 0, 21.875), glm::vec3(11.3281, 0, -3.51), glm::vec3(-3.71, 0, 27.929) };

std::vector<glm::vec3> edificios = { glm::vec3(15.5, 7.2, 13.1), glm::vec3(1.2f, 9.5f, 4.8f)};

// Blending model unsorted
std::map<std::string, glm::vec3> blendingUnsorted = { { "enemigo", glm::vec3(0.0) }};

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
#define NUM_BUFFERS 11
#define NUM_SOURCES 16
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
// Source 3
ALfloat source3Pos[] = { -2.0, 0.0, 0.0 };
ALfloat source3Vel[] = { 0.0, 0.0, 0.0 };
// Source 4
ALfloat source4Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source4Vel[] = { 0.0, 0.0, 0.0 };
// Source 5
ALfloat source5Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source5Vel[] = { 0.0, 0.0, 0.0 };
// Source 6
ALfloat source6Pos[] = { -2.0, 0.0, 0.0 };
ALfloat source6Vel[] = { 0.0, 0.0, 0.0 };
// Source 7
ALfloat source7Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source7Vel[] = { 0.0, 0.0, 0.0 };
// Source 8
ALfloat source8Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source8Vel[] = { 0.0, 0.0, 0.0 };
// Source 9
ALfloat source9Pos[] = { -2.0, 0.0, 0.0 };
ALfloat source9Vel[] = { 0.0, 0.0, 0.0 };
// Source 10
ALfloat source10Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source10Vel[] = { 0.0, 0.0, 0.0 };
// Source 11
ALfloat source11Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source11Vel[] = { 0.0, 0.0, 0.0 };
// Source 12
ALfloat source12Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source12Vel[] = { 0.0, 0.0, 0.0 };
// Source 13
ALfloat source13Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source13Vel[] = { 0.0, 0.0, 0.0 };
// Source 14
ALfloat source14Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source14Vel[] = { 0.0, 0.0, 0.0 };
// Source 15
ALfloat source15Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source15Vel[] = { 0.0, 0.0, 0.0 };
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
std::vector<bool> sourcesPlay = { true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };//Si quiero que se prenda al inicio
std::vector<bool> sourcesPlaying = { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };//Si quiero que se prenda al inicio


// Se definen todos las funciones.
void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow* window, int key, int scancode, int action,
	int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
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
void cameraMove2();
bool excepCollider(std::string string1, std::string string2);
bool excepCollider2(std::string string1, std::string string2);
void updateBotonCollider(std::map<std::string, bool> collisionDetection);
void updatePalancaCollider(std::map<std::string, bool> collisionDetection);
void updateEscenario1();
void updateEscenario2();
void preRender1();
void collidersManagmentEs1();
void soundEscene1();
void inicialMatrixs();
void updateEscenario2();
void preRender2();
void collidersManagmentEs2();
void soundEscene2();
glm::vec3 colorGenerador(std::vector<bool> combBotones);
void luzMenus();

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

	terrain2.init();
	terrain2.setShader(&shaderTerrain);
	terrain2.setPosition(glm::vec3(100, 0, 100));

	pivoteCam.init();
	pivoteCam.setShader(&shaderMulLighting);

	boxReferencia.init();
	boxReferencia.setShader(&shaderMulLighting);

	boxMenu.init();
	boxMenu.setShader(&shaderMulLighting);
	boxMenu.setPosition(glm::vec3(0.0, -6.0, 0.0));
	boxMenu.setScale(glm::vec3(38.0, 33.0, 1.0));
	boxMenu.setOrientation(glm::vec3(-25.0, 0.0, 0.0));

	////Rokas
	modelRokas.loadModel("../models/rokas/Piedras2.obj");
	modelRokas.setShader(&shaderMulLighting);
	modelRokas2.loadModel("../models/rokas/Piedras.obj");
	modelRokas2.setShader(&shaderMulLighting);

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

	//Plataforma
	modelPlataforma.loadModel("../models/plataforma/Plataforma.fbx");
	modelPlataforma.setShader(&shaderMulLighting);

	//Enemy
	enemyModelAnimate.loadModel("../models/Enemigo/Enemy.fbx");
	enemyModelAnimate.setShader(&shaderMulLighting);

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

	////Escenario2
	modelEscenario2.loadModel("../models/SegundoEscenario/Escenario2.obj");
	modelEscenario2.setShader(&shaderMulLighting);

	//Bidones
	modelBidones.loadModel("../models/SegundoEscenario/Bidones.obj");
	modelBidones.setShader(&shaderMulLighting);

	////Caja1
	modelCajaCuadrada.loadModel("../models/SegundoEscenario/Caja1.obj");
	modelCajaCuadrada.setShader(&shaderMulLighting);

	////Caja2
	modelCajaLowPoly.loadModel("../models/SegundoEscenario/Caja2.obj");
	modelCajaLowPoly.setShader(&shaderMulLighting);

	////Compu
	modelCompu.loadModel("../models/SegundoEscenario/Compu.obj");
	modelCompu.setShader(&shaderMulLighting);

	////Cuerpo
	modelCuerpo.loadModel("../models/SegundoEscenario/Cuerpo.obj");
	modelCuerpo.setShader(&shaderMulLighting);

	////Estanteria
	modelEstanteria.loadModel("../models/SegundoEscenario/Estanteria.obj");
	modelEstanteria.setShader(&shaderMulLighting);


	////Palanca
	modelPalanca.loadModel("../models/SegundoEscenario/Palanca.fbx");
	modelPalanca.setShader(&shaderMulLighting);

	////PuertaIzquierda
	modelPuertaIzq.loadModel("../models/SegundoEscenario/PuertaIzquierda.obj");
	modelPuertaIzq.setShader(&shaderMulLighting);

	////PuertaDerecha
	modelPuertaDer.loadModel("../models/SegundoEscenario/PuertaDerecha.obj");
	modelPuertaDer.setShader(&shaderMulLighting);

	////MarcoPuerta
	modelMarcoPuerta.loadModel("../models/SegundoEscenario/MarcoPuerta.obj");
	modelMarcoPuerta.setShader(&shaderMulLighting);

	//Box
	boxCD.init();
	boxCD.setShader(&shaderMulLighting);
	boxCIS.init();
	boxCIS.setShader(&shaderMulLighting);
	boxCII.init();
	boxCII.setShader(&shaderMulLighting);
	boxCS.init();
	boxCS.setShader(&shaderMulLighting);

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

	// Definiendo la textura a utilizar | Negro BlendMap2
	Texture textureTerrainBackground("../Textures/BlendMap/TierraColor.png");
	bitmap = textureTerrainBackground.loadImage();
	data = textureTerrainBackground.convertToData(bitmap, imageWidth,
		imageHeight);
	glGenTextures(1, &textureTerrainBackgroundID);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainBackground.freeImage(bitmap);

	// Definiendo la textura a utilizar | Negro BlendMap3
	Texture textureTerrainBackground2("../Textures/BlendMap/PisoFondoNegro.png");
	bitmap = textureTerrainBackground2.loadImage();
	data = textureTerrainBackground2.convertToData(bitmap, imageWidth,
		imageHeight);
	glGenTextures(1, &textureTerrainBackgroundID2);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainBackground2.freeImage(bitmap);

	// Definiendo la textura a utilizar | Rojo Blendmap 2
	Texture textureTerrainR("../Textures/BlendMap/Cables.png");
	bitmap = textureTerrainR.loadImage();
	data = textureTerrainR.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureTerrainRID);
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainR.freeImage(bitmap);

	// Definiendo la textura a utilizar | Rojo BlendMap 3
	Texture textureTerrainR2("../Textures/BlendMap/PisoRojo.png");
	bitmap = textureTerrainR2.loadImage();
	data = textureTerrainR2.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureTerrainRID2);
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainR2.freeImage(bitmap);

	// Definiendo la textura a utilizar | Verde BlendMap2
	Texture textureTerrainG("../Textures/BlendMap/RocasColor.png");
	bitmap = textureTerrainG.loadImage();
	data = textureTerrainG.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureTerrainGID);
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainG.freeImage(bitmap);

	// Definiendo la textura a utilizar | Verde BlendMap3
	Texture textureTerrainG2("../Textures/BlendMap/PisoVerde.png");
	bitmap = textureTerrainG2.loadImage();
	data = textureTerrainG2.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureTerrainGID2);
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainG2.freeImage(bitmap);

	// Definiendo la textura a utilizar | Azul BlendMap2
	Texture textureTerrainB("../Textures/BlendMap/Plano.png");
	bitmap = textureTerrainB.loadImage();
	data = textureTerrainB.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureTerrainBID);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainB.freeImage(bitmap);

	// Definiendo la textura a utilizar | Azul BlendMap3
	Texture textureTerrainB2("../Textures/BlendMap/PisoAzul.png");
	bitmap = textureTerrainB2.loadImage();
	data = textureTerrainB2.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureTerrainBID2);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainB2.freeImage(bitmap);

	// Definiendo la textura a utilizar para el INICIO
	Texture textureMenu1("../Textures/Transiciones/Menu.png");
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

	// Definiendo la textura a utilizar
	Texture textureTran("../Textures/Transicion.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTran.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTran.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTranID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTranID);
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
	textureTran.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureEnd("../Textures/FinalGracias.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureEnd.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTran.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureEndID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureEndID);
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
	textureEnd.freeImage(bitmap);

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
	 alutInit(0, nullptr);
	 alListenerfv(AL_POSITION, listenerPos);
	 alListenerfv(AL_VELOCITY, listenerVel);
	 alListenerfv(AL_ORIENTATION, listenerOri);
	 alGetError(); // clear any error messages
	 if (alGetError() != AL_NO_ERROR) {
	 	printf("- Error creating buffers !!\n");
	 	exit(1);
	 } else {
	 	printf("init() - No errors yet.");
	 }
	 // Config source 0
	 // Generate buffers, or else no sound will happen!
	 alGenBuffers(NUM_BUFFERS, buffer);
	 buffer[0] = alutCreateBufferFromFile("../sounds/SpaceMision.wav");
	 buffer[1] = alutCreateBufferFromFile("../sounds/PisadaExterior.wav");
	 buffer[2] = alutCreateBufferFromFile("../sounds/PisadasInterior.wav");
	 buffer[3] = alutCreateBufferFromFile("../sounds/Puerta.wav");
	 buffer[4] = alutCreateBufferFromFile("../sounds/Generador.wav");
	 buffer[5] = alutCreateBufferFromFile("../sounds/Interruptor.wav");
	 buffer[6] = alutCreateBufferFromFile("../sounds/MovimientoAlien.wav");
	 buffer[7] = alutCreateBufferFromFile("../sounds/AmbientalExterior.wav");
	 buffer[8] = alutCreateBufferFromFile("../sounds/AmbientalInterior.wav");
	 buffer[9] = alutCreateBufferFromFile("../sounds/ErrorCodigo.wav");
	 buffer[10] = alutCreateBufferFromFile("../sounds/GameOver.wav");

	 int errorAlut = alutGetError();
	 if (errorAlut != ALUT_ERROR_NO_ERROR) {
	 	printf("- Error open files with alut %d !!\n", errorAlut);
	 	exit(2);
	 }

	 alGetError(); /* clear error */
	 alGenSources(NUM_SOURCES, source);

	 if (alGetError() != AL_NO_ERROR) {
	 	printf("- Error creating sources !!\n");
	 	exit(2);
	 } else {
	 	printf("init - no errors after alGenSources\n");
	 }
	 // Musica Intro
	 alSourcef(source[0], AL_PITCH, 1.0f);//PITCH
	 alSourcef(source[0], AL_GAIN, 1.0f);//GANANCIA
	 alSourcefv(source[0], AL_POSITION, source0Pos);//Posición
	 alSourcefv(source[0], AL_VELOCITY, source0Vel);//Velocidad
	 alSourcei(source[0], AL_BUFFER, buffer[0]);//De donde va a tomar el sonido
	 alSourcei(source[0], AL_LOOPING, AL_TRUE);//Loop
	 alSourcef(source[0], AL_MAX_DISTANCE, 2000);//DISTANCIA A LA QUE SE ESCUCHA
	 //Pisadas Exterior
	 alSourcef(source[1], AL_PITCH, 1.0f);
	 alSourcef(source[1], AL_GAIN, 0.7f);
	 alSourcefv(source[1], AL_POSITION, source1Pos);
	 alSourcefv(source[1], AL_VELOCITY, source1Vel);
	 alSourcei(source[1], AL_BUFFER, buffer[1]);
	 alSourcei(source[1], AL_LOOPING, AL_TRUE);
	 alSourcef(source[1], AL_MAX_DISTANCE, 1000);
	 //Pisadas interior
	 alSourcef(source[2], AL_PITCH, 1.0f);
	 alSourcef(source[2], AL_GAIN, 1.3f);
	 alSourcefv(source[2], AL_POSITION, source2Pos);
	 alSourcefv(source[2], AL_VELOCITY, source2Vel);
	 alSourcei(source[2], AL_BUFFER, buffer[2]);
	 alSourcei(source[2], AL_LOOPING, AL_TRUE);
	 alSourcef(source[2], AL_MAX_DISTANCE, 1000);
	 //Puerta y compuerta
	 alSourcef(source[3], AL_PITCH, 1.0f);
	 alSourcef(source[3], AL_GAIN, 2.0f);
	 alSourcefv(source[3], AL_POSITION, source3Pos);
	 alSourcefv(source[3], AL_VELOCITY, source3Vel);
	 alSourcei(source[3], AL_BUFFER, buffer[3]);
	 alSourcei(source[3], AL_LOOPING, AL_TRUE);
	 alSourcef(source[3], AL_MAX_DISTANCE, 3000);

	 alSourcef(source[4], AL_PITCH, 1.0f);
	 alSourcef(source[4], AL_GAIN, 2.0f);
	 alSourcefv(source[4], AL_POSITION, source4Pos);
	 alSourcefv(source[4], AL_VELOCITY, source4Vel);
	 alSourcei(source[4], AL_BUFFER, buffer[3]);
	 alSourcei(source[4], AL_LOOPING, AL_FALSE);
	 alSourcef(source[4], AL_MAX_DISTANCE, 2000);

	 //Generadores
	 alSourcef(source[5], AL_PITCH, 1.0f);
	 alSourcef(source[5], AL_GAIN, 3.0f);
	 alSourcefv(source[5], AL_POSITION, source5Pos);
	 alSourcefv(source[5], AL_VELOCITY, source5Vel);
	 alSourcei(source[5], AL_BUFFER, buffer[4]);
	 alSourcei(source[5], AL_LOOPING, AL_TRUE);
	 alSourcef(source[5], AL_MAX_DISTANCE, 50);

	 alSourcef(source[6], AL_PITCH, 1.0f);
	 alSourcef(source[6], AL_GAIN, 3.0f);
	 alSourcefv(source[6], AL_POSITION, source6Pos);
	 alSourcefv(source[6], AL_VELOCITY, source6Vel);
	 alSourcei(source[6], AL_BUFFER, buffer[4]);
	 alSourcei(source[6], AL_LOOPING, AL_TRUE);
	 alSourcef(source[6], AL_MAX_DISTANCE, 50);

	 alSourcef(source[7], AL_PITCH, 1.0f);
	 alSourcef(source[7], AL_GAIN, 3.0f);
	 alSourcefv(source[7], AL_POSITION, source7Pos);
	 alSourcefv(source[7], AL_VELOCITY, source7Vel);
	 alSourcei(source[7], AL_BUFFER, buffer[4]);
	 alSourcei(source[7], AL_LOOPING, AL_TRUE);
	 alSourcef(source[7], AL_MAX_DISTANCE, 50);

	 //Interruptores
	 alSourcef(source[8], AL_PITCH, 1.0f);
	 alSourcef(source[8], AL_GAIN, 1.8f);
	 alSourcefv(source[8], AL_POSITION, source8Pos);
	 alSourcefv(source[8], AL_VELOCITY, source8Vel);
	 alSourcei(source[8], AL_BUFFER, buffer[5]);
	 alSourcei(source[8], AL_LOOPING, AL_FALSE);
	 alSourcef(source[8], AL_MAX_DISTANCE, 1000);

	 alSourcef(source[9], AL_PITCH, 1.0f);
	 alSourcef(source[9], AL_GAIN, 1.8f);
	 alSourcefv(source[9], AL_POSITION, source9Pos);
	 alSourcefv(source[9], AL_VELOCITY, source9Vel);
	 alSourcei(source[9], AL_BUFFER, buffer[5]);
	 alSourcei(source[9], AL_LOOPING, AL_FALSE);
	 alSourcef(source[9], AL_MAX_DISTANCE, 1000);

	 alSourcef(source[10], AL_PITCH, 1.0f);
	 alSourcef(source[10], AL_GAIN, 1.8f);
	 alSourcefv(source[10], AL_POSITION, source10Pos);
	 alSourcefv(source[10], AL_VELOCITY, source10Vel);
	 alSourcei(source[10], AL_BUFFER, buffer[5]);
	 alSourcei(source[10], AL_LOOPING, AL_FALSE);
	 alSourcef(source[10], AL_MAX_DISTANCE, 1000);

	 //MovimientoAlien
	 alSourcef(source[11], AL_PITCH, 1.0f);
	 alSourcef(source[11], AL_GAIN, 2.0f);
	 alSourcefv(source[11], AL_POSITION, source11Pos);
	 alSourcefv(source[11], AL_VELOCITY, source11Vel);
	 alSourcei(source[11], AL_BUFFER, buffer[6]);
	 alSourcei(source[11], AL_LOOPING, AL_TRUE);
	 alSourcef(source[11], AL_MAX_DISTANCE, 1500);

	 //AmbientalExterior
	 alSourcef(source[12], AL_PITCH, 1.0f);
	 alSourcef(source[12], AL_GAIN, 2.3f);
	 alSourcefv(source[12], AL_POSITION, source12Pos);
	 alSourcefv(source[12], AL_VELOCITY, source12Vel);
	 alSourcei(source[12], AL_BUFFER, buffer[7]);
	 alSourcei(source[12], AL_LOOPING, AL_TRUE);
	 alSourcef(source[12], AL_MAX_DISTANCE, 1000);

	 //AmbientalInterior
	 alSourcef(source[13], AL_PITCH, 1.0f);
	 alSourcef(source[13], AL_GAIN, 5.0f);
	 alSourcefv(source[13], AL_POSITION, source13Pos);
	 alSourcefv(source[13], AL_VELOCITY, source13Vel);
	 alSourcei(source[13], AL_BUFFER, buffer[8]);
	 alSourcei(source[13], AL_LOOPING, AL_TRUE);
	 alSourcef(source[13], AL_MAX_DISTANCE, 2000);
	 //ErrorCódigo
	 alSourcef(source[14], AL_PITCH, 1.0f);
	 alSourcef(source[14], AL_GAIN, 2.0f);
	 alSourcefv(source[14], AL_POSITION, source14Pos);
	 alSourcefv(source[14], AL_VELOCITY, source14Vel);
	 alSourcei(source[14], AL_BUFFER, buffer[9]);
	 alSourcei(source[14], AL_LOOPING, AL_FALSE);
	 alSourcef(source[14], AL_MAX_DISTANCE, 2000);
	 //GameOver
	 alSourcef(source[15], AL_PITCH, 1.0f);
	 alSourcef(source[15], AL_GAIN, 2.0f);
	 alSourcefv(source[15], AL_POSITION, source2Pos);
	 alSourcefv(source[15], AL_VELOCITY, source2Vel);
	 alSourcei(source[15], AL_BUFFER, buffer[10]);
	 alSourcei(source[15], AL_LOOPING, AL_FALSE);
	 alSourcef(source[15], AL_MAX_DISTANCE, 2000);
	 
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

	 //--------- IMPORTANTE ----------
	 //Eliminar los shader y buffers creados.

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
	modelRokas.destroy();
	modelRokas2.destroy();
	modelLuzBotones.destroy();
	modelLuzGenerador.destroy();
	boxReferencia.destroy();
	boxMenu.destroy();
	boxCD.destroy();
	boxCII.destroy();
	boxCIS.destroy();
	boxCS.destroy();
	modelPalanca.destroy();
	modelPuertaIzq.destroy();
	modelPuertaDer.destroy();
	modelMarcoPuerta.destroy();

	// Custom objects animate
	enemyModelAnimate.destroy();

	//modelos segundo escenario
	modelEscenario2.destroy();
	modelBidones.destroy();

	modelCajaCuadrada.destroy();

	modelCajaLowPoly.destroy();

	modelCompu.destroy();

	modelCuerpo.destroy();

	modelEstanteria.destroy();


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
	glDeleteTextures(1, &textureEndID);
	glDeleteTextures(1, &textureTranID);

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
	if (!empiezaJuego || cambianivel2 || cambianivel3) {
		bool pressEnter = glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS;
		if (pressEnter) {
			empiezaJuego = true;
			cambianivel2 = false;
			if (cambianivel3) {
				exitApp = true;
			}
			//sourcesPlay[0] = false;
		}
		fontbandera = true;
	}

	enemigo1.setMatrixEnemigo(modelMatrixEnemigo);


	if (glfwJoystickPresent(GLFW_JOYSTICK_1) == GL_TRUE) {
		int axesCount, buttonCount;
		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);

		const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1,
			&buttonCount);

		//Start
		if (!empiezaJuego || cambianivel2 || cambianivel3) {
			bool pressEnter = buttons[7] == GLFW_PRESS;
			if (pressEnter) {
				empiezaJuego = true;
				cambianivel2 = false;
				if (cambianivel3) {
					exitApp = true;
				}
				//sourcesPlay[0] = false;
			}
			fontbandera = true;
		}

		//Back
		if (buttons[6] == GLFW_PRESS) {
			exitApp = true;
		}
		if ((playerRespawn == false && empiezaJuego == true) || (playerRespawn == false && cambianivel2 == true) || (playerRespawn == false && cambianivel3 == true)) {

			if (enableAction && buttons[0] == GLFW_PRESS) {
				enableAction = false;
				actionE = true;
				deControl = true;
				animationIndex = 2;
			}
			else if (buttons[0] == GLFW_RELEASE) {
				enableAction = true;
				actionE = false;
			}

			//Cruzeta izquierda
			if (modelSelected == 2 && buttons[13] == GLFW_PRESS) {
				modelMatrixAstroProta = glm::rotate(modelMatrixAstroProta, glm::radians(3.5f),
					glm::vec3(0, 1, 0));
				animationIndex = 0;
				astroPosition = modelMatrixAstroProta[3];
				if (escenario2) {
					enemigo1.setDistance(enemigo1.distanciaAProta(modelMatrixEnemigo[3], modelMatrixAstroProta[3]));
					astroPosition.x += -55.0f;
					astroPosition.z += -23.0f;
				}

			}
			//Cruzeta Derecha
			else if (modelSelected == 2 && buttons[11] == GLFW_PRESS) {
				modelMatrixAstroProta = glm::rotate(modelMatrixAstroProta, glm::radians(-3.5f),
					glm::vec3(0, 1, 0));
				animationIndex = 0;
				astroPosition = modelMatrixAstroProta[3];
				if (escenario2) {
					enemigo1.setDistance(enemigo1.distanciaAProta(modelMatrixEnemigo[3], modelMatrixAstroProta[3]));
					astroPosition.x += -55.0f;
					astroPosition.z += -23.0f;
				}
				
			}
			//Cruzeta Arriba
			if (modelSelected == 2 && buttons[10] == GLFW_PRESS) {
				if (escenario1) {
					pasado = terrain.getXCoordTerrain(modelMatrixAstroProta[3][0]);
				}
				if (escenario2) {
					pasado = terrain2.getXCoordTerrain(modelMatrixAstroProta[3][0]);
				}
				modelMatrixAstroProta = glm::translate(modelMatrixAstroProta,
					glm::vec3(0.0, 0.0, 0.1));
				animationIndex = 0;
				timer += 0.0075f;
				//std::cout << timer << std::endl;
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

				if (escenario1) {
					cameraMove();
				}
				if (escenario2) {
					cameraMove2();
				}
				astroPosition = modelMatrixAstroProta[3];
				if (escenario2) {
					enemigo1.setDistance(enemigo1.distanciaAProta(modelMatrixEnemigo[3], modelMatrixAstroProta[3]));
					astroPosition.x += -55.0f;
					astroPosition.z += -23.0f;
				}

				animationIndex = 0;
			}
			//Cruzeta abajo
			else if (modelSelected
				== 2 && buttons[12] == GLFW_PRESS) {
				if (escenario1) {
					pasado = terrain.getXCoordTerrain(modelMatrixAstroProta[3][0]);
				}
				if (escenario2) {
					pasado = terrain.getXCoordTerrain(modelMatrixAstroProta[3][0]);
				}

				modelMatrixAstroProta = glm::translate(modelMatrixAstroProta,
					glm::vec3(0.0, 0.0, -0.1));
				animationIndex = 0;
				if (escenario1) {
					cameraMove();
				}
				if (escenario2) {
					cameraMove2();
				}
				astroPosition = modelMatrixAstroProta[3];
				if (escenario2) {
					enemigo1.setDistance(enemigo1.distanciaAProta(modelMatrixEnemigo[3], modelMatrixAstroProta[3]));
				}

				if (escenario1) {
					cameraMove();
				}
				if (escenario2) {
					cameraMove2();
				}
				astroPosition = modelMatrixAstroProta[3];
				if (escenario2) {
					enemigo1.setDistance(enemigo1.distanciaAProta(modelMatrixEnemigo[3], modelMatrixAstroProta[3]));
					astroPosition.x += -55.0f;
					astroPosition.z += -23.0f;
				}
			}

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


	//if (enableCameraSelected && glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS
	//	&& (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS
	//		|| glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)) {
	//	enableCameraSelected = false;
	//	cameraSelected++;
	//	if (cameraSelected > 1)
	//		cameraSelected = 0;
	//	std::cout << "cameraSelected:" << cameraSelected << std::endl;
	//}
	//else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE
	//	|| (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE
	//		&& glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_RELEASE))
	//	enableCameraSelected = true;

	if ((playerRespawn == false && empiezaJuego == true) || (playerRespawn == false && cambianivel2 == true) || (playerRespawn == false && cambianivel3 == true)) {

		if (enableAction && glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			enableAction = false;
			actionE = true;
			animationIndex = 2;
		}
		else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE && !deControl) {
			enableAction = true;
			actionE = false;
		}

		if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			modelMatrixAstroProta = glm::rotate(modelMatrixAstroProta, glm::radians(3.5f),
				glm::vec3(0, 1, 0));
			animationIndex = 0;
			astroPosition = modelMatrixAstroProta[3];
			if (escenario2) {
				enemigo1.setDistance(enemigo1.distanciaAProta(modelMatrixEnemigo[3], modelMatrixAstroProta[3]));
				astroPosition.x += -55.0f;
				astroPosition.z += -23.0f;
			}
		}
		else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			modelMatrixAstroProta = glm::rotate(modelMatrixAstroProta, glm::radians(-3.5f),
				glm::vec3(0, 1, 0));
			animationIndex = 0;
			astroPosition = modelMatrixAstroProta[3];
			if (escenario2) {
				enemigo1.setDistance(enemigo1.distanciaAProta(modelMatrixEnemigo[3], modelMatrixAstroProta[3]));
				astroPosition.x += -55.0f;
				astroPosition.z += -23.0f;
			}
		}
		if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			if (escenario1) {
				pasado = terrain.getXCoordTerrain(modelMatrixAstroProta[3][0]);
			}
			if (escenario2) {
				pasado = terrain2.getXCoordTerrain(modelMatrixAstroProta[3][0]);
			}
			modelMatrixAstroProta = glm::translate(modelMatrixAstroProta,
				glm::vec3(0.0, 0.0, 0.1));
			animationIndex = 0;
			timer += 0.0075f;
			//std::cout << timer << std::endl;
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

			if (escenario1) {
				cameraMove();
			}
			if (escenario2) {
				cameraMove2();
			}
			astroPosition = modelMatrixAstroProta[3];
			if (escenario2) {
				enemigo1.setDistance(enemigo1.distanciaAProta(modelMatrixEnemigo[3], modelMatrixAstroProta[3]));
				astroPosition.x += -55.0f;
				astroPosition.z += -23.0f;
			}

			animationIndex = 0;
		}
		else if (modelSelected
			== 2 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			if (escenario1) {
				pasado = terrain.getXCoordTerrain(modelMatrixAstroProta[3][0]);
			}
			if (escenario2) {
				pasado = terrain.getXCoordTerrain(modelMatrixAstroProta[3][0]);
			}
			
			modelMatrixAstroProta = glm::translate(modelMatrixAstroProta,
				glm::vec3(0.0, 0.0, -0.1));
			animationIndex = 0;
			if (escenario1) {
				cameraMove();
			}
			if (escenario2) {
				cameraMove2();
			}
			astroPosition = modelMatrixAstroProta[3];
			if (escenario2) {
				enemigo1.setDistance(enemigo1.distanciaAProta(modelMatrixEnemigo[3], modelMatrixAstroProta[3]));
			}

			if (escenario1) {
				cameraMove();
			}
			if (escenario2) {
				cameraMove2();
			}
			astroPosition = modelMatrixAstroProta[3];
			if (escenario2) {
				enemigo1.setDistance(enemigo1.distanciaAProta(modelMatrixEnemigo[3], modelMatrixAstroProta[3]));
				astroPosition.x += -55.0f;
				astroPosition.z += -23.0f;
			}
		}

	}

	//************************INTERACCIONES DE COLLIDERS ENEMIGO JUGADOR ******************************/

	if (escenario2) {
		vectorDireccionEnemigo = enemigo1.calcularDireccionDeMovimiento(astroPosition, modelMatrixEnemigo[3]);
	}

	if (playerRespawn == true) {

		if (escenario1) {
			defaultMatrix = glm::translate(defaultMatrix,
				glm::vec3(0.0f, 0.0f, 20.0f));
		}
		modelMatrixAstroProta = defaultMatrix;
		modelMatrixAstroProta = glm::translate(modelMatrixAstroProta, astroOrigin);
		modelMatrixAstroProta2 = defaultMatrix2;
		modelMatrixPivoteCam = glm::translate(modelMatrixAstroProta, camPivOrigin);
		modelMatrixPivoteCam = glm::translate(modelMatrixPivoteCam,
			glm::vec3(0.0f, 5.0f, 3.0f));
		modelMatrixPivoteCam = glm::rotate(modelMatrixPivoteCam, glm::radians(-180.0f),
			glm::vec3(0, 0, 1));
		//empiezaJuego = false;
		tiempoRespawnProta++;
		if (tiempoRespawnProta > 50) {
			playerRespawn = false;
			tiempoRespawnProta = 0;
		}
		defaultMatrix = glm::mat4(1.0f);

	}

	if (enemigo1.cercaDeProta(enemigo1.distanceToPersonaje) == true)
		enemigo1.velocidad = 0.05 * 1.2f;
	else if (enemigo1.cercaDeProta(enemigo1.distanceToPersonaje) == false)
		enemigo1.velocidad = 0.05 * 1.5f;

	if (enemigo1.respawn == true) {
		//Descomentar
		if (escenario2) {
			modelMatrixEnemigo = glm::translate(modelMatrixEnemigo,
				glm::vec3(0.0f, 0.0f, 0.0f));
		}
		tiempo += 1;


		if (tiempo > 150) {
			enemigo1.respawn = false;
			tiempo = 0.0f;
		}

	}
	else if (enemigo1.respawn == false) {
		if (escenario2) {
			modelMatrixEnemigo = glm::translate(modelMatrixEnemigo, vectorDireccionEnemigo * enemigo1.velocidad);
		}
		 //Descomentar
		//modelMatrixEnemigo = glm::rotate(modelMatrixEnemigo, enemigo1.faceDirection(vectorDireccionEnemigo), glm::vec3(0,1,0));
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
			if (escenario1) {
				axis = glm::axis(glm::quat_cast(modelMatrixPivoteCam));
				angleTarget = glm::angle(glm::quat_cast(modelMatrixPivoteCam));
				target = modelMatrixPivoteCam[3];
				if (std::isnan(angleTarget))
					angleTarget = 0.0;
				if (axis.y < 0)
					angleTarget = -angleTarget;
				camera->setCameraTarget(target);
				camera->setAngleTarget(angleTarget);

				camera->updateCamera();
				view = camera->getViewMatrix();
			}
			if (escenario2 || !escenario1 && !escenario2) {
				camera->setDistanceFromTarget(25);
				axis = glm::axis(glm::quat_cast(modelMatrixPivoteCam2));
				angleTarget = glm::angle(glm::quat_cast(modelMatrixPivoteCam2));
				target = modelMatrixPivoteCam2[3];
				if (std::isnan(angleTarget))
					angleTarget = 0.0;
				if (axis.y < 0)
					angleTarget = -angleTarget;
				camera->setCameraTarget(target);
				camera->setAngleTarget(angleTarget);

				camera->updateCamera();
				view = camera->getViewMatrix();
			}
		}
		else {
			view = cameraFP->getViewMatrix();
		}

		// Listener de Sonido
			/****************************+
		 * Open AL sound data
		 */
		 // Listener for the Thris person camera
		listenerPos[0] = modelMatrixAstroProta[3].x;
		listenerPos[1] = modelMatrixAstroProta[3].y;
		listenerPos[2] = modelMatrixAstroProta[3].z;
		alListenerfv(AL_POSITION, listenerPos);

		glm::vec3 upModel = glm::normalize(modelMatrixAstroProta[1]);
		glm::vec3 frontModel = glm::normalize(modelMatrixAstroProta[2]);

		listenerOri[0] = frontModel.x;
		listenerOri[1] = frontModel.y;
		listenerOri[2] = frontModel.z;
		listenerOri[3] = upModel.x;
		listenerOri[4] = upModel.y;
		listenerOri[5] = upModel.z;
		alListenerfv(AL_ORIENTATION, listenerOri);
		for (unsigned int i = 0; i < sourcesPlay.size(); i++) {
			if (!sourcesPlay[i] && sourcesPlaying[i]) {
				//std::cout << "Apagando Source: " << i << std::endl;
				sourcesPlaying[i] = false;
				alSourceStop(source[i]);
			}
			if (sourcesPlay[i] && !sourcesPlaying[i]) {
				//std::cout << "Reproduciendo Source: " << i << std::endl;
				sourcesPlaying[i] = true;
				alSourcePlay(source[i]);
			}
		}

		//Sonido GAME OVER
		source15Pos[0] = modelMatrixAstroProta[3].x;
		source15Pos[1] = modelMatrixAstroProta[3].y + 3;
		source15Pos[2] = modelMatrixAstroProta[3].z;
		alSourcefv(source[15], AL_POSITION, source15Pos);
		//sourcesPlay[15] = true;

		if (escenario1) {
			updateEscenario1();
			lucesEscenari1(shadowBox, &view);
			if (!empiezaJuego) {
				luzMenus();
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glViewport(0, 0, screenWidth, screenHeight);
				shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
				shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureActivaID);
				shaderTexture.setInt("outTexture", 0);
				boxMenu.setPosition(glm::vec3(0.0, -6.0, 0.0));
				boxMenu.setScale(glm::vec3(38.0, 33.0, 1.0));
				boxMenu.setOrientation(glm::vec3(-25.0, 0.0, 0.0));
				boxMenu.render();
				glfwSwapBuffers(window);
				continue;
			}
			//std::cout << "Entra linea renderizados" << std::endl;
			empiezaJuego = true;
			//musicaIntro = false;
			sourcesPlay[0] = false;
			preRender1();
			renderScene();
			collidersManagmentEs1();
			soundEscene1();
		}
		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		if (escenario2) {
			updateEscenario2();
			lucesEscenari2(shadowBox, &view);
			if (cambianivel2) {
				luzMenus();
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glViewport(0, 0, screenWidth, screenHeight);
				shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
				shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureTranID);
				shaderTexture.setInt("outTexture", 0);
				boxMenu.setPosition(glm::vec3(modelMatrixPivoteCam2[3].x, modelMatrixPivoteCam2[3].y - 6.0, modelMatrixPivoteCam2[3].z - 10.0));
				boxMenu.setScale(glm::vec3(45.0, 33.0, 1.0));
				boxMenu.setOrientation(glm::vec3(25.0, 0.0, 180.0));
				//boxMenu.setOrientation(glm::vec3(0.0, 180.0, 0.0));
				boxMenu.render();
				glfwSwapBuffers(window);
				timer = 0.0f;
				continue;
			}
			empiezaJuego = true;
			preRender2();
			renderScene2();
			collidersManagmentEs2();
			soundEscene2();
		}

		if (!escenario1 && !escenario2) {
			updateEscenario2();
			lucesEscenari2(shadowBox, &view);
			for (unsigned int i = 1; i < sourcesPlay.size(); i++) {
				sourcesPlay[0] = true;
				sourcesPlay[i] = false;
			}
			if (cambianivel3) {
				luzMenus();
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glViewport(0, 0, screenWidth, screenHeight);
				shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
				shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureEndID);
				shaderTexture.setInt("outTexture", 0);
				boxMenu.setPosition(glm::vec3(modelMatrixPivoteCam2[3].x, modelMatrixPivoteCam2[3].y - 6.0, modelMatrixPivoteCam2[3].z - 10.0));
				boxMenu.setScale(glm::vec3(45.0, 33.0, 1.0));
				boxMenu.setOrientation(glm::vec3(25.0, 0.0, 180.0));
				boxMenu.render();
				glfwSwapBuffers(window);
				continue;
			}
		}

		animationIndex = 1;

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
			case 2:// Game Over
				modelText2->render("Te quedaste sin oxigeno!", -0.5, 0, 30, 1.0, 0.0, 0.0, 1.0);
				sourcesPlay[5] = true;//Game Over
				tiempoOxigeno++;
				combBotones = {
					{false, false, false},
					{false, false, false},
					{false, false, false},
					{false, false, false} };
				lucesBotones = { false, false, false, false };
				indPalanca = 0;
				animaPalancas = { 0, 0, 0, 0 };
				lucesPalancas = { false, false, false, false };
				enablePuerta = false;
				enableEscotilla1 = false;
				movimientoPuerta = 0.0f;
				if (tiempoOxigeno > 250) {
					situacion = 3;
				}
				break;
			case 3:
				playerRespawn2 = true;
				if (playerRespawn2 == true) {
					sourcesPlay[5] = false;//Reinicio
					if (escenario1) {
						defaultMatrix = glm::translate(defaultMatrix,
							glm::vec3(0.0f, 0.0f, 20.0f));
					}
					modelMatrixAstroProta = defaultMatrix;
					modelMatrixAstroProta = glm::translate(modelMatrixAstroProta, astroOrigin);
					modelMatrixPivoteCam = glm::translate(modelMatrixAstroProta, camPivOrigin);
					modelMatrixPivoteCam = glm::translate(modelMatrixPivoteCam,
						glm::vec3(0.0f, 5.0f, 3.0f));
					modelMatrixPivoteCam = glm::rotate(modelMatrixPivoteCam, glm::radians(-180.0f),
						glm::vec3(0, 0, 1));
					//empiezaJuego = false; //linea que genera problemas
					tiempoRespawnProta++;
					if (tiempoRespawnProta > 50) {
						playerRespawn2 = false;
						tiempoRespawnProta = 0;
						timer = 0.0f;
						banderaCaminar = 0;
						tiempoOxigeno = 0;
						situacion = 1;
					}
					defaultMatrix = glm::mat4(1.0f);
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

	enemyModelAnimate.setShader(&shaderMulLighting);

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

	modelRokas.setShader(&shaderMulLighting);

	modelRokas2.setShader(&shaderMulLighting);

	boxReferencia.setShader(&shaderMulLighting);

	modelLuzBotones.setShader(&shaderMulLighting);

	modelLuzGenerador.setShader(&shaderMulLighting);
}

void prepareDepthScene() {

	skyboxSphere.setShader(&shaderDepth);

	terrain.setShader(&shaderDepth);

	enemyModelAnimate.setShader(&shaderDepth);

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

	modelRokas.setShader(&shaderDepth);

	modelRokas2.setShader(&shaderDepth);

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
				botonesPos[i].z + 1.4), botonesPos[i].z + 1.4));
		modelLuzGenerador.setScale(glm::vec3(1.0, 1.0, 1.0));
		modelLuzGenerador.setOrientation(glm::vec3(0.0, 140.0, 0.0));
		//modelBotones.setOrientation(glm::vec3(0, lamp2Orientation[i], 0));
		modelLuzGenerador.render();
	}

	for (int i = 0; i < rokas2Pos.size(); i++) {
		rokas2Pos[i].y = terrain.getHeightTerrain(rokas2Pos[i].x,
		rokas2Pos[i].z) - 0.5f;
		modelRokas2.setPosition(rokas2Pos[i]);
		modelRokas2.setScale(glm::vec3(1.5, 1.5, 1.5));
		//modelRokas.setOrientation(glm::vec3(0.0, 0.0, 0));
		modelRokas2.render();
	}
	for (int i = 0; i < rokasPos.size(); i++) {
		rokasPos[i].y = terrain.getHeightTerrain(rokasPos[i].x,
		rokasPos[i].z) - 0.5f;
		modelRokas.setPosition(rokasPos[i]);
		modelRokas.setScale(glm::vec3(1.5, 1.5, 1.5));
		//modelRokas.setOrientation(glm::vec3(0.0, 0.0, 0));
		modelRokas.render();
	}
	//Compuerta
	modelCompuerta.setPosition(glm::vec3(3.3f, 2.8f, 19.1f));
	modelCompuerta.setScale(glm::vec3(0.03, 0.03, 0.03));
	modelCompuerta.setAnimationIndex(animationIndexEscotilla);
	modelCompuerta.render();
	
	//Plataforma
	modelPlataforma.setPosition(glm::vec3(10.445, 3.5, 13.789));
	modelPlataforma.setOrientation(glm::vec3(-90.0, 0.0, 0.0));
	modelPlataforma.setScale(glm::vec3(3.3, 3.3, 3.3));
	modelPlataforma.render();

	//Edificio Plataforma
	modelEdCompuerta.setPosition(glm::vec3(2.0, 9.5, 4.8));
	modelEdCompuerta.setOrientation(glm::vec3(-90.0, -90.0, 0.0));
	modelEdCompuerta.setScale(glm::vec3(0.9, 0.9, 0.9));
	modelEdCompuerta.render();

	modelEdCompuerta.setPosition(glm::vec3(13.9, 7.2, 13.1));
	modelEdCompuerta.setOrientation(glm::vec3(-90.0, 180.0, 0.0));
	modelEdCompuerta.setScale(glm::vec3(0.9, 0.9, 0.9));
	modelEdCompuerta.render();

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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glEnable(GL_CULL_FACE);
}

void inicialMatrixs() {
	//Pivote de Camara
	modelMatrixPivoteCam = glm::translate(modelMatrixPivoteCam,
		glm::vec3(0.0f, 5.0f, 23.0f));
	modelMatrixPivoteCam = glm::rotate(modelMatrixPivoteCam, glm::radians(-180.0f),
		glm::vec3(0, 0, 1));

	//Pivote de Camara 2
	modelMatrixPivoteCam2 = glm::translate(modelMatrixPivoteCam2,
		glm::vec3(-58.0f, 5.0f, -10.0f));
	modelMatrixPivoteCam2 = glm::rotate(modelMatrixPivoteCam2, glm::radians(-180.0f),
		glm::vec3(0, 0, 1));
	
	modelMatrixEnemigo = glm::mat4(1.0f);
	modelMatrixEnemigo = glm::translate(modelMatrixEnemigo, glm::vec3(13.0f, 3.0f, -5.0f));

	//Matriz
	modelMatrixAstroProta = glm::translate(modelMatrixAstroProta,
		glm::vec3(0.0f, 0.0f, 20.0f));



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

	//Model escenario 2
	modelMatrixEscenario2 = glm::translate(modelMatrixEscenario2,
		glm::vec3(25.5f, 0.0f, -5.0f));
	modelMatrixEscenario2 = glm::scale(modelMatrixEscenario2,
		glm::vec3(2.9f, 2.0f, 3.1f));
	modelMatrixEscenario2 = glm::rotate(modelMatrixEscenario2, glm::radians(-90.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	//Posicion de los muros escenario 2
	modelMatrixMuroFondo2 = glm::translate(modelMatrixMuroFondo2,
		glm::vec3(0.0f, 0.0f, -38.0f));
	modelMatrixMuroFondo2 = glm::scale(modelMatrixMuroFondo2,
		glm::vec3(75.0f, 20.0f, 0.0f));
	modelMatrixMuroFrontal2 = glm::translate(modelMatrixMuroFrontal2,
		glm::vec3(0.0f, 0.0f, -1.50f));
	modelMatrixMuroFrontal2 = glm::scale(modelMatrixMuroFrontal2,
		glm::vec3(75.0f, 20.0f, 0.0f));
	modelMatrixMuroDerecho2 = glm::translate(modelMatrixMuroDerecho2,
		glm::vec3(54.5f, 0.0f, -10.0f));
	modelMatrixMuroDerecho2 = glm::scale(modelMatrixMuroDerecho2,
		glm::vec3(0.0f, 20.0f, 70.0f));
	modelMatrixMuroIzquierdo2 = glm::translate(modelMatrixMuroIzquierdo2,
		glm::vec3(-63.0f, 0.0f, -10.0f));
	modelMatrixMuroIzquierdo2 = glm::scale(modelMatrixMuroIzquierdo2,
		glm::vec3(0.0f, 20.0f, 70.0f));

	//Marco puerta
	modelMatrixMarcoPuerta = translate(modelMatrixMarcoPuerta,
		glm::vec3(7.5f, 0.0f, -36.5f));
	modelMatrixPuertaDer = translate(modelMatrixMarcoPuerta,
		glm::vec3(0.0f, 0.0f, 0.0f));
	modelMatrixPuertaDer = glm::rotate(modelMatrixPuertaDer, glm::radians(-90.0f),
		glm::vec3(0, 1, 0));
	modelMatrixPuertaIzq = translate(modelMatrixMarcoPuerta,
		glm::vec3(0.0f, 0.0f, 0.0f));
	modelMatrixPuertaIzq = glm::rotate(modelMatrixPuertaIzq, glm::radians(-90.0f),
		glm::vec3(0, 1, 0));
	modelMatrixMarcoPuerta = glm::rotate(modelMatrixMarcoPuerta, glm::radians(-90.0f),
		glm::vec3(0, 1, 0));
	modelMatrixMarcoPuerta = scale(modelMatrixMarcoPuerta,
		glm::vec3(2.9f, 2.0f, 3.1f));

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
		glm::value_ptr(glm::vec3(0.3, 0.3, 0.5)));
	shaderMulLighting.setFloat("density", 0.02);
	shaderTerrain.setVectorFloat3("fogColor",
		glm::value_ptr(glm::vec3(0.3, 0.3, 0.5)));
	shaderTerrain.setFloat("density", 0.02);
	shaderSkybox.setVectorFloat3("fogColor",
		glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));

	/*******************************************
	 * Propiedades Luz direccional
	 *******************************************/
	if (cameraSelected == 0)
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
	else
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(cameraFP->getPosition()));
	shaderMulLighting.setVectorFloat3("directionalLight.light.ambient",
		glm::value_ptr(glm::vec3(0.14, 0.15, 0.31)));
	shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse",
		glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
	shaderMulLighting.setVectorFloat3("directionalLight.light.specular",
		glm::value_ptr(glm::vec3(0.9, 0.9, 0.9)));
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
		glm::value_ptr(glm::vec3(0.14, 0.15, 0.31)));
	shaderTerrain.setVectorFloat3("directionalLight.light.diffuse",
		glm::value_ptr(glm::vec3(0.9, 0.9, 0.9)));
	shaderTerrain.setVectorFloat3("directionalLight.light.specular",
		glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
	shaderTerrain.setVectorFloat3("directionalLight.direction",
		glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

			 /*******************************************
			  * Propiedades PointLights
			  *******************************************/
	shaderMulLighting.setInt("pointLightCount",
		botonesPos.size()*2 );
	shaderTerrain.setInt("pointLightCount",
		botonesPos.size()*2 );
	//Luces sobre generadores
	for (int i = 0; i < botonesPos.size(); i++) {
		glm::mat4 matrixLuzPuzzle = glm::mat4(1.0f);
		matrixLuzPuzzle = glm::translate(matrixLuzPuzzle,
			glm::vec3(botonesPos[i].x,
				botonesPos[i].y + 0.6, botonesPos[i].z + 1.15));
		glm::vec3 lampPosition = glm::vec3(matrixLuzPuzzle[3]);
		//std::cout << "combBotones= " << i << std::endl;
		glm::vec3 color = colorGenerador(combBotones[i]);
		if (i == 1) {
			color = glm::vec3(0.0, 0.0, 0.0);
		}
		shaderMulLighting.setVectorFloat3(
			"pointLights[" + std::to_string(i) + "].light.ambient",
			glm::value_ptr(color));
		shaderMulLighting.setVectorFloat3(
			"pointLights[" + std::to_string(i) + "].light.diffuse",
			glm::value_ptr(color));
		shaderMulLighting.setVectorFloat3(
			"pointLights[" + std::to_string(i) + "].light.specular",
			glm::value_ptr(color));
		shaderMulLighting.setVectorFloat3(
			"pointLights[" + std::to_string(i) + "].position",
			glm::value_ptr(lampPosition));
		shaderMulLighting.setFloat(
			"pointLights[" + std::to_string(i) + "].constant", 0.5);
		shaderMulLighting.setFloat(
			"pointLights[" + std::to_string(i) + "].linear", 0.09);
		shaderMulLighting.setFloat(
			"pointLights[" + std::to_string(i) + "].quadratic", 0.01);
		shaderTerrain.setVectorFloat3(
			"pointLights[" + std::to_string(i) + "].light.ambient",
			glm::value_ptr(color));
		shaderTerrain.setVectorFloat3(
			"pointLights[" + std::to_string(i) + "].light.diffuse",
			glm::value_ptr(color));
		shaderTerrain.setVectorFloat3(
			"pointLights[" + std::to_string(i) + "].light.specular",
			glm::value_ptr(color));
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

	//Lamparas generadores
	for (int i = 0; i < botonesPos.size(); i++) {
		glm::mat4 matrixLuzGen = glm::mat4(1.0f);
		matrixLuzGen = glm::translate(matrixLuzGen,
			glm::vec3(botonesPos[i].x + 3.9,
				terrain.getHeightTerrain(botonesPos[i].x + 3.9,
					botonesPos[i].z + 1.4), botonesPos[i].z + 1.4));
		glm::vec3 lampPosition = glm::vec3(matrixLuzGen[3]);
		if (lucesBotones[i]) {
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + std::to_string(botonesPos.size() + i)
				+ "].light.ambient",
				glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + std::to_string(botonesPos.size() + i)
				+ "].light.diffuse",
				glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + std::to_string(botonesPos.size() + i)
				+ "].light.specular",
				glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
		}
		else {
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + std::to_string(botonesPos.size() + i)
				+ "].light.ambient",
				glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + std::to_string(botonesPos.size() + i)
				+ "].light.diffuse",
				glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + std::to_string(botonesPos.size() + i)
				+ "].light.specular",
				glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
		}
		//shaderMulLighting.setVectorFloat3(
		//	"pointLights[" + std::to_string(botonesPos.size() + i)
		//	+ "].light.ambient",
		//	glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
		//shaderMulLighting.setVectorFloat3(
		//	"pointLights[" + std::to_string(botonesPos.size() + i)
		//	+ "].light.diffuse",
		//	glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
		//shaderMulLighting.setVectorFloat3(
		//	"pointLights[" + std::to_string(botonesPos.size() + i)
		//	+ "].light.specular",
		//	glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
		shaderMulLighting.setVectorFloat3(
			"pointLights[" + std::to_string(botonesPos.size() + i)
			+ "].position", glm::value_ptr(lampPosition));
		shaderMulLighting.setFloat(
			"pointLights[" + std::to_string(botonesPos.size() + i)
			+ "].constant", 1.0);
		shaderMulLighting.setFloat(
			"pointLights[" + std::to_string(botonesPos.size() + i)
			+ "].linear", 0.09);
		shaderMulLighting.setFloat(
			"pointLights[" + std::to_string(botonesPos.size() + i)
			+ "].quadratic", 0.01);
		if (lucesBotones[i]) {
			shaderTerrain.setVectorFloat3(
				"pointLights[" + std::to_string(botonesPos.size() + i)
				+ "].light.ambient",
				glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderTerrain.setVectorFloat3(
				"pointLights[" + std::to_string(botonesPos.size() + i)
				+ "].light.diffuse",
				glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderTerrain.setVectorFloat3(
				"pointLights[" + std::to_string(botonesPos.size() + i)
				+ "].light.specular",
				glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
		}
		else {
			shaderTerrain.setVectorFloat3(
				"pointLights[" + std::to_string(botonesPos.size() + i)
				+ "].light.ambient",
				glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
			shaderTerrain.setVectorFloat3(
				"pointLights[" + std::to_string(botonesPos.size() + i)
				+ "].light.diffuse",
				glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
			shaderTerrain.setVectorFloat3(
				"pointLights[" + std::to_string(botonesPos.size() + i)
				+ "].light.specular",
				glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
		}
		shaderTerrain.setVectorFloat3(
			"pointLights[" + std::to_string(botonesPos.size() + i)
			+ "].position", glm::value_ptr(lampPosition));
		shaderTerrain.setFloat(
			"pointLights[" + std::to_string(botonesPos.size() + i)
			+ "].constant", 1.0);
		shaderTerrain.setFloat(
			"pointLights[" + std::to_string(botonesPos.size() + i)
			+ "].linear", 0.09);
		shaderTerrain.setFloat(
			"pointLights[" + std::to_string(botonesPos.size() + i)
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
		botonColliderY.e = modelBotones.getObb().e * glm::vec3(0.15, 0.5, 0.6);
		//B boton
		botonColliderB.u = glm::quat_cast(modelMatrixColliderBotonB);
		modelMatrixColliderBotonB = glm::scale(modelMatrixColliderBotonB,
			glm::vec3(0.3, 0.5, 0.3));
		botonColliderB.c = glm::vec3(modelMatrixColliderBotonB[3]);
		botonColliderB.e = modelBotones.getObb().e * glm::vec3(0.15, 0.5, 0.6);
		//R boton
		botonColliderR.u = glm::quat_cast(modelMatrixColliderBotonR);
		modelMatrixColliderBotonR = glm::scale(modelMatrixColliderBotonR,
			glm::vec3(0.3, 0.5, 0.3));
		botonColliderR.c = glm::vec3(modelMatrixColliderBotonR[3]);
		botonColliderR.e = modelBotones.getObb().e * glm::vec3(0.15, 0.5, 0.6);
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
	// Generadores rocas Rokas1
	for (int i = 0; i < rokasPos.size(); i++) {
		AbstractModel::OBB generadorRokas1;
		glm::mat4 modelMatrixColliderRokas1 = glm::mat4(1.0);
		modelMatrixColliderRokas1 = glm::translate(modelMatrixColliderRokas1,
			glm::vec3(rokasPos[i].x, terrain.getHeightTerrain(rokasPos[i].x,
				rokasPos[i].z) - 1.0f, rokasPos[i].z));
		addOrUpdateColliders(collidersOBB, "geneRocks1-" + std::to_string(i),
			generadorRokas1, modelMatrixColliderRokas1);
		// Set the orientation of collider before doing the scale
		generadorRokas1.u = glm::quat_cast(modelMatrixColliderRokas1);
		modelMatrixColliderRokas1 = glm::scale(modelMatrixColliderRokas1,
			glm::vec3(1.2, 2.0, 1.2));
		modelMatrixColliderRokas1 = glm::translate(modelMatrixColliderRokas1,
			modelRokas.getObb().c);
		generadorRokas1.c = glm::vec3(modelMatrixColliderRokas1[3]);
		generadorRokas1.e = modelRokas.getObb().e
			* glm::vec3(1.2, 2.0, 1.2);
		std::get<0>(collidersOBB.find("geneRocks1-" + std::to_string(i))->second) =
			generadorRokas1;
	}

	// Generadores rocas Rokas2
	for (int i = 0; i < rokas2Pos.size(); i++) {
		AbstractModel::OBB generadorRokas2;
		glm::mat4 modelMatrixColliderRokas2 = glm::mat4(1.0);
		modelMatrixColliderRokas2 = glm::translate(modelMatrixColliderRokas2,
			glm::vec3(rokas2Pos[i].x, terrain.getHeightTerrain(rokas2Pos[i].x,
				rokas2Pos[i].z) - 1.0f, rokas2Pos[i].z));
		modelMatrixColliderRokas2 = glm::rotate(modelMatrixColliderRokas2,
			glm::radians(120.0f), glm::vec3(0, 1, 0));
		addOrUpdateColliders(collidersOBB, "geneRocks2-" + std::to_string(i),
			generadorRokas2, modelMatrixColliderRokas2);
		// Set the orientation of collider before doing the scale
		generadorRokas2.u = glm::quat_cast(modelMatrixColliderRokas2);
		modelMatrixColliderRokas2 = glm::scale(modelMatrixColliderRokas2,
			glm::vec3(1.6, 1.0, 1.0));
		modelMatrixColliderRokas2 = glm::translate(modelMatrixColliderRokas2,
			modelRokas.getObb().c);
		generadorRokas2.c = glm::vec3(modelMatrixColliderRokas2[3]);
		generadorRokas2.e = modelRokas2.getObb().e
			* glm::vec3(1.6, 1.0, 1.0);
		std::get<0>(collidersOBB.find("geneRocks2-" + std::to_string(i))->second) =
			generadorRokas2;
	}

	// Collider de compuerta
	AbstractModel::OBB compuertaCollider;
	glm::mat4 modelmatrixColliderCompuerta = glm::mat4(1.0);
	modelmatrixColliderCompuerta = glm::translate(modelmatrixColliderCompuerta,glm::vec3(1.9f, terrain.getHeightTerrain(2.2f,
		13.0f) + 1.0f , 12.7f));
	compuertaCollider.u = glm::quat_cast(modelmatrixColliderCompuerta);
	modelmatrixColliderCompuerta = glm::scale(modelmatrixColliderCompuerta,
		glm::vec3(4.0, 4.0, 4.0));
	modelmatrixColliderCompuerta = glm::translate(modelmatrixColliderCompuerta,
		glm::vec3(modelCompuerta.getObb().c.x,
			modelCompuerta.getObb().c.y,
			modelCompuerta.getObb().c.z));
	compuertaCollider.e = modelCompuerta.getObb().e
		* glm::vec3(4.0, 4.0, 4.0);
	compuertaCollider.c = glm::vec3(modelmatrixColliderCompuerta[3]);
	addOrUpdateColliders(collidersOBB, "compuerta", compuertaCollider,
		modelmatrixColliderCompuerta);


	// Collider de edificio compuerta edificios
	for (int i = 0; i < edificios.size(); i++) {
		AbstractModel::OBB edCompuertaCollider;
		glm::mat4 modelmatrixColliderEdCompuerta = glm::mat4(1.0);
		modelmatrixColliderEdCompuerta = glm::translate(modelmatrixColliderEdCompuerta,
			glm::vec3(edificios[i].x, terrain.getHeightTerrain(edificios[i].x,
				edificios[i].z) - 1.0f, edificios[i].z));
		addOrUpdateColliders(collidersOBB, "edificios-" + std::to_string(i),
			edCompuertaCollider, modelmatrixColliderEdCompuerta);
		// Set the orientation of collider before doing the scale
		edCompuertaCollider.u = glm::quat_cast(modelmatrixColliderEdCompuerta);
		if (i == 1) {
			modelmatrixColliderEdCompuerta = glm::scale(modelmatrixColliderEdCompuerta,
				glm::vec3(0.45, 2.0, 0.3));
			modelmatrixColliderEdCompuerta = glm::translate(modelmatrixColliderEdCompuerta,
				modelEdCompuerta.getObb().c);
			edCompuertaCollider.c = glm::vec3(modelmatrixColliderEdCompuerta[3]);
			edCompuertaCollider.e = modelEdCompuerta.getObb().e
				* glm::vec3(0.45, 2.0, 0.3);
			std::get<0>(collidersOBB.find("edificios-" + std::to_string(i))->second) =
				edCompuertaCollider;
		}
		else {
					modelmatrixColliderEdCompuerta = glm::scale(modelmatrixColliderEdCompuerta,
			glm::vec3(0.8, 2.0, 0.6));
		modelmatrixColliderEdCompuerta = glm::translate(modelmatrixColliderEdCompuerta,
			modelEdCompuerta.getObb().c);
		edCompuertaCollider.c = glm::vec3(modelmatrixColliderEdCompuerta[3]);
		edCompuertaCollider.e = modelEdCompuerta.getObb().e
			* glm::vec3(0.8, 2.0, 0.6);
		std::get<0>(collidersOBB.find("edificios-" + std::to_string(i))->second) =
			edCompuertaCollider;
		}

	}

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

	//Collider de actionAstroProta
	AbstractModel::OBB actionCollider;
	glm::mat4 modelmatrixColliderAction = glm::mat4(modelMatrixAstroProta);
	// Set the orientation of collider before doing the scale
	actionCollider.u = glm::quat_cast(modelmatrixColliderAction);
	//modelmatrixColliderAction[3].z += -20.0;
	modelmatrixColliderAction = glm::scale(modelmatrixColliderAction,
		glm::vec3(0.8, 4.2, 2.0));
	modelmatrixColliderAction = glm::translate(modelmatrixColliderAction,
		glm::vec3(astroProta.getObb().c.x,
			astroProta.getObb().c.y + 0.28,
			astroProta.getObb().c.z + 0.28));
	actionCollider.e = astroProta.getObb().e
		* glm::vec3(0.7, 4.2, 2.1);
	actionCollider.c = glm::vec3(modelmatrixColliderAction[3]);
	addOrUpdateColliders(collidersOBB, "action", actionCollider,
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
	//for (std::map<std::string,
	//	std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
	//	collidersOBB.begin(); it != collidersOBB.end(); it++) {
	//	glm::mat4 matrixCollider = glm::mat4(1.0);
	//	matrixCollider = glm::translate(matrixCollider,
	//		std::get<0>(it->second).c);
	//	matrixCollider = matrixCollider
	//		* glm::mat4(std::get<0>(it->second).u);
	//	matrixCollider = glm::scale(matrixCollider,
	//		std::get<0>(it->second).e * 2.0f);
	//	boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
	//	boxCollider.enableWireMode();
	//	boxCollider.render(matrixCollider);
	//}

	//for (std::map<std::string,
	//	std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
	//	collidersSBB.begin(); it != collidersSBB.end(); it++) {
	//	glm::mat4 matrixCollider = glm::mat4(1.0);
	//	matrixCollider = glm::translate(matrixCollider,
	//		std::get<0>(it->second).c);
	//	matrixCollider = glm::scale(matrixCollider,
	//		glm::vec3(std::get<0>(it->second).ratio * 2.0f));
	//	sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
	//	sphereCollider.enableWireMode();
	//	sphereCollider.render(matrixCollider);
	//}

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
					//std::cout << "Colision " << it->first << " with "
					//	<< jt->first << std::endl;

					//if ((it->first.compare("enemigo") == 0 || it->first.compare("astroProta") == 0)
					//	&& (jt->first.compare("enemigo") == 0 || jt->first.compare("astroProta") == 0))
					//	isCollisionEnemy = true;

					isCollision = true;
					isCollisionG = true;
				}
			}
		}
		addOrUpdateCollisionDetection(collisionDetection, it->first,
			isCollision);
	}
	//std::cout << "En collider " << std::endl;
	//std::cout << "actionE " << actionE <<std::endl;
	//std::cout << "isCollisionG " << isCollisionG << std::endl;
	if (isCollisionG && actionE && !enableEscotilla1) {
		//std::cout << "EntraISC " << std::endl;
		updateBotonCollider(collisionDetection);
		actionE = false;
	}
	//else {
	//	actionE = false;
	//}


	//std::cout << "Colision escotilla " << collisionDetection.find("escotilla")->second << std::endl;
	if (isCollisionG && enableEscotilla1 && collisionDetection.find("compuerta")->second) {
		cambianivel2 = true;
		escenario1 = false;
		escenario2 = true;
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
				//std::cout << "Colision " << it->first << " with "
				//	<< jt->first << std::endl;

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
				if (jt->first.compare("enemigo") == 0) {
					if (isCollisionEnemy == true) {
						enemigo1.respawn = true;
						//modelMatrixEnemy = glm::translate(modelMatrixEnemy, enemigo1.calculaReaparicion(enemigo1.origen, modelMatrixEnemy[3]));
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

	//Musica Intro
	source0Pos[0] = modelMatrixAstroProta[3].x;
	source0Pos[1] = modelMatrixAstroProta[3].y;
	source0Pos[2] = modelMatrixAstroProta[3].z;
	alSourcefv(source[0], AL_POSITION, source0Pos);

	//Caminata
	source1Pos[0] = modelMatrixAstroProta[3].x;
	source1Pos[1] = modelMatrixAstroProta[3].y + 7;
	source1Pos[2] = modelMatrixAstroProta[3].z;
	alSourcefv(source[1], AL_POSITION, source1Pos);
	if (animationIndex != 1) {
		//std::cout << "Cambia sourcesPlay[1] " << std::endl;
		sourcesPlay[1] = true;
	}
	else {
		sourcesPlay[1] = false;
	}

	//Generadores
	source5Pos[0] = -66.6;
	source5Pos[1] = terrain.getHeightTerrain(-66.6, -2) + 1.0;
	source5Pos[2] = -2;
	alSourcefv(source[5], AL_POSITION, source5Pos);
	sourcesPlay[5] = true;

	source6Pos[0] = 78;
	source6Pos[1] = terrain.getHeightTerrain(78, 12) + 1.0;
	source6Pos[2] = 12;
	alSourcefv(source[6], AL_POSITION, source6Pos);
	sourcesPlay[6] = true;

	source7Pos[0] = 34.57;
	source7Pos[1] = terrain.getHeightTerrain(34.57, -3) + 1.0;
	source7Pos[2] = -3;
	alSourcefv(source[7], AL_POSITION, source7Pos);
	sourcesPlay[7] = true;

	//Ambiental Exterior
	source12Pos[0] = modelMatrixAstroProta[3].x;
	source12Pos[1] = modelMatrixAstroProta[3].y + 3;
	source12Pos[2] = modelMatrixAstroProta[3].z;
	alSourcefv(source[12], AL_POSITION, source12Pos);
	sourcesPlay[12] = true;

	//Generador Código correcto G1
	source8Pos[0] = modelMatrixAstroProta[3].x;
	source8Pos[1] = modelMatrixAstroProta[3].y;
	source8Pos[2] = modelMatrixAstroProta[3].z;
	alSourcefv(source[8], AL_POSITION, source8Pos);
	//sourcesPlay[8] = true;

	//Generador Código correcto G2
	source9Pos[0] = modelMatrixAstroProta[3].x;
	source9Pos[1] = modelMatrixAstroProta[3].y;
	source9Pos[2] = modelMatrixAstroProta[3].z;
	alSourcefv(source[9], AL_POSITION, source9Pos);
	//sourcesPlay[8] = true;

	//Generador Código correcto G3
	source10Pos[0] = modelMatrixAstroProta[3].x;
	source10Pos[1] = modelMatrixAstroProta[3].y;
	source10Pos[2] = modelMatrixAstroProta[3].z;
	alSourcefv(source[10], AL_POSITION, source10Pos);
	//sourcesPlay[8] = true;

	//Compuerta Abierta
	source3Pos[0] = 1.9f;
	source3Pos[1] = terrain.getHeightTerrain(2.2f, 13.0f) + 1.0f;
	source3Pos[2] = 12.7f;
	alSourcefv(source[3], AL_POSITION, source3Pos);

}

void prepareScene2() {
	skyboxSphere.setShader(&shaderSkybox);

	terrain2.setShader(&shaderTerrain);

	enemyModelAnimate.setShader(&shaderMulLighting);

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

	modelRokas.setShader(&shaderMulLighting);

	boxReferencia.setShader(&shaderMulLighting);

	modelLuzBotones.setShader(&shaderMulLighting);

	modelLuzGenerador.setShader(&shaderMulLighting);

	modelEscenario2.setShader(&shaderMulLighting);

	modelBidones.setShader(&shaderMulLighting);

	modelCompu.setShader(&shaderMulLighting);

	modelCuerpo.setShader(&shaderMulLighting);

	modelEstanteria.setShader(&shaderMulLighting);

	modelCajaCuadrada.setShader(&shaderMulLighting);

	modelCajaLowPoly.setShader(&shaderMulLighting);

	modelPalanca.setShader(&shaderMulLighting);

	modelPuertaDer.setShader(&shaderMulLighting);

	modelPuertaIzq.setShader(&shaderMulLighting);

	modelMarcoPuerta.setShader(&shaderMulLighting);
}

void prepareDepthScene2() {
	skyboxSphere.setShader(&shaderDepth);

	terrain2.setShader(&shaderDepth);

	enemyModelAnimate.setShader(&shaderDepth);

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

	modelRokas.setShader(&shaderDepth);

	boxReferencia.setShader(&shaderDepth);

	modelLuzBotones.setShader(&shaderDepth);

	modelLuzGenerador.setShader(&shaderDepth);

	modelEscenario2.setShader(&shaderDepth);

	modelBidones.setShader(&shaderDepth);

	modelCompu.setShader(&shaderDepth);

	modelCuerpo.setShader(&shaderDepth);

	modelEstanteria.setShader(&shaderDepth);

	modelCajaCuadrada.setShader(&shaderDepth);

	modelCajaLowPoly.setShader(&shaderDepth);

	modelPalanca.setShader(&shaderDepth);

	modelPuertaDer.setShader(&shaderDepth);

	modelPuertaIzq.setShader(&shaderDepth);

	modelMarcoPuerta.setShader(&shaderDepth);
}

void renderScene2(bool renderParticles) {
	// Se activa la textura del background
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
	shaderTerrain.setInt("backgroundTexture", 0);
	// Se activa la textura de tierra
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID2);
	shaderTerrain.setInt("rTexture", 1);
	// Se activa la textura de hierba
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID2);
	shaderTerrain.setInt("gTexture", 2);
	// Se activa la textura del camino
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID2);
	shaderTerrain.setInt("bTexture", 3);
	// Se activa la textura del blend map
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID2);
	shaderTerrain.setInt("blendMapTexture", 4);
	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(40, 40)));
	terrain2.render();
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
	glEnable(GL_CULL_FACE);

	//astroProta
	glDisable(GL_CULL_FACE);
	modelMatrixAstroProta[3][1] = 0.4;
	glm::mat4 modelMatrixAstroBody = glm::mat4(modelMatrixAstroProta);
	modelMatrixAstroBody[3].z = modelMatrixAstroBody[3].z - 23.0;
	modelMatrixAstroBody[3].x = modelMatrixAstroBody[3].x - 55.0;
	//modelMatrixAstroBody[3].y = modelMatrixAstroBody[3].y + 0.4;
	modelMatrixAstroBody = glm::scale(modelMatrixAstroBody,
		glm::vec3(0.021, 0.021, 0.021));
	astroProta.setAnimationIndex(animationIndex);
	astroProta.render(modelMatrixAstroBody);
	glEnable(GL_CULL_FACE);

	astroOrigin = modelMatrixAstroProta[3];

	//escenario2
	glDisable(GL_CULL_FACE);
	modelEscenario2.render(modelMatrixEscenario2);

	//Bidones
	modelMatrixBidones = glm::mat4(1.0);
	modelMatrixBidones = glm::translate(modelMatrixBidones, glm::vec3(-59.0, 0.2, -34.0));
	modelMatrixBidones = glm::scale(modelMatrixBidones, glm::vec3(2.0, 2.0, 2.0));
	modelBidones.render(modelMatrixBidones);

	modelMatrixBidones2 = glm::mat4(1.0);
	modelMatrixBidones2 = glm::translate(modelMatrixBidones2, glm::vec3(-31.6, 0.2, -18.1));
	modelMatrixBidones2 = glm::scale(modelMatrixBidones2, glm::vec3(2.0, 2.0, 2.0));
	modelBidones.render(modelMatrixBidones2);

	modelMatrixBidones3 = glm::mat4(1.0);
	modelMatrixBidones3 = glm::translate(modelMatrixBidones3, glm::vec3(-28.7, 0.2, -15.0));
	modelMatrixBidones3 = glm::scale(modelMatrixBidones3, glm::vec3(2.0, 2.0, 2.0));
	modelBidones.render(modelMatrixBidones3);

	modelMatrixBidones4 = glm::mat4(1.0);
	modelMatrixBidones4 = glm::translate(modelMatrixBidones4, glm::vec3(38.0, 0.2, -12.3));
	modelMatrixBidones4 = glm::scale(modelMatrixBidones4, glm::vec3(2.0, 2.0, 2.0));
	modelBidones.render(modelMatrixBidones4);

	//Caja1 (Cuadradas)
	modelMatrixCajaCuadrada = glm::mat4(1.0);
	modelMatrixCajaCuadrada = glm::translate(modelMatrixCajaCuadrada, glm::vec3(-60.3, 0.2, -17.3));
	modelMatrixCajaCuadrada = glm::scale(modelMatrixCajaCuadrada, glm::vec3(2.0, 2.0, 2.0));
	modelCajaCuadrada.render(modelMatrixCajaCuadrada);

	modelMatrixCajaCuadrada2 = glm::mat4(1.0);
	modelMatrixCajaCuadrada2 = glm::translate(modelMatrixCajaCuadrada2, glm::vec3(-47, 0.2, -7.0));
	modelMatrixCajaCuadrada2 = glm::scale(modelMatrixCajaCuadrada2, glm::vec3(2.0, 2.0, 2.0));
	modelCajaCuadrada.render(modelMatrixCajaCuadrada2);

	modelCajaCuadrada.setOrientation(glm::vec3(0.0, 30.0, 0.0));
	modelMatrixCajaCuadrada3 = glm::mat4(1.0);
	modelMatrixCajaCuadrada3 = glm::translate(modelMatrixCajaCuadrada3, glm::vec3(-17.0, 0.2, -6.25));
	modelMatrixCajaCuadrada3 = glm::scale(modelMatrixCajaCuadrada3, glm::vec3(2.0, 2.0, 2.0));
	modelMatrixCajaCuadrada3 = glm::rotate(modelMatrixCajaCuadrada3, glm::radians(30.0f), glm::vec3(0, 1, 0));
	modelCajaCuadrada.render(modelMatrixCajaCuadrada3);

	modelCajaCuadrada.setOrientation(glm::vec3(0.0, -30.0, 0.0));
	modelMatrixCajaCuadrada4 = glm::mat4(1.0);
	modelMatrixCajaCuadrada4 = glm::translate(modelMatrixCajaCuadrada4, glm::vec3(-12.3, 0.2, -6.2));
	modelMatrixCajaCuadrada4 = glm::scale(modelMatrixCajaCuadrada4, glm::vec3(2.0, 2.0, 2.0));
	modelMatrixCajaCuadrada4 = glm::rotate(modelMatrixCajaCuadrada4, glm::radians(-30.0f), glm::vec3(0, 1, 0));
	modelCajaCuadrada.render(modelMatrixCajaCuadrada4);

	modelCajaCuadrada.setOrientation(glm::vec3(0.0, 30.0, 0.0));
	modelMatrixCajaCuadrada5 = glm::mat4(1.0);
	modelMatrixCajaCuadrada5 = glm::translate(modelMatrixCajaCuadrada5, glm::vec3(17.0, 0.2, -25.5));
	modelMatrixCajaCuadrada5 = glm::scale(modelMatrixCajaCuadrada5, glm::vec3(2.0, 2.0, 2.0));
	modelMatrixCajaCuadrada5 = glm::rotate(modelMatrixCajaCuadrada5, glm::radians(30.0f), glm::vec3(0, 1, 0));
	modelCajaCuadrada.render(modelMatrixCajaCuadrada5);

	//Caja2 (Low Poly)
	modelCajaLowPoly.setOrientation(glm::vec3(0.0, -30.0, 0.0));
	modelMatrixCajaLowPoly = glm::mat4(1.0);
	modelMatrixCajaLowPoly = glm::translate(modelMatrixCajaLowPoly, glm::vec3(-42.0, 0.2, -7.0));
	modelMatrixCajaLowPoly = glm::scale(modelMatrixCajaLowPoly, glm::vec3(2.0, 2.0, 2.0));
	modelMatrixCajaLowPoly = glm::rotate(modelMatrixCajaLowPoly, glm::radians(-30.0f), glm::vec3(0, 1, 0));
	modelCajaLowPoly.render(modelMatrixCajaLowPoly);

	modelCajaLowPoly.setOrientation(glm::vec3(0.0, -30.0, 0.0));
	modelMatrixCajaLowPoly2 = glm::mat4(1.0);
	modelMatrixCajaLowPoly2 = glm::translate(modelMatrixCajaLowPoly2, glm::vec3(-18.0, 0.2, -23.0));
	modelMatrixCajaLowPoly2 = glm::scale(modelMatrixCajaLowPoly2, glm::vec3(2.0, 2.0, 2.0));
	modelMatrixCajaLowPoly2 = glm::rotate(modelMatrixCajaLowPoly2, glm::radians(-30.0f), glm::vec3(0, 1, 0));
	modelCajaLowPoly.render(modelMatrixCajaLowPoly2);

	modelCajaLowPoly.setOrientation(glm::vec3(0.0, -30.0, 0.0));
	modelMatrixCajaLowPoly3 = glm::mat4(1.0);
	modelMatrixCajaLowPoly3 = glm::translate(modelMatrixCajaLowPoly3, glm::vec3(-22.6, 0.2, -28.9));
	modelMatrixCajaLowPoly3 = glm::scale(modelMatrixCajaLowPoly3, glm::vec3(2.0, 2.0, 2.0));
	modelMatrixCajaLowPoly3 = glm::rotate(modelMatrixCajaLowPoly3, glm::radians(-30.0f), glm::vec3(0, 1, 0));
	modelCajaLowPoly.render(modelMatrixCajaLowPoly3);

	modelCajaLowPoly.setOrientation(glm::vec3(0.0, -30.0, 0.0));
	modelMatrixCajaLowPoly4 = glm::mat4(1.0);
	modelMatrixCajaLowPoly4 = glm::translate(modelMatrixCajaLowPoly4, glm::vec3(-23.6, 0.2, -34.2));
	modelMatrixCajaLowPoly4 = glm::scale(modelMatrixCajaLowPoly4, glm::vec3(2.0, 2.0, 2.0));
	modelMatrixCajaLowPoly4 = glm::rotate(modelMatrixCajaLowPoly4, glm::radians(-30.0f), glm::vec3(0, 1, 0));
	modelCajaLowPoly.render(modelMatrixCajaLowPoly4);

	modelCajaLowPoly.setOrientation(glm::vec3(0.0, -30.0, 0.0));
	modelMatrixCajaLowPoly5 = glm::mat4(1.0);
	modelMatrixCajaLowPoly5 = glm::translate(modelMatrixCajaLowPoly5, glm::vec3(12.3, 0.2, -20.5));
	modelMatrixCajaLowPoly5 = glm::scale(modelMatrixCajaLowPoly5, glm::vec3(2.0, 2.0, 2.0));
	modelMatrixCajaLowPoly5 = glm::rotate(modelMatrixCajaLowPoly5, glm::radians(-30.0f), glm::vec3(0, 1, 0));
	modelCajaLowPoly.render(modelMatrixCajaLowPoly5);

	modelCajaLowPoly.setOrientation(glm::vec3(0.0, -30.0, 0.0));
	modelMatrixCajaLowPoly6 = glm::mat4(1.0);
	modelMatrixCajaLowPoly6 = glm::translate(modelMatrixCajaLowPoly6, glm::vec3(50.6, 0.2, -34.5));
	modelMatrixCajaLowPoly6 = glm::scale(modelMatrixCajaLowPoly6, glm::vec3(2.0, 2.0, 2.0));
	modelMatrixCajaLowPoly6 = glm::rotate(modelMatrixCajaLowPoly6, glm::radians(-30.0f), glm::vec3(0, 1, 0));
	modelCajaLowPoly.render(modelMatrixCajaLowPoly6);

	//Compus
	modelCompu.setPosition(glm::vec3(17.0, 2.8, -25.5));
	modelCompu.setScale(glm::vec3(2.0, 2.0, 2.0));
	modelCompu.setOrientation(glm::vec3(0.0, -90.0, 0.0));
	modelCompu.render();

	modelCompu.setPosition(glm::vec3(-17.77, 3.3, -22.65));
	modelCompu.setScale(glm::vec3(2.0, 2.0, 2.0));
	modelCompu.setOrientation(glm::vec3(0.0, -90.0, 0.0));
	modelCompu.render();

	//Cuerpos
	modelCuerpo.setPosition(glm::vec3(-31.71, 0.0, -15.03));
	modelCuerpo.setScale(glm::vec3(2.0, 2.0, 2.0));
	modelCuerpo.setOrientation(glm::vec3(0.0, -90.0, 0.0));
	modelCuerpo.render();

	modelCuerpo.setPosition(glm::vec3(50.58, 3.3, -34.5));
	modelCuerpo.setScale(glm::vec3(2.0, 2.0, 2.0));
	modelCuerpo.setOrientation(glm::vec3(0.0, -45.0, 0.0));
	modelCuerpo.render();

	modelCuerpo.setPosition(glm::vec3(-21.0, 0.0, -25.0));
	modelCuerpo.setScale(glm::vec3(2.0, 2.0, 2.0));
	modelCuerpo.setOrientation(glm::vec3(0.0, -45.0, 0.0));
	modelCuerpo.render();

	//Estanteria
	modelEstanteria.setOrientation(glm::vec3(0.0, 30.0, 0.0));
	modelMatrixEstanteria1 = glm::mat4(1.0);
	modelMatrixEstanteria1 = glm::translate(modelMatrixEstanteria1, glm::vec3(-29.10, 0.2, -27.73));
	modelMatrixEstanteria1 = glm::scale(modelMatrixEstanteria1, glm::vec3(2.0, 2.0, 2.0));
	modelMatrixEstanteria1 = glm::rotate(modelMatrixEstanteria1, glm::radians(30.0f), glm::vec3(0, 1, 0));
	modelEstanteria.render(modelMatrixEstanteria1);

	modelEstanteria.setOrientation(glm::vec3(0.0, -30.0, 0.0));
	modelMatrixEstanteria2 = glm::mat4(1.0);
	modelMatrixEstanteria2 = glm::translate(modelMatrixEstanteria2, glm::vec3(-7.03, 0.2, -7.22));
	modelMatrixEstanteria2 = glm::scale(modelMatrixEstanteria2, glm::vec3(2.0, 2.0, 2.0));
	modelMatrixEstanteria2 = glm::rotate(modelMatrixEstanteria2, glm::radians(-45.0f), glm::vec3(0, 1, 0));
	modelEstanteria.render(modelMatrixEstanteria2);

	//Marco puerta
	modelMarcoPuerta.render(modelMatrixMarcoPuerta);

	if (enablePuerta && movimientoPuerta <= 4.0f) {
		movimientoPuerta += 0.01f;
	}

	//Puerta Izquierda
	glm::mat4 modelMatrixPuertaIzqAux = glm::mat4(modelMatrixPuertaIzq);
	modelMatrixPuertaIzqAux = glm::translate(modelMatrixPuertaIzqAux,
		glm::vec3(0.0f, 0.0f, movimientoPuerta));
	modelMatrixPuertaIzqAux = glm::scale(modelMatrixPuertaIzqAux,
		glm::vec3(2.9f, 2.0f, 3.1f));
	modelPuertaIzq.render(modelMatrixPuertaIzqAux);

	//Puerta Derecha
	glm::mat4 modelMatrixPuertaDerAux = glm::mat4(modelMatrixPuertaDer);
	modelMatrixPuertaDerAux = glm::translate(modelMatrixPuertaDerAux,
		glm::vec3(0.0f, 0.0f, -movimientoPuerta));
	modelMatrixPuertaDerAux = glm::scale(modelMatrixPuertaDerAux,
		glm::vec3(2.9f, 2.0f, 3.1f));
	modelPuertaDer.render(modelMatrixPuertaDerAux);

	//Palanca
	for (int i = 0; i < palancaPos.size(); i++) {
		modelPalanca.setPosition(glm::vec3(palancaPos[i].x,
			palancaPos[i].y, palancaPos[i].z));
		modelPalanca.setScale(glm::vec3(0.021, 0.021, 0.021));
		modelPalanca.setOrientation(glm::vec3(0.0, palancaOrientation[i], 0.0));
		modelPalanca.setAnimationIndex(animaPalancas[i]);
		modelPalanca.render();
	}

	glEnable(GL_CULL_FACE);
	modelMatrixEnemigo[3][1] = terrain2.getHeightTerrain(
		modelMatrixEnemigo[3][0], modelMatrixEnemigo[3][2]);
	blendingUnsorted.find("enemigo")->second = glm::vec3(modelMatrixEnemigo[3]);

	// Ordenar los objetos con color alpha
	std::map<float, std::pair<std::string, glm::vec3> > blendingSorted;
	std::map<std::string, glm::vec3>::iterator itBlend = blendingUnsorted.begin();
	for (; itBlend != blendingUnsorted.end(); itBlend++) {
		float distanceFormView = glm::length(camera->getPosition() - itBlend->second);
		blendingSorted[distanceFormView] = std::make_pair(itBlend->first, itBlend->second);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	for (std::map<float, std::pair<std::string, glm::vec3> >::reverse_iterator it =
		blendingSorted.rbegin(); it != blendingSorted.rend(); it++) {
		if (it->second.first.compare("enemigo") == 0) {
			glm::mat4 modelMatrixEnemyBody = glm::mat4(modelMatrixEnemigo);
			modelMatrixEnemyBody = glm::scale(modelMatrixEnemyBody,
				glm::vec3(0.021, 0.021, 0.021));
			enemyModelAnimate.setAnimationIndex(animationIndexEnemy);
			enemyModelAnimate.render(modelMatrixEnemyBody);
		}
	}
	glEnable(GL_CULL_FACE);
	//glDisable(GL_BLEND);

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
	 * Propiedades Fog
	 *******************************************/
	shaderMulLighting.setVectorFloat3("fogColor",
		glm::value_ptr(glm::vec3(0.5, 0.3, 0.5)));
	shaderMulLighting.setFloat("density", 0.01);
	shaderTerrain.setVectorFloat3("fogColor",
		glm::value_ptr(glm::vec3(0.5, 0.3, 0.5)));
	shaderTerrain.setFloat("density", 0.01);
	shaderSkybox.setVectorFloat3("fogColor",
		glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));

	/*******************************************
	 * Propiedades Luz direccional
	 *******************************************/
	if (cameraSelected == 0)
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
	else
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(cameraFP->getPosition()));
	shaderMulLighting.setVectorFloat3("directionalLight.light.ambient",
		glm::value_ptr(glm::vec3(0.14, 0.15, 0.0)));
	shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse",
		glm::value_ptr(glm::vec3(0.5, 0.5, 0.2)));
	shaderMulLighting.setVectorFloat3("directionalLight.light.specular",
		glm::value_ptr(glm::vec3(0.5, 0.5, 0.9)));
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
		glm::value_ptr(glm::vec3(0.14, 0.15, 0.0)));
	shaderTerrain.setVectorFloat3("directionalLight.light.diffuse",
		glm::value_ptr(glm::vec3(0.5, 0.5, 0.0)));
	shaderTerrain.setVectorFloat3("directionalLight.light.specular",
		glm::value_ptr(glm::vec3(0.5, 0.5, 0.0)));
	shaderTerrain.setVectorFloat3("directionalLight.direction",
		glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

	shaderMulLighting.setInt("pointLightCount",
		lucesPuzzleEsc2.size());
	shaderTerrain.setInt("pointLightCount",
		lucesPuzzleEsc2.size());

	//Lamparas generadores
	for (int i = 0; i < lucesPuzzleEsc2.size(); i++) {
		glm::mat4 matrixLuzPuzz = glm::mat4(1.0f);
		matrixLuzPuzz = glm::translate(matrixLuzPuzz,
			glm::vec3(lucesPuzzleEsc2[i]));
		glm::vec3 lampPosition = glm::vec3(matrixLuzPuzz[3]);
		if (lucesPalancas[i]) {
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.ambient",
				glm::value_ptr(glm::vec3(0.0, 0.4, 0.0)));
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.diffuse",
				glm::value_ptr(glm::vec3(0.0, 0.02, 0.0)));
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.specular",
				glm::value_ptr(glm::vec3(0.0, 0.09, 0.0)));
		}
		else {
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.ambient",
				glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.ambient",
				glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.ambient",
				glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
		}
		shaderMulLighting.setVectorFloat3(
			"pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
		shaderMulLighting.setFloat(
			"pointLights[" + std::to_string(i) + "].constant", 1.0);
		shaderMulLighting.setFloat(
			"pointLights[" + std::to_string(i) + "].linear", 0.09);
		shaderMulLighting.setFloat(
			"pointLights[" + std::to_string(i) + "].quadratic", 0.01);
		if (lucesPalancas[i]) {
			shaderTerrain.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.ambient",
				glm::value_ptr(glm::vec3(0.0, 0.4, 0.0)));
			shaderTerrain.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.diffuse",
				glm::value_ptr(glm::vec3(0.0, 0.9, 0.0)));
			shaderTerrain.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.specular",
				glm::value_ptr(glm::vec3(0.0, 0.2, 0.0)));
		}
		else {
			shaderTerrain.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.ambient",
				glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
			shaderTerrain.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.ambient",
				glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
			shaderTerrain.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.ambient",
				glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
		}
		shaderTerrain.setVectorFloat3(
			"pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
		shaderTerrain.setFloat(
			"pointLights[" + std::to_string(i) + "].constant", 1.0);
		shaderTerrain.setFloat(
			"pointLights[" + std::to_string(i) + "].linear", 0.09);
		shaderTerrain.setFloat(
			"pointLights[" + std::to_string(i) + "].quadratic", 0.02);
	}

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
	modelmatrixColliderAstroProta[3].z += - 23.0;
	modelmatrixColliderAstroProta[3].x += - 55.0;
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

	//Collider de actionAstroProta
	AbstractModel::OBB actionCollider;
	glm::mat4 modelmatrixColliderAction = glm::mat4(modelMatrixAstroProta);
	// Set the orientation of collider before doing the scale
	actionCollider.u = glm::quat_cast(modelmatrixColliderAction);
	modelmatrixColliderAction[3].z += -23.0;
	modelmatrixColliderAction[3].x += -55.0;
	modelmatrixColliderAction = glm::scale(modelmatrixColliderAction,
		glm::vec3(1.5, 4.0, 1.4));
	modelmatrixColliderAction = glm::translate(modelmatrixColliderAction,
		glm::vec3(astroProta.getObb().c.x,
			astroProta.getObb().c.y + 0.28,
			astroProta.getObb().c.z + 0.28));
	actionCollider.e = astroProta.getObb().e
		* glm::vec3(2.0, 4.2, 2.8);
	actionCollider.c = glm::vec3(modelmatrixColliderAction[3]);
	addOrUpdateColliders(collidersOBB2, "action", actionCollider,
		modelMatrixAstroProta);

	// Collider de enemigo
	AbstractModel::OBB enemyCollider;
	glm::mat4 modelmatrixColliderEnemy = glm::mat4(modelMatrixEnemigo);
	modelmatrixColliderEnemy = glm::rotate(modelmatrixColliderEnemy,
		glm::radians(-90.0f), glm::vec3(1, 0, 0));
	// Set the orientation of collider before doing the scale
	enemyCollider.u = glm::quat_cast(modelmatrixColliderEnemy);
	modelmatrixColliderEnemy = glm::scale(modelmatrixColliderEnemy,
		glm::vec3(0.021, 0.021, 0.021));
	modelmatrixColliderEnemy = glm::translate(modelmatrixColliderEnemy,
		glm::vec3(enemyModelAnimate.getObb().c.x,
			enemyModelAnimate.getObb().c.y + 10.0f,
			enemyModelAnimate.getObb().c.z));
	enemyCollider.e = enemyModelAnimate.getObb().e
		* glm::vec3(3.0, 3.0, 3.0)
		* glm::vec3(1.0f, 1.0f, 1.0f);
	enemyCollider.c = glm::vec3(modelmatrixColliderEnemy[3]);
	addOrUpdateColliders(collidersOBB2, "enemigo", enemyCollider,
		modelMatrixEnemigo);

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

	//Collider Estanteria
	AbstractModel::OBB modelEstanteriaCollider1;
	glm::mat4 modelMatrixEstanteriaCollider1 = glm::mat4(modelMatrixEstanteria1);
	// Set the orientation of collider before doing the scale
	modelEstanteriaCollider1.u = glm::quat_cast(modelMatrixEstanteriaCollider1);
	modelMatrixEstanteriaCollider1 = glm::translate(modelMatrixEstanteriaCollider1,
		glm::vec3(modelEstanteria.getObb().c.x,
			modelEstanteria.getObb().c.y,
			modelEstanteria.getObb().c.z));
	modelEstanteriaCollider1.e = modelEstanteria.getObb().e * glm::vec3(2.0, 2.0, 2.0)
		* glm::vec3(0.9, 0.9, 0.9);
	modelEstanteriaCollider1.c = glm::vec3(modelMatrixEstanteriaCollider1[3]);
	addOrUpdateColliders(collidersOBB2, "Estanteria1", modelEstanteriaCollider1,
		modelMatrixEstanteriaCollider1);

	//Collider Estanteria2
	AbstractModel::OBB modelEstanteriaCollider2;
	glm::mat4 modelMatrixEstanteriaCollider2 = glm::mat4(modelMatrixEstanteria2);
	// Set the orientation of collider before doing the scale
	modelEstanteriaCollider2.u = glm::quat_cast(modelMatrixEstanteriaCollider2);
	modelMatrixEstanteriaCollider2 = glm::translate(modelMatrixEstanteriaCollider2,
		glm::vec3(modelEstanteria.getObb().c.x,
			modelEstanteria.getObb().c.y,
			modelEstanteria.getObb().c.z));
	modelEstanteriaCollider2.e = modelEstanteria.getObb().e * glm::vec3(2.0, 2.0, 2.0)
		* glm::vec3(0.9, 0.9, 0.9);
	modelEstanteriaCollider2.c = glm::vec3(modelMatrixEstanteriaCollider2[3]);
	addOrUpdateColliders(collidersOBB2, "Estanteria2", modelEstanteriaCollider2,
		modelMatrixEstanteriaCollider2);

	//Collider modelCajaCuadrada
	AbstractModel::OBB modelCajaCuadradaCollider;
	glm::mat4 modelMatrixCajaCuadradaCollider = glm::mat4(modelMatrixCajaCuadrada);
	// Set the orientation of collider before doing the scale
	modelCajaCuadradaCollider.u = glm::quat_cast(modelMatrixCajaCuadradaCollider);
	modelMatrixCajaCuadradaCollider = glm::translate(modelMatrixCajaCuadradaCollider,
		glm::vec3(modelCajaCuadrada.getObb().c.x,
			modelCajaCuadrada.getObb().c.y,
			modelCajaCuadrada.getObb().c.z));
	modelCajaCuadradaCollider.e = modelCajaCuadrada.getObb().e * glm::vec3(2.0, 2.0, 2.0)
		* glm::vec3(1.0, 1.0, 1.0);
	modelCajaCuadradaCollider.c = glm::vec3(modelMatrixCajaCuadradaCollider[3]);
	addOrUpdateColliders(collidersOBB2, "cajaCuadrada1", modelCajaCuadradaCollider,
		modelMatrixCajaCuadradaCollider);

	//Collider modelCajaCuadrada2
	AbstractModel::OBB modelCajaCuadradaCollider2;
	glm::mat4 modelMatrixCajaCuadradaCollider2 = glm::mat4(modelMatrixCajaCuadrada2);
	// Set the orientation of collider before doing the scale
	modelCajaCuadradaCollider2.u = glm::quat_cast(modelMatrixCajaCuadradaCollider2);
	modelMatrixCajaCuadradaCollider2 = glm::translate(modelMatrixCajaCuadradaCollider2,
		glm::vec3(modelCajaCuadrada.getObb().c.x,
			modelCajaCuadrada.getObb().c.y,
			modelCajaCuadrada.getObb().c.z));
	modelCajaCuadradaCollider2.e = modelCajaCuadrada.getObb().e * glm::vec3(2.0, 2.0, 2.0)
		* glm::vec3(1.0, 1.0, 1.0);
	modelCajaCuadradaCollider2.c = glm::vec3(modelMatrixCajaCuadradaCollider2[3]);
	addOrUpdateColliders(collidersOBB2, "cajaCuadrada2", modelCajaCuadradaCollider2,
		modelMatrixCajaCuadradaCollider2);

	//Collider modelCajaCuadrada3
	AbstractModel::OBB modelCajaCuadradaCollider3;
	glm::mat4 modelMatrixCajaCuadradaCollider3 = glm::mat4(modelMatrixCajaCuadrada3);
	// Set the orientation of collider before doing the scale
	modelCajaCuadradaCollider3.u = glm::quat_cast(modelMatrixCajaCuadradaCollider3);
	modelMatrixCajaCuadradaCollider3 = glm::translate(modelMatrixCajaCuadradaCollider3,
		glm::vec3(modelCajaCuadrada.getObb().c.x,
			modelCajaCuadrada.getObb().c.y,
			modelCajaCuadrada.getObb().c.z));
	modelCajaCuadradaCollider3.e = modelCajaCuadrada.getObb().e * glm::vec3(2.0, 2.0, 2.0)
		* glm::vec3(0.9, 0.9, 0.9);
	modelCajaCuadradaCollider3.c = glm::vec3(modelMatrixCajaCuadradaCollider3[3]);
	addOrUpdateColliders(collidersOBB2, "cajaCuadrada3", modelCajaCuadradaCollider3,
		modelMatrixCajaCuadradaCollider3);

	//Collider modelCajaCuadrada4
	AbstractModel::OBB modelCajaCuadradaCollider4;
	glm::mat4 modelMatrixCajaCuadradaCollider4 = glm::mat4(modelMatrixCajaCuadrada4);
	// Set the orientation of collider before doing the scale
	modelCajaCuadradaCollider4.u = glm::quat_cast(modelMatrixCajaCuadradaCollider4);
	modelMatrixCajaCuadradaCollider4 = glm::translate(modelMatrixCajaCuadradaCollider4,
		glm::vec3(modelCajaCuadrada.getObb().c.x,
			modelCajaCuadrada.getObb().c.y,
			modelCajaCuadrada.getObb().c.z));
	modelCajaCuadradaCollider4.e = modelCajaCuadrada.getObb().e * glm::vec3(2.0, 2.0, 2.0)
		* glm::vec3(0.9, 0.9, 0.9);
	modelCajaCuadradaCollider4.c = glm::vec3(modelMatrixCajaCuadradaCollider4[3]);
	addOrUpdateColliders(collidersOBB2, "cajaCuadrada4", modelCajaCuadradaCollider4,
		modelMatrixCajaCuadradaCollider4);


	//Collider modelCajaCuadrada5
	AbstractModel::OBB modelCajaCuadradaCollider5;
	glm::mat4 modelMatrixCajaCuadradaCollider5 = glm::mat4(modelMatrixCajaCuadrada5);
	// Set the orientation of collider before doing the scale
	modelCajaCuadradaCollider5.u = glm::quat_cast(modelMatrixCajaCuadradaCollider5);
	modelMatrixCajaCuadradaCollider5 = glm::translate(modelMatrixCajaCuadradaCollider5,
		glm::vec3(modelCajaCuadrada.getObb().c.x,
			modelCajaCuadrada.getObb().c.y,
			modelCajaCuadrada.getObb().c.z));
	modelCajaCuadradaCollider5.e = modelCajaCuadrada.getObb().e * glm::vec3(2.0, 2.0, 2.0)
		* glm::vec3(0.9, 0.9, 0.9);
	modelCajaCuadradaCollider5.c = glm::vec3(modelMatrixCajaCuadradaCollider5[3]);
	addOrUpdateColliders(collidersOBB2, "cajaCuadrada5", modelCajaCuadradaCollider5,
		modelMatrixCajaCuadradaCollider5);

	//Collider modelCajaLowPoly
	AbstractModel::OBB modelCajaLowPolyCollider;
	glm::mat4 modelMatrixCajaLowPolyCollider = glm::mat4(modelMatrixCajaLowPoly);
	// Set the orientation of collider before doing the scale
	modelCajaLowPolyCollider.u = glm::quat_cast(modelMatrixCajaLowPolyCollider);
	modelMatrixCajaLowPolyCollider = glm::translate(modelMatrixCajaLowPolyCollider,
		glm::vec3(modelCajaLowPoly.getObb().c.x,
			modelCajaLowPoly.getObb().c.y,
			modelCajaLowPoly.getObb().c.z));
	modelCajaLowPolyCollider.e = modelCajaLowPoly.getObb().e * glm::vec3(2.0, 2.0, 2.0)
		* glm::vec3(0.9, 0.9, 0.9);
	modelCajaLowPolyCollider.c = glm::vec3(modelMatrixCajaLowPolyCollider[3]);
	addOrUpdateColliders(collidersOBB2, "cajaLowPoly1", modelCajaLowPolyCollider,
		modelMatrixCajaLowPolyCollider);

	//Collider modelCajaLowPoly2
	AbstractModel::OBB modelCajaLowPolyCollider2;
	glm::mat4 modelMatrixCajaLowPolyCollider2 = glm::mat4(modelMatrixCajaLowPoly2);
	// Set the orientation of collider before doing the scale
	modelCajaLowPolyCollider2.u = glm::quat_cast(modelMatrixCajaLowPolyCollider2);
	modelMatrixCajaLowPolyCollider2 = glm::translate(modelMatrixCajaLowPolyCollider2,
		glm::vec3(modelCajaLowPoly.getObb().c.x,
			modelCajaLowPoly.getObb().c.y,
			modelCajaLowPoly.getObb().c.z));
	modelCajaLowPolyCollider2.e = modelCajaLowPoly.getObb().e * glm::vec3(2.0, 2.0, 2.0)
		* glm::vec3(0.9, 0.9, 0.9);
	modelCajaLowPolyCollider2.c = glm::vec3(modelMatrixCajaLowPolyCollider2[3]);
	addOrUpdateColliders(collidersOBB2, "cajaLowPoly2", modelCajaLowPolyCollider2,
		modelMatrixCajaLowPolyCollider2);

	//Collider modelCajaLowPoly3
	AbstractModel::OBB modelCajaLowPolyCollider3;
	glm::mat4 modelMatrixCajaLowPolyCollider3 = glm::mat4(modelMatrixCajaLowPoly3);
	// Set the orientation of collider before doing the scale
	modelCajaLowPolyCollider3.u = glm::quat_cast(modelMatrixCajaLowPolyCollider3);
	modelMatrixCajaLowPolyCollider3 = glm::translate(modelMatrixCajaLowPolyCollider3,
		glm::vec3(modelCajaLowPoly.getObb().c.x,
			modelCajaLowPoly.getObb().c.y,
			modelCajaLowPoly.getObb().c.z));
	modelCajaLowPolyCollider3.e = modelCajaLowPoly.getObb().e * glm::vec3(2.0, 2.0, 2.0)
		* glm::vec3(0.9, 0.9, 0.9);
	modelCajaLowPolyCollider3.c = glm::vec3(modelMatrixCajaLowPolyCollider3[3]);
	addOrUpdateColliders(collidersOBB2, "cajaLowPoly3", modelCajaLowPolyCollider3,
		modelMatrixCajaLowPolyCollider3);

	//Collider modelCajaLowPoly4
	AbstractModel::OBB modelCajaLowPolyCollider4;
	glm::mat4 modelMatrixCajaLowPolyCollider4 = glm::mat4(modelMatrixCajaLowPoly4);
	// Set the orientation of collider before doing the scale
	modelCajaLowPolyCollider4.u = glm::quat_cast(modelMatrixCajaLowPolyCollider4);
	modelMatrixCajaLowPolyCollider4 = glm::translate(modelMatrixCajaLowPolyCollider4,
		glm::vec3(modelCajaLowPoly.getObb().c.x,
			modelCajaLowPoly.getObb().c.y,
			modelCajaLowPoly.getObb().c.z));
	modelCajaLowPolyCollider4.e = modelCajaLowPoly.getObb().e * glm::vec3(2.0, 2.0, 2.0)
		* glm::vec3(0.9, 0.9, 0.9);
	modelCajaLowPolyCollider4.c = glm::vec3(modelMatrixCajaLowPolyCollider4[3]);
	addOrUpdateColliders(collidersOBB2, "cajaLowPoly4", modelCajaLowPolyCollider4,
		modelMatrixCajaLowPolyCollider4);

	//Collider modelCajaLowPoly5
	AbstractModel::OBB modelCajaLowPolyCollider5;
	glm::mat4 modelMatrixCajaLowPolyCollider5 = glm::mat4(modelMatrixCajaLowPoly5);
	// Set the orientation of collider before doing the scale
	modelCajaLowPolyCollider5.u = glm::quat_cast(modelMatrixCajaLowPolyCollider5);
	modelMatrixCajaLowPolyCollider5 = glm::translate(modelMatrixCajaLowPolyCollider5,
		glm::vec3(modelCajaLowPoly.getObb().c.x,
			modelCajaLowPoly.getObb().c.y,
			modelCajaLowPoly.getObb().c.z));
	modelCajaLowPolyCollider5.e = modelCajaLowPoly.getObb().e * glm::vec3(2.0, 2.0, 2.0)
		* glm::vec3(0.9, 0.9, 0.9);
	modelCajaLowPolyCollider5.c = glm::vec3(modelMatrixCajaLowPolyCollider5[3]);
	addOrUpdateColliders(collidersOBB2, "cajaLowPoly5", modelCajaLowPolyCollider5,
		modelMatrixCajaLowPolyCollider5);

	//Collider modelCajaLowPoly6
	AbstractModel::OBB modelCajaLowPolyCollider6;
	glm::mat4 modelMatrixCajaLowPolyCollider6 = glm::mat4(modelMatrixCajaLowPoly6);
	// Set the orientation of collider before doing the scale
	modelCajaLowPolyCollider6.u = glm::quat_cast(modelMatrixCajaLowPolyCollider6);
	modelMatrixCajaLowPolyCollider6 = glm::translate(modelMatrixCajaLowPolyCollider6,
		glm::vec3(modelCajaLowPoly.getObb().c.x,
			modelCajaLowPoly.getObb().c.y,
			modelCajaLowPoly.getObb().c.z));
	modelCajaLowPolyCollider6.e = modelCajaLowPoly.getObb().e * glm::vec3(2.0, 2.0, 2.0)
		* glm::vec3(0.9, 0.9, 0.9);
	modelCajaLowPolyCollider6.c = glm::vec3(modelMatrixCajaLowPolyCollider6[3]);
	addOrUpdateColliders(collidersOBB2, "cajaLowPoly6", modelCajaLowPolyCollider6,
		modelMatrixCajaLowPolyCollider6);

	//Collider modelBidones
	AbstractModel::OBB modelBidonesCollider;
	glm::mat4 modelMatrixBidonesCollider = glm::mat4(modelMatrixBidones);
	// Set the orientation of collider before doing the scale
	modelBidonesCollider.u = glm::quat_cast(modelMatrixBidonesCollider);
	modelMatrixBidonesCollider = glm::translate(modelMatrixBidonesCollider,
		glm::vec3(modelBidones.getObb().c.x,
			modelBidones.getObb().c.y,
			modelBidones.getObb().c.z));
	modelBidonesCollider.e = modelBidones.getObb().e * glm::vec3(2.0, 2.0, 2.0)
		* glm::vec3(0.9, 0.9, 0.9);
	modelBidonesCollider.c = glm::vec3(modelMatrixBidonesCollider[3]);
	addOrUpdateColliders(collidersOBB2, "bidones1", modelBidonesCollider,
		modelMatrixBidonesCollider);

	//Collider modelBidones2
	AbstractModel::OBB modelBidonesCollider2;
	glm::mat4 modelMatrixBidonesCollider2 = glm::mat4(modelMatrixBidones2);
	// Set the orientation of collider before doing the scale
	modelBidonesCollider2.u = glm::quat_cast(modelMatrixBidonesCollider2);
	modelMatrixBidonesCollider2 = glm::translate(modelMatrixBidonesCollider2,
		glm::vec3(modelBidones.getObb().c.x,
			modelBidones.getObb().c.y,
			modelBidones.getObb().c.z));
	modelBidonesCollider2.e = modelBidones.getObb().e * glm::vec3(2.0, 2.0, 2.0)
		* glm::vec3(0.9, 0.9, 0.9);
	modelBidonesCollider2.c = glm::vec3(modelMatrixBidonesCollider2[3]);
	addOrUpdateColliders(collidersOBB2, "bidones2", modelBidonesCollider2,
		modelMatrixBidonesCollider2);

	//Collider modelBidones3
	AbstractModel::OBB modelBidonesCollider3;
	glm::mat4 modelMatrixBidonesCollider3 = glm::mat4(modelMatrixBidones3);
	// Set the orientation of collider before doing the scale
	modelBidonesCollider3.u = glm::quat_cast(modelMatrixBidonesCollider3);
	modelMatrixBidonesCollider3 = glm::translate(modelMatrixBidonesCollider3,
		glm::vec3(modelBidones.getObb().c.x,
			modelBidones.getObb().c.y,
			modelBidones.getObb().c.z));
	modelBidonesCollider3.e = modelBidones.getObb().e * glm::vec3(2.0, 2.0, 2.0)
		* glm::vec3(0.9, 0.9, 0.9);
	modelBidonesCollider3.c = glm::vec3(modelMatrixBidonesCollider3[3]);
	addOrUpdateColliders(collidersOBB2, "bidones3", modelBidonesCollider3,
		modelMatrixBidonesCollider3);

	//Collider modelBidones4
	AbstractModel::OBB modelBidonesCollider4;
	glm::mat4 modelMatrixBidonesCollider4 = glm::mat4(modelMatrixBidones4);
	// Set the orientation of collider before doing the scale
	modelBidonesCollider4.u = glm::quat_cast(modelMatrixBidonesCollider4);
	modelMatrixBidonesCollider4 = glm::translate(modelMatrixBidonesCollider4,
		glm::vec3(modelBidones.getObb().c.x,
			modelBidones.getObb().c.y,
			modelBidones.getObb().c.z));
	modelBidonesCollider4.e = modelBidones.getObb().e * glm::vec3(2.0, 2.0, 2.0)
		* glm::vec3(0.9, 0.9, 0.9);
	modelBidonesCollider4.c = glm::vec3(modelMatrixBidonesCollider4[3]);
	addOrUpdateColliders(collidersOBB2, "bidones4", modelBidonesCollider4,
		modelMatrixBidonesCollider4);

	//Collider boxCD
	AbstractModel::OBB boxCDCollider;
	glm::mat4 modelmatrixColliderBoxCD = glm::mat4(modelMatrixBoxCD);
	// Set the orientation of collider before doing the scale
	boxCDCollider.u = glm::quat_cast(modelmatrixColliderBoxCD);
	modelmatrixColliderBoxCD = glm::translate(modelmatrixColliderBoxCD,
		glm::vec3(boxCD.getObb().c.x + 33.5,
			boxCD.getObb().c.y,
			boxCD.getObb().c.z - 14.45));
	boxCDCollider.e = glm::vec3(2.0f, 10.0f, 12.0f);
	boxCDCollider.c = glm::vec3(modelmatrixColliderBoxCD[3]);
	addOrUpdateColliders(collidersOBB2, "boxCD", boxCDCollider,
		modelMatrixBoxCD);

	//Collider boxCIS
	AbstractModel::OBB boxCISCollider;
	glm::mat4 modelmatrixColliderboxCIS = glm::mat4(modelMatrixBoxCIS);
	// Set the orientation of collider before doing the scale
	boxCISCollider.u = glm::quat_cast(modelmatrixColliderboxCIS);
	modelmatrixColliderboxCIS = glm::translate(modelmatrixColliderboxCIS,
		glm::vec3(boxCIS.getObb().c.x + 19.7,
			boxCIS.getObb().c.y,
			boxCIS.getObb().c.z - 22.85));
	boxCISCollider.e = glm::vec3(0.3f, 10.0f, 4.5f);
	boxCISCollider.c = glm::vec3(modelmatrixColliderboxCIS[3]);
	addOrUpdateColliders(collidersOBB2, "boxCIS", boxCISCollider,
		modelMatrixBoxCIS);

	//Collider boxCII
	AbstractModel::OBB boxCIICollider;
	glm::mat4 modelmatrixColliderboxCII = glm::mat4(modelMatrixBoxCII);
	// Set the orientation of collider before doing the scale
	boxCIICollider.u = glm::quat_cast(modelmatrixColliderboxCII);
	modelmatrixColliderboxCII = glm::translate(modelmatrixColliderboxCII,
		glm::vec3(boxCII.getObb().c.x + 27.25,
			boxCII.getObb().c.y,
			boxCII.getObb().c.z - 6.4));
	boxCIICollider.e = glm::vec3(8.1f, 10.0f, 4.0f);
	boxCIICollider.c = glm::vec3(modelmatrixColliderboxCII[3]);
	addOrUpdateColliders(collidersOBB2, "boxCII", boxCIICollider,
		modelMatrixBoxCII);

	//Collider boxCS
	AbstractModel::OBB boxCSCollider;
	glm::mat4 modelmatrixColliderboxCS = glm::mat4(modelMatrixBoxCS);
	// Set the orientation of collider before doing the scale
	boxCSCollider.u = glm::quat_cast(modelmatrixColliderboxCS);
	modelmatrixColliderboxCS = glm::translate(modelmatrixColliderboxCS,
		glm::vec3(boxCS.getObb().c.x + 27.25,
			boxCS.getObb().c.y,
			boxCS.getObb().c.z - 27.55));
	boxCSCollider.e = glm::vec3(8.1f, 10.0f, 0.3f);
	boxCSCollider.c = glm::vec3(modelmatrixColliderboxCS[3]);
	addOrUpdateColliders(collidersOBB2, "boxCS", boxCSCollider,
		modelMatrixBoxCS);


	//Colliders Palancas
	for (int i = 0; i < palancaPos.size(); i++) {
		AbstractModel::OBB palancaCollider;
		glm::mat4 modelMatrixColliderPalanca = glm::mat4(1.0);
		modelMatrixColliderPalanca = glm::translate(modelMatrixColliderPalanca,
			palancaPos[i]);
		modelMatrixColliderPalanca = glm::rotate(modelMatrixColliderPalanca,
			glm::radians(90.0f), glm::vec3(0, 0, 1));
		addOrUpdateColliders(collidersOBB2, "palanca-" + std::to_string(i),
			palancaCollider, modelMatrixColliderPalanca);
		palancaCollider.u = glm::quat_cast(modelMatrixColliderPalanca);
		modelMatrixColliderPalanca = glm::scale(modelMatrixColliderPalanca,
			glm::vec3(1.0, 1.0, 1.0));
		modelMatrixColliderPalanca = glm::translate(modelMatrixColliderPalanca,
			modelPalanca.getObb().c);
		palancaCollider.c = glm::vec3(modelMatrixColliderPalanca[3]);
		palancaCollider.e = modelGenerador.getObb().e
			* glm::vec3(1.0, 1.0, 1.0);
		std::get<0>(collidersOBB2.find("palanca-" + std::to_string(i))->second) =
			palancaCollider;
	}

	//Collider puerta
	AbstractModel::OBB puertaCollider;
	glm::mat4 modelmatrixColliderPuerta = glm::mat4(1.0);
	modelmatrixColliderPuerta = glm::translate(modelmatrixColliderPuerta,
		glm::vec3(7.5f, 0.0f, -36.5f));
	modelmatrixColliderPuerta = glm::rotate(modelmatrixColliderPuerta,
		glm::radians(-90.0f), glm::vec3(0, 1, 0));
	// Set the orientation of collider before doing the scale
	puertaCollider.u = glm::quat_cast(modelmatrixColliderPuerta);
	modelmatrixColliderPuerta = glm::scale(modelmatrixColliderPuerta,
		glm::vec3(2.9f, 2.0f, 3.1f));
	modelmatrixColliderPuerta = glm::translate(modelmatrixColliderPuerta,
		glm::vec3(modelMarcoPuerta.getObb().c));
	puertaCollider.e = modelMarcoPuerta.getObb().e 
		* glm::vec3(2.9f, 2.0f, 3.1f);;
	puertaCollider.c = glm::vec3(modelmatrixColliderPuerta[3]);
	addOrUpdateColliders(collidersOBB2, "marcoPuerta", puertaCollider,
		modelMatrixMarcoPuerta);

	////Render colliders
	//for (std::map<std::string,
	//	std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
	//	collidersOBB2.begin(); it != collidersOBB2.end(); it++) {
	//	glm::mat4 matrixCollider = glm::mat4(1.0);
	//	matrixCollider = glm::translate(matrixCollider,
	//		std::get<0>(it->second).c);
	//	matrixCollider = matrixCollider
	//		* glm::mat4(std::get<0>(it->second).u);
	//	matrixCollider = glm::scale(matrixCollider,
	//		std::get<0>(it->second).e * 2.0f);
	//	//boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
	//	//boxCollider.enableWireMode();
	//	//boxCollider.render(matrixCollider);
	//}

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
					/*std::cout << "Colision " << it->first << " with "
						<< jt->first << std::endl;*/
					if ((it->first.compare("enemigo") == 0 || it->first.compare("astroProta") == 0)
						&& (jt->first.compare("enemigo") == 0 || jt->first.compare("astroProta") == 0))
							isCollisionEnemy = true;

					isCollision = true;
					isCollisionG = true;
				}
			}
		}
		addOrUpdateCollisionDetection(collisionDetection, it->first,
			isCollision);
	}

	if (isCollisionG && actionE && !enablePuerta) {
		//std::cout << "EntraPalancas " << std::endl;
		updatePalancaCollider(collisionDetection);
		actionE = false;
	}

	if (isCollisionG && enablePuerta && collisionDetection.find("marcoPuerta")->second) {
		cambianivel3 = true;
		escenario1 = false;
		escenario2 = false;
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
				//std::cout << "Colision " << it->first << " with "
				//	<< jt->first << std::endl;

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
		/*std::cout << "Primero" << std::endl;*/
		if (it != collidersSBB2.end()) {
			/*std::cout << "Segundo " << std::endl;*/
			if (!colIt->second)
				addOrUpdateColliders(collidersSBB2, it->first);
		}
		if (jt != collidersOBB2.end()) {
			/*std::cout << "Tercero " << std::endl;*/
			if (!colIt->second) {
				addOrUpdateColliders(collidersOBB2, jt->first);
			}
			else {
				//std::cout << "Cuarto " << jt->first << std::endl;
				if (jt->first.compare("enemigo") == 0) {
					if (isCollisionEnemy == true) {
						enemigo1.respawn = true;
						modelMatrixEnemigo = glm::translate(modelMatrixEnemigo, enemigo1.calculaReaparicion(enemigo1.origen, modelMatrixEnemigo[3]));
						playerRespawn = true;
						isCollisionEnemy = false;
						timer += (0.0075f * 20);
					}
				}
				if (jt->first.compare("astroProta") == 0) {
					/*std::cout << "Entra astroProta " << std::endl;*/
					modelMatrixAstroProta = std::get<1>(jt->second);
				}
			}
		}
	}

}

void soundEscene2() {
	if (sourcesPlay[3]) {
		// Apagado de sonidos primer nivel
		sourcesPlay[0] = false;
		sourcesPlay[1] = false;
		sourcesPlay[3] = false;
		sourcesPlay[5] = false;
		sourcesPlay[6] = false;
		sourcesPlay[7] = false;
		sourcesPlay[12] = false;
		sourcesPlay[8] = true;
		sourcesPlay[9] = true;
		sourcesPlay[10] = true;
	}

	//Interruptores reiniciar
	//Caminata
	source2Pos[0] = modelMatrixAstroProta[3].x;
	source2Pos[1] = modelMatrixAstroProta[3].y + 7;
	source2Pos[2] = modelMatrixAstroProta[3].z;
	alSourcefv(source[2], AL_POSITION, source2Pos);
	if (animationIndex != 1) {
		//std::cout << "Cambia sourcesPlay[2] " << std::endl;
		sourcesPlay[2] = true;
	}
	else {
		sourcesPlay[2] = false;
	}

	//Puerta
	source4Pos[0] = 7.5f;
	source4Pos[1] = terrain.getHeightTerrain(7.5f, -36.5f) + 1.0f;
	source4Pos[2] = -36.5f;
	alSourcefv(source[4], AL_POSITION, source4Pos);

	//Luces
	//Luz Código correcto G1
	source8Pos[0] = modelMatrixAstroProta[3].x;
	source8Pos[1] = modelMatrixAstroProta[3].y;
	source8Pos[2] = modelMatrixAstroProta[3].z;
	alSourcefv(source[8], AL_POSITION, source8Pos);
	//sourcesPlay[8] = true;

	//Luz Código correcto G2
	source9Pos[0] = modelMatrixAstroProta[3].x;
	source9Pos[1] = modelMatrixAstroProta[3].y;
	source9Pos[2] = modelMatrixAstroProta[3].z;
	alSourcefv(source[9], AL_POSITION, source9Pos);
	//sourcesPlay[8] = true;

	//Luz Código correcto G3
	source10Pos[0] = modelMatrixAstroProta[3].x;
	source10Pos[1] = modelMatrixAstroProta[3].y;
	source10Pos[2] = modelMatrixAstroProta[3].z;
	alSourcefv(source[10], AL_POSITION, source10Pos);
	//sourcesPlay[8] = true;

	//Movimiento Alien
	source11Pos[0] = modelMatrixEnemigo[3].x;
	source11Pos[1] = modelMatrixEnemigo[3].y;
	source11Pos[2] = modelMatrixEnemigo[3].z;
	alSourcefv(source[11], AL_POSITION, source11Pos);
	sourcesPlay[11] = true;
	
	//Ambiental Interior
	source13Pos[0] = modelMatrixAstroProta[3].x;
	source13Pos[1] = modelMatrixAstroProta[3].y;
	source13Pos[2] = modelMatrixAstroProta[3].z;
	alSourcefv(source[13], AL_POSITION, source13Pos);
	sourcesPlay[13] = true;

	//Código error
	source14Pos[0] = modelMatrixAstroProta[3].x;
	source14Pos[1] = modelMatrixAstroProta[3].y;
	source14Pos[2] = modelMatrixAstroProta[3].z;
	alSourcefv(source[14], AL_POSITION, source14Pos);
	//sourcesPlay[13] = true;
}

void cameraMove() {
	int camaraXcoord = 0;
	posterior = terrain.getXCoordTerrain(modelMatrixAstroProta[3][0]);
	camaraXcoord = terrain.getXCoordTerrain(modelMatrixPivoteCam[3][0]);
	if (camaraXcoord < posterior) {
		if (escenario1) {
			if (terrain.getXCoordTerrain(modelMatrixPivoteCam[3][0]) < limiteDerecho && terrain.getXCoordTerrain(modelMatrixAstroProta[3][0]) > limiteIzquierdo)
				modelMatrixPivoteCam = glm::translate(modelMatrixPivoteCam,
					glm::vec3(-0.1, 0, 0.0));
		}
		if (escenario2) {
			if (terrain2.getXCoordTerrain(modelMatrixPivoteCam[3][0]) < limiteDerecho && terrain2.getXCoordTerrain(modelMatrixAstroProta[3][0]) > limiteIzquierdo)
				modelMatrixPivoteCam = glm::translate(modelMatrixPivoteCam,
					glm::vec3(-0.1, 0, 0.0));
		}
	}
	if (camaraXcoord > posterior) {
		if (terrain.getXCoordTerrain(modelMatrixPivoteCam[3][0]) > limiteIzquierdo && terrain.getXCoordTerrain(modelMatrixAstroProta[3][0]) < limiteDerecho)
			modelMatrixPivoteCam = glm::translate(modelMatrixPivoteCam,
				glm::vec3(0.1, 0, 0.0));
	}

}

void cameraMove2() {
	int camaraXcoord = 0;
	posterior = terrain2.getXCoordTerrain(modelMatrixAstroProta[3][0]) - 74;
	camaraXcoord = terrain2.getXCoordTerrain(modelMatrixPivoteCam2[3][0]);
	//std::cout << "Posterior " << posterior <<std::endl;
	//std::cout << "camaraXcord " << camaraXcoord  <<std::endl;
	//std::cout << "Limite derecho " << (limiteDerecho - 30) << std::endl;
	if (camaraXcoord < posterior) {
		
		if (terrain2.getXCoordTerrain(modelMatrixPivoteCam2[3][0]) < (limiteDerecho - 30) && terrain2.getXCoordTerrain(modelMatrixAstroProta[3][0]) >= limiteIzquierdo) {
			modelMatrixPivoteCam2 = glm::translate(modelMatrixPivoteCam2,
				glm::vec3(-0.1, 0, 0.0));
			/*std::cout << "derecha " << std::endl;*/
		}
	}
	if (camaraXcoord > posterior) {
		
		if (terrain2.getXCoordTerrain(modelMatrixPivoteCam2[3][0]) > limiteIzquierdo) {
			modelMatrixPivoteCam2 = glm::translate(modelMatrixPivoteCam2,
				glm::vec3(0.1, 0, 0.0));
			/*std::cout << "izquierda " << std::endl;*/
		}
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
	if ((string1.compare("compuerta") == 0 || string1.compare("edCompuerta") == 0) &&
		(string2.compare("compuerta") == 0 || string2.compare("edCompuerta") == 0)) {
		return true;
	}
	if ((string1.compare("muroFrontal") == 0 || string1.compare("geneRocks2-3") == 0) &&
		(string2.compare("muroFrontal") == 0 || string2.compare("geneRocks2-3") == 0)) {
		return true;
	}
	if ((string1.compare("astroProta") == 0 || string1.compare("action") == 0) &&
		(string2.compare("astroProta") == 0 || string2.compare("action") == 0)) {
		return true;
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
	
	if ((string1.compare("boxCII") == 0 || string1.compare("muroFrontal2") == 0 ||
		string1.compare("boxCD") == 0) &&
		(string2.compare("boxCII") == 0 || string2.compare("muroFrontal2") == 0 ||
			string2.compare("boxCD") == 0)) {
		return true;
	}

	if ((string1.compare("boxCIS") == 0 || string1.compare("boxCS") == 0 ||
		string1.compare("boxCD") == 0) &&
		(string2.compare("boxCIS") == 0 || string2.compare("boxCS") == 0 ||
			string2.compare("boxCD") == 0)) {
		return true;
	}

	if ((string1.compare("palanca-0") == 0 || string1.compare("muroIzquierdo2") == 0 ) &&
		(string2.compare("palanca-0") == 0 || string2.compare("muroIzquierdo2") == 0)) {
		return true;
	}

	if ((string1.compare("palanca-1") == 0 || string1.compare("muroFondo2") == 0) &&
		(string2.compare("palanca-1") == 0 || string2.compare("muroFondo2") == 0)) {
		return true;
	}

	if ((string1.compare("palanca-2") == 0 || string1.compare("boxCS") == 0) &&
		(string2.compare("palanca-2") == 0 || string2.compare("boxCS") == 0)) {
		return true;
	}

	if ((string1.compare("palanca-3") == 0 || string1.compare("muroDerecho2") == 0) &&
		(string2.compare("palanca-3") == 0 || string2.compare("muroDerecho2") == 0)) {
		return true;
	}

	if ((string1.compare("astroProta") == 0 || string1.compare("action") == 0) &&
		(string2.compare("astroProta") == 0 || string2.compare("action") == 0)) {
		return true;
	}

	if ((string1.compare("muroFondo2") == 0 || string1.compare("marcoPuerta") == 0) &&
		(string2.compare("muroFondo2") == 0 || string2.compare("marcoPuerta") == 0)) {
		return true;
	}
	
	return false;
}

void updateBotonCollider(std::map<std::string, bool> collisionDetection) {
	for (int i = 0; i < botonesPos.size(); i++) {
		if (collisionDetection.find("botonBox-Y" + std::to_string(i))->second) {
			//std::cout << "collisionDetection " << collisionDetection.find("botonBox-Y" + std::to_string(i))->second << " de "
			//	<< "botonBox-Y" + std::to_string(i) << std::endl;
			//std::cout << "previo a update " << combBotones[i][0] << std::endl;
			combBotones[i][0] = !combBotones[i][0];
			//std::cout << "despues de update " << combBotones[i][0] << std::endl;
		}
		if (collisionDetection.find("botonBox-B" + std::to_string(i))->second) {
			//std::cout << "collisionDetection " << collisionDetection.find("botonBox-B" + std::to_string(i))->second << " de "
			//	<< "botonBox-B" + std::to_string(i) << std::endl;
			combBotones[i][1] = !combBotones[i][1];
		}
		if (collisionDetection.find("botonBox-R" + std::to_string(i))->second) {
			//std::cout << "collisionDetection " << collisionDetection.find("botonBox-R" + std::to_string(i))->second << " de "
			//	<< "botonBox-R" + std::to_string(i) << std::endl;
			combBotones[i][2] = !combBotones[i][2];
		}
	}
}

void updatePalancaCollider(std::map<std::string, bool> collisionDetection) {
	for (int i = 0; i < palancaPos.size(); i++) {
		if (collisionDetection.find("palanca-" + std::to_string(i))->second && !(animaPalancas[i] == 1)) {
			/*std::cout << "collisionDetection " << collisionDetection.find("palanca-" + std::to_string(i))->second << " de "
				<< "palanca-" + std::to_string(i) << std::endl;*/
			animaPalancas[i] = 1;
			indPalanca++;
		}
	}
}

void updateEscenario1() {
	if (!enableEscotilla1) {
		if (combBotones[0][0] && !combBotones[0][1] && !combBotones[0][2]) {
			lucesBotones[0] = true;
			sourcesPlay[8] = true;
			//std::cout << "combBotones[0] " << lucesBotones[0] << std::endl;
		}
		else {
			lucesBotones[0] = false;
		}
		if (combBotones[2][0] && !combBotones[2][1] && combBotones[2][2]) {
			lucesBotones[2] = true;
			sourcesPlay[9] = true;
			//std::cout << "combBotones[2] " << lucesBotones[2] << std::endl;
		}
		else {
			lucesBotones[2] = false;
		}
		if (combBotones[3][0] && combBotones[3][1] && !combBotones[3][2]) {
			lucesBotones[3] = true;
			alSourcePlay(source[8]);
			sourcesPlay[10] = true;
			//std::cout << "combBotones[3] " << lucesBotones[3] << std::endl;
		}
		else {
			lucesBotones[3] = false;
		}
		if (lucesBotones[0] && lucesBotones[2] && lucesBotones[3]) {
			enableEscotilla1 = true;
			animationIndexEscotilla = 2;
			sourcesPlay[3] = true;
			//std::cout << "enableEscotilla1 " << enableEscotilla1 << std::endl;
		}
	}
}

glm::vec3 colorGenerador(std::vector<bool> combBotones) {
	glm::vec3 color;
	//Blanco
	if (!combBotones[0] && !combBotones[1] && !combBotones[2]) {
		color = glm::vec3(0.0, 0.0, 0.0);
		return color;
	}
	//Amarillo
	if (combBotones[0] && !combBotones[1] && !combBotones[2]) {
		color = glm::vec3(1.2, 1.2, 0.0);
		//std::cout << "Amarillo " << std::endl;
		return color;
	}
	//Azul
	if (!combBotones[0] && combBotones[1] && !combBotones[2]) {
		color = glm::vec3(0.0, 0.0, 0.9);
		return color;
	}
	//Rojo
	if (!combBotones[0] && !combBotones[1] && combBotones[2]) {
		color = glm::vec3(0.9, 0.0, 0.0);
		return color;
	}
	//Verde
	if (combBotones[0] && combBotones[1] && !combBotones[2]) {
		color = glm::vec3(0.0, 0.9, 0.0);
		return color;
	}
	//Naranja
	if (combBotones[0] && !combBotones[1] && combBotones[2]) {

		color = glm::vec3(1.3, 0.5, 0.0);
		return color;
	}
	//Morado
	if (!combBotones[0] && combBotones[1] && combBotones[2]) {

		color = glm::vec3(0.3, 0.0, 0.7);
		return color;
	}
	//Cafe
	if (combBotones[0] && combBotones[1] && combBotones[2]) {
		color = glm::vec3(0.5, 0.25, 0.0);
		return color;
	}

}

void updateEscenario2() {
	if (!enablePuerta) {
		if (!lucesPalancas[0] && indPalanca == 1) {
			if ((animaPalancas[2] == 1)) {
				sourcesPlay[14] = false;
				sourcesPlay[8] = true;
				lucesPalancas[0] = true;
				//std::cout << "lucesPalancas[0] " << lucesPalancas[0] << std::endl;
			}
			else {
				for (int i = 0; i < lucesPalancas.size(); i++){
					sourcesPlay[8] = false;
					lucesPalancas[i] = false;
				}
				for (int i = 0; i < animaPalancas.size(); i++) {
					animaPalancas[i] = 0;
				}
				indPalanca=0;
				sourcesPlay[14] = true;
				//std::cout << "Combinación Incorrecta "<< std::endl;
			}
		}
		if (!lucesPalancas[1] && indPalanca == 2) {
			if ((animaPalancas[0] == 1)) {
				sourcesPlay[14] = false;
				sourcesPlay[9] = true;
				lucesPalancas[1] = true;
				//std::cout << "lucesPalancas[1] " << lucesPalancas[1] << std::endl;
			}
			else {
				for (int i = 0; i < lucesPalancas.size(); i++) {
					sourcesPlay[9] = false;
					lucesPalancas[i] = false;
				}
				for (int i = 0; i < animaPalancas.size(); i++) {
					animaPalancas[i] = 0;
				}
				indPalanca = 0;
				sourcesPlay[14] = true;
				//std::cout << "Combinación Incorrecta " << std::endl;
			}
		}
		if (!lucesPalancas[2] && indPalanca == 3) {
			if ((animaPalancas[3] == 1)) {
				sourcesPlay[14] = false;
				sourcesPlay[10] = true;
				lucesPalancas[2] = true;
				//std::cout << "lucesPalancas[2] " << lucesPalancas[2] << std::endl;
			}
			else {
				for (int i = 0; i < lucesPalancas.size(); i++) {
					sourcesPlay[10] = false;
					lucesPalancas[i] = false;
				}
				for (int i = 0; i < animaPalancas.size(); i++) {
					animaPalancas[i] = 0;
				}
				indPalanca = 0;
				sourcesPlay[14] = true;
				//std::cout << "Combinación Incorrecta " << std::endl;
			}
		}
		if ((lucesPalancas[0] && lucesPalancas[1] && lucesPalancas[2])) {
			enablePuerta = true;
			sourcesPlay[4] = true;
			//std::cout << "Combinación Correcta " << std::endl;
		}

	}
}

void luzMenus() {
	shaderMulLighting.setVectorFloat3("fogColor",
		glm::value_ptr(glm::vec3(0.5, 0.5, 0.0)));
	shaderMulLighting.setFloat("density", 0.00);
	shaderTerrain.setVectorFloat3("fogColor",
		glm::value_ptr(glm::vec3(0.5, 0.5, 0.0)));
	shaderTerrain.setFloat("density", 0.00);
	shaderSkybox.setVectorFloat3("fogColor",
		glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
	if (cameraSelected == 0)
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
	else
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(cameraFP->getPosition()));
	shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.4, 0.4, 0.4)));
	shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.7, 0.7, 0.7)));
	shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.9, 0.9, 0.9)));
	shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));
}

int main(int argc, char** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}