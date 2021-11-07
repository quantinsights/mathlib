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
	Frequency(const Frequency& f);	//Copy constructor

	frequency getPeriod();
	void setPeriod(frequency f);

private:
	frequency period;
};
#endif