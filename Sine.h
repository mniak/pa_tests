#pragma once

class Sine
{
public:
	Sine();

	bool open(PaDeviceIndex index);

	bool close();


	bool start();

	bool stop();

private:
	int paCallbackMethod(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags);

	static int paCallback(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void *userData);


	void paStreamFinishedMethod();

	static void paStreamFinished(void* userData);

	PaStream *stream;
	float sine[SINE_TABLE_SIZE];
	int left_phase;
	int right_phase;
	std::string message;
};
