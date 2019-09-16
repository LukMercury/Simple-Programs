//------------------------------------------------------------------------------
// A program I made to help me with text analysis
// Input: a text file containing some text
// Output: word frequency sorted alphabetically & by frequency (descending)


#include "std_lib_facilities.h"

//------------------------------------------------------------------------------

// for output
const string separator = " ";
const string separator_line = "\n-----\n\n";
const string default_output_file = "wfout.txt";

//------------------------------------------------------------------------------

// all words lowercase
void lowercase(string& s)
{
    for (size_t i = 0; i < s.size(); ++i)
        s[i] = tolower(s[i]);
}

//------------------------------------------------------------------------------

// buffer for text parsing
struct Buffer {
    bool full = false;
    string bufferData;
    string get() { full = false; return bufferData; }
    void putback(const string& s, int i);
};

void Buffer::putback(const string& s, int i)
{
    full = true;
    bufferData = s.substr(i);
}

Buffer buffer;

//------------------------------------------------------------------------------

// parse text
// word can become an empty string, so test for that
// no numbers because they cram output
bool get_word(string& word, istream& is)
{
    string s, temp;
    if (buffer.full)
        s = buffer.get();
    else
        is >> s;

    if (!is)
        return false;

    for (size_t i = 0; i < s.size(); ++i) {
        switch (s[i]) {
            // separate words from special characters
            case '(':
            case ')':
            case '[':
            case ']':
            case '{':
            case '}':
            case '"':
            case '.':
            case ',':
            case ':':
            case ';':
            case '!':
            case '?':
            case '~':
            case '`':
            case '^':
            case '+':
            case '*':
            case '$':
            case '<':
            case '>':
            case '/':
            case '\\':
            // _ is ok: underscores can be part of names
            // & is ok: R&D, etc
            {
                if (i+1 != s.size())
                    buffer.putback(s,i+1);
                word = temp;
                return true;
            }
            // special case: "#"
            case '#':
            {
                // treat tags as separate words
                if ((i == 0) && (i+1 != s.size()) && isalpha(s[i+1]))
                    temp = temp + s[i];
                else {
                    if (i+1 != s.size())
                        buffer.putback(s,i+1);
                    word = temp;
                    return true;
                }
                break;
            }
            // special case: "-"
            case '-':
            {
                // ignore leading "-"
                if (i == 0)
                {
                    if (i+1 != s.size())
                        buffer.putback(s,i+1);
                    word = temp;
                    return true;
                }
                // add regular hyphens
                else if ((i+1 != s.size()) && isalpha(s[i+1]))
                    temp = temp + s[i];

                // series of "-"
                if ((i+1 != s.size()) && (s[i+1] == '-')) {
                    while ((i+1 != s.size()) && (s[i+1] == '-'))
                        ++i;
                    if (i+1 != s.size())
                        buffer.putback(s,i+1);
                    word = temp;
                    return true;
                }
                break;
            }
            // special case: apostrophe
            case '\'':
            {
                // ignore leading apostrophe
                if (i == 0)
                {
                    if (i+1 != s.size())
                        buffer.putback(s,i+1);
                    word = temp;
                    return true;
                }
                // ignore genitives
                else if ((i+1 != s.size()) &&
                        ((s[i+1] == 's') || (s[i+1] == 'S')))
                    ++i;
                // include all the rest (won't, don't, etc)
                else if ((i+1 != s.size()) && isalpha(s[i+1]))
                    temp = temp + s[i];
                break;
            }
            default:
            {
                // no numbers
                if (isdigit(s[i])) {
                    // remove numbers efficiently
                    while ((i+1 != s.size()) && isdigit(s[i+1]))
                        ++i;
                    if (i+1 != s.size())
                        buffer.putback(s,i+1);
                    word = temp;
                    return true;
                }
                else
                    temp = temp + s[i];
            }
        }
    }

    word = temp;
    return true;
}

//------------------------------------------------------------------------------

// template for creating map: sorted alphabetically
template<typename A, typename B>
map<A,B> copy_to_map(const unordered_map<A,B>& src)
{
    map<A,B> dst;
    copy(src.begin(), src.end(), inserter(dst, dst.begin()));
    return dst;
}

//------------------------------------------------------------------------------

// template for creating multimap: sorted by frequency
template<typename A, typename B>
pair<B,A> flip_pair(const pair<A,B>& p)
{
    return pair<B,A>(p.second, p.first);
}

template<typename A, typename B>
multimap<B,A> flip_umap(const unordered_map<A,B>& src)
{
    multimap<B,A> dst;
    transform(src.begin(), src.end(), inserter(dst, dst.begin()),
        flip_pair<A,B>);
    return dst;
}


//------------------------------------------------------------------------------

// output sorted alphabetically
ostream& operator<<(ostream& os, const map<string,int>& m)
{
    for (auto it = m.begin(); it != m.end(); ++it)
        os << it->first << separator << it->second << '\n';
    return os;
}

//------------------------------------------------------------------------------

// output sorted by frequency (descending)
ostream& operator<<(ostream& os, const multimap<int,string>& mm)
{
    for (auto it = mm.rbegin(); it != mm.rend(); ++it)
        os << it->second << separator << it->first << '\n';
    return os;
}

//------------------------------------------------------------------------------

int main(int argc, char* argv[])
try{
    // get input and output file names
    string input_file, output_file;

    // optional: run with parameters
    if (argc == 1) {
        cout << "read from file: ";
        cin >> input_file;
        cout << "write to file: ";
        cin >> output_file;
    }
    else if (argc == 2) {
        input_file = string(argv[1]);
        output_file = default_output_file;
    }
    else if (argc == 3) {
        input_file = string(argv[1]);
        output_file = string(argv[2]);
    }
    else
        error("Usage: word-frequency [input_file] [output_file]");

    ifstream is {input_file};
    if (!is)
        error("Couldn't open input file");

    if (output_file.size() == 0)
        output_file = default_output_file;
    ofstream os {output_file};

    // create unordered_map, count words
    unordered_map<string,int> wordcount;

    string word;
    int total_words {0};

    while (get_word(word, is)) {
        if (word.size() != 0) {
            lowercase(word);
            ++wordcount[word];
            ++total_words;
        }
    }

    // calculate average word frequency
    double average_frequency {0};
    if (wordcount.size() != 0)
        average_frequency = double(total_words)/wordcount.size();

    // create map (sorted alphabetically)
    map<string,int> alphabetical {copy_to_map(wordcount)};

    // create multimap (sorted by frequency)
    multimap<int,string> frequencies {flip_umap(wordcount)};

    // summary
    os << "Total number of words: " << total_words << '\n';
    os << "Number of different words: " << wordcount.size() << '\n';
    os << "Average word frequency: " << average_frequency << '\n';
    // separator line
    os << separator_line;
    // output sorted alphabetically
    os << alphabetical;
    // separator line
    os << separator_line;
    // output sorted by frequency (descending)
    os << frequencies;
}
catch(runtime_error& e)
{
    cout << e.what() << '\n';
}
catch(...)
{
    cout << "Unknown error\n";
}

//------------------------------------------------------------------------------
