// Convert height in cm to feet and inches
// Usage: cm <value1> <value2> ...

#include "std_lib_facilities.h"

constexpr int inches_per_foot = 12;
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
        cout << fixed << setprecision(2);
        
        for (int i = 1; i < argc; ++i) {
            double cm = cstring_to_double(argv[i]);
            double inches = cm / cm_per_inch;
            int feet = static_cast<int>(inches) / inches_per_foot;
            inches -= feet * inches_per_foot;

            if (feet)
                cout << feet << "' ";
            cout << inches << "''\n";
        }
}
catch(exception& e) {
    cout << e.what() << '\n';
}
catch(...) {
    cout << "Unknown error\n";
}
