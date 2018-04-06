#pragma once
#include "common.h"
#include <SDL.h>
#include <SDL_audio.h>
#include <queue>

struct BeepObject
{
	double freq;
	int samplesLeft;
};

class Beeper
{
private:
	double phase;
	std::queue<BeepObject> beeps;
public:
	Beeper();
	~Beeper();
	void beep(double freq, int duration);
	void generateSamples(Sint16 *stream, int length);
	void wait();
};
