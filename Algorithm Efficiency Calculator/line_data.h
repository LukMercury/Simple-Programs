//------------------------------------------------------------------------------

const char number = '8';
const char end = 'e';
const char variable = 'x';
const char function_name = 'f';

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
    
    Token get();
    void putback();
private:
    vector<Token>::iterator curr;

};

//------------------------------------------------------------------------------
