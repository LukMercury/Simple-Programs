// Convert miles to kilometers
// Usage: milestokm <distance1> <distance2> ...

#include "std_lib_facilities.h"

constexpr double miles_to_km = 1.609344;

double cstring_to_double(char* s)
{
    istringstream is {s};
    double d;
    if (!(is >> d))
        error("Couldn't get double argument");
    return d;
}

int main(int argc, char* argv[])
try{
        cout << fixed << setprecision(2);
        
        for (int i = 1; i < argc; ++i) {
            double kilometers = miles_to_km * cstring_to_double(argv[i]);
            cout << kilometers << '\n';
        }
}
catch(exception& e) {
    cout << e.what() << '\n';
}
catch(...) {
    cout << "Unknown error\n";
}
