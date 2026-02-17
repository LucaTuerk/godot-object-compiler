#!/bin/sh

shopt -s globstar
ls ./src/**/*.{cpp,h} > ./.files.txt
ls ./tests/**/*.{cpp,h} > ./.testfiles.txt

clang-format -i --files=./.files.txt
clang-format -i --files=./.testfiles.txt

rm ./.files.txt
rm ./.testfiles.txt
