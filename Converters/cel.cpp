
// Convert height in cm to feet and inches
// Usage: cm <value1> <value2> ...

#include "std_lib_facilities.h"

constexpr int offset = 32;
constexpr double scale = 1.8;

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
            double cel = cstring_to_double(argv[i]);
            double far = scale * cel + offset;

            cout << far << " F\n";
        }
}
catch(exception& e) {
    cout << e.what() << '\n';
}
catch(...) {
    cout << "Unknown error\n";
}