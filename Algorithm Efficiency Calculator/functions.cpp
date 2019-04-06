//------------------------------------------------------------------------------

#include "std_lib_facilities.h"
#include "functions.h"

//------------------------------------------------------------------------------
// Custom functions
// <-- add additional functions here

// for factorial: tail call elimination
int tail_fact(int n, int result)
{
    return (n <= 1)
    ? result
    : tail_fact(n-1, n*result);
}

// factorial
double fact(double n)   // function might get a double argument
                        // when resolving expression
{
    int i = static_cast<int>(n);
    int result = 1;
    return tail_fact(i, 1);
}


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
    else if (s == "fact")
        return fact(x);
    // add each new function here too ...
    else
        error("Unknown function (one arg): ", s);
}

//------------------------------------------------------------------------------
// Match function by name: two arguments

double match_function_2arg(const string& s, double x1, double x2)
{
    if (s == "pow")
        return pow(x1, x2);
    // ... or here
    else
        error("Unknown function (two args): ", s);
}

//------------------------------------------------------------------------------
