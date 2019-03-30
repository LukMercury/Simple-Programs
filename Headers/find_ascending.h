//------------------------------------------------------------------------------
/*
You have an ascending function f(x) defined on [0,...)
and an integer n
You have to find the maximum integer x such that f(x) < n still holds

Solution: perform a binary search for f(x) == n on an interval
        where "==" means "largest return value smaller than"
        -1 means no solution

Useful for comparing algorithm speeds based on input size n
*/

/*
Example function class:

struct My_function {
    int n;
    double fct(int x) { return x/log10(x); }
};
*/

//------------------------------------------------------------------------------

namespace compare_alg {

//------------------------------------------------------------------------------
// Comparison operators for binary search

template<typename Result>
bool operator==(int x, Result r) {
    if ((r.fct(x) <= r.n) && (r.fct(x+1) > r.n))
        return true;
    else
        return false;
}

template<typename Result>
bool operator>(int x, Result r) {
    return r.fct(x) > r.n;
}

/*
template<typename Result>
bool operator<(int x, Result r) {
    return r.fct(x) < r.n;
}
*/

//------------------------------------------------------------------------------
// Binary search

// returns the sought for number x
template<typename Result>
int binary_search(int lower, int upper, Result r) {

    while (lower <= upper) {
        int middle = (lower + upper) / 2;
        if (middle == r)
            return middle;
        else if (middle > r)
            upper = middle - 1;
        else
            lower = middle + 1;
    }
    // -1 means value not found
    return -1;
}

//------------------------------------------------------------------------------

} // compare_alg

//------------------------------------------------------------------------------
