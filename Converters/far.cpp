
// Convert height in cm to feet and inches
// Usage: cm <value1> <value2> ...

#include "std_lib_facilities.h"

constexpr int offset = -32;
constexpr double scale = .5556;

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
            double far = cstring_to_double(argv[i]);
            double cel = scale * (far + offset);

            cout << cel << " C\n";
        }
}
catch(exception& e) {
    cout << e.what() << '\n';
}
catch(...) {
    cout << "Unknown error\n";
}