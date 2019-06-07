#!/bin/bash

while read pass
do
	echo "___"
	echo $pass
	echo $pass | ./bomb input
done < strings
