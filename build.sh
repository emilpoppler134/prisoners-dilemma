#!/bin/bash
mkdir -p bin
eval cc src/main.c -o bin/PrisonersDilemma

./bin/PrisonersDilemma
