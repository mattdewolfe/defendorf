#pragma once
#include <time.h>
#include "Character.h"
#include "Projectile.h"

class Player : public Character 
{

public:
	// number of bombs the player has
	int bombs;
	// flag for animating thrusters on engine
	bool isMoving;
	// ticker for tail animation
	float randomFlames;
	// x y positions and hitbox width/height
	Player(float _x, float _y, float _hitBoxWidth, float _hitBoxHeight) {
		x = _x;
		y = _y;
		hitBoxWidth = _hitBoxWidth;
		hitBoxHeight = _hitBoxHeight;
		leftFacing = false;
		isMoving = false;
		srand(time(NULL));
		randomFlames = 0;
		bombs = 3;
	}
	// draw character art
	void Character::Draw() 
	{
		glPushMatrix();
		glTranslatef(x, y, 0); // base offset for screen edge
		// if facing left direction, rotate 180 around y
		if (leftFacing == true) {
			glRotatef(180.0, 0, 1, 0);
		}
		// cockpit 
		glColor3f(0.3, 0.3, 0.3);
		glBegin(GL_TRIANGLES);
			glVertex3f(-10.0, 6.0, -0.1);
			glVertex3f(0.0, 0.0, -0.1);
			glVertex3f(10.0, 1.0, -0.1);
		glEnd();
		// ship body rear
		glBegin(GL_QUADS);
			glVertex3f(-10.0, 6.0, -0.1);
			glVertex3f(-9.0, -4.0, -0.1);
			glVertex3f(1.0, 0.0, -0.1);
			glVertex3f(-5.0, 3.0, -0.1);
		glEnd();
		// cockpit window
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_TRIANGLES);
			glVertex3f(-7.0, 4.0, -0.1);
			glVertex3f(0.0, 0.0, -0.1);
			glVertex3f(7.0, 1.0, -0.1);
		glEnd();
		// weapons (below ship)
		glColor3f(0.0, 0.8, 0.2);
		glBegin(GL_TRIANGLES);
			glVertex3f(-9.0, -5.0, -0.1);
			glVertex3f(10.0, -1.0, -0.1);
			glVertex3f(-8.0, -3.0, -0.1);
		glEnd();
		if (isMoving == true) { 
			randomFlames += 0.1;
			glColor3f(0.5, randomFlames/2, 0.2);
			glBegin(GL_TRIANGLES);
				glVertex3f(-10.0, 6.0, -0.1);
				glVertex3f(-12.0, 4.0, -0.1);
				glVertex3f(-10.0, 2.0, -0.1);
			glEnd();
			glColor3f(0.8, randomFlames, 0.2);
			glBegin(GL_TRIANGLES);
				glVertex3f(-10.0, 5.0, -0.1);
				glVertex3f(-13.0, 3.0, -0.1);
				glVertex3f(-10.0, 1.0, -0.1);
			glEnd();
			glColor3f(0.8, randomFlames/2, 0.2);
			glBegin(GL_TRIANGLES);
				glVertex3f(-10.0, 4.0, -0.1);
				glVertex3f(-12.0, 2.0, -0.1);
				glVertex3f(-10.0, 0.0, -0.1);
			glEnd();
			if (randomFlames >= 1.0) { randomFlames = 0.1; }
		}
		glPopMatrix();
		// Draw the player on the radar screen
		glPushMatrix();
		glTranslatef(RADARX + x/RADARWIDTH, RADARY + y/RADARHEIGHT, 0);
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);
			glVertex3f(-1, 0, 0);
			glVertex3f(1, 0, 0);
		glEnd();
		glPopMatrix();
	}
	// move down to left
	void Player::MoveLeft() { 
		leftFacing = true;
		x -= speed;
	}
	// move up to right
	void Player::MoveRight() { 
		leftFacing = false;
		x += speed;
	}
	// move down to right
	void Player::MoveDown() {
		if (y >= 12.0) { y -= speed+1.0; }
	}
	// move up to left
	void Player::MoveUp() { 
		if (y <= 168.0) { y += speed+1.0; }
	}
	// Check if player was hit by a projectile
	bool IsHit (float otherX, float otherY)
	{
		if (abs(x - otherX) < 6)
		{
			if (abs(y - otherY) < 6)
			{
				return true;
			}
		}
		else
			return false;
	}
	// check bomb count
	int GetBombCount() { return bombs; }
	// add a bomb
	void AddABomb() { bombs++; }
	// check if the player can use a bomb
	bool UseBomb()
	{
		if (bombs > 0)
		{
			bombs -= 1;
			return true;
		}
		else 
		{
			return false;
		}
	}
};