//------------------------------------------------------------------------------

const char number = '8';
const char variable = 'x';
const char function_name = 'f';
const char line_end = 'e';

//------------------------------------------------------------------------------

struct Token {
    char kind;
    double value;
    string fct_name;
    Token(char ch) : kind{ch}, value{0} {}
    Token(char ch, double val) : kind{ch}, value{val} {}
    Token(char ch, const string& fn) : kind{ch}, value{0}, fct_name{fn} {}
};

//------------------------------------------------------------------------------

class Line_data {
public:
    vector<Token> expr;
    double ref_value;
    int lower;
    int upper;

    Line_data() : ref_value{0}, lower{0}, upper{0}
        { curr = expr.begin(); }

    Line_data(const Line_data& ld);
    Line_data& operator=(const Line_data& ld);

    Token get();
    void putback();
    void reset()
        { curr = expr.begin(); }
private:
    vector<Token>::iterator curr;
};

//------------------------------------------------------------------------------
