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

HolidayCalendar::HolidayCalendar() {
	//Default holiday calendar
	firstWeekendDay = greg_weekday{ Saturday };
	secondWeekendDay = greg_weekday{ Sunday };
	holidayCalendarId = HolidayCalendarId::CUST;
}


HolidayCalendar::HolidayCalendar(HolidayCalendarId id) : holidayCalendarId{ id }
{
	generateCalendar();
}

HolidayCalendar::HolidayCalendar(std::vector<date> h, gregorian_calendar::day_of_week_type f, gregorian_calendar::day_of_week_type s, HolidayCalendarId id) : holidays{ h }, firstWeekendDay{ f }, secondWeekendDay{ s }, holidayCalendarId{ id }
{
}

HolidayCalendar::HolidayCalendar(const HolidayCalendar& h) : HolidayCalendar{ h.holidays, h.firstWeekendDay, h.secondWeekendDay, h.holidayCalendarId }
{
}

vector<date> HolidayCalendar::getHolidays() const
{
	return holidays;
}

gregorian_calendar::day_of_week_type HolidayCalendar::getFirstWeekendDay() const
{
	return firstWeekendDay;
}

gregorian_calendar::day_of_week_type HolidayCalendar::getSecondWeekendDay() const
{
	return secondWeekendDay;
}

HolidayCalendarId HolidayCalendar::getHolidayCalendarId() const
{
	return holidayCalendarId;
}

void HolidayCalendar::generateCalendar()
{
	if (holidayCalendarId == HolidayCalendarId::GBLO)
	{
		for (year_type year{ 1950 }; year <= 2099; year = year + 1)
		{

			// New Year
			if (year >= 1974)
			{
				holidays.push_back(bumpToMon(date{ year, 1, 1 }));
			}

			// Easter
			holidays.push_back(date{ easter(year) - days(2) });
			holidays.push_back(date{ easter(year) + days(1) });


			// Early May
			if (year == 1995 || year == 2020)
			{
				holidays.push_back(date{ year, 5, 8 });
			}
			else if (year >= 1978)
			{
				holidays.push_back(firstInMonth(year, 5, Monday));
			}

			// Bank holiday to mark queen's golden jubilee, diamond jubilee and platinum jubilee
			if (year == 2002)
			{
				//golden jubilee
				holidays.push_back(date{ 2002, 6, 3 });
				holidays.push_back(date{ 2002, 6, 4 });
			}
			else if (year == 2012)
			{
				//diamond jubilee
				holidays.push_back(date{ 2012, 6, 4 });
				holidays.push_back(date{ 2012, 6, 5 });

			}
			else if (year == 2022)
			{
				//platinum jubilee
				holidays.push_back(date{ 2022, 6, 2 });
				holidays.push_back(date{ 2022, 6, 3 });
			}
			else if (year == 1967 || year == 1970)
			{
				holidays.push_back(lastInMonth(year, 5, Monday));
			}
			else if (year < 1971)
			{
				//white sunday
				holidays.push_back(easter(year) + days(50));
			}
			else
			{
				holidays.push_back(lastInMonth(year, 5, Monday));
			}

			// Summer
			if (year < 1965)
			{
				holidays.push_back(firstInMonth(year, 8, Monday));
			}
			else if (year < 1971)
			{
				holidays.push_back(lastInMonth(year, 8, Saturday) + days(2));
			}
			else
			{
				holidays.push_back(lastInMonth(year, 8, Monday));
			}

			// Christmas
			holidays.push_back(christmasBumpedSatSun(year));
			holidays.push_back(boxingDayBumpedSatSun(year));


			holidays.push_back(date{ 2011, 4, 29 }); // royal wedding
			holidays.push_back(date{ 1999, 12, 31 }); //millenium
			firstWeekendDay = greg_weekday{ Saturday };
			secondWeekendDay = greg_weekday{ Sunday };
		}
	}
}

/// <summary>
/// The first date in a month that falls on a day of the week, specified by the ``dayOfWeek`` argument.
/// </summary>
/// <param name="year"></param>
/// <param name="month"></param>
/// <param name="dayOfWeek"></param>
/// <returns></returns>
date HolidayCalendar::firstInMonth(int year, int month, gregorian_calendar::day_of_week_type dayOfWeek)
{
	date firstOfTheMonth{ static_cast<year_type>(year),static_cast<month_type>(month), 1 };
	date result{ firstOfTheMonth };
	while (result.day_of_week() != dayOfWeek)
		result += days(1);

	return result;
}

date HolidayCalendar::lastInMonth(int year, int month, gregorian_calendar::day_of_week_type dayOfWeek)
{
	day_type eom_day{ gregorian_calendar::end_of_month_day(year, month) };
	date endOfMonth{ static_cast<year_type>(year),static_cast<month_type>(month), eom_day };
	date result{ endOfMonth };
	while (result.day_of_week() != dayOfWeek)
		result -= days(1);

	return result;
}

date HolidayCalendar::bumpToMon(date d)
{
	if (d.day_of_week() == Sunday)
	{
		d += days(1);
	}
	return d;
}

date HolidayCalendar::christmasBumpedSatSun(int year)
{
	date christmas{ static_cast<year_type>(year), 12, 25 };
	if (christmas.day_of_week() == Saturday || christmas.day_of_week() == Sunday)
	{
		return date{ static_cast<year_type>(year), 12, 27 };
	}
	return christmas;
}

date HolidayCalendar::boxingDayBumpedSatSun(int year)
{
	date boxingDay{ static_cast<year_type>(year), 12, 26 };
	if (boxingDay.day_of_week() == Saturday || boxingDay.day_of_week() == Sunday)
	{
		return date{ static_cast<year_type>(year), 12, 28 };
	}
	return boxingDay;
}

bool isSatSun(date d)
{
	return (d.day_of_week() == Saturday || d.day_of_week() == Sunday);
}

void HolidayCalendar::removeSatSun()
{
	holidays.erase(
		std::remove_if(
			holidays.begin(), holidays.end(),
			isSatSun),
		holidays.end()
	);
}



bool HolidayCalendar::isHoliday(date d)
{
	if (d.day_of_week() == firstWeekendDay || d.day_of_week() == secondWeekendDay)
		return true;
	else
		return false;

	vector<date>::iterator it;
	it = find(holidays.begin(), holidays.end(), d);
	if (it != holidays.end())
		return true;
	else
		return false;
}

bool HolidayCalendar::isBusinessDay(date d)
{
	return !isHoliday(d);
}

date HolidayCalendar::adjust(const date& d, BusinessDayConventions c)
{
	if (c.getBusDayConvention() == businessDayConventions::NO_ADJUST)
		return d;

	date d1 = d;
	if (c.getBusDayConvention() == businessDayConventions::FOLLOWING ||
		c.getBusDayConvention() == businessDayConventions::MODIFIED_FOLLOWING)
	{
		while (isHoliday(d1))
			d1 += days(1);
		if (c.getBusDayConvention() == businessDayConventions::MODIFIED_FOLLOWING)
		{
			if (d1.month() != d.month())
			{
				BusinessDayConventions preceding{ "Preceding" };
				adjust(d, preceding);
			}

		}
	}
	else if (c.getBusDayConvention() == businessDayConventions::PRECEDING ||
		c.getBusDayConvention() == businessDayConventions::MODIFIED_PRECEDING)
	{
		while (isHoliday(d1))
			d1 -= days(1);
		if (c.getBusDayConvention() == businessDayConventions::MODIFIED_PRECEDING)
		{
			if (d1.month() != d.month())
			{
				BusinessDayConventions following{ "Following" };
				adjust(d, following);
			}

		}
	}

	return d1;
}



date HolidayCalendar::easter(int year)
{
	int a{ year % 19 };
	int b{ year / 100 };
	int c{ year % 100 };
	int d{ b / 4 };
	int e{ b % 4 };
	int f{ (b + 8) / 25 };
	int g{ (b - f + 1) / 3 };
	int h{ (19 * a + b - d - g + 15) % 30 };
	int i{ c / 4 };
	int k{ c % 4 };
	int l{ (32 + 2 * e + 2 * i - h - k) % 7 };
	int m{ (a + 11 * h + 22 * l) / 451 };
	int month{ (h + l - 7 * m + 114) / 31 };
	int day{ ((h + l - 7 * m + 114) % 31) + 1 };
	return date{ static_cast<year_type>(year), static_cast<month_type>(month), static_cast<day_type>(day) };
}
#endif