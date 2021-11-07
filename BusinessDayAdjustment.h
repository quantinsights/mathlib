#ifndef BusinessDayAdjustment_H
#define BusinessDayAdjustment_H

#include "BusinessDayConventions.h"
#include "HolidayCalendar.h"

using namespace std;

class BusinessDayAdjustment
{
public:
	BusinessDayAdjustment() = default;
	~BusinessDayAdjustment();

private:
	BusinessDayConventions busDayConv;
	HolidayCalendarId id;
};

BusinessDayAdjustment::BusinessDayAdjustment()
{
}

BusinessDayAdjustment::~BusinessDayAdjustment()
{
}
#endif // !BusinessDayAdjustment_H
