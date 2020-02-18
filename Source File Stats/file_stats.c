/**
 * The program uses a grammar to parse a C or C++ source file and displays stats
 * File is read character by character
 * The character reads and stats are handeled on many levels:
 *      - text
 *      - line
 *      - word
 *      - per character
 *      - inside comments
 *      - inside quotes
 * Characters are read with getline(), character is sometimes passed as
 * an argument to the next level
 * To modify or add a specific stat, modify the function for the appropriate
 * level or levels.
 * Examples:
 *      1. Stats updated and checks made for each character regardless of
 *      location are done inside per_char().
 *      2. Stats pertaining to the whole text, like average characters per line
 *      are done inside text(), since the total characters and number of lines
 *      are inside the stats[] array (see main()).
 *      3. Sometimes you have to make changes on more than one level.
 */

#include <stdio.h>
#include <stdlib.h>

/**
 * Length of "stats" array and all connected arrays (stat names, etc.,
 * see main() function)
 */
#define NO_STATS 17

/**
 * Function declarations
 */
void text(double stats[], int len);
void line(char input_ch, double stats[], int len);
void per_char(char ch, double stats[], int len);
void per_char_in_comment(char ch, double stats[], int len);

void quote(char open_quote, double stats[], int len);

void comment(double stats[], int len);
void single_line_comment(double stats[], int len);
void multiline_comment(double stats[], int len);

void print(char *stat_names[], int show_stat[], int is_percentage[],
           double stats[], int len);

int main()
{
    /**
     * Names for stats
     * Entering a new stat name will result in a new stat being added. Note that
     * you also have to compute the stat
     */
    char * stat_names[] = {
            "characters",                   // 0
            "leading spaces",               // 1
            "lines",                        // 2
            "blank lines",                  // 3
            "blank line percentage",        // 4
            "characters after tab",         // 5
            "spaces after tab",             // 6
            "leading spaces after tab",     // 7
            "average characters per line",  // 8
            "average chars/line ignoring leading spaces",   // 9
            "leading spaces per line",      // 10
            "spaces",                       // 11
            "spaces per line ignoring leading spaces",      // 12
            "number of comments",           // 13
            "characters in comments",       // 14
            "comments percentage of total lines",           // 15
            "percentage of chars in comments to chars",     // 16
    };

    /**
     * Decide which stats are shown(1) and which not(0)
     */
    int show_stat[NO_STATS];
    for (int i = 0; i < NO_STATS; ++i) show_stat[i] = 1;
    show_stat[1] = 0;
    show_stat[11] = 0;

    /**
     * Set stats which are pecentages(1)
     */
    int is_percentage[NO_STATS] = {0};
    is_percentage[4] = 1;
    is_percentage[15] = 1;
    is_percentage[16] = 1;

    /**
     * Init stats
     */
    double stats[NO_STATS] = {0};

    /**
     * Start parsing
     */
    text(stats, NO_STATS);

    print(stat_names, show_stat, is_percentage, stats, NO_STATS);
}

/**
 * Handle quotes.
 * Doesn't count identifiers
 * @param open_quote
 * @param stats
 * @param len
 */
void quote(char open_quote, double stats[], int len)
{
    ++stats[0];
    char ch;
    char prev_ch = 0;
    while ((ch = getchar()) >= 0) {
        ++stats[0];
        switch (ch) {
            case '\'':
            case '"':
                if (ch == open_quote && prev_ch != '\\') {
                    return;
                }
                break;
            case ' ':
            case '\t':
                ++stats[5];
                ++stats[6];
                ++stats[11];
                break;
            case '\\':
                break;
        }
        prev_ch = ch;
    }
    printf("Error: Quote never closed. File won't compile.\n");
    exit(1);
}


/**
 * Process comments
 * Pass control to single_line_comment() or multiline_comment()
 */
void comment(double stats[], int len)
{
    char ch = getchar();
    ++stats[0];
    switch (ch) {
        case '/':
            ++stats[13];
            single_line_comment(stats, len);
            break;
        case '*':
            ++stats[13];
            multiline_comment(stats, len);
            break;
    }
}

/**
 * Look for closing of single line comments
 * and process stats separately from code
 * @param stats
 * @param len
 */
void single_line_comment(double stats[], int len)
{
    char ch;
    char prev_ch = 0;
    while ((ch = getchar()) != '\n') {
        per_char_in_comment(ch, stats, len);
        prev_ch = ch;
        continue;
    }
    if (prev_ch == '\\') single_line_comment(stats, len);
    ungetc(ch, stdin);
}

/**
 * Look for closing of multiline comments
 * and process stats separately from code
 * @param stats
 * @param len
 */
void multiline_comment(double stats[], int len)
{
    char ch;
    // while will leave an unprocessed '*' in "buffer". notation: '*'b
    while ((ch = getchar()) != '*') {
        if (ch < 0) {
            printf("Error: Multiline comment not closed.\n");
            exit(1);
        }
        per_char_in_comment(ch, stats, len);
        continue;
    }
    // '*'b (in "buffer") is skipped for now but processed below
    while ((ch = getchar()) == '*') {
        // '*'b is processed here if subsequent '*' are found and then '/' is found
        per_char_in_comment(ch, stats, len);
        continue;
    }
    if (ch == '/') return;
    else {
        // '*'b is processed if there was no comment close but just a sequence of '*'
        per_char_in_comment('*', stats, len);
        per_char_in_comment(ch, stats, len);
        multiline_comment(stats, len);
    }
    if (ch < 0) {
        printf("Error: Multiline comment not closed.\n");
        exit(1);
    }
}

/**
 * Checks and stats to be done for each char regadless of place
 * @param ch
 * @param stats
 * @param len
 */
void per_char(char ch, double stats[], int len)
{
    ++stats[0];                                         // characters
    if (ch == ' ' || ch == '\t') ++stats[11]; // spaces (including tabs)
    if (ch == '/') comment(stats, len);                 // comments
    if (ch == '"' || ch == '\'') quote(ch, stats, len); // quotes
}

/**
 * Checks and stats to be done for each char inside comments
 * @param ch
 * @param stats
 * @param len
 */
void per_char_in_comment(char ch, double stats[], int len)
{
    ++stats[0];
    ++stats[14];
    if (ch == '\n') ++stats[2];
}

/**
 * Compute stats at line level (leading tabs & spaces, etc)
 * Pass control and a character to word by word processing
 * @param input_ch
 * @param stats
 * @param len
 */
void line(char input_ch, double stats[], int len)
{
    per_char(input_ch, stats, NO_STATS);
    // 1. Deal with input_ch (passed from calling function)
    if (input_ch == '\n') {
        ++stats[2]; // lines
        ++stats[3]; // blank lines
        return;
    }
    // tab expansion
    int has_tab = 0;
    int true_line_start = 0;
    if (input_ch == '\t') {
        ++stats[1];     // leading spaces
        has_tab = 1;
    }
    // 2. Read rest of line
    char ch;
    while (ch = getchar(), ch >= 0 && ch != '\n') {
        per_char(ch, stats, NO_STATS);
        // tab expansion
        if (has_tab) {
            if (ch == ' ') ++stats[6];  // spaces after tab
            if (!true_line_start && (ch == ' ' || ch == '\t')) {
                ++stats[1]; // leading spaces
                ++stats[7]; // leading spaces after tab
            } // leading spaces & leading spaces after tab
            if (!true_line_start && ch != ' ' && ch != '\t') true_line_start = 1;
            if (true_line_start) ++stats[5];    // characters after tab
        }
    }
    ++stats[2]; // end of line
}

/**
 * Compute stats at text level (percentages, averages)
 * Pass control and a character to line by line processing
 * @param stats
 * @param len
 */
void text(double stats[], int len)
{
    char ch;
    while ((ch = getchar()) >= 0) {
        line(ch, stats, len);
    }
    // Compute stats. If global stats, always at the end of this function
    stats[4] = stats[3] / stats[2] * 100;   // blank line percentage
    stats[8] = stats[0] / stats[2];         // avg characters per line
    stats[9] = (stats[0] - stats[1]) / stats[2];    // avg chars/line ignoring \
                                                    leading spaces
    stats[10] = stats[1] / stats[2];        // leading spaces per line
    stats[12] = (stats[11] - stats[1]) / stats[2];  // spaces per line \
                                                    ignoring leading spaces
    stats[15] = stats[13] / stats[2] * 100;     // comments percentage of \
                                                   total lines
    stats[16] = stats[14] / stats[0] * 100;     // percentage of chars in \
                                                   comments to chars
}

/**
 * Controls the way the data is printed.
 * @param stat_names - in - array of stat names
 * @param show_stat - in - array of stats displayed(1) or not(0)
 * @param is_percentage - in - array of stats to be displayed as percentage(1)
 * @param stats - in - array of stat values
 * @param len - in - number of stats
 */
void print(char *stat_names[], int show_stat[], int is_percentage[],
           double stats[], int len)
{
    printf("\n");
    for (int i = 0; i < len; ++i) {
        if (show_stat[i]) {
            if (is_percentage[i]) {
                printf("%s: %.2lf%%\n", stat_names[i], stats[i]);
            } else if ((int) stats[i] == stats[i]) {
                printf("%s: %.0lf\n", stat_names[i], stats[i]);
            } else {
                printf("%s: %.2lf\n", stat_names[i], stats[i]);
            }
        }
    }
}
