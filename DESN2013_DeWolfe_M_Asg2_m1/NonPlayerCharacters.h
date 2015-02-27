#pragma once
#include <time.h>
#include "Character.h"
#include "Map.h"
#include "Projectile.h"
#define LANDERWIDTH 10
#define LANDERHEIGHT 6
#define HUMANWIDTH 6
#define HUMANHEIGHT 6
#define MAXFALLHEIGHT 40
class Lander : public Character 
{
public:
	float targetX, targetY;
	int range;
	bool hasHuman, hasTarget, isMutant;
	int targetID;
	// lander is height of 10, width of 6
	Lander(float _x, float _y, float _hitBoxWidth, float _hitBoxHeight, int _fireDelay)
		: Character(_x, _y, _hitBoxWidth, _hitBoxHeight) 
	{
		aiDelay = _fireDelay;
		targetX = 1400;
		targetY = 77;
		range = 100;
		speed = 1.0f;
		hasHuman = false;
		hasTarget = false;
		isMutant = false;
		targetID = -1;
	}
	// increase targeting range of this lander
	void IncreaseRange(int _newRange) { range = _newRange; }
	// find human to abduct
	bool FindTarget(float _targetX, float _targetY, int _targetID) 
	{
		if (abs(_targetX - x) < abs(targetX - x))
		{
			targetX = _targetX;
			targetY = _targetY;
			targetID = _targetID;
			hasTarget = true;
			return true;
		}
		return false;
	}
	// draw visuals for lander
	void Character::Draw() 
	{
		glPushMatrix();
			glTranslatef(x, y, 0);
			// lander body
			if (isMutant == true)
				glColor3f(0.4, 0.2, 0.7);
			else
				glColor3f(0.0, 0.5, 0.1);
			glBegin(GL_TRIANGLES);
				glVertex3f(0.0, 5.0, -0.1);
				glVertex3f(-3.0, 2.0, -0.1);
				glVertex3f(0.0, -1.0, -0.1);
			glEnd();
			glBegin(GL_TRIANGLES);
				glVertex3f(0.0, 5.0, -0.1);
				glVertex3f(0.0, -1.0, -0.1);
				glVertex3f(3.0, 2.0, -0.1);
			glEnd();
			// lander legs
			glLineWidth(2.0);
			// left leg
			glBegin(GL_LINE_STRIP);
				glVertex3f(0.0, 0.0, -0.1);
				glVertex3f(-3.0, -1.0, -0.1);
				glVertex3f(-3.0, -4.0, -0.1);
			glEnd();
			// right leg
			glBegin(GL_LINE_STRIP);
				glVertex3f(0.0, 0.0, -0.1);
				glVertex3f(3.0, -1.0, -0.1);
				glVertex3f(3.0, -4.0, -0.1);
			glEnd();
			glLineWidth(1.0);
			// lander cockpit window
			glColor3f(0.6, 0.8, 0.8);
			glBegin(GL_TRIANGLES);
				glVertex3f(-1.0, 3.0, -0.1);
				glVertex3f(0.0, 0.0, -0.1);
				glVertex3f(1.0, 3.0, -0.1);
			glEnd();
		glPopMatrix();
		// Draw the landers on the radar screen
		glPushMatrix();
		glTranslatef(RADARX + x/RADARWIDTH, RADARY + y/RADARHEIGHT, 0);
		if (isMutant == true)
				glColor3f(0.4, 0.2, 0.7);
			else
				glColor3f(0.0, 1.0, 0.0);
		glBegin(GL_TRIANGLES);
			glVertex3f(0, 1, 0);
			glVertex3f(1, 0, 0);
			glVertex3f(-1, 0, 0);
		glEnd();
		glPopMatrix();
	}
	// reset target values so lander will seek a new target on next AI loop
	void ResetTargetting()
	{
		targetID = -1;
		targetX = 700;
		targetY = 50;
		hasTarget = false;
		hasHuman = false;
	}
	// move lander
	void Move() 
	{
		// basic movement
		if (hasTarget == false)
		{
			x += speed;
		}
		//movement when a target has been acquired
		else
		{
			if (hasHuman == true)
			{
				y += speed;
				return;
			}
			else if (abs(x - targetX) > 3)
			{
				// move towards target along x
				if (targetX > x)
				{
					x += speed;
				}
				else if (targetX < x)
				{
					x -= speed;
				}
				// move on y to simulate more random motion
				if ((int)x%3 == 0 && y < 181 & y > 20)
				{
					if (rand()%3 < 1)
						y += rand()%2;
					else
						y -= rand()%2;
				}
			}
			else 
			{
				if (y > targetY + 2)
				{
					y -= speed;
				}
				else if (y < targetY)
				{
					y += speed;
				}
			}
		}
	}
	// mutate the lander
	void Mutate()
	{
		hasHuman = false;
		isMutant = true;
		speed += 1.0f;
	}
	// move human
	void ScreenMove(float _movement) 
	{
		x += _movement;
		targetX += _movement;
		// after applying movement check if they need to swap to other side of screen
		// for map scrolling effect
		if (x < -600) 
		{
			x += 1200;
			targetX += 1200;
		}
		if (x > 600) 
		{
			x -= 1200;
			targetX -= 1200;
		}
	}
	bool IsColliding(Projectile weapon)
	{
		if (abs(x - weapon.x) < 5)
		{
			if (abs(y - weapon.y) < 5)
			{
				return true;
			}
		}
		else
			return false;
	}
	~Lander() {}
};

// humans stand on the ground and get abducted by landers
// if they fall from a great enough height they will die
class Human : public Character 
{
public:
	int ID;
	bool isAbducted, isTarget, isFalling, isAlive, isOnPlayer;
	float startY, fallY;
	// human is width 6 and height 8
	Human(float _x, float _y, float _hitBoxWidth, float _hitBoxHeight, int _ID)
		:Character(_x, _y, _hitBoxWidth, _hitBoxHeight) 
	{
		ID = _ID;
		isAbducted = false;
		isTarget = false;
		isFalling = false;
		isAlive = true;
		isOnPlayer = false;
		startY = y;
		fallY = 0;
	}
	// Call this when player deposits a human
	void PlayerSaved()
	{
		isOnPlayer = false;
		isTarget = false;
		isFalling = false;
	}
	// Call this when player touches a falling human
	void PlayerGrab()
	{
		isOnPlayer = true;
		isAbducted = false;
	}
	// call this when a human is abducted or falls to death
	void Dead()
	{
		isAlive = false;
		isFalling = false;
	}
	// draw visual for human
	void Character::Draw() 
	{
		if (isAlive == false)
			return;
		glPushMatrix();
		glTranslatef(x, y, 0);
		glColor3f(0.4, 0.1, 0.1);
		// human head
		glBegin(GL_QUADS);
			glVertex3f(-2.0, 3.0, -0.1);
			glVertex3f(-3.0, 0.0, -0.1);
			glVertex3f(3.0, 0.0, -0.1);
			glVertex3f(2.0, 3.0, -0.1);
		glEnd();
		// human torso
		glBegin(GL_TRIANGLES);
			glVertex3f(0.0, 0.0, -0.1);
			glVertex3f(-2.0, -2.0, -0.1);
			glVertex3f(2.0, -2.0, -0.1);
		glEnd();
		// legs
		glLineWidth(2.0);
		// left leg
		glBegin(GL_LINES);
			glVertex3f(-1.0, -1.0, -0.1);
			glVertex3f(-1.0, -4.0, -0.1);
			glVertex3f(1.0, -1.0, -0.1);
			glVertex3f(1.0, -4.0, -0.1);
		glEnd();
		glLineWidth(1.0);
		glPopMatrix();
		// Draw the human on the radar screen
		glPushMatrix();
		glTranslatef(RADARX + x/RADARWIDTH, RADARY + y/RADARHEIGHT, 0);
		glColor3f(1.0, 0, 0);
		glBegin(GL_LINE_STRIP);
			glVertex3f(0.5, 0.5, 0);
			glVertex3f(-0.5, 0.5, 0);
			glVertex3f(-0.5, -0.5, 0);
			glVertex3f(0.5, -0.5, 0);
			glVertex3f(0.5, 0.5, 0);
		glEnd();
		glPopMatrix();
	}
	void Dropped()
	{
		isAbducted = false;
		isTarget = false;
		isFalling = true;
		fallY = y;
	}
	// returns true if player died from a fall
	bool FallCheck()
	{
		if(isFalling == true && isAlive == true && isOnPlayer == false)
		{
			if (y > startY)
			{
				y -= 2;
				return false;
			}
			else if (fallY - startY < MAXFALLHEIGHT && y <= startY)
			{
				isFalling = false;
				return false;
			}
			else if (fallY - startY > MAXFALLHEIGHT && y <= startY)
			{
				isAlive = false;
				return true;
			}
		}	
		return false;
	}
	// move human
	void ScreenMove(float _movement) 
	{
		x += _movement;
		// after applying movement check if they need to swap to other side of screen
		// for map scrolling effect
		if (x < -600) {
			x += 1200;
		}
		if (x > 600) {
			x -= 1200;
		}
	}
	~Human() {}
};