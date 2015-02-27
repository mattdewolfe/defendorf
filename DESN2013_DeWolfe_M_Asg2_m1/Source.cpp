#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <vector>
#include <glut.h>
#include <list>
#include <Windows.h>
#include "Score.h"
#include "Player.h"
#include "Character.h"
#include "VisualText.h"
#include "GameTimer.h"
#include "Map.h"
#include "NonPlayerCharacters.h"
#include "Projectile.h"
#include "Explosion.h"
#define PROJECTILELIFE 300
#define ENEMYPROJECTILES 60
#define PLAYERPROJECTILES 10
#define MAXENEMIES 10
#define MAXEXPLOSIONS 10
#define RADARWIDTH 66
#define RADARHEIGHT 10
#define RADARX 100
#define RADARY 183

using namespace std;

enum STATE {MAIN_MENU, LOAD_GAME, PLAY_GAME, PAUSE, RESPAWN, GAME_OVER, HELP, LEADER_BOARD, LEVEL_CLEAR};
STATE gameState = MAIN_MENU;

static long font = (long)GLUT_BITMAP_8_BY_13; // font selection
static char theStringBuffer[10]; // string buffer
static int animationPeriod = 33; // time interval between frames (in ms)
static int currentLevel = 1; // stores current level
static double screenWidth, screenHeight;

// movement flags for player
bool moveLeft = false, moveRight = false, moveDown = false, moveUp = false, playerFire = false;
int smartBombDelay = 5, lastSmartBomb = 0, currentPlayerShot = 0, currentEnemyShot = 0, currentExplosion = 0;
int enemiesInLevel; // number of enemies for this level
int humansOnMap; // number of humans left in level
int landersOnMap; // number of enemeis on the map

Score *playerScore;
Player *playerShip;
VisualText *drawText;
GameTimer *gameTimer;
Map *map;
std::vector<Lander> landers;
std::vector<Human> humans;
Explosion explosions[10];
Projectile landerProjectiles[ENEMYPROJECTILES];
Projectile playerProjectiles[PLAYERPROJECTILES];

void keyInput(unsigned char key, int x, int y); // key listener for ascii keys
void specKeyInput(int key, int x, int y); // key down listener for special keys
void specKeyUp(int key, int x, int y); // key up listener for special keys
void levelCleared(); // called after beating a level
void checkPositions(); // collision detection and check remaining blocks to hit
void moveCharacters(); // move all actors as needed and move player based on movement flags
void shiftMapElements(float _moveElements); // shifts all elements based on player input
void triggerHyperspace(); // move player ahead a random amount, shift elements as required
void useSmartBomb(); // check if the player can use a smartbomb
void checkCollision(); // check collision
void spawnLander(); // spawn another lander
void writeData(); // used for rasterizing text
void clearMap(); // creates blank map
void drawMap(); // used in drawScene for map visuals
void drawCharacters(); // used in drawScene for all character visuals
void drawMenu(); // draw main menu
void drawHelp(); // draw help screen
void drawLeaderBoard(); // draw leaderboard screen
void drawCube(); // draw individual map cube
void drawGameOver(); // draw game over screen
void drawLevelClear(); // draw info for level cleared
void drawRespawn(); // screen displayed on character death, but not game over
void levelSetup(); // sets up values for next level, generate new enemies/humans
void floatToString(char * destStr, int precision, int val);
void clearProjectileLocations(); // set all projectiles off map
void gameOverKeyCheck(unsigned char _key); // key listener for entering initials at game over screen

// call draw functions and game logic based on gameState
void drawScene () 
{
	
	glClear (GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	writeData();
	glPushMatrix();
		glBegin(GL_LINE_LOOP);
		glColor3f(0.3, 0.3, 1.0);
		glVertex3f(0.5, 0.5, 0);
		glVertex3f(265.5, 0.5, 0);
		glVertex3f(265.5, 199.5, 0);
		glVertex3f(0.5, 199.5, 0);
		glVertex3f(0.5, 0.5, 0);
		glEnd();
	glPopMatrix();
	switch (gameState) {
	case MAIN_MENU:
		drawMenu();
		// wipe values for player starting game over
		currentLevel = 1;
		enemiesInLevel = 15;
		landersOnMap = 8;
		break;
	
	case LEVEL_CLEAR:
		drawLevelClear();
		drawMap();
		drawCharacters();
		break;

	case PLAY_GAME:
		drawMap();
		moveCharacters();
		checkPositions();
		drawCharacters();
		checkCollision();
		gameTimer->Increment();
		if (enemiesInLevel == 0)
		{
			levelCleared();
		}
		else if (gameTimer->CheckTime()%40 == 0 && landersOnMap < enemiesInLevel && landersOnMap < MAXENEMIES)
		{
			spawnLander();
		}
		break;

	case GAME_OVER:
		drawGameOver();
		
		if (playerScore->currentLetterEntered >= 3)
		{
			playerScore->UpdateScoreBoard();
			gameState = LEADER_BOARD;
		}
		gameTimer->Stop();
		break;

	case LEADER_BOARD:
		drawLeaderBoard();
		break;

	case LOAD_GAME:
		levelSetup();
		shiftMapElements(0);
		gameState = PLAY_GAME;		
		break;

	case RESPAWN:
		drawRespawn();
		drawMap();
		break;

	case HELP:
		drawHelp();
		break;
	}
	glutSwapBuffers();
}
// animation timer
void animate(int value)
{
 	glutPostRedisplay();
	glutTimerFunc(animationPeriod, animate, 1);
}
// setup any variables that need to change for new game
void levelSetup() 
{
	landers.clear();
	humans.clear();
	clearProjectileLocations();
	
	enemiesInLevel = 9;
	landersOnMap = 8;
	playerShip->lastFired = 0;

	switch (currentLevel) 
	{
	case 1:
		// reset player score/lives
		playerScore->Reset();
		break;
	default:
		enemiesInLevel += 2 * currentLevel;
		break;
	}
	for (int i = 0; i < 8 + currentLevel; i++) 
	{
		if ( i < landersOnMap ) 
		{
			// spread lander appropriately - random value to put lander at negative Y instead of positive Y
			if (rand()%2 == 1) { landers.push_back(Lander(i * -50 - rand()%100, 80 + rand()%80, 5.0f, 3.0f, 10 + rand()%20)); }
			else { landers.push_back(Lander(i * 50 + rand()%200, 80 + rand()%80, 5.0f, 3.0f, 10 + rand()%20)); }
		}
		humans.push_back(Human((i * 75) + rand()%100, 10 + rand()%10, 3.0f, 4.0f, i));
	}
	humansOnMap = humans.size();
	// reset player position
	playerShip->SetPos(133.0f, 100.0f);
	// reset and start the timer again
	gameTimer->Reset();
	gameTimer->Start();
	// generate a new map
	map->GenerateMap();
}
// initialization
void setup(void) 
{
	glClearColor(0.0, 0.0, 0.0, 0.0); 
	// setup drawtext font
	drawText = new VisualText(12.0);
	drawText->SetColorFloatRGB(0.3, 0.3, 0.7);
	// setup player object
	playerShip = new Player(133.0f, 50.0f, 10.0f, 6.0f);
	playerShip->SetSpeed(4.0f);
	// setup player score class
	playerScore = new Score(2);
	playerScore->ReadScores();
	// setup timer
	gameTimer = new GameTimer(animationPeriod, 0);
	// load textures
	screenWidth = 266;
	screenHeight = 200;
	// setup map class
	map = new Map();
	// non player character setup
	enemiesInLevel = 15;
	landersOnMap = 8;
}
// openGL window reshape routine
void resize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h); 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, screenWidth, 0, screenHeight, -5.0, 50.0);
	glMatrixMode(GL_MODELVIEW);
}
// routine to output interaction instructions to the C++ window
void printInteraction(void)
{
	std::cout << " User arrow keys to navigate.\n" << " Press Space to fire - A to use Hyperspace - S to user Smart Bombs.\n";
	std::cout << " DEBUG: Press 1, 2, or 3 to simulate point scoring.";
}
// keyboard input routine
void keyInput(unsigned char key, int x, int y)
{
	switch(gameState) {
	// main menu key watcher
	case MAIN_MENU:
		switch(key) {
		case 27:
			exit(0);
			break;
		case 'p':
			currentLevel = 1;
			gameState = LOAD_GAME;
			break;
		case 'P':
			currentLevel = 1;
			gameState = LOAD_GAME;
			break;
		case 't':
			gameState = HELP;
			break;
		case 'T':
			gameState = HELP;
			break;
		case 'h':
			gameState = LEADER_BOARD;
			break;
		case 'H':
			gameState = LEADER_BOARD;
			break;
		}
		break;
	// help screen key watcher
	case HELP:
		switch(key) 
		{
		case 27:
			gameState = MAIN_MENU;
			break;
		}
		break;
	// game play key watcher
	case PLAY_GAME:
		switch(key) 
		{
		case 27:
			gameState = MAIN_MENU;
			break;
		case 'p':
			gameState = PAUSE;
			break;
		case 'P':
			gameState = PAUSE;
			break;
		case 'a':
			triggerHyperspace();
			break;
		case 'A':
			triggerHyperspace();
			break;
		case 's':
			useSmartBomb();
			break;
		case 'S':
			useSmartBomb();
			break;
		case '1':
			playerScore->PointsScored(3000);
			break;
		case ' ':
			playerFire = true;
			break;
		}
		break;
	// pause screen key watcher
	case PAUSE:
		switch(key) 
		{
		case 27:
			gameState = MAIN_MENU;
			break;
		case 'p':
			gameState = PLAY_GAME;
			break;
		case 'P':
			gameState = PLAY_GAME;
			break;
		}
		break;
	case LEVEL_CLEAR:
		switch(key) 
		{
		case 'p':
			levelSetup();
			shiftMapElements(0);
			gameState = PLAY_GAME;
			break;
		case 'P':
			levelSetup();
			shiftMapElements(0);
			gameState = PLAY_GAME;
			break;
		}
		break;
	// leader board key watcher
	case LEADER_BOARD:
		switch(key) 
		{
		case 27:
			gameState = MAIN_MENU;
			break;
		}
		break;
	// after death key listener
	case RESPAWN:
		switch(key)
		{
		case 'p':
			clearProjectileLocations();
			shiftMapElements(0);
			gameState = PLAY_GAME;
			break;
		case 'P':
			clearProjectileLocations();
			shiftMapElements(0);
			gameState = PLAY_GAME;
			break;
		case 27:
			gameState = MAIN_MENU;
			break;
		}
		break;
	// game over screen
	case GAME_OVER:
		gameOverKeyCheck(key);
		break;
	default:
		break;
	}
}
// key listener for players entering initials
void gameOverKeyCheck(unsigned char _key)
{
	switch(_key)
		{
		case 8:
			if (playerScore->currentLetterEntered != 0)
			{
				playerScore->currentLetterEntered--;
				playerScore->currentPlayerInitials[playerScore->currentLetterEntered] = ' ';
			}
			break;
		case 'a':
			playerScore->currentPlayerInitials[playerScore->currentLetterEntered] = 'a';
			playerScore->currentLetterEntered++;
			break;
		case 'b':
			playerScore->currentPlayerInitials[playerScore->currentLetterEntered] = 'b';
			playerScore->currentLetterEntered++;
			break;
		case 'c':
			playerScore->currentPlayerInitials[playerScore->currentLetterEntered] = 'c';
			playerScore->currentLetterEntered++;
			break;
		case 'd':
			playerScore->currentPlayerInitials[playerScore->currentLetterEntered] = 'd';
			playerScore->currentLetterEntered++;
			break;
		case 'e':
			playerScore->currentPlayerInitials[playerScore->currentLetterEntered] = 'e';
			playerScore->currentLetterEntered++;
			break;
		case 'f':
			playerScore->currentPlayerInitials[playerScore->currentLetterEntered] = 'f';
			playerScore->currentLetterEntered++;
			break;
		case 'g':
			playerScore->currentPlayerInitials[playerScore->currentLetterEntered] = 'g';
			playerScore->currentLetterEntered++;
			break;
		case 'h':
			playerScore->currentPlayerInitials[playerScore->currentLetterEntered] = 'h';
			playerScore->currentLetterEntered++;
			break;
		case 'i':
			playerScore->currentPlayerInitials[playerScore->currentLetterEntered] = 'i';
			playerScore->currentLetterEntered++;
			break;
		case 'j':
			playerScore->currentPlayerInitials[playerScore->currentLetterEntered] = 'j';
			playerScore->currentLetterEntered++;
			break;
		case 'k':
			playerScore->currentPlayerInitials[playerScore->currentLetterEntered] = 'k';
			playerScore->currentLetterEntered++;
			break;
		case 'l':
			playerScore->currentPlayerInitials[playerScore->currentLetterEntered] = 'l';
			playerScore->currentLetterEntered++;
			break;
		case 'm':
			playerScore->currentPlayerInitials[playerScore->currentLetterEntered] = 'm';
			playerScore->currentLetterEntered++;
			break;
		case 'n':
			playerScore->currentPlayerInitials[playerScore->currentLetterEntered] = 'n';
			playerScore->currentLetterEntered++;
			break;
		case 'o':
			playerScore->currentPlayerInitials[playerScore->currentLetterEntered] = 'o';
			playerScore->currentLetterEntered++;
			break;
		case 'p':
			playerScore->currentPlayerInitials[playerScore->currentLetterEntered] = 'p';
			playerScore->currentLetterEntered++;
			break;
		case 'q':
			playerScore->currentPlayerInitials[playerScore->currentLetterEntered] = 'q';
			playerScore->currentLetterEntered++;
			break;
		case 'r':
			playerScore->currentPlayerInitials[playerScore->currentLetterEntered] = 'r';
			playerScore->currentLetterEntered++;
			break;
		case 's':
			playerScore->currentPlayerInitials[playerScore->currentLetterEntered] = 's';
			playerScore->currentLetterEntered++;
			break;
		case 't':
			playerScore->currentPlayerInitials[playerScore->currentLetterEntered] = 't';
			playerScore->currentLetterEntered++;
			break;
		case 'u':
			playerScore->currentPlayerInitials[playerScore->currentLetterEntered] = 'u';
			playerScore->currentLetterEntered++;
			break;
		case 'v':
			playerScore->currentPlayerInitials[playerScore->currentLetterEntered] = 'v';
			playerScore->currentLetterEntered++;
			break;
		case 'w':
			playerScore->currentPlayerInitials[playerScore->currentLetterEntered] = 'w';
			playerScore->currentLetterEntered++;
			break;
		case 'x':
			playerScore->currentPlayerInitials[playerScore->currentLetterEntered] = 'x';
			playerScore->currentLetterEntered++;
			break;
		case 'y':
			playerScore->currentPlayerInitials[playerScore->currentLetterEntered] = 'y';
			playerScore->currentLetterEntered++;
			break;
		case 'z':
			playerScore->currentPlayerInitials[playerScore->currentLetterEntered] = 'z';
			playerScore->currentLetterEntered++;
			break;
		case 13:
			playerScore->currentLetterEntered = 3;
			break;
		}
}
// keyboard input routine
void keyUp(unsigned char key, int x, int y)
{
	if (key == ' ')
	{
		playerFire = false;
	}
}
// key down for movement flags
void specKeyInput(int key, int x, int y) {
	if (gameState == PLAY_GAME) {
		if (key == GLUT_KEY_DOWN) {
			moveDown = true;
		}
		
		if (key == GLUT_KEY_UP) {
			moveUp = true;
		}
		
	 
		if (key == GLUT_KEY_RIGHT) {
			moveRight = true;
		}
		
		if (key == GLUT_KEY_LEFT) {
			moveLeft = true;
		}
	}
}
// key up for movement flags
void specKeyUp(int key, int x, int y) {
	if (key == GLUT_KEY_DOWN) {
		moveDown = false;
	}
		
	if (key == GLUT_KEY_UP) {
		moveUp = false;
	}
		
	 
	if (key == GLUT_KEY_RIGHT) {
		moveRight = false;
	}
		
	if (key == GLUT_KEY_LEFT) {
		moveLeft = false;
	}
}
// check victory conditions
void levelCleared() {
	// award points for clearing level
	playerScore->PointsScored(humans.size()*100);
	currentLevel++;
	gameState = LEVEL_CLEAR;
}
// check character positions for death/block changes/ai targetting
void checkPositions() 
{
	// iterate through all landers AI calls
	for (int i = 0; i < landers.size(); i++)
	{
		// this checks for landers target
		if (landers[i].hasTarget == false)
		{
			// iterate through humans to find a target to abduct
			for (int j = 0; j < humans.size(); j++)
			{
				// statement to avoid multiple landers targetting same human
				if (humans[j].isTarget == false)
				{
					// pass location values to ai, if target is in range set target to true on human
					if (landers[i].FindTarget(humans[j].x, humans[j].y, humans[j].ID) == true)
						humans[j].isTarget = true;
				}
			}
		}
		// this checks to see if the lander has reached the ceiling with its target, making it a mutant
		else if (landers[i].hasHuman == true && landers[i].y > 175)
		{
			if (humans[landers[i].targetID].isAlive == true)
			{
				humans[landers[i].targetID].Dead();
				humansOnMap--;
				landers[i].Mutate();
			}
			else
			{
				landers[i].ResetTargetting();
			}
		}
		// check players position and fire if within range
		if (abs(playerShip->GetXPos() - landers[i].x) < landers[i].range)
		{
			if (landers[i].CanFire(gameTimer->CheckTime()))
			{
				// if ship is ahead of lander, direction value is positive
				if (playerShip->GetXPos() - landers[i].x > 0)
				{
					landerProjectiles[currentEnemyShot%ENEMYPROJECTILES].TargetPlayer(landers[i].x, landers[i].y, 
						playerShip->GetXPos() + 2.0f, playerShip->GetYPos() + 3.0f, 1.0, gameTimer->CheckTime());
					currentEnemyShot++;
				}
				// otherwise direction value is negative
				else
				{
					landerProjectiles[currentEnemyShot%ENEMYPROJECTILES].TargetPlayer(landers[i].x, landers[i].y, 
						playerShip->GetXPos() + 2.0f, playerShip->GetYPos() + 3.0f, -1.0, gameTimer->CheckTime());
					currentEnemyShot++;
					PlaySound(TEXT("lander_weapon.wav"), NULL, SND_ASYNC);
				}
			}
		}
		// Lastly, if lander is a mutant update its target coords to playership position
		if (landers[i].isMutant == true)
		{
			landers[i].targetX = playerShip->x + 1.0f;
			landers[i].targetY = playerShip->y - 1.0f;
		}
	}
	// check if player can fire, based on weapon cooldown time
	if (playerFire == true && playerShip->CanFire(gameTimer->CheckTime()) == true)
	{
		playerProjectiles[currentPlayerShot%PLAYERPROJECTILES].PlayerShot(playerShip->GetXPos(), 
			playerShip->GetYPos(), playerShip->GetFacing(), gameTimer->CheckTime());
		currentPlayerShot++;
		PlaySound(TEXT("player_weapon.wav"), NULL, SND_ASYNC);
	}
	// check for falling humans
	for (std::vector<Human>::iterator it = humans.begin(); it != humans.end(); it++)
	{
		// if human died from a fall
		if (it->FallCheck() == true)
		{
			it->Dead();
			humansOnMap--;
		}
		else
		{
			// check if human is falling and in range of player
			if (it->isFalling == true && it->isOnPlayer == false)
				if (abs(it->y - playerShip->y) < 4 && abs(it->x - playerShip->x) < 5)
					it->PlayerGrab();

			// Check if human is low enough to ground
			if (it->isOnPlayer == true && it->y < 18)
			{
				it->PlayerSaved();
				PlaySound(TEXT("save_human.wav"), NULL, SND_ASYNC);
				playerScore->PointsScored(500);
			}
			else if (it->isOnPlayer == true)
			{
				it->y = playerShip->y - 2;
			}
		}
	}	
}
// use a smart bomb, if possible
void useSmartBomb()
{
	if (playerShip->UseBomb() == true)
	{
		for (int i = 0; i < landers.size();)
		{
			if (abs(landers[i].x - playerShip->GetXPos()) < 300)
			{
				explosions[currentExplosion%MAXEXPLOSIONS].SpawnExplosion(landers[i].x, landers[i].y, gameTimer->CheckTime());
				currentExplosion++;
				if (landers[i].targetID >= 0 && landers[i].targetID < humans.size() && humans[landers[i].targetID].isAbducted == true)
				{
					humans[landers[i].targetID].Dropped();
					PlaySound(TEXT("explosion.wav"), NULL, SND_ASYNC);
					playerScore->PointsScored(500);
				}
				landers.erase(landers.begin() + i);
				enemiesInLevel--;
				landersOnMap--;
			}
			else
			{
				i++;
			}
		}
	}
}
// move all characters based on AI or movement flags
void moveCharacters() {
	if (moveDown == true) 
	{
		playerShip->MoveDown();
	}
	if (moveUp == true) 
	{
		playerShip->MoveUp();
	}
	if (moveLeft == true) 
	{
		// if player is 1/6th left of center move map
		if (playerShip->GetXPos() < screenWidth/2 - screenWidth/6) 
		{
			shiftMapElements((playerShip->GetSpeed()));
			// everything else needs to shift as well
		}
		// otherwise just move the player
		else { playerShip->MoveLeft(); }
	}
	if (moveRight == true) 
	{
		// if player is 1/6th right of center, move map
		if (playerShip->GetXPos() > screenWidth/2 + screenWidth/6) 
		{
			shiftMapElements(-(playerShip->GetSpeed()));
			// everything else needs to shift as well
		}
		// otherwise just move the player
		else { playerShip->MoveRight(); }
	}
	// added final flag to ensure proper map scrolling when player is idle
	if (moveLeft == false && moveRight == false)
		shiftMapElements(0);
	// if any movement flag is true, set isMoving true to animate thrusters
	if (moveRight || moveLeft || moveDown || moveUp) { playerShip->isMoving = true; }
	else { playerShip->isMoving = false; }
	
	// all projectiles move to a max, when false is returned they are destroyed
	for(int i = 0; i < ENEMYPROJECTILES; i++) 
	{
		// move each enemy projectile that is not idle
		landerProjectiles[i].Travel();
		if (landerProjectiles[i].fireTime + PROJECTILELIFE < gameTimer->CheckTime())
		{
			landerProjectiles[i].Idle();
		}
	}
	for(int i = 0; i < PLAYERPROJECTILES; i++) 
	{
		// move each player projectile that is not idle
		playerProjectiles[i].Travel();
		if (playerProjectiles[i].fireTime + PROJECTILELIFE/5 < gameTimer->CheckTime())
		{
			playerProjectiles[i].Idle();
		}
	}
	// Move landers
	for (std::vector<Lander>::iterator it = landers.begin(); it != landers.end(); it++)
	{
		it->Move();
		// If this lander has a human abducted
		if (it->hasHuman == true)
		{
			humans[it->targetID].y = it->y - 5;
		}
	}
}
// spawn another lander on the map
void spawnLander()
{
	landers.push_back(Lander(150 + rand()%500, 170.0f, 5.0f, 3.0f, 10 + rand()%30));
	landersOnMap++;
}
// move all map elements based on player movement
void shiftMapElements(float _moveElements) {
	map->ShiftMap(_moveElements);
	for (int i = 0; i < landers.size(); i++) { landers[i].ScreenMove(_moveElements); }
	for (int i = 0; i < humans.size(); i++) { humans[i].ScreenMove(_moveElements); }
	for (int i = 0; i < PLAYERPROJECTILES; i++) { playerProjectiles[i].ScreenMove(_moveElements); }
	for (int i = 0; i < ENEMYPROJECTILES; i++) { landerProjectiles[i].ScreenMove(_moveElements); }
	for (int i = 0; i < MAXEXPLOSIONS; i++) { explosions[i].ScreenMove(_moveElements); }
}
// moves the player ahead a random amount, shift map as needed
void triggerHyperspace() {
	srand(time(NULL));
	float jumpDistance = 250 + rand()%100;
	// if player is facing left, jump value is set to negative
	if (playerShip->GetFacing() == false) { jumpDistance *= -1; }
	clearProjectileLocations();
	shiftMapElements(jumpDistance);
}
// check collision with weapons/players/etc
void checkCollision()
{
	// Check collision with landers and lasers
	for (int i = 0; i < landers.size(); )
	{
		// First check if the lander has reached its target human
		// If it has a target with legimate ID, and it does not have a human yet
		if (landers[i].isMutant == false && landers[i].targetID != -1 && landers[i].hasHuman == false)
		{
			// Check distance on X
			if (abs(humans[landers[i].targetID].x - landers[i].x) < 5)
			{
				// Check distance on Y
				if (abs(humans[landers[i].targetID].y - landers[i].y) < 6)
				{
					// If all good, set human:abducted and lander:hasHuman true
					humans[landers[i].targetID].isAbducted = true;
					landers[i].hasHuman = true;
				}
			}
		}
		int add = 1;
		// Check if any lander was hit by a projectile
		for (int j = 0; j < PLAYERPROJECTILES; j++)
		{
			if (landers[i].IsColliding(playerProjectiles[j]) == true)
			{
				explosions[currentExplosion%MAXEXPLOSIONS].SpawnExplosion(landers[i].x, landers[i].y, gameTimer->CheckTime());
				currentExplosion++;
				if (landers[i].targetID >= 0 && landers[i].targetID < humans.size() && humans[landers[i].targetID].isAbducted == true)
				{
					humans[landers[i].targetID].Dropped();
					playerScore->PointsScored(500);
				}
				PlaySound(TEXT("explosion.wav"), NULL, SND_ASYNC);
				landers.erase(landers.begin() + i);
				playerProjectiles[j].Idle();
				enemiesInLevel--;
				landersOnMap--;
				playerScore->PointsScored(150);
				add = 0;
				break;
			}
		}
		// Check if player collided with a lander, only if add == 1 meaning the lander
		// was not hit by a projectile in previous check
		if (add == 1 && playerShip->IsHit(landers[i].x, landers[i].y) == true)
		{
			explosions[currentExplosion%MAXEXPLOSIONS].SpawnExplosion(playerShip->x, playerShip->y, gameTimer->CheckTime());
			++currentExplosion;
			// if player collides with abducted human and lander, human is destroyed
			if (landers[i].targetID >= 0 && landers[i].targetID < humans.size())
			{
				humans[landers[i].targetID].Dead();
				humansOnMap--;
			}
			PlaySound(TEXT("explosion.wav"), NULL, SND_ASYNC);
			landers.erase(landers.begin() + i);
			enemiesInLevel--;
			landersOnMap--;
			playerScore->PointsScored(150);
			add = 0;
			if (playerScore->CheckLives() <= 0)
			{

				playerScore->CheckScore();
				gameState = GAME_OVER;
			}
			else
			{
				playerScore->AddLives(-1);
				gameState = RESPAWN;
			}
		}
		i+=add;
	}
	// Check collision with player and landers/bullets
	for (int i = 0; i < ENEMYPROJECTILES; i++)
	{
		if (playerShip->IsHit(landerProjectiles[i].x, landerProjectiles[i].y) == true)
		{
			explosions[currentExplosion%MAXEXPLOSIONS].SpawnExplosion(playerShip->x, playerShip->y, gameTimer->CheckTime());
			++currentExplosion;
			PlaySound(TEXT("explosion.wav"), NULL, SND_ASYNC);
			if (playerScore->CheckLives() <= 0)
			{
				playerScore->CheckScore();
				gameState = GAME_OVER;
			}
			else
			{
				playerScore->AddLives(-1);
				gameState = RESPAWN;
			}
		}
	}
}

// visual calls
// draw info for level cleared
void drawLevelClear()
{
	drawText->ReSizeFont(10.0);
	drawText->SetColorFloatRGB(0.3, 0.4, 0.8);
	drawText->WriteBitmapString(80.0, 150.0, "LEVEL CLEARED");

	drawText->ReSizeFont(4.0);
	drawText->SetColorFloatRGB(0.8, 0.8, 0.8);
	drawText->WriteBitmapString(100.0, 130.0, "YOU SAVED ");
	floatToString(theStringBuffer, 8, humansOnMap);
	drawText->WriteBitmapString(135.0, 130.0, theStringBuffer);
	drawText->WriteBitmapString(140.0, 130.0, " HUMANS!");

	floatToString(theStringBuffer, 8, humansOnMap*100);
	drawText->WriteBitmapString(85.0, 110.0, theStringBuffer);
	drawText->WriteBitmapString(105.0, 110.0, " BONUS POINTS AWARDED! ");

	drawText->ReSizeFont(10.0);
	drawText->SetColorFloatRGB(0.3, 0.4, 0.8);
	drawText->WriteBitmapString(60.0, 50.0, "PRESS P TO CONTINUE");
}
// draw screen for player death, but not game over
void drawRespawn()
{
	drawText->ReSizeFont(10.0);
	drawText->SetColorFloatRGB(0.9, 0.3, 0.2);
	drawText->WriteBitmapString(90.0, 150.0, "YOU DIED");

	drawText->ReSizeFont(10.0);
	drawText->SetColorFloatRGB(0.3, 0.8, 0.8);
	drawText->WriteBitmapString(50.0, 100.0, "PRESS P TO CONTINUE");
}
// draw all characters
void drawCharacters() {
	playerShip->Draw();
	for (int i = 0; i < landers.size(); i++) {
		landers[i].Draw();
	}
	for (int i = 0; i < humans.size(); i++) {
		humans[i].Draw();
	}
	for (int i = 0; i < PLAYERPROJECTILES; i++) {
		playerProjectiles[i].Draw();
	}
	for (int i = 0; i < ENEMYPROJECTILES; i++) {
		landerProjectiles[i].Draw();
	}
	for (int i = 0; i < MAXEXPLOSIONS; i++) {
		explosions[i].Draw(gameTimer->CheckTime());
	}
}
// draw map
void drawMap () {
	// blue line that divides UI from map
	glPushMatrix();
		glColor3f(0.3, 0.3, 1.0);
		glBegin(GL_LINES);
		glVertex3f(0.5, 180.0, 0);
		glVertex3f(265.5, 180.0, 0);
		glEnd();
	glPopMatrix();
	// Red box that highlights radar
	glPushMatrix();
		glColor3f(0.9, 0.1, 0.1);
		glBegin(GL_LINE_LOOP);
		glVertex3f(RADARX, RADARY, 0);
		glVertex3f(RADARX, RADARY + RADARHEIGHT, 0);
		glVertex3f(RADARX + RADARWIDTH, RADARY + RADARHEIGHT, 0);
		glVertex3f(RADARX + RADARWIDTH, RADARY, 0);
	glEnd();
	glPopMatrix();
	// Call the draw function in the map
	map->DrawMap();
}
// draw main menu
void drawMenu() {
	drawText->ReSizeFont(25.0);
	drawText->SetColorFloatRGB(0.5, 0.2, 0.8);
	drawText->WriteBitmapString(42, screenHeight/1.5, "DEFENDORF");
	drawText->SetColorFloatRGB(0.8, 0.3, 0.0);
	drawText->WriteBitmapString(44, screenHeight/1.5, "DEFENDORF");
	drawText->ReSizeFont(8.0);
	drawText->SetColorFloatRGB(0.7, 0.1, 0.8);
	drawText->WriteBitmapString(78.0, 80.0, "Press P to Play");
	drawText->SetColorFloatRGB(0.2, 0.5, 0.8);
	drawText->WriteBitmapString(74.0, 65.0, "Press T for tips");
	drawText->SetColorFloatRGB(0.4, 0.9, 0.1);
	drawText->WriteBitmapString(54.0, 50.0, "Press H for high scores");
	drawText->SetColorFloatRGB(0.8, 0.1, 0.1);
	drawText->WriteBitmapString(63.0, 35.0, "Press Escape to quit");
}
// draw help screen
void drawHelp() {
	drawText->ReSizeFont(20.0);
	drawText->SetColorFloatRGB(0.3, 0.4, 0.8);
	drawText->WriteBitmapString(50.0, 170.0, "how to play");
	drawText->ReSizeFont(4.0);
	drawText->SetColorFloatRGB(0.8, 0.8, 0.8);
	// info on movement and blocks
	drawText->WriteBitmapString(90.0, 140.0, "1: the arrow keys move your ship");
	drawText->WriteBitmapString(90.0, 130.0, "2: space bar fires your lasers");
	glPushMatrix();
		glTranslatef(-80.0, 40.0, 0.0);
		playerShip->Draw();
	glPopMatrix();
	// info on target display
	drawText->WriteBitmapString(10.0, 90.0, " 3: destroy all enemy ships and");
	drawText->WriteBitmapString(10.0, 80.0, "    prevent them from abducting");
	drawText->WriteBitmapString(10.0, 70.0, "    humans.");
	glPushMatrix();
		glTranslatef(60.0, 55.0, 0.0);
		for (int i = 0; i < 4; i++)
		{
			glPushMatrix();
			glTranslatef(i * 15, 0, 0);
			// lander body
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

			glPushMatrix();
			glTranslatef(i * 15, -15, 0);
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
			glPopMatrix();
		}
		glLineWidth(1.0);
	glPopMatrix();
	drawText->WriteBitmapString(130.0, 70.0, " 4: press s to use smart bombs");
	drawText->WriteBitmapString(130.0, 60.0, " 5: press a to active hyperspace");
	drawText->SetColorFloatRGB(0.9, 0.1, 0.3);
	drawText->WriteBitmapString(80.0, 5.0, "press escape to return to main menu");
}
// draw leaderboard screen
void drawLeaderBoard() 
{
	drawText->ReSizeFont(20.0);
	drawText->SetColorFloatRGB(0.2, 0.4, 0.8);
	drawText->WriteBitmapString(37.0, 170.0, "HIGH SCORES");
	drawText->SetColorFloatRGB(0.7, 0.5, 0.2);
	drawText->WriteBitmapString(39.0, 170.0, "HIGH SCORES");
	drawText->ReSizeFont(10.0);
	for (int i = 0; i < SCOREBOARD; i++) {
		drawText->SetColorFloatRGB(0.1*i, 0.8, 0.4);
		drawText->WriteBitmapString(79.0, 145.0 - (i * 13.0), *playerScore->scoreBoardNames[i]);
		drawText->WriteBitmapString(129.0, 145.0 - (i * 13.0), *playerScore->scoreBoardValues[i]);
	}
	drawText->ReSizeFont(5.0);
	drawText->SetColorFloatRGB(0.9, 0.1, 0.3);
	drawText->WriteBitmapString(60.0, 5.0, "press escape to return to main menu");
}
// draws game over screen
void drawGameOver()
{
	drawText->ReSizeFont(20.0);
	drawText->SetColorFloatRGB(0.4, 0.5, 0.5);
	drawText->WriteBitmapString(8.0, 175.0, "you're dead");
	drawText->SetColorFloatRGB(0.9, 0.1, 0.2);
	drawText->WriteBitmapString(10.0, 175.0, "you're dead");
	drawText->ReSizeFont(8.0);
	drawText->SetColorFloatRGB(0.0, 0.8, 0.0);
	drawText->WriteBitmapString(16.0, 45.0, "ENTER YOUR INITIALS: ");
	if (playerScore->currentLetterEntered > 0)
		drawText->WriteBitmapString(155.0, 45.0, playerScore->currentPlayerInitials);
}
// routine to draw a bitmap character string
void writeBitmapString(void *font, char *string)
{  
   char *c;
   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}
// routine to onvert floating point to char string
void floatToString(char * destStr, int precision, int val) 
{
   sprintf(destStr,"%d",val);
   destStr[precision] = '\0';
}
// write standard text for ui elements
void writeData(void)
{
	glColor3f(0.8, 0.1, 0);
	switch (gameState) {
	case PLAY_GAME:
		// display score in GUI
		floatToString(theStringBuffer, 8, playerScore->GetScore());
		glRasterPos3f(screenWidth - 50, screenHeight - 6, 0);
		writeBitmapString((void*)font, "SCORE: ");
		writeBitmapString((void*)font, theStringBuffer);

		// display number of lives in GUI
		floatToString(theStringBuffer, 3, playerScore->CheckLives());
		glRasterPos3f(screenWidth - 50, screenHeight - 11, 0);
		writeBitmapString((void*)font, "LIVES: ");
		writeBitmapString((void*)font, theStringBuffer);
		
		// display number of bombs in GUI
		floatToString(theStringBuffer, 3, playerShip->GetBombCount());
		glRasterPos3f(screenWidth - 50, screenHeight - 16, 0);
		writeBitmapString((void*)font, "BOMBS: ");
		writeBitmapString((void*)font, theStringBuffer);
		
		// display current level number in GUI
		glRasterPos3f(8, screenHeight - 6, 0);
		floatToString(theStringBuffer, 3, currentLevel);
		writeBitmapString((void*)font, "LEVEL: ");
		writeBitmapString((void*)font, theStringBuffer);
		
		// display number of humans still alive in GUI
		floatToString(theStringBuffer, 3, humansOnMap);
		glRasterPos3f(8, screenHeight - 11, 0);
		writeBitmapString((void*)font, "REMAING HUMANS: ");
		writeBitmapString((void*)font, theStringBuffer);
		
		// display number of enemy ships alive in GUI
		floatToString(theStringBuffer, 3, enemiesInLevel);
		glRasterPos3f(8, screenHeight - 16, 0);
		writeBitmapString((void*)font, "REMAING ENEMIES: ");
		writeBitmapString((void*)font, theStringBuffer);
		break;

	case RESPAWN:
	// display score in GUI
		floatToString(theStringBuffer, 8, playerScore->GetScore());
		glRasterPos3f(screenWidth - 50, screenHeight - 6, 0);
		writeBitmapString((void*)font, "SCORE: ");
		writeBitmapString((void*)font, theStringBuffer);

		// display number of lives in GUI
		floatToString(theStringBuffer, 3, playerScore->CheckLives());
		glRasterPos3f(screenWidth - 50, screenHeight - 11, 0);
		writeBitmapString((void*)font, "LIVES: ");
		writeBitmapString((void*)font, theStringBuffer);
		
		// display number of bombs in GUI
		floatToString(theStringBuffer, 3, playerShip->GetBombCount());
		glRasterPos3f(screenWidth - 50, screenHeight - 16, 0);
		writeBitmapString((void*)font, "BOMBS: ");
		writeBitmapString((void*)font, theStringBuffer);
		
		// display current level number in GUI
		glRasterPos3f(8, screenHeight - 6, 0);
		floatToString(theStringBuffer, 3, currentLevel);
		writeBitmapString((void*)font, "LEVEL: ");
		writeBitmapString((void*)font, theStringBuffer);
		
		// display number of humans still alive in GUI
		floatToString(theStringBuffer, 3, humansOnMap);
		glRasterPos3f(8, screenHeight - 11, 0);
		writeBitmapString((void*)font, "REMAING HUMANS: ");
		writeBitmapString((void*)font, theStringBuffer);
		
		// display number of enemy ships alive in GUI
		floatToString(theStringBuffer, 3, enemiesInLevel);
		glRasterPos3f(8, screenHeight - 16, 0);
		writeBitmapString((void*)font, "REMAING ENEMIES: ");
		writeBitmapString((void*)font, theStringBuffer);
		break;

	case PAUSE:
		// display score in GUI
		floatToString(theStringBuffer, 8, playerScore->GetScore());
		glRasterPos3f(screenWidth - 50, screenHeight - 6, 0);
		writeBitmapString((void*)font, "SCORE: ");
		writeBitmapString((void*)font, theStringBuffer);

		// display number of lives in GUI
		floatToString(theStringBuffer, 3, playerScore->CheckLives());
		glRasterPos3f(screenWidth - 50, screenHeight - 11, 0);
		writeBitmapString((void*)font, "LIVES: ");
		writeBitmapString((void*)font, theStringBuffer);
		
		// display number of bombs in GUI
		floatToString(theStringBuffer, 3, playerShip->GetBombCount());	
		glRasterPos3f(screenWidth - 50, screenHeight - 16, 0);
		writeBitmapString((void*)font, "BOMBS: ");
		writeBitmapString((void*)font, theStringBuffer);
		
		// display current level number in GUI
		glRasterPos3f(8, screenHeight - 6, 0);
		floatToString(theStringBuffer, 3, currentLevel);
		writeBitmapString((void*)font, "LEVEL: ");
		writeBitmapString((void*)font, theStringBuffer);
		
		// display number of humans still alive in GUI
		floatToString(theStringBuffer, 3, humansOnMap);
		glRasterPos3f(8, screenHeight - 11, 0);
		writeBitmapString((void*)font, "REMAING HUMANS: ");
		writeBitmapString((void*)font, theStringBuffer);
		
		// display number of enemy ships alive in GUI
		floatToString(theStringBuffer, 3, enemiesInLevel);
		glRasterPos3f(8, screenHeight - 16, 0);
		writeBitmapString((void*)font, "REMAING ENEMIES: ");
		writeBitmapString((void*)font, theStringBuffer);

		// pause info
		glRasterPos3f(screenWidth/2 - 9, screenHeight/2, 0);
		writeBitmapString((void*)font, "PAUSED");
		glRasterPos3f(screenWidth/2 - 22, 20, 0);
		writeBitmapString((void*)font, "Press P to resume.");
		glRasterPos3f(screenWidth/2 - 25, 10, 0);
		writeBitmapString((void*)font, "Press Escape to quit.");
		break;

	case LEVEL_CLEAR:
		// display score in GUI
		floatToString(theStringBuffer, 8, playerScore->GetScore());
		glRasterPos3f(screenWidth - 50, screenHeight - 6, 0);
		writeBitmapString((void*)font, "SCORE: ");
		writeBitmapString((void*)font, theStringBuffer);

		// display number of lives in GUI
		floatToString(theStringBuffer, 3, playerScore->CheckLives());
		glRasterPos3f(screenWidth - 50, screenHeight - 11, 0);
		writeBitmapString((void*)font, "LIVES: ");
		writeBitmapString((void*)font, theStringBuffer);
		
		// display number of bombs in GUI
		floatToString(theStringBuffer, 3, playerShip->GetBombCount());
		glRasterPos3f(screenWidth - 50, screenHeight - 16, 0);
		writeBitmapString((void*)font, "BOMBS: ");
		writeBitmapString((void*)font, theStringBuffer);
		
		// display current level number in GUI
		glRasterPos3f(8, screenHeight - 6, 0);
		floatToString(theStringBuffer, 3, currentLevel);
		writeBitmapString((void*)font, "LEVEL: ");
		writeBitmapString((void*)font, theStringBuffer);
		
		// display number of humans still alive in GUI
		floatToString(theStringBuffer, 3, humansOnMap);
		glRasterPos3f(8, screenHeight - 11, 0);
		writeBitmapString((void*)font, "REMAING HUMANS: ");
		writeBitmapString((void*)font, theStringBuffer);
		
		// display number of enemy ships alive in GUI
		floatToString(theStringBuffer, 3, enemiesInLevel);
		glRasterPos3f(8, screenHeight - 16, 0);
		writeBitmapString((void*)font, "REMAING ENEMIES: ");
		writeBitmapString((void*)font, theStringBuffer);
		break;
   }
}
// remove all projectiles from player sight
void clearProjectileLocations()
{
	currentPlayerShot = 0;
	currentEnemyShot = 0;
	currentExplosion = 0;
	for (int i = 0; i < ENEMYPROJECTILES; i++) { landerProjectiles[i].Idle(); }
	for (int i = 0; i < PLAYERPROJECTILES; i++) { playerProjectiles[i].Idle(); }
	for (int i = 0; i < MAXEXPLOSIONS; i++) { explosions[i].Idle(); }
}
// main glut setup and start glut loop
int main(int argc, char **argv) 
{
//	srand(time(NULL));
	printInteraction();
//  openGL Setup	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); 
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(400, 400); 
	glutCreateWindow("DEFENDORF"); 
	setup();
	glutDisplayFunc(drawScene); 
	glutReshapeFunc(resize);  
	glutKeyboardFunc(keyInput);
	glutKeyboardUpFunc(keyUp);
	glutSpecialFunc(specKeyInput);
	glutSpecialUpFunc(specKeyUp);
	glutTimerFunc(animationPeriod, animate, 1);
	glutMainLoop(); 
	return 0;  
}