#include "pch.h"
#include "BusinessDayConventions.h"

BusinessDayConventions::BusinessDayConventions(string c)
{
	busDayConv = businessDayConventions::NO_ADJUST;

	if (c == "No Adjustment")
		busDayConv = businessDayConventions::NO_ADJUST;
	else if (c == "Following")
		busDayConv = businessDayConventions::FOLLOWING;
	else if (c == "Modified Following")
		busDayConv = businessDayConventions::MODIFIED_FOLLOWING;
	else if (c == "Preceding")
		busDayConv = businessDayConventions::PRECEDING;
	else if (c == "Modified Preceding")
		busDayConv = businessDayConventions::MODIFIED_PRECEDING;
}

businessDayConventions BusinessDayConventions::getBusDayConvention() const
{
	return busDayConv;
}

void BusinessDayConventions::setBusDayConvention(businessDayConventions c)
{
	busDayConv = c;
}