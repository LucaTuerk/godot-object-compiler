#!/bin/sh

shopt -s globstar
ls ./library/**/*.{cpp,h} > ./.libfiles.txt
ls ./application/**/*.{cpp,h} > ./.appfiles.txt
ls ./test_files/**/*.{cpp,h} > ./.testfiles.txt

clang-format -i --files=./.libfiles.txt
clang-format -i --files=./.appfiles.txt
clang-format -i --files=./.testfiles.txt

rm ./.libfiles.txt
rm ./.appfiles.txt
rm ./.testfiles.txt