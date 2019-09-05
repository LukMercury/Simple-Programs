#!/usr/bin/env python3

"""
    Run multiple executables (scripts) in separate threads to make full use
    of your processor cores
    Usage: run-multiple-threads.py [search pattern] [search directory]
"""

import os, fnmatch, subprocess, threading, sys

def find(pattern, path):
    result = []
    for root, dirs, files in os.walk(path):
        for name in files:
            if fnmatch.fnmatch(name, pattern):
                result.append(os.path.join(root, name))
    return result

scripts = find(sys.argv[1], sys.argv[2])

class RunExecutable(threading.Thread):
    def __init__(self, infile):
        threading.Thread.__init__(self)
        self.infile = infile

    def run(self):
        subprocess.call(self.infile)

threads = []
for script in scripts:
    threads.append(RunExecutable(script))

for thread in threads:
    thread.start()

for thread in threads:
    thread.join()
