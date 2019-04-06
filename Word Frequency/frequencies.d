#!/usr/bin/rdmd

// Simpler D program added for comparison
// Output is roughly the same
// The program is about 4 times slower due to the complicated regex
// For a simpler regex ("[ \t,.;:?]+") it still is 1.3 times slower
// Conclusion: regex is a lot slower than switch parsing

import std.stdio, std.string, std.algorithm, std.regex, std.uni;

void main() {
    // Compute counts
    uint[string] freqs;
    uint total_words;
    static r = ctRegex!"('s)*-*'*-*[ \\\\\\d\\[\\]{}()\t,.;:!?\"~`/+*$]+-*'*-*|'*-{2,}'*|'s$|^-*'+|'+-* +|'+-*$|^-+|-+ +|-+$";
    foreach (line; stdin.byLine()) {
        foreach (word; split(toLower(strip(line)), r)) {
            if (word.length) ++freqs[word.idup];
            ++total_words;
        }
    }
    // Summary
    double avg_freq = double(total_words)/freqs.length;
    writeln("Total number of words: ", total_words);
    writeln("Number of different words: ", freqs.length);
    writeln("Average word frequency: ", avg_freq);
    writeln("\n-----\n");
    // Print counts
    string[] words = freqs.keys;
    // Aphabetically
    sort!((a, b) { return a < b; }) (words);
    foreach (word; words) {
        writefln("%s %u", word, freqs[word]);
    }
    // Separator
    writeln("\n-----\n");
    // By frequency
    sort!((a, b) { return freqs[a] > freqs[b]; }) (words);
    foreach (word; words) {
        writefln("%s %u", word, freqs[word]);
    }
}
