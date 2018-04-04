#include "stdafx.h"
#include "ScopedPaHandler.h"


ScopedPaHandler::ScopedPaHandler()
	: _result(Pa_Initialize())
{
}
ScopedPaHandler::~ScopedPaHandler()
{
	if (_result == paNoError)
	{
		Pa_Terminate();
	}
}

PaError ScopedPaHandler::result() const { return _result; }
