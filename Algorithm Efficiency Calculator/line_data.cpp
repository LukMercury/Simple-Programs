//------------------------------------------------------------------------------

#include "std_lib_facilities.h"
#include "line_data.h"

//------------------------------------------------------------------------------

Token Line_data::get()
{
    if (curr == expr.end()) {
        return (Token(line_end));
    }
    Token t = *curr;
    ++curr;
    return t;
}

//------------------------------------------------------------------------------

void Line_data::putback()
{
    if (curr == expr.begin())
        error("Nothing left to putback()");
    --curr;
}

//------------------------------------------------------------------------------

Line_data::Line_data(const Line_data& ld)
    : expr{ld.expr}, ref_value{ld.ref_value},
    lower{ld.lower}, upper{ld.upper}
{
    curr = expr.begin();
}

//------------------------------------------------------------------------------

Line_data& Line_data::operator=(const Line_data& ld)
{
    if (this == &ld) return *this;
    expr = ld.expr;
    ref_value = ld.ref_value;
    lower = ld.lower;
    upper = ld.upper;
    curr = expr.begin();
    return *this;
}

//------------------------------------------------------------------------------
