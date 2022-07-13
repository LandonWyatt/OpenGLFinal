#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <SOIL2/SOIL2.h>
#include "vertexArrays.h"
#include "ImportedModel.h"
#include "particles.h"

using namespace std;
#define numVAOs 1
#define numVBOs 16

float cameraX, cameraY, cameraZ;
float cubeLocX, cubeLocY, cubeLocZ;

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];
GLuint mvLoc, projLoc, nLoc;
GLuint globalAmbLoc, ambLoc, diffLoc, specLoc, posLoc, mAmbLoc, mDiffLoc, mSpecLoc, mShiLoc;

string readShaderSource(const char* filePath) {
	string content;
	ifstream fileStream(filePath, ios::in);
	string line = "";
	while (!fileStream.eof()) {
		getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
}

GLuint createShaderProgram(const char* vshader, const char* fshader) {
	string vertShaderStr = readShaderSource(vshader);
	string fragShaderStr = readShaderSource(fshader);
	const char* vertShaderSrc = vertShaderStr.c_str();
	const char* fragShaderSrc = fragShaderStr.c_str();

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vShader, 1, &vertShaderSrc, NULL);
	glShaderSource(fShader, 1, &fragShaderSrc, NULL);

	glCompileShader(vShader);
	glCompileShader(fShader);

	GLuint vfProgram = glCreateProgram();
	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);
	glLinkProgram(vfProgram);

	return vfProgram;
}

int width, height;
float aspect, rotAmt = 0;
float deltaTime = 0.0f, pitch, yaw = -90.0f, lastX = 500, lastY = 500, lastFrame = 0.0f;
int initial_time = time(NULL), final_time, frame_count, fps;
bool firstMouse = true, freeControl = false;
glm::mat4 pMat, vMat, mMat, mvMat, invTrMat;
glm::vec3 currentLightPos, lightPosV;
glm::vec3 cameraPos = glm::vec3(-3.0, 1.4, 12.0);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(-1.5f, 2.0f, 0.0f);

float lightPos[3];
float lightLocationX = 5.0f, lightLocationY = 3.0f, lightLocationZ = -5.0f;
GLuint skydome, grass, barnText, standingTallGrassImg, standingShortGrassImg, wheatImg, particleIMG, wheatBackImg;
GLuint loadTexture(const char* texImagePath);
void installLights(glm::mat4 vMatrix);
void findRandomValues();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void takeInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void setupVertices(void);

ImportedModel skyDome("models/skydome.obj");
ImportedModel Barn("models/barn.obj");

const float numTallGrassObjects = 1600;
const float numWheatObjects = 450;

float globalAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
// light properties
float lightAmbient[4] = { 0.2f, 0.2f, 0.25f, 1.0f };
float lightDiffuse[4] = { 0.2f, 0.2f, 0.25f, 1.0f };
float lightSpecular[4] = { 0.2f, 0.2f, 0.25f, 1.0f };
float lightDir[3] = { 0.0f, -1.0f, -0.3f };

// Materials
// // Used for particles
float* skydomeAmbient() { static float a[4] = { 1.0f, 1.0f, 1.0f, 1 }; return (float*)a; }
float* skydomeDiffuse() { static float a[4] = { 0.0f, 0.0f, 0.0f, 1 }; return (float*)a; }
float* skydomeSpecular() { static float a[4] = { 0.0f, 0.0f, 0.0f, 1 }; return (float*)a; }
float skydomeShininess() { return 1.0f; }
// Ground used for ground and grass objects
float* groundAmbient() { static float a[4] = { 0.4f, 0.45f, 0.4f, 1 }; return (float*)a; }
float* groundDiffuse() { static float a[4] = { 0.2f, 0.25f, 0.2f, 1 }; return (float*)a; }
float* groundSpecular() { static float a[4] = { 0.3f, 0.35f, 0.3f, 1 }; return (float*)a; }
float groundShininess() { return 1.0f; }
// Used for wheat
float* wheatAmbient() { static float a[4] = { 0.3f, 0.3f, 0.2f, 1 }; return (float*)a; }
float* wheatDiffuse() { static float a[4] = { 0.3f, 0.3f, 0.2f, 1 }; return (float*)a; }
float* wheatSpecular() { static float a[4] = { 0.35f, 0.35f, 0.2f, 1 }; return (float*)a; }
float wheatShininess() { return 1.0f; }
// Used for barn
float* barnAmbient() { static float a[4] = { 0.3f, 0.3f, 0.3f, 1 }; return (float*)a; }
float* barnDiffuse() { static float a[4] = { 0.4f, 0.4f, 0.4f, 1 }; return (float*)a; }
float* barnSpecular() { static float a[4] = { 0.7f, 0.7f, 0.7f, 1 }; return (float*)a; }
float barnShininess() { return 1.0f; }
// Used for particles
float* partAmbient() { static float a[4] = { 1.0f, 1.0f, 1.0f, 1 }; return (float*)a; }
float* partDiffuse() { static float a[4] = { 1.0f, 1.0f, 1.0f, 1 }; return (float*)a; }
float* partSpecular() { static float a[4] = { 1.0f, 1.0f, 1.0f, 1 }; return (float*)a; }
float partShininess() { return 5.0f; }

float* matAmb = skydomeAmbient();
float* matDif = skydomeDiffuse();
float* matSpe = skydomeSpecular();
float matShi = skydomeShininess();

void init(GLFWwindow* window) {
	renderingProgram = createShaderProgram("vshaderSource.glsl", "fshaderSource.glsl");
	// Load all textures
	skydome = loadTexture("textures/skydomeNight.jpg");
	grass = loadTexture("textures/grass.jpg");
	barnText = loadTexture("textures/barnIMG.png");
	standingTallGrassImg = loadTexture("textures/standingTallGrassText.png");
	wheatImg = loadTexture("textures/wheatImg.png");
	particleIMG = loadTexture("textures/particle.png");
	wheatBackImg = loadTexture("textures/wheatBack.png");
	// Call function to send all vertices, normals, and textures to buffers
	setupVertices();
	// Call function to find all random values for wheat and grass
	findRandomValues();
}

void display(GLFWwindow* window, double currentTime) {
	glClearColor(0.2f, 0.2f, 0.5f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(renderingProgram);
	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	if (freeControl) { takeInput(window); }
	
	// Keep track of framerate for testing purposes
	frame_count++;
	final_time = time(NULL);
	if (final_time - initial_time > 0) {
		cout << "FPS: " << frame_count / (final_time - initial_time) << endl;
		fps = frame_count / (final_time - initial_time);
		frame_count = 0;
		initial_time = final_time;
	}

	// Set values for attenuation for the positional lighting
	GLuint constant = glGetUniformLocation(renderingProgram, "light.constant");
	GLuint linear = glGetUniformLocation(renderingProgram, "light.linear");
	GLuint quadratic = glGetUniformLocation(renderingProgram, "light.quadratic");
	glUniform1f(constant, 1.0f);
	glUniform1f(linear, 0.045f);
	glUniform1f(quadratic, 0.0075f);

	// get the uniform variables for the MV and projection matrices
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
	nLoc = glGetUniformLocation(renderingProgram, "norm_matrix");
	// build perspective matrix
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees

	glm::mat4 view;
	// Check if freecontrol is active, if so, camera is unlocked and able to move
	if (!freeControl) {
		glm::vec3 cameraPos = glm::vec3(-3.0, 1.4, 12.0);
		glm::vec3 cameraUp = glm::vec3(-1.5f, 2.0f, 0.0f);
		view = glm::lookAt(cameraPos, cameraUp, glm::vec3(0.0, 1.0, 0.0));
	}
	else {
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}
	vMat = view;
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ));

	glm::vec3 initialLightLoc = glm::vec3(lightLocationX, lightLocationY, lightLocationZ);
	currentLightPos = glm::vec3(initialLightLoc.x, initialLightLoc.y, initialLightLoc.z);
	// Set materials for skydome
	matAmb = skydomeAmbient();
	matDif = skydomeDiffuse();
	matSpe = skydomeSpecular();
	matShi = skydomeShininess();
	installLights(vMat);
	// Scale skydome to less than half size
	mMat = glm::scale(mMat, glm::vec3(0.4f));
	// Rotate skydome slowly
	rotAmt += 0.0001;
	mMat = glm::rotate(mMat, rotAmt, glm::vec3(0.0f, 1.0f, 0.0f));
	mvMat = vMat * mMat;

	invTrMat = glm::transpose(glm::inverse(mvMat));

	// copy perspective and MV matrices to corresponding uniform variables
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));

	// Skydome
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, skydome);
	glGenerateMipmap(GL_TEXTURE_2D);

	glDrawArrays(GL_TRIANGLES, 0, skyDome.getNumVertices());

	// Set materials for ground and grass
	matAmb = groundAmbient();
	matDif = groundDiffuse();
	matSpe = groundSpecular();
	matShi = groundShininess();
	installLights(vMat);

	// Reset mMat
	mMat = glm::mat4(1.0f);
	mMat = glm::scale(mMat, glm::vec3(1.5f));
	mvMat = vMat * mMat;

	invTrMat = glm::transpose(glm::inverse(mvMat));

	// copy perspective and MV matrices to corresponding uniform variables
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));

	// Ground
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, grass);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);

	if (glewIsSupported("GL_EXT_texture_filter_anistropic")) {
		GLfloat anisoSetting = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting);
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glm::vec3 grassCoords = glm::vec3(0.0f, 0.0f, 0.0f);
	// Create Tall grass objects
	for (int i = 0; i < numTallGrassObjects; i++) {
		grassCoords.x = standingTallGrassPositions[i].x;
		grassCoords.z = standingTallGrassPositions[i].y;

		// Place throughout map with randomized information
		mMat = glm::translate(glm::mat4(1.0f), grassCoords);
		mMat = glm::rotate(mMat, glm::radians(standingTallGrassRotations[i]), glm::vec3(0.0f, 1.0f, 0.0f));
		mMat = glm::scale(mMat, standingTallGrassSizes[i]);

		// Referenced: https://rneff9.wixsite.com/projects/project
		// Develop "wind", skews the grass to a certain amount based on current time
		float windStrength = 10.0f;
		float windPower = cos(grassCoords.x / 10  + grassCoords.z / 10 + glfwGetTime() * (1.2f + windStrength / 20.0f));
		if (windPower < 0.0f) {
			windPower = windPower * 0.2f;
		}
		else {
			windPower = windPower * 0.3f;
		}

		float windEffect = sin(glfwGetTime()) * windPower;
		// Change position in model matrix to skew plants
		mMat[1][0] = windEffect;

		mvMat = vMat * mMat;

		invTrMat = glm::transpose(glm::inverse(mvMat));
		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
		glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, standingTallGrassImg);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// Set materials for wheat
	matAmb = wheatAmbient();
	matDif = wheatDiffuse();
	matSpe = wheatSpecular();
	matShi = wheatShininess();
	installLights(vMat);
	mvMat = vMat * mMat;

	invTrMat = glm::transpose(glm::inverse(mvMat));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));

	glm::vec3 wheatCoords = glm::vec3(0.0f, -0.1f, 0.0f);
	// Create wheat objects objects
	for (int i = 0; i < numWheatObjects; i++) {
		// Find X coordinate for first half from what is calculated in findRandomValues()
		if (i < numWheatObjects / 2) {
			wheatCoords.x = wheatPosition[i];
		}
		// Find Z coordinate for second half
		else {
			wheatCoords.z = wheatPosition[i];
		}
		
		// Calculate Z coordinate for first half of wheat
		// First Row
		if (i < (numWheatObjects / 6)) {
			wheatCoords.z = -15 + i * 0.4f;
		}
		// Second Row
		else if (i < (numWheatObjects / 6) * 2) {
			wheatCoords.z = -45 + (i * 0.4f);
		}
		// Third Row
		else if (i < (numWheatObjects / 6) * 3) {
			wheatCoords.z = -75 + (i * 0.4f);
		}
		// Calculate X coordinate for second half of wheat
		// Fourth Row
		else if (i < (numWheatObjects / 6) * 4) {
			wheatCoords.x = -112 + (i * 0.4f);
		}
		// Fifth Row
		else if (i < (numWheatObjects / 6) * 5) {
			wheatCoords.x = -142 + (i * 0.4f);
		}
		// Sixth Row
		else {
			wheatCoords.x = -172 + (i * 0.4f);
		}

		// Place in specific rows, rotate w/ random values, and scale w/ random values
		mMat = glm::translate(glm::mat4(1.0f), wheatCoords);
		mMat = glm::rotate(mMat, glm::radians(wheatRotations[i]), glm::vec3(0.0f, 1.0f, 0.0f));
		mMat = glm::scale(mMat, glm::vec3(1.0f, wheatHeights[i], 1.0f));
		
		// Referenced: https://rneff9.wixsite.com/projects/project
		// Develop "wind", skews the grass to a certain amount based on current time
		float windStrength = 2.0f;
		float windPower = cos(wheatCoords.x / 10 + wheatCoords.z / 10 + glfwGetTime() * (1.2f + windStrength / 20.0f));
		if (windPower < 0.0f) {
			windPower = windPower * 0.1f;
		}
		else {
			windPower = windPower * 0.2f;
		}

		float windEffect = sin(glfwGetTime()) * windPower;
		// Change position in model matrix to skew plants
		mMat[1][0] = windEffect;

		mvMat = vMat * mMat;

		invTrMat = glm::transpose(glm::inverse(mvMat));
		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
		glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[9]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[10]);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, wheatImg);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// Back of wheat to prevent seeing too much of skydome through wheat
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(-10.5f,0.0f,-17.0f));
	invTrMat = glm::transpose(glm::inverse(mvMat));
	mvMat = vMat * mMat;
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[14]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[15]);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, wheatBackImg);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Barn
	// Set barn materials
	matAmb = barnAmbient();
	matDif = barnDiffuse();
	matSpe = barnSpecular();
	matShi = barnShininess();
	installLights(vMat);

	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(15.0f, 0.0f, -15.0f));
	mMat = glm::rotate(mMat, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	mMat = glm::scale(mMat, glm::vec3(0.8f));
	mvMat = vMat * mMat;

	invTrMat = glm::transpose(glm::inverse(mvMat));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[12]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[13]);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, barnText);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	if (glewIsSupported("GL_EXT_texture_filter_anistropic")) {
		GLfloat anisoSetting = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting);
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, Barn.getNumVertices());

	// Set particle materials
	matAmb = partAmbient();
	matDif = partDiffuse();
	matSpe = partSpecular();
	matShi = partShininess();
	installLights(vMat);

	// Set for free control movement and updating particles
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	// Particle updating
	updateParticles(deltaTime);
	// translates particle position and draws particle as a point
	for (Particle particle : particles) {
		if (particle.life > 0.0f) {
			// Set new position for particle
			mMat = glm::translate(glm::mat4(1.0f), particle.position);
			mMat = glm::scale(mMat, glm::vec3(0.01f));
			mvMat = vMat * mMat;

			invTrMat = glm::transpose(glm::inverse(mvMat));

			glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
			glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));

			glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			
			glBindBuffer(GL_ARRAY_BUFFER, vbo[8]);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(2);
			
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, particleIMG);
			
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			
			// Draw for each particle
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}
}

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 16);
	glEnable(GL_MULTISAMPLE);
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "Final Project", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);
	init(window);
	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void setupVertices(void) {
	// Get skydome information
	std::vector<glm::vec3> vert = skyDome.getVertices();
	std::vector<glm::vec3> norm = skyDome.getNormals();
	std::vector<glm::vec2> tex = skyDome.getTextureCoords();
	int numObjVertices = skyDome.getNumVertices();
	std::vector<float> pvalues; // vertex positions
	std::vector<float> nvalues; // normal vectors
	std::vector<float> tvalues; // texture coordinates
	for (int i = 0; i < numObjVertices; i++) {
		pvalues.push_back((vert[i]).x);
		pvalues.push_back((vert[i]).y);
		pvalues.push_back((vert[i]).z);
		nvalues.push_back((norm[i]).x);
		nvalues.push_back((norm[i]).y);
		nvalues.push_back((norm[i]).z);
		tvalues.push_back((tex[i]).s);
		tvalues.push_back((tex[i]).t);
	}

	// Get barn information
	std::vector<glm::vec3> barnVert = Barn.getVertices();
	std::vector<glm::vec3> barnNorm = Barn.getNormals();
	std::vector<glm::vec2> barnTex = Barn.getTextureCoords();
	int barnNumObjVertices = Barn.getNumVertices();
	std::vector<float> barnPvalues; // vertex positions
	std::vector<float> barnNvalues; // normal vectors
	std::vector<float> barnTvalues; // texture coordinates
	for (int i = 0; i < barnNumObjVertices; i++) {
		barnPvalues.push_back((barnVert[i]).x);
		barnPvalues.push_back((barnVert[i]).y);
		barnPvalues.push_back((barnVert[i]).z);
		barnNvalues.push_back((barnNorm[i]).x);
		barnNvalues.push_back((barnNorm[i]).y);
		barnNvalues.push_back((barnNorm[i]).z);
		barnTvalues.push_back((barnTex[i]).s);
		barnTvalues.push_back((barnTex[i]).t);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);
	// Skydome information
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

	// Ground information (vertex information is found in vertexArrays.h)
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground), ground, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(groundTexPos), groundTexPos, GL_STATIC_DRAW);
	// Tall Grass
	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(standingTallGrass), standingTallGrass, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(standingGrassText), standingGrassText, GL_STATIC_DRAW);

	// Particles
	glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particleObject), particleObject, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[8]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particleObjectText), particleObjectText, GL_STATIC_DRAW);

	// Wheat
	glBindBuffer(GL_ARRAY_BUFFER, vbo[9]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wheat), wheat, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[10]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wheatText), wheatText, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[14]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wheatBack), wheatBack, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[15]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wheatBackText), wheatBackText, GL_STATIC_DRAW);

	// Barn information
	glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	glBufferData(GL_ARRAY_BUFFER, barnPvalues.size() * 4, &barnPvalues[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[12]);
	glBufferData(GL_ARRAY_BUFFER, barnNvalues.size() * 4, &barnNvalues[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[13]);
	glBufferData(GL_ARRAY_BUFFER, barnTvalues.size() * 4, &barnTvalues[0], GL_STATIC_DRAW);
}

void installLights(glm::mat4 vMatrix) {
	// convert light’s position to view space, and save it in a float array
	lightPosV = glm::vec3(vMatrix * glm::vec4(currentLightPos, 1.0));
	lightPos[0] = lightPosV.x;
	lightPos[1] = lightPosV.y;
	lightPos[2] = lightPosV.z;
	// get the locations of the light and material fields in the shader
	globalAmbLoc = glGetUniformLocation(renderingProgram, "globalAmbient");
	ambLoc = glGetUniformLocation(renderingProgram, "light.ambient");
	diffLoc = glGetUniformLocation(renderingProgram, "light.diffuse");
	specLoc = glGetUniformLocation(renderingProgram, "light.specular");
	posLoc = glGetUniformLocation(renderingProgram, "light.direction");
	mAmbLoc = glGetUniformLocation(renderingProgram, "material.ambient");
	mDiffLoc = glGetUniformLocation(renderingProgram, "material.diffuse");
	mSpecLoc = glGetUniformLocation(renderingProgram, "material.specular");
	mShiLoc = glGetUniformLocation(renderingProgram, "material.shininess");
	// set the uniform light and material values in the shader
	glProgramUniform4fv(renderingProgram, globalAmbLoc, 1, globalAmbient);
	glProgramUniform4fv(renderingProgram, ambLoc, 1, lightAmbient);
	glProgramUniform4fv(renderingProgram, diffLoc, 1, lightDiffuse);
	glProgramUniform4fv(renderingProgram, specLoc, 1, lightSpecular);
	glProgramUniform3fv(renderingProgram, posLoc, 1, lightPos);
	glProgramUniform4fv(renderingProgram, mAmbLoc, 1, matAmb);
	glProgramUniform4fv(renderingProgram, mDiffLoc, 1, matDif);
	glProgramUniform4fv(renderingProgram, mSpecLoc, 1, matSpe);
	glProgramUniform1f(renderingProgram, mShiLoc, matShi);
}

void findRandomValues() {
	srand((unsigned int)time(0));
	// GRASS
	// Grass positions
	for (int i = 0; i < numTallGrassObjects; i++) {
		float randX = (rand() % 400 - 150) / 10.0f, randZ = (rand() % 350 - 200) / 10.0f;
		// Prevent placing grass in barn or behind barn
		glm::vec2 inBarnX = glm::vec2(8.0f, 22.0f), inBarnZ = glm::vec2(-25.0f, -5.0f);
		while ((randX > inBarnX.x && randX < inBarnX.y) && (randZ < inBarnZ.y)) {
			if (randX > inBarnX.x && randX < inBarnX.y) {
				randX = (rand() % 500 - 230) / 10.0f;
			}
			if (randZ < inBarnZ.y) {
				randZ = (rand() % 500 - 300) / 10.0f;
			}
		}
		standingTallGrassPositions[i] = glm::vec2(randX, randZ);
	}
	// Grass Rotations
	for (int i = 0; i < numTallGrassObjects; i++) {
		standingTallGrassRotations[i] = rand() % 360 + 1;
	}
	float tempVal;
	// Grass sizes
	for (int i = 0; i < numTallGrassObjects; i++) {
		tempVal = ((float) (rand()) / (float) (RAND_MAX)) * 0.4f;
		standingTallGrassSizes[i].x = tempVal + 0.4f;
		standingTallGrassSizes[i].y = tempVal + 0.4f;
		standingTallGrassSizes[i].z = tempVal + 0.4f;
	}

	// WHEAT
	// Wheat rotations
	for (int i = 0; i < numWheatObjects; i++) {
		wheatRotations[i] = rand() % 360 + 1;
	}
	// Wheat positions
	// First row
	// Calculates x values
	for (int i = 0; i < (numWheatObjects / 6); i++) {
		// Get a random small value between -31 - -30
		wheatPosition[i] = ((rand() % 10) - 160) / 10.0f;
	}
	// Second row
	// Calculates x values
	for (int i = (numWheatObjects / 6); i < (numWheatObjects / 6) * 2; i++) {
		// Get a random small value between -32 - -31
		wheatPosition[i] = ((rand() % 10) - 170) / 10.0f;
	}
	// Third row
	// Calculates x values
	for (int i = (numWheatObjects / 6) * 2; i < (numWheatObjects / 6) * 3; i++) {
		// Get a random small value between -32 - -31
		wheatPosition[i] = ((rand() % 10) - 180) / 10.0f;
	}
	// Fourth row
	// Calculates z values
	for (int i = (numWheatObjects / 6) * 3; i < (numWheatObjects / 6) * 4; i++) {
		// Get a random small value between -32 - -31
		wheatPosition[i] = ((rand() % 10) - 160) / 10.0f;
	}
	// Fifth row
	// Calculates z values
	for (int i = (numWheatObjects / 6) * 4; i < (numWheatObjects / 6) * 5; i++) {
		// Get a random small value between -32 - -31
		wheatPosition[i] = ((rand() % 10) - 170) / 10.0f;
	}
	// Sixth row
	// Calculates z values
	for (int i = (numWheatObjects / 6) * 5; i < numWheatObjects; i++) {
		// Get a random small value between -32 - -31
		wheatPosition[i] = ((rand() % 10) - 180) / 10.0f;
	}
	// Wheat sizes
	for (int i = 0; i < numWheatObjects; i++) {
		tempVal = ((float)(rand()) / (float)(RAND_MAX)) * 0.2f;
		wheatHeights[i] = tempVal + 0.9;
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		if (freeControl) {
			freeControl = false;
		}
		else {
			freeControl = true;
			glm::vec3 cameraPos = glm::vec3(0.0, 1.6, 20.0);
		}
	}
	else if (key == GLFW_KEY_LEFT_BRACKET && action == GLFW_PRESS) {
		lightAmbient[0] = 0.2f, lightAmbient[1] = 0.2f, lightAmbient[2] = 0.25f;
		lightDiffuse[0] = 0.2f, lightDiffuse[1] = 0.2f, lightDiffuse[2] = 0.25f;
		lightSpecular[0] = 0.2f, lightSpecular[1] = 0.2f, lightSpecular[2] = 0.25f;
		skydome = loadTexture("textures/skydomeNight.jpg");
	}
	else if (key == GLFW_KEY_RIGHT_BRACKET && action == GLFW_PRESS) {
		lightAmbient[0] = 0.8f, lightAmbient[1] = 0.8f, lightAmbient[2] = 0.8f;
		lightDiffuse[0] = 0.8f, lightDiffuse[1] = 0.8f, lightDiffuse[2] = 0.8f;
		lightSpecular[0] = 0.8f, lightSpecular[1] = 0.8f, lightSpecular[2] = 0.8f;
		skydome = loadTexture("textures/skydomeDay.jpg");
	}
	else if (key == GLFW_KEY_ESCAPE) {
		glfwDestroyWindow(window);
		glfwTerminate();
		exit(EXIT_SUCCESS);
	}
}

void takeInput(GLFWwindow* window) {
	float cameraSpeed = 10.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraFront;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraFront;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwDestroyWindow(window);
		glfwTerminate();
		exit(EXIT_SUCCESS);
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

GLuint loadTexture(const char* texImagePath) {
	GLuint textureID;
	textureID = SOIL_load_OGL_texture(texImagePath,
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (textureID == 0) cout << "could not find texture file" << texImagePath << endl;
	return textureID;
}