#!/usr/bin/rdmd

// simpler D program added for comparison

import std.stdio, std.string, std.algorithm;

void main() {
    // Compute counts
    uint[string] freqs;
    uint total_words;
    foreach (line; stdin.byLine()) {
        foreach (word; split(strip(line))) {
            ++freqs[word.idup];
            ++total_words;
        }
    }
    // Summary
    double avg_freq = total_words/freqs.length;
    writeln("Total words: ", total_words);
    writeln("Different words: ", freqs.length);
    writeln("Average frequency: ", avg_freq);
    writeln("\n----\n");
    // Print counts
    string[] words = freqs.keys;
    // Aphabetically
    sort!((a, b) { return a < b; }) (words);
    foreach (word; words) {
        writefln("%s\t% u", word, freqs[word]);
    }
    // Separator
    writeln("\n----\n");
    // By frequency
    sort!((a, b) { return freqs[a] > freqs[b]; }) (words);
    foreach (word; words) {
        writefln("%6u\t%s", freqs[word], word);
    }
}
