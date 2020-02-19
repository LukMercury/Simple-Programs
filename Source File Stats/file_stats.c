#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NO_STATS 20
#define WORD_LENGTH 50

void text(double stats[]);
void line(double stats[]);
void word(double stats[]);
void identifier(double stats[]);
void not_identifier(double stats[]);
void per_char(double stats[]);
void quote(char quote_type, double stats[]);
void comment(double stats[]);
void single_line_comment(double stats[]);
void multiline_comment(double stats[]);

void compute_text_stats(double stats[]);

void print(char *stat_names[], int show_stat[], int is_percentage[],
           double stats[], int len);

int regular_space(char ch);
int part_of_word(char ch);
void toggle_escaped(char ch);

enum {
    has_tab, in_quote, in_scomment, in_mcomment, escaped, preprocessor
} flag_tags;


int flags[6] = {0};

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
    total_comments,                                 // 13
    ch_in_comments,                                 // 14
    com_to_lines_per,                               // 15
    ch_in_com_to_ch_per,                            // 16
    identifier_occurences,                          // 17
    ch_in_identifiers,                              // 18
    avg_identififer_length,                         // 19
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
            "comments",                                     // 13
            "characters in comments",                       // 14
            "comments to lines percentage",                 // 15
            "chars in comments to chars percentage",        // 16
            "identifier occurences",                        // 17
            "total characters in identifiers",              // 18
            "average identifier length",                    // 19
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
    is_percentage[com_to_lines_per] = 1;
    is_percentage[ch_in_com_to_ch_per] = 1;


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
    if (flags[in_scomment]) single_line_comment(stats);
    if (flags[in_mcomment]) multiline_comment(stats);
    /**
     * Handle preprocess statements differently
     */
    if ((ch = getchar()) >= 0 && ch == '#') flags[preprocessor] = 1;
    ungetc(ch, stdin);
    while ((ch = getchar()) >= 0 && ch != '\n') {
        ungetc(ch, stdin);
        if (ch == '/') comment(stats);
        else if (ch == '"' || ch == '\'') quote(ch, stats);
        else if (ispunct(ch) || regular_space(ch)) per_char(stats);
        else word(stats);
    }
    ungetc(ch, stdin);
    per_char(stats);
    flags[preprocessor] = 0;
}

void word(double stats[])
{
    char ch = getchar();
    ungetc(ch, stdin);
    if (isalpha(ch)) identifier(stats);
    else if (!part_of_word(ch)) per_char(stats);
    else not_identifier(stats);
}

void identifier(double stats[])
{
    if (flags[preprocessor]) {
        not_identifier(stats);
        return;
    }
    ++stats[identifier_occurences];
    char ch;
    while ((ch = getchar()) >= 0) {
        ungetc(ch, stdin);
        if (ch == '\n') return;
        if (part_of_word(ch)) {
            ++stats[ch_in_identifiers];
            per_char(stats);
        } else return;
    }
    ungetc(ch, stdin);
}

void not_identifier(double stats[])
{
    char ch;
    while ((ch = getchar()) >= 0) {
        ungetc(ch, stdin);
        if (ch == '\n') return;
        if (part_of_word(ch)) per_char(stats);
        else return;
    }
    ungetc(ch, stdin);
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
    if (flags[in_scomment] || flags[in_mcomment]) ++stats[ch_in_comments];
}

void quote(char quote_type, double stats[])
{
    per_char(stats);
    flags[in_quote] = 1;
    char ch;
    while ((ch = getchar()) >= 0) {
        ungetc(ch, stdin);
        if (ch == quote_type && !flags[escaped]) {
            flags[in_quote] = 0;
            per_char(stats);
            return;
        }
        toggle_escaped(ch);
        per_char(stats);
    }
    ungetc(ch, stdin);
}

void comment(double stats[])
{
    per_char(stats);
    char ch;
    if ((ch = getchar()) < 0 || ch == '\n') {
        ungetc(ch, stdin);
        return;
    }
    ungetc(ch, stdin);
    if (ch == '/') {
        ++stats[total_comments];
        per_char(stats);
        single_line_comment(stats);
    } else if (ch == '*') {
        ++stats[total_comments];
        multiline_comment(stats);
    } else per_char(stats);
}

void single_line_comment(double stats[])
{
    flags[in_scomment] = 1;
    char ch;
    while ((ch = getchar()) >= 0) {
        ungetc(ch, stdin);
        if (ch == '\n') {
            if (flags[escaped]) {
                flags[escaped] = 0;
                return;
            } else {
                flags[in_scomment] = 0;
                return;
            }
        }
        per_char(stats);
        toggle_escaped(ch);
    }
    ungetc(ch, stdin);
}

void multiline_comment(double stats[])
{
    flags[in_mcomment] = 1;
    char ch;
    while ((ch = getchar()) >= 0) {
        if (ch == '\n') {
            ungetc(ch, stdin);
            return;
        }
        if (ch == '*') {
            char next_ch;
            if ((next_ch = getchar()) < 0) {
                ungetc(next_ch, stdin);
                return;
            }
            if (next_ch == '/') {
                flags[in_mcomment] = 0;
                ungetc(next_ch, stdin);
                ungetc(ch, stdin);
                per_char(stats);
                per_char(stats);
                return;
            }
            ungetc(next_ch, stdin);
        }
    ungetc(ch, stdin);
    per_char(stats);
    }
    ungetc(ch, stdin);
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
        stats[com_to_lines_per] = stats[total_comments] / stats[lines] * 100;
    }
    if (stats[chars]) {
        stats[ch_in_com_to_ch_per] = stats[ch_in_comments] / stats[chars] * 100;
    }
    if (stats[identifier_occurences]) {
        stats[avg_identififer_length] = stats[ch_in_identifiers] /
                                        stats[identifier_occurences];
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

int part_of_word(char ch)
{
    if (regular_space(ch) || (ispunct(ch) && ch != '_'))
            return 0;
    return 1;
}

void toggle_escaped(char ch)
{
    if (!flags[escaped] && ch == '\\') {
        flags[escaped] = 1;
        return;
    }
    if (flags[escaped]) flags[escaped] = 0;
}
