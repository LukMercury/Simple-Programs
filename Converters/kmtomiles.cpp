// Convert kilometers to miles
// Usage: kmtomiles <distance1> <distance2> ...

#include "std_lib_facilities.h"

double km_to_miles = 0.6213712;

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
        double miles {0};
        cout << fixed << setprecision(2);
        for (int i = 1; i < argc; ++i) {
            miles = km_to_miles * cstring_to_double(argv[i]);
            cout << miles << '\n';
        }
}
catch(exception& e) {
    cout << e.what() << '\n';
}
catch(...) {
    cout << "Unknown error\n";
}
