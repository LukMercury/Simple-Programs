//------------------------------------------------------------------------------

#include "std_lib_facilities.h"
#include "line_data.h"

//------------------------------------------------------------------------------
// Match function by name: one argument

double match_function(const string& s, double x)
{
    if (s == "log")
        return log(x);
    else if (s == "log2")
        return log2(x);
    else if (s == "log10")
        return log10(x);
    else if (s == "sqrt")
        return sqrt(x);
    else
        error("Unknown function", s);
}

//------------------------------------------------------------------------------
// Match function by name: two arguments

double match_function_2arg(const string& s, double x1, double x2)
{
    if (s == "pow")
        return pow(x1, x2);
    else
        error("Unknown function", s);
}

//------------------------------------------------------------------------------

Token Line_data::get()
{
    if (curr == expr.end())
        return (Token(line_end));
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

Line_data buffer; // lines will be read to this "buffer"

//------------------------------------------------------------------------------
// Deal with numbers, variables, parantheses and functions

double primary(double x)
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

double term(double x)
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

double expression(double x)
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
