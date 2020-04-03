#!/bin/env python3

import os, sys

files = []

cwd = os.environ['PWD']

if len(sys.argv) == 0:
	sys.exit()

for file in sys.argv[1:]:
	if os.path.isfile(os.path.join(cwd, file)):
		files.append(file)

renamed = []

for file in files:
	renamed.append(file.upper())

dictionary  = dict(zip(files, renamed))

for file, renamed in dictionary.items():
	os.rename(os.path.join(cwd, file), os.path.join(cwd, renamed))
