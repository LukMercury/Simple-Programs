//------------------------------------------------------------------------------

#include "std_lib_facilities.h"
#include "functions.h"

//------------------------------------------------------------------------------
// Custom functions
// <-- add additional functions here

// factorial
int fact(int n)   
{
    int res = 1;
    for (int i = 1; i <= n; ++i)
        res *= i;
    return res;
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
