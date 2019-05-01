#!/bin/bash

while read pass
do
#	echo "___"
	echo $pass
	echo $pass | ./bomb input | tail -n 2
done < strings
