// 
// Author : Quasar C.
//

#ifndef Frequency_H
#define Frequency_H

#include <boost/date_time/gregorian/gregorian.hpp>
#include <string>

using namespace boost::gregorian;
using namespace std;

enum class frequency {
	P0D, P1D, P2D, P7D, P1W, P2W, P3W, P1M, P2M, P3M, P4M, P6M, P9M, P12M, P1Y, P2Y, P3Y, P4Y, P5Y, P10Y, P15Y, P20Y, P30Y
};

class Frequency
{
public:
	Frequency() = default;		//Default Constructor
	Frequency(string f);		//Define a frequency object by passing a string representation of the periodic frequency
	//Frequency(const Frequency& f);	//Copy constructor

	frequency getPeriod() const;
	void setPeriod(frequency f);

private:
	frequency period;
};


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

frequency Frequency::getPeriod() const
{
	return period;
}

void Frequency::setPeriod(frequency f)
{
	period = f;
}

#endif