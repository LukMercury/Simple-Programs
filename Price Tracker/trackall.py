#!/usr/bin/env python3

# Script searches for folders in current folder and runs all .py files
# in them (no recurse subdirectories)
# Add folders to be excluded (no path, just folder names) to the .ignoredirs
# file, one per line

import os, subprocess, threading, sys

# Multithreading class
class RunExecutable(threading.Thread):
    def __init__(self, infile):
        threading.Thread.__init__(self)
        self.infile = infile

    def run(self):
        subprocess.call(self.infile)

# Reset notifications.html

# Basic files and directories used        
script_dir = os.path.dirname(os.path.realpath(__file__))
dirs = []
ignore_dirs = [script_dir, script_dir + '/_Generic', script_dir + '/_Test']
ignore_file_user = os.path.join(script_dir, '.ignoredirs')
notifications_file = os.path.join(script_dir, 'notifications.html')
open(notifications_file, 'w').close()

# Ignore directories excluded by user (.ignoredirs file)
try: 
	with open(ignore_file_user) as ignore_file:
		ignore_dirs_user = ignore_file.readlines()
	ignore_dirs_user = [x.strip() for x in ignore_dirs_user]

	for dir_user in ignore_dirs_user:
		ignore_dirs.append(script_dir + '/' + dir_user)
except:
	pass

# Get all subdirectories for tracker
for (dirname, dirpath, filenames) in os.walk(script_dir):
	if dirname not in ignore_dirs:
		dirs.append(dirname)

# Find all trackers in subdirectories
trackers = []
for dir in dirs:
	for (dirname, dirpath, filenames) in os.walk(dir):
		for file in filenames:
			if os.path.splitext(file)[-1] == '.py':
				trackers.append(dir + '/' + file)

# Run each tracker in a separate thread
threads = []
for tracker in trackers:
	threads.append(RunExecutable(tracker))

for thread in threads:
	thread.start()

for thread in threads:
	thread.join()
