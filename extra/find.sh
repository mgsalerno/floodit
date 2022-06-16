#!/bin/bash

if [[ -z ${1} || -z ${2} || -z ${3} ]]
then
  echo "Usage: ${0} <size> <size> <colors>"
  exit
fi

if [ ! -f "../src/floodit" ]
then
  echo "Error: floodit not found"
  exit
fi

echo "Running tests with ${1}x${2} map size and ${3} colors"

val=0

while [ ${val} -eq 0 ]
do
  if [[ -z ${4} ]]
  then
    ./generatemap ${1} ${2} ${3} > map-found
  else
    ./generatemap ${1} ${2} ${3} ${4} > map-found
  fi

  ../src/floodit < map-found > map-result
  cat map-found map-result | ./verify
  val=$(echo ${?})
done
