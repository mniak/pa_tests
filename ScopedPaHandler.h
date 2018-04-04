#pragma once
class ScopedPaHandler
{
public:
	ScopedPaHandler();
	~ScopedPaHandler();

	PaError result() const;

private:
	PaError _result;
};

