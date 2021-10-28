#!/bin/bash

echo "~/$(cmus-remote -C status | head -2 | tail -1 | cut -d' ' -f2- | cut -d'/' -f4-)" > song.txt
