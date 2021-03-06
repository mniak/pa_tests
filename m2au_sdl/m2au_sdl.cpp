#include "common.h"
#include "Beeper.h"

#include <iostream>
#include <conio.h>
using namespace std;

#include <SDL.h>


int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
		return -1;

	int duration = 60 * 1000;
	double Hz = 440;

	Beeper b;
	b.beep(Hz, duration);
	b.wait();

	return 0;
}