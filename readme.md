# data structure and algorithm

Copyright(c) 2015 Muggle Wei (mugglewei@gmail.com)

## Introduction

This repo contain some data structure and algorithm with benchmark write in c/c++. The purpose of this repo is to practise theory, can work and fast run is main target.

## Add dsaa lib
1. Copy dsaa to target directory. 
2. Copy text between === DSAA lib start and === DSAA lib end from CMakeLists.txt to target cmakelists.
3. Modify relative path in cmakelists.

## Build
Use cmake

## Auto build
The auto_build script need cmake and python 2.7.*, it will auto build and compile project.
Use auto_build_win.bat in windows, auto_build_unix.sh in unix, and remember change build project type in script dependent on different environment.