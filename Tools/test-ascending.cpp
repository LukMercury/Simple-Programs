//------------------------------------------------------------------------------
/*
Being given an ascending function f(x) defined on [0,...)
and a double n
You have to find the maximum integer x such that f(x) < n still holds

Solution: perform a binary search for f(x) == n on an interval
        where "==" means "largest return value smaller than"
        -1 means no solution

Useful for computing algorithm speeds: input size x based on execute time n
and algorithm comparison: input size x such that alg1 is faster than alg2
*/

//------------------------------------------------------------------------------//------------------------------------------------------------------------------

#include "std_lib_facilities.h"
#include "find_ascending.h"

//------------------------------------------------------------------------------
// Define functions here or use lambdas inside main()
// Function type: double f(int)

double fact(int x) {
    return (x == 0) ? 1 : x*fact(x-1);
}

//------------------------------------------------------------------------------

int main()
try{
    // add functions to a vector and a search interval for each function
    vector<My_function> vf;
    vector<int> low;
    vector<int> up;

//------------------------------------------------------------------------------
// Edit to add functions to vector
// v.push_back(function, reference value);
// low.push_back(inverval lower limit);
// up.push_back(interval upper limit);

    vf.push_back(My_function([](int x) -> double { return x/log10(x); }, 8));
    low.push_back(0);
    up.push_back(100);

    int n = 1000000;     // ok, converts to double

    vf.push_back(My_function([](int x) -> double { return x; }, n));
    low.push_back(0);
    up.push_back(2000000);

    vf.push_back(My_function([](int x) -> double { return x*log10(x); }, n));
    low.push_back(0);
    up.push_back(1000000);

    vf.push_back(My_function([](int x) -> double { return x*x; }, n));
    low.push_back(0);
    up.push_back(2000);

    vf.push_back(My_function([](int x) -> double { return x*x*x; }, n));
    low.push_back(0);
    up.push_back(1000);

    vf.push_back(My_function([](int x) -> double { return pow(2,x); }, n));
    low.push_back(0);
    up.push_back(1000);

    vf.push_back(My_function(fact, n));
    low.push_back(0);
    up.push_back(100);


//------------------------------------------------------------------------------

    // must have equal numbers of functions and search intervals
    if ((vf.size() != low.size()) || (low.size() != up.size()))
        error("Missing data");

    // compute
    for (size_t i = 0; i < vf.size(); ++i)
        cout << compare_alg::binary_search(low[i], up[i], vf[i]) << '\n';

}
catch(exception& e) {
    cout << e.what() << '\n';
}
catch(...) {
    cout << "Unknown error\n";
}
