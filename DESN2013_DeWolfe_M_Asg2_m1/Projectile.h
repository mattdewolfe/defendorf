#pragma once
#include <glut.h>
#include <math.h>
#define FIREDELAY 30
#define RADARWIDTH 19
#define RADARHEIGHT 25
#define RADARX 133
#define RADARY 185
class Projectile {

public:
	// X and Y position, speed along X and Y axis, starting X location (for destruction after set range)
	// and direction (used in movement and screen movement offsets), fireTime stores when shot was fired
	float x, y, speedX, startX, speedY, direction;
	int fireTime;
	// bool for animation call - if true draws laser, else draws bullet
	bool isLaser;
	// flag for whether projectile is in play or not
	bool isIdle;
	Projectile()
	{
		x = 0;
		y = -50;
		startX = 0;
		speedX = 1.0;
		isLaser = false;
		isIdle = true;
	}
		// constructor for player shots
	void PlayerShot(float _x, float _y, bool _facingLeft, int _startTime)
	{
		x = _x;
		startX = x;
		y = _y;
		if (_facingLeft == true)
		{
			direction = -1.0;
		}
		else
		{
			direction = 1.0;
		}
		speedX = 8.0*direction;
		speedY = 0;
		isLaser = true;
		isIdle = false;
		fireTime = _startTime;
	}
	// AI targetting call
	void TargetPlayer(float _x, float _y, float _targetX, float _targetY, float _direction, int _startTime)
		{
		direction = _direction;
		x = _x;
		startX = x;
		y = _y;
		
		// calculate hypotonuse of triangle to target along
		float slope = ((_targetY - y)/(_targetX - x));
		// clamp slope values to prevent high speed projectiles on vertical paths
		if (slope > 1.0) { slope = 1.0; }
		else if (slope < -1.0) { slope = -1.0; }
		speedX = 4.0*direction;
		speedY = speedX*slope;
		isLaser = false;
		isIdle = false;
		fireTime = _startTime;
	}
	// called on screen moves to offset position to map
	void ScreenMove(float _movement)
	{
		if (isIdle == false)
		{
			x += _movement;
			startX += _movement;
			// after applying movement check if they need to swap to other side of screen
			// for map scrolling effect
			if (x < -600) {
				x += 1200;
				startX += 1200;
			}
			if (x > 600) {
				x -= 1200;
				startX += 1200;
			}
		}
	}
	// should be called every update for projectile movement
	// returns false if projectile is beyond max range
	void Travel()
	{
		if (isIdle == false)
		{
			// move along X based on speed
			x+=speedX;
			y+=speedY;
		}
	}
	// set projectile to idle, move off map
	void Idle()
	{
		isIdle = true;
		y = -150;
		startX = 0;
	}
	// draw projectile image
	void Draw()
	{
		if (isLaser == true)
		{
			glPushMatrix();
			glColor3f(0.8, 0.8, 0.1);
			glTranslatef(x, y, -0.1);
			glLineWidth(3.0);
			glBegin(GL_LINES);
			glVertex3f(-4.0, 0.0, 0.0);
			glVertex3f(4.0, 0.0, 0.0);
			glEnd();
			glLineWidth(1.0);
			glPopMatrix();
		}
		else 
		{
			glPushMatrix();
			glColor3f(0.8, 0.8, 0.8);
			glTranslatef(x, y, -0.1);
			glLineWidth(3.0);
			glBegin(GL_LINES);
			glVertex3f(-1.0, 0.0, 0.0);
			glVertex3f(1.0, 0.0, 0.0);
			glEnd();
			glLineWidth(1.0);
			glPopMatrix();
		}
		if (y < 180 && y > 0)
		{
			glPushMatrix();
			glTranslatef(RADARX + x/RADARWIDTH, RADARY + y/RADARHEIGHT, 0);
			glColor3f(0.8, 0.8, 0.1);
			glBegin(GL_LINES);
				glVertex3f(-0.5, 0, 0);
				glVertex3f(0.5, 0, 0);
			glEnd();
			glPopMatrix();
		}
	}
	~Projectile() {}
};