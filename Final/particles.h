#include <glm/glm.hpp>
#include <fstream>
#include <random>

using namespace std;
// referenced: https://learnopengl.com/In-Practice/2D-Game/Particles
struct Particle {
	glm::vec3 position, velocity;
	float life;

	Particle()
		: position(-4.0f, -2.0f, 10.0f), velocity(8.0f, 0.0f, -2.0f), life(0.0f) { }
};
const int particlesCount = 20;
unsigned int lastUsedParticle = 0;
static Particle particles[particlesCount];

// Once a particles life is over, respawn with random position
void respawnParticle(Particle &particle) {
	particle = Particle();
	float random = ((rand() % 60) - 30) / 10.0f; // (-3 - 3)
	particle.position.x = particle.position.x + random;
	random = ((rand() % 40) - 20) / 10.0f; // (-2 - 2)
	particle.position.y = particle.position.y + random + 3.0f;
	random = ((rand() % 40) - 20) / 10.0f; // (-2 - 2)
	particle.position.z = particle.position.z + random;
	random = ((rand() % 40) - 20) / 10.0f; // (-2 - 2)
	particle.life = 6.0f + random;
	particle.velocity = particle.velocity * 0.1f;
}

// Find the first unused particle
unsigned int firstUnusedParticle() {
	// Find last used particle, usually returns instantly
	for (int i = lastUsedParticle; i < particlesCount; i++) {
		if (particles[i].life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// otherwise, do a linear search
	for (int i = 0; i < lastUsedParticle; i++) {
		if (particles[i].life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// override first particle if all others are alive
	lastUsedParticle = 0;
	return 0;
}

// Update particles
void updateParticles(float deltaTime) {
	// add new particles
	for (int i = 0; i < 2; i++) {
		int unusedParticle = firstUnusedParticle();
		respawnParticle(particles[unusedParticle]);
	}
	// update all particles
	for (int i = 0; i < particlesCount; i++) {
		Particle &p = particles[i];

		// Create a random chance that the particle will move in a different direction and a random speed
		int randomChance = (rand() % 50);
		if (randomChance == 1) {
			p.velocity = glm::vec3(((rand() % 40) - 20) / 10.0f, ((rand() % 40) - 20) / 10.0f, ((rand() % 40) - 20) / 10.0f);
		}

		// reduce particle's life
		p.life -= deltaTime;
		// if particle's life is not 0, move forward
		if (p.life > 0.0f) {
			p.position -= deltaTime * -p.velocity;
		}
	}
}