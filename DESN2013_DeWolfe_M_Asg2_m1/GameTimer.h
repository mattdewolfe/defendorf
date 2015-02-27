#pragma once

class GameTimer {

public:
	// handle spawn duration and respawn event
	int spawnDuration;
	GameTimer(int _fps, int _respawnDelay) {
		timer = 0;
		fps = _fps;
		running = false;
		spawnDuration = _respawnDelay;
	}
	// flag clock for ticking
	void GameTimer::Start() { running = true; }
	// flag clock to stop
	void GameTimer::Stop() { running = false; }
	// returns timer/fps for time in seconds
	int GameTimer::CheckTime() {
		return timer;
	}
	// increment time, called every frame
	void GameTimer::Increment() {
		if (running == true) 
		{
			timer++;
		}
		if (timer > 10002)
		{
			timer = 1;
		}
	}
	// reset clock
	void GameTimer::Reset() { timer = 0; }

private:
	// holds FPS from openGL animation
	int fps;
	bool running;
	// increments every frame
	int timer;	
};