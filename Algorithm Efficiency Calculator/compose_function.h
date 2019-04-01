//------------------------------------------------------------------------------

struct Compose_fct {
    Line_data buffer;

    Compose_fct(const Line_data& line) : buffer{line} {}

    Compose_fct(const Compose_fct& cf);
    Compose_fct& operator=(const Compose_fct& cf);

    double expression(double);
    double term(double);
    double primary(double);
};

//------------------------------------------------------------------------------

int solve(int lower, int upper, const Compose_fct& f);

//------------------------------------------------------------------------------
