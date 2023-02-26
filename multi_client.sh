#!/bin/bash

echo "Enter No of Clients: "

read N

for ((i=1;i<=N;i++))
do
	gnome-terminal -e "./client"
done
wait

