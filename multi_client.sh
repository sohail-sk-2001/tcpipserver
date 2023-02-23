#!/bin/bash

for N in {1..10}
do
	gnome-terminal -e "./client"
done
wait

