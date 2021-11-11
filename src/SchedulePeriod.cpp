#include "pch.h"
#include "SchedulePeriod.h"
#include "Frequency.h"

// Parametrized constructor
SchedulePeriod::SchedulePeriod(date s, date e, date us, date ue) :adjustedStartDate{ s }, adjustedEndDate{ e }, unAdjustedStartDate{ us }, unAdjustedEndDate{ ue }
{
}

SchedulePeriod::SchedulePeriod(const SchedulePeriod& s):SchedulePeriod{s.adjustedStartDate, s.adjustedEndDate, s.unAdjustedStartDate, s.unAdjustedEndDate}
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