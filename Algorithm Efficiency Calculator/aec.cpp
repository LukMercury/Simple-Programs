//------------------------------------------------------------------------------

#include "std_lib_facilities.h"
#include "line_data.h"
#include "parse.h"
#include "compose_function.h"
#include "functions.h"

//------------------------------------------------------------------------------
// get data from file or stdin

void get_data(vector<Line_data>& vd, istream& is)
{
    string raw_line;
    while (getline(is, raw_line)) {
        Line_data line = parse(raw_line);
        vd.push_back(line);
    }
}

//------------------------------------------------------------------------------

int main(int argc, char* argv[])
try{
    // read from standard input
    if (argc == 1) {
        // compute line by line
        while (cin) {
            cout << "> ";
            string raw_line;
            getline(cin, raw_line);
            if (!cin)
                return 0;
            Line_data line = parse(raw_line);
            Compose_fct line_fct(line);
            cout << solve(line_fct.buffer.lower,
                line_fct.buffer.upper, line_fct) << '\n';
        }
    }

    // read from file
    else if (argc == 2) {
        string input_file {string(argv[1])};
        ifstream is {input_file};
        // save lines from file into a vector
        vector<Line_data> vlines;
        get_data(vlines, is);

        // compute
        for (Line_data line : vlines) {
            Compose_fct line_fct(line);
            cout << solve(line_fct.buffer.lower,
                line_fct.buffer.upper, line_fct) << '\n';
        }
    }

    else
        error("Usage: aec [input_file]");

}
catch(exception& e)
{
    cout << e.what() << '\n';
}
catch(...)
{
    cout << "Unknown error\n";
}
