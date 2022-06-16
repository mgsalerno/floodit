#!/bin/bash

if [[ -z ${1} || -z ${2} || -z ${3} ]]
then
  echo "Usage: ${0} <size> <size> <colors> [<random_seed>]"
  exit
fi

if [ ! -f "../src/floodit" ]
then
  echo "Error: floodit not found"
  exit
fi

if [[ -z ${4} ]]
then
  ./generatemap ${1} ${2} ${3} > test-map
else
  ./generatemap ${1} ${2} ${3} ${4} > test-map
fi

../src/floodit < test-map
