#ifndef BusinessDayConventions_H
#define BusinessDayConventions_H

#include <boost/date_time/gregorian/gregorian.hpp>
#include <string>

using namespace boost::gregorian;
using namespace std;

enum class businessDayConventions {
	NO_ADJUST, FOLLOWING, MODIFIED_FOLLOWING, PRECEDING, MODIFIED_PRECEDING
};

/// Constants and implementations for standard business day conventions.
// 
// Author : Quasar C.
//
/// The purpose of each convention is to define how to handle non-business days.
/// When processing dates in finance, it is typically intended that non-business days,
/// such as weekends and holidays, are converted to a nearby valid business day.
/// The convention, in conjunction with a HolidayCalendar defines exactly how the adjustment
/// should be made.

/// Here is a description of different conventions.
/// - NO_ADJUST : Make no adjustment.
/// - FOLLOWING : Move to the next valid business day
/// - MODIFIED_FOLLOWING : Move to the next valid business day, unless that is in the next month in which case move to the previous valid business day
/// - PRECEDING : Move to the previous valid business day
/// - MODIFIED_PRECEDING : Move to the previous valid business day, unless that is in the previous month, in which case move to the next valid business day

class BusinessDayConventions
{
public:
	BusinessDayConventions() = default;					//Default Constructor
	BusinessDayConventions(string f);					//Define a frequency object by passing a string representation of the periodic frequency
	//BusinessDayConventions(const BusinessDayConventions& b);	//Copy constructor

	businessDayConventions getBusDayConvention() const;
	void setBusDayConvention(businessDayConventions f);
private:
	businessDayConventions busDayConv;
};



#endif