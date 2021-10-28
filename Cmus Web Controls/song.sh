#!/bin/bash

echo "$(cmus-remote -C status | head -2 | tail -1 | tr -s '/' | cut -d' ' -f2- | cut -d'/' -f5-)" > song.txt
