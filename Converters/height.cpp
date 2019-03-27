// Convert height in feet and inches to cm
// Usage: height [feet] [inches]

#include "std_lib_facilities.h"

constexpr double inches_per_foot = 12.0;
constexpr double cm_per_inch = 2.54;

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
        double feet {0};
        if (argc >= 2)
            feet = cstring_to_double(argv[1]);

        double inches {0};
        if (argc >= 3)
            inches = cstring_to_double(argv[2]);

        double cm = (feet * inches_per_foot + inches) * cm_per_inch;

        cout << fixed << setprecision(2);
        cout << cm << '\n';
}
catch(exception& e) {
    cout << e.what() << '\n';
}
catch(...) {
    cout << "Unknown error\n";
}
