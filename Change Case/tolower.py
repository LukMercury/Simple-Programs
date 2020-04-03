#!/bin/env python3

import os, sys

files = []

for file in sys.argv[:]:
	if os.path.isfile(file):
		files.append(file)

renamed = []

for file in files:
	renamed.append(file.lower())

dictionary  = dict(zip(files, renamed))

for file, renamed in dictionary.items():
	os.rename(file, renamed)
