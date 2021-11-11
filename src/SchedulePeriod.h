#ifndef SchedulePeriod_H
#define SchedulePeriod_H

#include <boost/date_time/gregorian/gregorian.hpp>
#include "Frequency.h"

using namespace boost::gregorian;

/// A period in a schedule.
// 
// Author : Quasar C.
//
/// This consists of a single period (date range) within a schedule.
/// This is typically used as the basis for financial calculations, such as accrual of interest.
///
/// Two pairs of dates are provided, start/end and unadjustedStart/unadjustedEnd
/// The period itself runs from startDate to endDate
/// The unadjustedStartDate and unadjustedEndDate are used to calculate the startDate and endDate
/// when applying business day adjustment. 
///
/// For example, consider the schedule that has periods every three months on the 10th of the month. 
/// From time-to-time, the scheduled date will be a weekend or holiday.
/// In this case, a rule may apply moving the date to a valid business day.
/// If this happens, then the unadjusted date is the original date in the SchedulePeriod
/// and the adjusted date is the related valid business day. 
/// Note that: all schedules apply a business day adjustment.
/// 
class SchedulePeriod
{
public:
	//Constructors
	SchedulePeriod() = default;		//Default Constructor
	SchedulePeriod(date startDate, date endDate, date unAdjustedStartDate, date unadjustedEndDate); //Parametrized constructor
	SchedulePeriod(const SchedulePeriod& s);	//Copy constructor

	int lengthInDays();		

private:
	date adjustedStartDate; // The start date in this period used for financial calculations such as interest accrual.
	date adjustedEndDate;	// The end date of this period used for financial calculations such as interest accrual.
	date unAdjustedStartDate;
	date unAdjustedEndDate;
};

// Parametrized constructor
SchedulePeriod::SchedulePeriod(date s, date e, date us, date ue) :adjustedStartDate{ s }, adjustedEndDate{ e }, unAdjustedStartDate{ us }, unAdjustedEndDate{ ue }
{
}

SchedulePeriod::SchedulePeriod(const SchedulePeriod& s) : SchedulePeriod{ s.adjustedStartDate, s.adjustedEndDate, s.unAdjustedStartDate, s.unAdjustedEndDate }
{
}

/// Calculates the number of days in the period.
/// 
/// This returns the actual number of days in the period, considering the adjusted start and the adjusted 
/// end dates. This calculation does not involve a DayCount or a holiday calendar. It includes the startDate
/// and excludes the end date.
int SchedulePeriod::lengthInDays()
{
	return (adjustedEndDate - adjustedStartDate).days();
}

#endif
