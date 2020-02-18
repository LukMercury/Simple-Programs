#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NO_STATS 13
#define WORD_LENGTH 50

void text(double stats[]);
void line(double stats[]);
void word(double stats[]);
void per_char(double stats[]);

void compute_text_stats(double stats[]);

void print(char *stat_names[], int show_stat[], int is_percentage[],
           double stats[], int len);

int regular_space(char ch);

enum {
    has_tab,
} flag_tags;


int flags[1] = {0};

/**
 * Enum of stat indices in stats[] array
 * For code readibility
 */
enum {
    chars,                                          // 0
    spaces,                                         // 1
    lead_spaces,                                    // 2
    lines,                                          // 3
    blank_lines,                                    // 4
    blank_percentage,                               // 5
    ch_after_tab,                                   // 6
    sp_after_tab,                                   // 7
    lead_sp_after_tab,                              // 8
    avg_chars_line,                                 // 9
    avg_chars_line_ign_lead_sp,                     // 10
    lead_sp_line,                                   // 11
    lead_sp_line_ign_lead_sp,                       // 12
} statistics;

int main()
{
    /**
     * Names for stats
     * Entering a new stat name will result in a new stat being added. Note that
     * you also have to compute the stat
     */
    char * stat_names[] = {
            "characters",                                   // 0
            "spaces",                                       // 1
            "leading spaces",                               // 2
            "lines",                                        // 3
            "blank lines",                                  // 4
            "blank line percentage",                        // 5
            "characters after tab",                         // 6
            "spaces after tab",                             // 7
            "leading spaces after tab",                     // 8
            "average characters per line",                  // 9
            "average chars/line ignoring leading spaces",   // 10
            "leading spaces per line",                      // 11
            "spaces per line ignoring leading spaces",      // 12
    };


    /**
     * Decide which stats are shown(1) and which not(0)
     */
    int show_stat[NO_STATS];
    for (int i = 0; i < NO_STATS; ++i) show_stat[i] = 1;
//    show_stat[spaces] = 0;
//    show_stat[lead_spaces] = 0;

    /**
     * Set stats which are pecentages(1)
     */
    int is_percentage[NO_STATS] = {0};
    is_percentage[blank_percentage] = 1;


    /**
     * Init stats
     */
    double stats[NO_STATS] = {0};

    /**
     * Start parsing
     */
    text(stats);

    print(stat_names, show_stat, is_percentage, stats, NO_STATS);
}

void text(double stats[])
{
    char ch;
    while ((ch = getchar()) >= 0) {
        ungetc(ch, stdin);
        line(stats);
    }
    compute_text_stats(stats);
}

void line(double stats[])
{
    ++stats[lines];
    char ch;
    flags[has_tab] = 0;
    /**
     * Check for leading tab
     */
    if ((ch = getchar()) >= 0) {
        if (ch == '\t') {
            ++stats[lead_spaces];
            ungetc(ch, stdin);
            per_char(stats);
            flags[has_tab] = 1;
        }
        else ungetc(ch, stdin);
    }
    else (ungetc, stdin);
    /**
     * Deal with leading spaces
     */
    while ((ch = getchar()) >= 0) {
        if (!regular_space(ch)) break;
        else {
            ++stats[lead_spaces];
            ++stats[lead_sp_after_tab];
            ungetc(ch, stdin);
            per_char(stats);
        }
    }
    /**
     * Deal with blank lines
     */
    if (ch < 0 || ch == '\n') ++stats[blank_lines];
    ungetc(ch, stdin);
    /**
     * Deal with the rest of the line
     */
    while ((ch = getchar()) >= 0 && ch != '\n') {
        ungetc(ch, stdin);
        word(stats);
    }
    ungetc(ch, stdin);
    per_char(stats);
}

void word(double stats[])
{
    per_char(stats);
}

void per_char(double stats[])
{
    char ch;
    if ((ch = getchar()) < 0) {
        ungetc(ch, stdin);
        return;
    }
    ++stats[chars];
    if (flags[has_tab]) ++stats[ch_after_tab];
    if (regular_space(ch)) {
        ++stats[spaces];
        if (flags[has_tab]) ++stats[sp_after_tab];
    }
}

void compute_text_stats(double stats[])
{
    if (stats[lines]) {
        stats[blank_percentage] = stats[blank_lines] / stats[lines] * 100;
        stats[avg_chars_line] = stats[chars] / stats[lines];
        stats[avg_chars_line_ign_lead_sp] = (stats[chars] - stats[lead_spaces])
                                            / stats[lines];
        stats[lead_sp_line] = stats[lead_spaces] / stats[lines];
        stats[lead_sp_line_ign_lead_sp] = (stats[spaces] - stats[lead_spaces])
                                          / stats[lines];
    }
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

int regular_space(char ch)
{
    if (ch == ' ' || ch == '\t') return 1;
    return 0;
}

