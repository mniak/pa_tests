#include "stdafx.h"
#include "Sine.h"
#include "ScopedPaHandler.h"

using namespace std;

int main(void)
{
	Sine sine;

	cout << "PortAudio Test: output sine wave. SR = " << SAMPLE_RATE << ", BufSize = " << FRAMES_PER_BUFFER << endl;

	ScopedPaHandler paInit;
	if (paInit.result() != paNoError) goto error;

	if (sine.open(Pa_GetDefaultOutputDevice()))
	{
		if (sine.start())
		{
			cout << "Play for " << NUM_SECONDS << " seconds" << endl;
			Pa_Sleep(NUM_SECONDS * 1000);


			sine.stop();
		}

		sine.close();
	}

	cout << "Test finished." << endl;

	getchar();

	return paNoError;

error:
	cerr << "An error occured while using the portaudio stream" << endl;
	cerr << "Error number: " << paInit.result() << endl;
	cerr << "Error message: " << Pa_GetErrorText(paInit.result()) << endl;
	return 1;
}
