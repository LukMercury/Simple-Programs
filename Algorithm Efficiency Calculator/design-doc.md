# Algorithm efficiency calculator



---



[TOC]

---



##### Input

>intput.txt -> line:
>
>function(x) : ref_value : lower : upper
>
>x*log(x) : 1000000 : 0 : 1000000
>
>x*x : 1000000 : 0 : 2000
>
>>  feature: also read from standard input



---



#####  1. Parsing  

- [x] 

- read from file using getline()

- `Line_data parse(line)` - use `istringstream`

- insert tokens into a vector up until first":"

- save ref_number after ":" followd by ":" lower ":" upper

- make struct Line_data with all this

- error if no "x" is read

  

##### 2. Composing function

- [x] 

- now use the tokens from the vector

- define a grammar

- replace "x" with current number

- probably `double expression(int x)`; `double term(int x)`; `double primary(int x)` use tokens from vector from Line_data

- `double expression(int x)` uses `Line_data.get()` and `Line_data.putback()`

- Line_data has an implicit "Buffer" since it can manipulate indices

- `find_ascending.h` can be used, call `double expression(int x)`

  

##### 3. Input loop

- [x] 

```c++
struct Token {
    char type;
    double value;
    string fname;
    Token(char);
    Token(char, double);
    Token(char, const string&);
};

class Line_data {
public:
    vector<Token> expr;
    double ref_value;
    int lower;
    int upper;
    Token get(); // retuns 
    void putback();
private:
    vector<Token>::iterator curr;
};

vector<Line_data> vl
while (getline(is, line)) {
	Line_data data = parse(line);
    vl.push_back(data);
}
```



##### 4. Solve loop

- [x] 

```c++
for (Line_data data : vline) {
    buffer = data; // expression() uses bufffer
    My_function function {expression, buffer.ref_value};
    os <<c compare_alg::binary_search(buffer.lower, buffer.upper, function));
}
```



##### 5. Extra features

- [x] read from standard input
- [ ] custom functions `functions.h` - would require recompile when adding new function



##### Types

```c++
double ref_value;
int lower;
int upper;
double (*function)(int); 
double expression(int x);
double term(int x);
double primary(int x);
Line_data parse(string& s);
```



##### #include

`std_lib_facilities.h`

`find_ascending.h` - had to give that up

---



[TOC]

---

