#ifndef Schedule_H

#include <iostream>
#include <vector>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "SchedulePeriod.h"
#include "Frequency.h"
#include "BusinessDayAdjustment.h"

using namespace std;

/// Schedules
// 
// Author : Quasar C.
//
/// A common problem in Finance is the derivation of a schedule of dates. The schedule is generally used to determine
/// accrued interest and payments. 

/// Building a schedule is conceptually simple, however the devil is in the details. How should end-of-month be handled?
/// What happens when a cashflow date falls on a holiday? What happens when trying to divide 22 months into 3 month units?

/// The ``Schedule`` class includes constructors that provide powerful mechanisms to create schedules. The ``Schedule``
/// class contains the following mandatory attributes.
/// - ``startDate``, the start of the first schedule period.
/// - ``endDate``, the end of the last schedule period.
/// - ``frequency``, regular periodic frequency to use.
/// - ``businessDayAdjustment``, the business day adjustment to apply

/// I store a Schedule as a vector of Schedule Periods.

/// The following optional items are also available to customize the schedule:
/// -``startDateBusinessDayAdjustment``, overrides the business day adjustment to be used for the start date.
/// -``endDateBusinessDayAdjustment``, overrides the business day adjustment to be used for the end date.
/// -``stubConvention``, convention defining how to handle stubs.
/// -``rollConvention``, convention defining how to roll dates.
/// -``firstRegularStartDate``, start date of the first regular schedule period, which is the end date of the initial stub
/// -``lastRegularStartDate``, end date of the last regular schedule period, which is the start date of the final stub
/// 
class Schedule {
private:
	date startDate;
	date endDate;
	Frequency frequency;
	BusinessDayAdjustment busDayAdj;

	BusinessDayAdjustment startDateBusDayAdj;
	BusinessDayAdjustment endDateBusDayAdj;
	date firstRegularStartDate;
	date lastRegularStartDate;

	vector<SchedulePeriod> schedulePeriods;
	
	//RollConvention rollConvention;
public:
	//Constructors
	/// Default constructor
	Schedule() = default;													
	Schedule(const date& s, const date& d,const Frequency& f, BusinessDayAdjustment& b);

	///Parametric constructor
	Schedule(const vector<SchedulePeriod>& periods, const Frequency& f);	
	Schedule(Schedule& s);													

};

#endif // !Schedule_H