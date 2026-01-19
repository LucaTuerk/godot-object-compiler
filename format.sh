#!/bin/sh

shopt -s globstar
ls ./library/**/*.{cpp,h} > ./.libfiles.txt
ls ./application/**/*.{cpp,h} > ./.appfiles.txt

clang-format -i --files=./.libfiles.txt
clang-format -i --files=./.appfiles.txt

rm ./.libfiles.txt
rm ./.appfiles.txt