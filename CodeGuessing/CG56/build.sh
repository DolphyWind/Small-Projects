#!/usr/bin/fish

gcc parser.c -g -o parser
./parser $argv
