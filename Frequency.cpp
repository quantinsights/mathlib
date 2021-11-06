#include "Frequency.h"

Frequency::Frequency(string f)
{
	period = frequency::P0D;

	if (f == "0D")
		period = frequency::P0D;
	else if (f == "1D")
		period = frequency::P1D;
	else if (f == "2D")
		period = frequency::P2D;
	else if (f == "7D")
		period = frequency::P7D;
	else if (f == "1W")
		period = frequency::P1W;
	else if (f == "2W")
		period = frequency::P2W;
	else if (f == "3W")
		period = frequency::P3W;
	else if (f == "1M")
		period = frequency::P1M;
	else if (f == "2M")
		period = frequency::P2M;
	else if (f == "3M")
		period = frequency::P3M;
	else if (f == "6M")
		period = frequency::P6M;
	else if (f == "9M")
		period = frequency::P9M;
	else if (f == "12M")
		period = frequency::P12M;
	else if (f == "1Y")
		period = frequency::P1Y;
}

frequency Frequency::getPeriod()
{
	return period;
}

void Frequency::setPeriod(frequency f)
{
	period = f;
}
