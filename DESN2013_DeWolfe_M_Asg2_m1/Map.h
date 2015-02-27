#pragma once
#include <glut.h>
#include <time.h>

// number of points in mapInfo array
#define MAPINFO 20

// classes uses 2 identital map images to scroll map
class Map {
private:
	// holds x and y coords for map
	float mapInfo1[MAPINFO][2];
	// position to start drawing maps at
	float startPoint1, startPoint2;
	// maximum height for terrain features
	int maxHeight;
	// measured width of map in pixels, 600 per map = 1200 an entire level
	int totalWidth;
public:
	Map()
	{
		totalWidth = 600;
		startPoint1 = -(totalWidth/2);
		startPoint2 = (totalWidth/2);
		maxHeight = 38;
	}
	void DrawMap()
	{
		// first half of map
		glPushMatrix();
		glTranslatef(startPoint1, 0, 0);
		// iterate through map image 1
		glColor3f(0.7, 0.4, 0.4);
		glBegin(GL_LINES);
		for (int i = 1; i < MAPINFO; i++)
		{
			glVertex3f(mapInfo1[i-1][0], mapInfo1[i-1][1], -0.1);
			glVertex3f(mapInfo1[i][0], mapInfo1[i][1], -0.1);

		}
		glEnd();
		glPopMatrix();
		// second half of map
		glPushMatrix();
		glTranslatef(startPoint2, 0, 0);
		// iterate through map image number 2
		glBegin(GL_LINES);
		for (int i = 1; i < MAPINFO; i++)
		{
			glVertex3f(mapInfo1[i-1][0], mapInfo1[i-1][1], -0.1);
			glVertex3f(mapInfo1[i][0], mapInfo1[i][1], -0.1);
		}
		glEnd();
		glPopMatrix();
	}
	// randomize a new map
	void GenerateMap()
	{
		srand(time(NULL));
		// randomly generate Y values for terrain below maxHeight
		float ratio = totalWidth/MAPINFO;
		// first and last position are never random to ensure a smooth transition
		mapInfo1[0][0] = -1.0f;
		mapInfo1[0][1] = 25.0f;
		mapInfo1[MAPINFO-1][0] = totalWidth + 1;
		mapInfo1[MAPINFO-1][1] = 25.0f;
		for (int i = 1; i < MAPINFO - 1; i++)
		{
			mapInfo1[i][0] = i * ratio;
			mapInfo1[i][1] = 5 + rand()%maxHeight;

		}
	}
	// shift the map starting point based on player movement
	void ShiftMap(float _offset)
	{
		startPoint1 += _offset;
		startPoint2 += _offset;
		// map layer 1 hits leftmost point
		if (startPoint1 < -totalWidth) {
			startPoint1 = totalWidth;
			startPoint2 = startPoint1 - totalWidth;
		}
		// map layer 1 hits rightmost point
		else if (startPoint1 > totalWidth) {
			startPoint1 = -totalWidth;
			startPoint2 = startPoint1 + totalWidth;
		}
		// map layer 2 hits leftmost point
		else if (startPoint2 < -totalWidth) {
			startPoint2 = totalWidth;
			startPoint1 = startPoint2 - totalWidth;
		}
		// map layer 2 hits rightmost point
		else if (startPoint2 > totalWidth) {
			startPoint2 = -totalWidth;
			startPoint1 = startPoint2 + totalWidth;
		}
	}
	float GetMapX()
	{ return startPoint1/2 + startPoint2/2; }

	~Map() {}
};