#include "stdafx.h"
#include "Sine.h"

using namespace std;

Sine::Sine() : stream(0), left_phase(0), right_phase(0)
{
	/* initialise sinusoidal wavetable */
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		sine[i] = (float)sin(((double)i / (double)TABLE_SIZE) * M_PI * 2.);
	}
	message = "No Message";
}

bool Sine::open(PaDeviceIndex index)
{
	PaStreamParameters outputParameters;

	outputParameters.device = index;
	if (outputParameters.device == paNoDevice) {
		return false;
	}

	const PaDeviceInfo* pInfo = Pa_GetDeviceInfo(index);
	if (pInfo != 0)
	{
		cout << "Output device name: '" << pInfo->name << "'" << endl;
	}

	outputParameters.channelCount = 2;       /* stereo output */
	outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	PaError err = Pa_OpenStream(
		&stream,
		NULL, /* no input */
		&outputParameters,
		SAMPLE_RATE,
		paFramesPerBufferUnspecified,
		paClipOff,      /* we won't output out of range samples so don't bother clipping them */
		&Sine::paCallback,
		this            /* Using 'this' for userData so we can cast to Sine* in paCallback method */
	);

	if (err != paNoError)
	{
		/* Failed to open stream to device !!! */
		return false;
	}

	err = Pa_SetStreamFinishedCallback(stream, &Sine::paStreamFinished);

	if (err != paNoError)
	{
		Pa_CloseStream(stream);
		stream = 0;

		return false;
	}

	return true;
}

bool Sine::close()
{
	if (stream == 0)
		return false;

	PaError err = Pa_CloseStream(stream);
	stream = 0;

	return (err == paNoError);
}


bool Sine::start()
{
	if (stream == 0)
		return false;

	PaError err = Pa_StartStream(stream);

	return (err == paNoError);
}

bool Sine::stop()
{
	if (stream == 0)
		return false;

	PaError err = Pa_StopStream(stream);

	return (err == paNoError);
}


/* The instance callback, where we have access to every method/variable in object of class Sine */
int Sine::paCallbackMethod(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags)
{
	float *out = (float*)outputBuffer;
	unsigned long i;

	(void)timeInfo; /* Prevent unused variable warnings. */
	(void)statusFlags;
	(void)inputBuffer;

	for (i = 0; i < framesPerBuffer; i++)
	{
		*out++ = sine[left_phase];  /* left */
		*out++ = sine[right_phase];  /* right */
		left_phase += 1;
		if (left_phase >= TABLE_SIZE) left_phase -= TABLE_SIZE;
		right_phase += 3; /* higher pitch so we can distinguish left and right. */
		if (right_phase >= TABLE_SIZE) right_phase -= TABLE_SIZE;
	}

	return paContinue;

}

/* This routine will be called by the PortAudio engine when audio is needed.
** It may called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
int Sine::paCallback(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData)
{
	/* Here we cast userData to Sine* type so we can call the instance method paCallbackMethod, we can do that since
	we called Pa_OpenStream with 'this' for userData */
	return ((Sine*)userData)->paCallbackMethod(inputBuffer, outputBuffer,
		framesPerBuffer,
		timeInfo,
		statusFlags);
}


void Sine::paStreamFinishedMethod()
{
	cout << "Stream Completed: ";
	cout << message << endl;
}

/*
* This routine is called by portaudio when playback is done.
*/
void Sine::paStreamFinished(void* userData)
{
	return ((Sine*)userData)->paStreamFinishedMethod();
}

