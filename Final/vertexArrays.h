#include <glm\glm.hpp>

// Ground
static float ground[] = {
	-45.0f, 0.0f, -40.0f, 0.0f, 1.0f, 0.0f,
	 35.0f, 0.0f,  40.0f, 0.0f, 1.0f, 0.0f,
	 35.0f, 0.0f, -40.0f, 0.0f, 1.0f, 0.0f,
	-45.0f, 0.0f, -40.0f, 0.0f, 1.0f, 0.0f,
	-45.0f, 0.0f,  40.0f, 0.0f, 1.0f, 0.0f,
	 35.0f, 0.0f,  40.0f, 0.0f, 1.0f, 0.0f
};
static float groundTexPos[] = {
	0.0f, 20.0f, 20.0f, 0.0f, 20.0f, 20.0f,
	0.0f, 20.0f,  0.0f, 0.0f, 20.0f,  0.0f
};

// Tall Grass
static float standingTallGrass[] = {
	// back-left to front-right
	// (facing front-left)
	-0.61f, 0.0f,-1.0f,-1.0f, 0.0f, 1.0f,
	 0.59f, 0.0f, 1.0f,-1.0f, 0.0f, 1.0f,
	 0.59f, 1.0f, 1.0f,-1.0f, 0.0f, 1.0f,
	 0.59f, 1.0f, 1.0f,-1.0f, 0.0f, 1.0f,
	-0.61f, 1.0f,-1.0f,-1.0f, 0.0f, 1.0f,
	-0.61f, 0.0f,-1.0f,-1.0f, 0.0f, 1.0f,
	// (facing back-right)
	 0.61f, 0.0f, 1.0f, 1.0f, 0.0f,-1.0f,
	-0.59f, 0.0f,-1.0f, 1.0f, 0.0f,-1.0f,
	-0.59f, 1.0f,-1.0f, 1.0f, 0.0f,-1.0f,
	-0.59f, 1.0f,-1.0f, 1.0f, 0.0f,-1.0f,
	 0.61f, 1.0f, 1.0f, 1.0f, 0.0f,-1.0f,
	 0.61f, 0.0f, 1.0f, 1.0f, 0.0f,-1.0f,

	 // front-left to back-right
	 // (facing front-right)
	 -0.61f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	  0.59f, 0.0f,-1.0f, 1.0f, 0.0f, 1.0f,
	  0.59f, 1.0f,-1.0f, 1.0f, 0.0f, 1.0f,
	  0.59f, 1.0f,-1.0f, 1.0f, 0.0f, 1.0f,
	 -0.61f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	 -0.61f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	 // (facing back-left)
	  0.61f, 0.0f,-1.0f,-1.0f, 0.0f,-1.0f,
	 -0.59f, 0.0f, 1.0f,-1.0f, 0.0f,-1.0f,
	 -0.59f, 1.0f, 1.0f,-1.0f, 0.0f,-1.0f,
	 -0.59f, 1.0f, 1.0f,-1.0f, 0.0f,-1.0f,
	  0.61f, 1.0f,-1.0f,-1.0f, 0.0f,-1.0f,
	  0.61f, 0.0f,-1.0f,-1.0f, 0.0f,-1.0f,

	  // Left to Right
	  // (front)
	  -0.6f, 1.0f, 0.01f, 0.0f, 0.0f, 1.0f,
	  -0.6f, 0.0f, 0.01f, 0.0f, 0.0f, 1.0f,
	   0.6f, 0.0f, 0.01f, 0.0f, 0.0f, 1.0f,
	   0.6f, 0.0f, 0.01f, 0.0f, 0.0f, 1.0f,
	   0.6f, 1.0f, 0.01f, 0.0f, 0.0f, 1.0f,
	  -0.6f, 1.0f, 0.01f, 0.0f, 0.0f, 1.0f,

	  // (back)
	   0.6f, 0.0f,-0.01f, 0.0f, 0.0f,-1.0f,
	  -0.6f, 0.0f,-0.01f, 0.0f, 0.0f,-1.0f,
	  -0.6f, 1.0f,-0.01f, 0.0f, 0.0f,-1.0f,
	  -0.6f, 1.0f,-0.01f, 0.0f, 0.0f,-1.0f,
	   0.6f, 1.0f,-0.01f, 0.0f, 0.0f,-1.0f,
	   0.6f, 0.0f,-0.01f, 0.0f, 0.0f,-1.0f,
};

//Short Grass
static float standingShortGrass[] = {
	// back-left to front-right
	// (facing front-left)
	-2.01f, 0.0f,-1.0f,-1.0f, 0.0f, 1.0f,
	 1.99f, 0.0f, 1.0f,-1.0f, 0.0f, 1.0f,
	 1.99f, 0.3f, 1.0f,-1.0f, 0.0f, 1.0f,
	 1.99f, 0.3f, 1.0f,-1.0f, 0.0f, 1.0f,
	-2.01f, 0.3f,-1.0f,-1.0f, 0.0f, 1.0f,
	-2.01f, 0.0f,-1.0f,-1.0f, 0.0f, 1.0f,
	// (facing back-right)
	 2.01f, 0.0f, 1.0f, 1.0f, 0.0f,-1.0f,
	-1.99f, 0.0f,-1.0f, 1.0f, 0.0f,-1.0f,
	-1.99f, 0.3f,-1.0f, 1.0f, 0.0f,-1.0f,
	-1.99f, 0.3f,-1.0f, 1.0f, 0.0f,-1.0f,
	 2.01f, 0.3f, 1.0f, 1.0f, 0.0f,-1.0f,
	 2.01f, 0.0f, 1.0f, 1.0f, 0.0f,-1.0f,

	 // front-left to back-right
	 // (facing front-right)
	 -2.01f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	  1.99f, 0.0f,-1.0f, 1.0f, 0.0f, 1.0f,
	  1.99f, 0.3f,-1.0f, 1.0f, 0.0f, 1.0f,
	  1.99f, 0.3f,-1.0f, 1.0f, 0.0f, 1.0f,
	 -2.01f, 0.3f, 1.0f, 1.0f, 0.0f, 1.0f,
	 -2.01f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	 // (facing back-left)
	  2.01f, 0.0f,-1.0f,-1.0f, 0.0f,-1.0f,
	 -1.99f, 0.0f, 1.0f,-1.0f, 0.0f,-1.0f,
	 -1.99f, 0.3f, 1.0f,-1.0f, 0.0f,-1.0f,
	 -1.99f, 0.3f, 1.0f,-1.0f, 0.0f,-1.0f,
	  2.01f, 0.3f,-1.0f,-1.0f, 0.0f,-1.0f,
	  2.01f, 0.0f,-1.0f,-1.0f, 0.0f,-1.0f,

	  // Left to Right
	  // (front)
	  -2.0f, 0.3f, 0.01f, 0.0f, 0.0f, 1.0f,
	  -2.0f, 0.0f, 0.01f, 0.0f, 0.0f, 1.0f,
	   2.0f, 0.0f, 0.01f, 0.0f, 0.0f, 1.0f,
	   2.0f, 0.0f, 0.01f, 0.0f, 0.0f, 1.0f,
	   2.0f, 0.3f, 0.01f, 0.0f, 0.0f, 1.0f,
	  -2.0f, 0.3f, 0.01f, 0.0f, 0.0f, 1.0f,

	  // (back)
	   2.0f, 0.0f,-0.01f, 0.0f, 0.0f,-1.0f,
	  -2.0f, 0.0f,-0.01f, 0.0f, 0.0f,-1.0f,
	  -2.0f, 0.3f,-0.01f, 0.0f, 0.0f,-1.0f,
	  -2.0f, 0.3f,-0.01f, 0.0f, 0.0f,-1.0f,
	   2.0f, 0.3f,-0.01f, 0.0f, 0.0f,-1.0f,
	   2.0f, 0.0f,-0.01f, 0.0f, 0.0f,-1.0f,
};
static float standingGrassText[] = {
	// back-left to front-right
	// (facing front-left)
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,
	// (facing back-right)
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,

	// front-left to back-right
	// (facing front-right)
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,
	// (facing back-left)
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,

	// Left to Right
	// (front)
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	// (back)
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f
};

// Wheat
static float wheat[] = {
	// back-left to front-right
	// (facing front-left)
	-0.51f, 0.0f,-0.5f,-1.0f, 0.0f, 1.0f,
	 0.49f, 0.0f, 0.5f,-1.0f, 0.0f, 1.0f,
	 0.49f, 2.3f, 0.5f,-1.0f, 0.0f, 1.0f,
	 0.49f, 2.3f, 0.5f,-1.0f, 0.0f, 1.0f,
	-0.51f, 2.3f,-0.5f,-1.0f, 0.0f, 1.0f,
	-0.51f, 0.0f,-0.5f,-1.0f, 0.0f, 1.0f,
	// (facing back-right)
	 0.51f, 0.0f, 0.5f, 1.0f, 0.0f,-1.0f,
	-0.49f, 0.0f,-0.5f, 1.0f, 0.0f,-1.0f,
	-0.49f, 2.3f,-0.5f, 1.0f, 0.0f,-1.0f,
	-0.49f, 2.3f,-0.5f, 1.0f, 0.0f,-1.0f,
	 0.51f, 2.3f, 0.5f, 1.0f, 0.0f,-1.0f,
	 0.51f, 0.0f, 0.5f, 1.0f, 0.0f,-1.0f,

	 // front-left to back-right
	 // (facing front-right)
	 -0.51f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f,
	  0.49f, 0.0f,-0.5f, 1.0f, 0.0f, 1.0f,
	  0.49f, 2.3f,-0.5f, 1.0f, 0.0f, 1.0f,
	  0.49f, 2.3f,-0.5f, 1.0f, 0.0f, 1.0f,
	 -0.51f, 2.3f, 0.5f, 1.0f, 0.0f, 1.0f,
	 -0.51f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f,
	 // (facing back-left)
	  0.51f, 0.0f,-0.5f,-1.0f, 0.0f,-1.0f,
	 -0.49f, 0.0f, 0.5f,-1.0f, 0.0f,-1.0f,
	 -0.49f, 2.3f, 0.5f,-1.0f, 0.0f,-1.0f,
	 -0.49f, 2.3f, 0.5f,-1.0f, 0.0f,-1.0f,
	  0.51f, 2.3f,-0.5f,-1.0f, 0.0f,-1.0f,
	  0.51f, 0.0f,-0.5f,-1.0f, 0.0f,-1.0f,

	  // Left to Right
	  // (front)
	  -0.5f, 2.3f, 0.01f, 0.0f, 0.0f, 1.0f,
	  -0.5f, 0.0f, 0.01f, 0.0f, 0.0f, 1.0f,
	   0.5f, 0.0f, 0.01f, 0.0f, 0.0f, 1.0f,
	   0.5f, 0.0f, 0.01f, 0.0f, 0.0f, 1.0f,
	   0.5f, 2.3f, 0.01f, 0.0f, 0.0f, 1.0f,
	  -0.5f, 2.3f, 0.01f, 0.0f, 0.0f, 1.0f,

	  // (back)
	   0.5f, 0.0f,-0.01f, 0.0f, 0.0f,-1.0f,
	  -0.5f, 0.0f,-0.01f, 0.0f, 0.0f,-1.0f,
	  -0.5f, 2.3f,-0.01f, 0.0f, 0.0f,-1.0f,
	  -0.5f, 2.3f,-0.01f, 0.0f, 0.0f,-1.0f,
	   0.5f, 2.3f,-0.01f, 0.0f, 0.0f,-1.0f,
	   0.5f, 0.0f,-0.01f, 0.0f, 0.0f,-1.0f,
};
static float wheatText[] = {
	// back-left to front-right
	// (facing front-left)
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,
	// (facing back-right)
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,

	// front-left to back-right
	// (facing front-right)
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,
	// (facing back-left)
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,
	
	// Left to Right
	// (front)
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	// (back)
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f
};
static float wheatBack[] = {
	-18.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	 18.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	 18.0f, 1.5f, 0.0f, 0.0f, 0.0f, 1.0f,
	 18.0f, 1.5f, 0.0f, 0.0f, 0.0f, 1.0f,
	-18.0f, 1.5f, 0.0f, 0.0f, 0.0f, 1.0f,
	-18.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};
static float wheatBackText[] = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,
};

// Create arrays for holding grass information
static float standingTallGrassRotations[1600] = {};
static glm::vec3 standingTallGrassSizes[1600] = {};
static glm::vec2 standingTallGrassPositions[1600] = {};
// Create arrays for holding wheat information
static float wheatRotations[450] = {};
static float wheatHeights[450] = {};
static float wheatPosition[450] = {};

static float particleObject[] = {
	 1.0f,-1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	 1.0f,-1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	-1.0f,-1.0f, 0.0f, 0.0f, 0.0f, 1.0f
};
static float particleObjectText[] = {
	1.0f, 0.0f, 
	1.0f, 1.0f, 
	0.0f, 1.0f,
	0.0f, 1.0f, 
	1.0f, 0.0f,
	0.0f, 0.0f
};