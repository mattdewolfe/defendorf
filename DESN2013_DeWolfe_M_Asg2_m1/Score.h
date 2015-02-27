/*	Shoot a Lander - 150 points
	Save a Humanoid from a Lander - 500 points
	Humanoid lands safely on his own - 250 points
	Depositing a saved Humanoid - 500 points (See Bonus)
	Bonus points at end of wave - 100 points per humanoid alive
*/
#pragma once
#include <fstream>
#include <sstream>

#define SCOREBOARD 10
class Score 
{
private:
	int score, lives, baseLives;

public:
	char *scoreBoardNames[SCOREBOARD][3], *scoreBoardValues[SCOREBOARD][12];
	int scoreBoardIntValues[SCOREBOARD];
	int currentScorePosition; // store the index for array use if player hits high score
	char currentPlayerInitials[4]; // store players initials for high score
	int currentLetterEntered; // track which position of initials player is at
	// constructor
	Score(int _baseLives) 
	{
		baseLives = _baseLives;
		for (int i = 0; i < SCOREBOARD; i++) 
		{
			*scoreBoardNames[i] = new char [3];
			*scoreBoardValues[i] = new char [12];
		}
		currentScorePosition = -1;
		currentPlayerInitials[0] = 'M';
		currentPlayerInitials[1] = ' ';
		currentPlayerInitials[2] = ' ';
		currentLetterEntered = 0;
	}
	// called when points are scored
	void Score::PointsScored(int bonus) { score += bonus; }
	// called to return score
	int Score::GetScore() { return score; }
	// add/subtract lives
	void Score::AddLives(int lifeChange) 
	{
		if (lives < 99) { lives += lifeChange; }
	}
	// check current lives
	int Score::CheckLives() { return lives; }
	// reset lives and score to new game values
	void Score::Reset() 
	{
		lives = baseLives;
		score = 0;
		currentLetterEntered = 0;
		currentScorePosition = -1;
	}

	void Score::ReadScores() 
	{
		// create file string and read in values
		std::ifstream openFile;
		openFile.open ("./Scoring.txt");
		int i = 0;
		while (openFile.good() == true) 
		{
			openFile.getline(*scoreBoardNames[i], 5);
			openFile.getline(*scoreBoardValues[i], 12);
			i++;
		}

		// Next convert strings to ints in order to compare values
		// at game end
		std::stringstream convertString;
		for (int i = 0; i < SCOREBOARD; i++)
		{
			scoreBoardIntValues[i] = 0;
			convertString << *scoreBoardValues[i];
			convertString >> scoreBoardIntValues[i];
			convertString.clear();
		}
	}
	
	// Check player score against scoreboard, set currentScorePosition to matching index of new score
	void CheckScore()
	{
		// iterate through scoreboard, 0 is highest position
		for (int i = 0; i < SCOREBOARD; i++)
		{
			if (score > scoreBoardIntValues[i])
			{
				currentScorePosition = i;
				break;
			}
		}
	}
	// Update the scoreboard with new players score
	void UpdateScoreBoard()
	{
		// Only update scoreboard if a new high score was set
		if (currentScorePosition != -1)
		{
			std::stringstream convertString;
			char tempScoreBoardNames[SCOREBOARD][4], tempScoreBoardValues[SCOREBOARD][13];
			// First, copy scores into temp arrays
			for (int i = 0; i < SCOREBOARD; i++)
			{
				strcpy(tempScoreBoardNames[i], *scoreBoardNames[i]);
				strcpy(tempScoreBoardValues[i], *scoreBoardValues[i]);
			}
			// Put new score and initials into the score array
			convertString << score;
			convertString >> *scoreBoardValues[currentScorePosition];
			convertString.clear();
		
			*scoreBoardNames[currentScorePosition] = currentPlayerInitials;
			// Lastly copy the positions after the scoring index back into the array
			for (int i = currentScorePosition + 1; i < SCOREBOARD; i++)
			{
				strcpy(*scoreBoardNames[i], tempScoreBoardNames[i-1]);
				strcpy(*scoreBoardValues[i], tempScoreBoardValues[i-1]);
			}
		}
		currentScorePosition = -1;
		currentPlayerInitials[0] = 'M';
		currentPlayerInitials[1] = ' ';
		currentPlayerInitials[2] = ' ';
		currentLetterEntered = 0;
	}
		
	// destructor
	~Score() {}
};