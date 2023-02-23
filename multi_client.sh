#!/bin/bash

for N in {1..5}
do
	gnome-terminal -e "./client"
done
wait

