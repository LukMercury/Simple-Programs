//------------------------------------------------------------------------------

struct Compose_fct {
    Line_data buffer;

    Compose_fct(const Line_data& line) : buffer{line} {}

    double expression(double);
    double term(double);
    double primary(double);
};

//------------------------------------------------------------------------------

int solve(const Compose_fct& f);

//------------------------------------------------------------------------------
