#!/bin/bash

echo "Must be only one process signals: "
echo $(pidof signals)

for i in $(seq 1 100); do
	echo "Sending SIGRTMIN no $i"
	kill -SIGRTMIN  $(pidof signals)
done

kill -SIGQUIT $(pidof signals)
