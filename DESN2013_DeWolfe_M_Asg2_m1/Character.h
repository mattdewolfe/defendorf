#pragma once
#include <glut.h>
#define FIREDELAY 30
#define RADARWIDTH 19
#define RADARHEIGHT 25
#define RADARX 133
#define RADARY 185

class Character {
protected:
	float speed; // player movement speed (defaults to 0)
	float hitBoxWidth, hitBoxHeight; // hitbox values are half actual size as characters are drawn with 0 at center
	bool leftFacing; // flag for rotating image when facing left
	int aiDelay; // delay for AI weapons

public:
	float x, y; // player x and y positions (defaults to 0 if not set in constructor)
	int lastFired; // store time interval of last shot
	// default consructor ! hitbox size defaults to 1x1
	Character() 
	{
		x = 0;
		y = 0;
		speed = 0.0;
		hitBoxWidth = 1.0;
		hitBoxHeight = 1.0;
		leftFacing = false;
		aiDelay = 0;
	}
	// x y positions and hitbox width/height
	Character(float _x, float _y, float _hitBoxWidth, float _hitBoxHeight) 
	{
		x = _x;
		y = _y;
		speed = 0.0;
		hitBoxWidth = _hitBoxWidth;
		hitBoxHeight = _hitBoxHeight;
		leftFacing = false;
		lastFired = 0;
		aiDelay = 0;
	}
	// checks to see if player can fire, returns true if so
	bool Character::CanFire(int _currentTime)
	{
		if (lastFired + aiDelay + FIREDELAY < _currentTime )
		{
			lastFired = _currentTime;
			return true;
		}
		else return false;
	}

	void Character::SetPos(float _x, float _y) {
		x = _x;
		y = _y;
	}

	// draws the graphic for this character
	virtual void Character::Draw() = 0 {}

	// set the players speed value
	void Character::SetSpeed(float _speed) { speed = _speed; }

	// return the players speed value
	float Character::GetSpeed() { return speed; }

	// change flag for facing direction when character turns around
	void Character::ChangeFacing() { leftFacing = !leftFacing; }
	
		// change flag for facing direction when character turns around
	bool Character::GetFacing() { return leftFacing; }

	float Character::GetXPos() { return x; }
	float Character::GetYPos() { return y; }
	// destructor
	~Character() {
	}
};