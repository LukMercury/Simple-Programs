#include "std_lib_facilities.h"
#include "find_ascending.h"
#include "my_function.h"
/* my_function.h:
struct My_function {
    double n { ref_value };
    double fct(int x) { return ...; }
};

int low {0};
int up {0};
*/

int main()
try{
    My_function f1;
    cout << compare_alg::binary_search(low, up, f1) << '\n';
}
catch(exception& e) {
    cout << e.what() << '\n';
}
catch(...) {
    cout << "Unknown error\n";
}
