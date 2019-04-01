//------------------------------------------------------------------------------

#include "std_lib_facilities.h"
#include "line_data.h"
#include "compose_function.h"
#include "functions.h"

//------------------------------------------------------------------------------

Compose_fct::Compose_fct(const Compose_fct& cf)
    : buffer{cf.buffer}
{
}

//------------------------------------------------------------------------------

Compose_fct& Compose_fct::operator=(const Compose_fct& cf)
{
    if (this == &cf)
        return *this;
    buffer = cf.buffer;
    return *this;
}

//------------------------------------------------------------------------------
// Deal with numbers, variables, parantheses and functions

double Compose_fct::primary(double x)
{
    Token t = buffer.get();
    switch (t.kind) {
        case '(':
        {
            double d = expression(x);
            t = buffer.get();
            if (t.kind != ')')
                error("')' expected");
            return d;
        }
        case function_name:
        {
            // remember function name
            string fname = t.fct_name;
            // '(' should follow after function name
            t = buffer.get();
            if (t.kind != '(')
                error("'(' expected");
            // (first) function argument
            double d = expression(x);
            // ')' or ','
            t = buffer.get();
            // two-variable functions
            if (t.kind == ',') {
                double d2 = expression(x);
                t = buffer.get();
                if (t.kind != ')')
                    error("')' expected");
                return match_function_2arg(fname,d,d2);
            }
            // one-variable functions
            else if (t.kind == ')') {
                return match_function(fname,d);
            }
            else
                error("')' or ',' expected");
        } // function name
        case number:
            return t.value;
        case variable:
            return x;
        case '-':
            return -primary(x);
        case '+':
            return primary(x);
        case line_end:
            error("Unexpended ending to function");
        default:
            error("Primary expected");
    } // switch
}

//------------------------------------------------------------------------------
// Deal with *, /

double Compose_fct::term(double x)
{
    double left = primary(x);
    Token t = buffer.get();

    while (true) {
        switch (t.kind) {
            case '*':
                left *= primary(x);
                t = buffer.get();
                break;
            case '/':
            {
                double d = primary(x);
                if (d == 0)
                    error("Divide by zero");
                left /= d;
                t = buffer.get();
                break;
            }
            case line_end:
                return left;
            default:
                buffer.putback();
                return left;
        }
    }
}

//------------------------------------------------------------------------------
// Deal with + and -

double Compose_fct::expression(double x)
{
    double left = term(x);
    Token t = buffer.get();

    while (true) {
        switch (t.kind) {
            case '+':
                left += term(x);
                t = buffer.get();
                break;
            case '-':
                left -= term(x);
                t = buffer.get();
                break;
            case line_end:
                return left;
            default:
                buffer.putback();
                return left;
        }
    }
}

//------------------------------------------------------------------------------
// Custom operators for binary search

bool operator==(double x, const Compose_fct& fct) {
    // expression(x) can resolve buffer only once
    Compose_fct f1 = fct;
    Compose_fct f2 = fct;
    if ((f1.expression(x) <= f1.buffer.ref_value) &&
        (f2.expression(x+1) > f2.buffer.ref_value))
        return true;
    else
        return false;
}

bool operator>(double x, const Compose_fct& fct) {
    Compose_fct f = fct;
    return f.expression(x) > f.buffer.ref_value;
}

//------------------------------------------------------------------------------
// Binary search

int solve(int lower, int upper, const Compose_fct& f) {

    while (lower <= upper) {
        int middle = (lower + upper) / 2;
        if (middle == f)
            return middle;
        else if (middle > f)
            upper = middle - 1;
        else
            lower = middle + 1;
    }
    // -1 means value not found
    return -1;
}

//------------------------------------------------------------------------------
