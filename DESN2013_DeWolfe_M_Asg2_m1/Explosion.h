#pragma once
#include <glut.h>
#include <time.h>
#include <stdlib.h>
#define TOTALPARTICLES 15
#define EXPLOSIONLIFETIME 30
class Explosion
{

public:
	struct Particle
	{
		// starting x and y position
		float newX, newY;
		// speed along x and y
		float speedX, speedY;
	};
	
	// explosion centre
	float x, y;
	// time the explosion began at
	float startTime;
	// number of particles in explosion, tracks offset
	Particle particles[TOTALPARTICLES];
	// is the explosion active
	bool isIdle;

	Explosion() 
	{
		isIdle = true;
		srand(time(NULL));
	}

	void SpawnExplosion (float _x, float _y, float _startTime)
	{
		x = _x;
		y = _y;
		// setup base x and y for each partcile of explosion
		for (int i = 0; i < TOTALPARTICLES; i++)
		{
			if (rand()%2 > 0.8)
				particles[i].speedX = rand()%4;
			else
				particles[i].speedX  = rand()%4 * -1;
			if (rand()%2 > 0.8)
				particles[i].speedY  = rand()%4;
			else
				particles[i].speedY  = rand()%4 * -1;
			particles[i].newX = 0;
			particles[i].newY = 0;
		}
		isIdle = false;
		startTime = _startTime;
	}

	void Draw(float _gameTime)
	{
		// if explosions has been on screen longer than lifetime
		if (startTime + EXPLOSIONLIFETIME < _gameTime)
		{
			isIdle = true;
		}
		// else draw
		if (isIdle == false)
		{
			glPushMatrix();
			glTranslatef(x, y, 0);
			// iterate through each particle, draw and move
			for (int i = 0; i < TOTALPARTICLES; i++)
			{
				glPushMatrix();
				glLineWidth(2.0);
				glColor3f(0.8, 0.8, 0.3);
				glBegin(GL_LINES);
					glVertex3f(0, 0, 0);
					glVertex3f(particles[i].newX, particles[i].newY, 0);
				glEnd();
				glPopMatrix();

				particles[i].newX += particles[i].speedX;
				particles[i].newY += particles[i].speedY;
			}
			glLineWidth(1.0);
			glPopMatrix();
		}
	}
	void Idle()
	{
		isIdle = true;
	}
	void ScreenMove(float _movement) {
		x += _movement;
		for (int i = 0; i < TOTALPARTICLES; i++)
		{
			particles[i].newX += _movement;
			if (particles[i].newX < -600) { particles[i].newX += 1200; }
			if (particles[i].newX > 600) { particles[i].newX -= 1200; }
		}
		// after applying movement check if they need to swap to other side of screen
		// for map scrolling effect
		if (x < -600) { x += 1200; }
		if (x > 600) { x -= 1200; }
	}
};