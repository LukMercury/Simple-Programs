//------------------------------------------------------------------------------

#include "std_lib_facilities.h"
#include "line_data.h"

//------------------------------------------------------------------------------
// Parse input line into Line_data with Tokens

Line_data parse(const string& s)
{
    istringstream is {s};
    Line_data data;
    char ch;
    int row_count {0};
    bool has_variable {false};

    while (is >> ch) {
        switch(ch) {
            // operators
            case '(':
            case ')':
            case '+':
            case '-':
            case '*':
            case '/':
            case ',':
            {
                data.expr.push_back(Token(ch));
                break;
            }
            // whitespaces
            case ' ': case '\t':
                break;
            // numbers
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
            {
                is.putback(ch);
                double val;
                is >> val;
                data.expr.push_back(Token(number,val));
                break;
            }
            // read ref_value, lower, upper
            case ':':
            {
                ++row_count;
                if (row_count == 1) {
                    double var;
                    is >> var;
                    if (!is)
                        error("double value expected");
                    data.ref_value = var;
                }
                else if (row_count == 2) {
                    int var;
                    is >> var;
                    if (!is)
                        error("integer value expected");
                    data.lower = var;
                }
                else if (row_count == 3) {
                    int var;
                    is >> var;
                    if (!is)
                        error("integer value expected");
                    data.upper = var;
                }
                else
                    error("Wrong line format");
                break;
            }
            // "x"
            case variable:
            {
                // check if character "x" is part of a function name
                char temp;
                is >> temp;
                if (is && !isalpha(temp)) {
                    is.putback(temp);
                    data.expr.push_back(Token(variable));
                    has_variable = true;
                    break;
                }
                if (is && isalpha(temp)) {
                    is.putback(temp);
                    // no break, move to "default:"
                }
            }
            // now ch==x is no longer a possibility
            // function names
            default:
            {
                if (isalpha(ch)) {
                    string fct {ch};
                    while ((is >> ch) && isalnum(ch))   // ex: log10
                        fct.push_back(ch);
                    data.expr.push_back(Token(function_name, fct));
                    if (is)
                        is.putback(ch);
                }
                else
                    error("Bad character");
            }
        } // end switch
    }   // end while

    if (row_count != 3)
        error("Wrong line format");
    if (!has_variable)
        error("Function contains no variable");
    return data;
}

//------------------------------------------------------------------------------
