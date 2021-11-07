#include "HolidayCalendar.h"

HolidayCalendar::HolidayCalendar() {
	//Default holiday calendar
	firstWeekendDay = greg_weekday {Saturday} ;
	secondWeekendDay = greg_weekday {Sunday};
	holidayCalendarId = HolidayCalendarId::CUST;
}


HolidayCalendar::HolidayCalendar(HolidayCalendarId id) : holidayCalendarId{ id } 
{
	generateCalendar();
}

HolidayCalendar::HolidayCalendar(std::vector<date> h, gregorian_calendar::day_of_week_type f, gregorian_calendar::day_of_week_type s, HolidayCalendarId id) : holidays{ h }, firstWeekendDay{ f }, secondWeekendDay{ s }, holidayCalendarId{id}
{
}

HolidayCalendar::HolidayCalendar(const HolidayCalendar& h) : HolidayCalendar{h.holidays, h.firstWeekendDay, h.secondWeekendDay, h.holidayCalendarId}
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
		for (int year{ 1950 }; year <= 2099; ++year)
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
	date firstOfTheMonth{ year, month, 1 };
	date result{ firstOfTheMonth };
	while (result.day_of_week() != dayOfWeek)
		result += days(1);

	return result;
}

date HolidayCalendar::lastInMonth(int year, int month, gregorian_calendar::day_of_week_type dayOfWeek)
{
	int eom_day{ gregorian_calendar::end_of_month_day(year, month) };
	date endOfMonth{ year, month, eom_day };
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
	date christmas{ year, 12, 25 };
	if (christmas.day_of_week() == Saturday || christmas.day_of_week() == Sunday)
	{
		return date{ year, 12, 27 };
	}
	return christmas;
}

date HolidayCalendar::boxingDayBumpedSatSun(int year)
{
	date boxingDay{ year, 12, 26 };
	if (boxingDay.day_of_week() == Saturday || boxingDay.day_of_week() == Sunday)
	{
		return date{ year, 12, 28 };
	}
	return boxingDay;
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

bool isSatSun(date d)
{
	return (d.day_of_week() == Saturday || d.day_of_week() == Sunday);
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
	return date{ year, month, day };
}