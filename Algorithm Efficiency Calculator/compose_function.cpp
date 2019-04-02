//------------------------------------------------------------------------------

#include "std_lib_facilities.h"
#include "line_data.h"
#include "compose_function.h"
#include "functions.h"

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
            // one-variable functions
            if (t.kind == ')') {
                return match_function(fname,d);
            }
            // two-variable functions
            else if (t.kind == ',') {
                double d2 = expression(x);
                t = buffer.get();
                if (t.kind != ')')
                    error("')' expected");
                return match_function_2arg(fname,d,d2);
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
            error("Unexpected ending to function");
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

bool operator==(double x, Compose_fct f) {
    if ((f.expression(x) <= f.buffer.ref_value) &&
        (f.expression(x+1) > f.buffer.ref_value))
        return true;
    else
        return false;
}

bool operator>(double x, Compose_fct f) {
    return f.expression(x) > f.buffer.ref_value;
}

//------------------------------------------------------------------------------
// Binary search

int solve(const Compose_fct& f) {
    int lower {f.buffer.lower};
    int upper {f.buffer.upper};
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
