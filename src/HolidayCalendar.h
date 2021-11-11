#pragma once

#ifndef HolidayCalendar_H
#define HolidayCalendar_H

#include <boost/date_time/gregorian/gregorian.hpp>
#include "BusinessDayConventions.h"
#include <string>
#include <vector>

using namespace std;
using namespace boost::gregorian;

typedef boost::gregorian::date::year_type year_type;
typedef boost::gregorian::date::month_type month_type;
typedef boost::gregorian::date::day_type day_type;

enum class HolidayCalendarId
{
	GBLO, // London(UK) Holidays
	NYSE, // New York Stock Exchange Holidays
	EUTA, // TARGET interbank payment holidays
	CUST  // Custom Holiday Calendar
};

/// Holidays.
// 
// Author : Quasar C.
//
/// A key problem when working with dates is the impact of holidays and weekends. To manage this complexity, 
/// a holiday calendar is used. 
/// 
/// A holiday calendar implementation keeps track of which dates are holidays and which are weekends.
/// Different countries have different holiday dates and thus different calendars. It's also not unusual
/// for individual exchanges or other financial entities to have their own calendar.

/// When a HolidayCalendar instance is created, an internal holidays vector is populated with the 
/// different holiday dates for the specified country.

/// My naive implementation of HolidayCalendars is inspired by the open-source pricing and risk 
/// analytics library, OpenGamma. See here : 
/// https://github.com/OpenGamma/Strata/blob/main/modules/basics/src/main/java/com/opengamma/strata/basics/date/GlobalHolidayCalendars.java
/// 
class HolidayCalendar {	
private:
	/// <summary>
	/// Holiday dates in a calendar.
	/// </summary>
	vector<date> holidays;			

	/// <summary>
	/// First weekend day.
	/// </summary>
	gregorian_calendar::day_of_week_type firstWeekendDay {Saturday};		

	/// <summary>
	/// Second Weekend day.
	/// </summary>
	gregorian_calendar::day_of_week_type secondWeekendDay {Sunday};		

	/// <summary>
	/// A unique calendar identifier e.g. NYSE, GBLO, EUTA(Target).
	/// </summary>
	HolidayCalendarId holidayCalendarId;						
public:
	// Constructors
	HolidayCalendar();														// Default Constructor
	HolidayCalendar(HolidayCalendarId id);									// Create a holiday calendar with a user-specified ID
	HolidayCalendar(std::vector<date> h, gregorian_calendar::day_of_week_type f, gregorian_calendar::day_of_week_type s, HolidayCalendarId id);
	HolidayCalendar(const HolidayCalendar& h);								// Copy Constructor

	//Getters
	vector<date> getHolidays() const;
	gregorian_calendar::day_of_week_type getFirstWeekendDay() const;
	gregorian_calendar::day_of_week_type getSecondWeekendDay() const;
	HolidayCalendarId getHolidayCalendarId() const;

	//Assignment Operator
	HolidayCalendar& operator = (const HolidayCalendar& h);

	/// <summary>
	/// This helper function applies rules determined by the holiday calendar
	/// id, to build a vector of holiday dates.
	/// </summary>
	void generateCalendar();

	//Utility functions
	
	/// <summary>
	/// Get the Easter date in a given year.
	/// </summary>
	/// <param name="year"></param>
	/// <returns></returns>
	date easter(int year);

	/// <summary>
	/// Bump the date passed as an input parameter to the following Monday.
	/// </summary>
	/// <param name="d"></param>
	/// <returns></returns>
	date bumpToMon(date d);

	/// <summary>
	/// Determine the Christmas holiday in year; if 25th December falls on a Saturday or
	/// Sunday, 27/12 would be a bank holiday.
	/// </summary>
	/// <param name="year"></param>
	/// <returns></returns>
	date christmasBumpedSatSun(int year);


	date boxingDayBumpedSatSun(int year);
	date firstInMonth(int year, int month, gregorian_calendar::day_of_week_type dayOfWeek);		//First date in the month with the specified dayOfWeek
	date lastInMonth(int year, int month, gregorian_calendar::day_of_week_type dayOfWeek);		//Last date in a month with the specified dayOfWeek
	void removeSatSun();																		//Remove any saturdays and sundays from the holiday calendar

	/// <summary>
	/// Scan the holiday calendar to check if a given date is a business holiday.
	/// </summary>
	/// <param name="d"></param>
	/// <returns></returns>
	bool isHoliday(date d);									// Check if a given date is a holiday
	bool isBusinessDay(date d);								// Check if a given date is a business day
	date adjust(const date& d, BusinessDayConventions c);	// Find the adjusted date for a given unadjusted date, according to the business day conventions.
};
#endif