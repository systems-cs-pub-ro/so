#!/bin/bash

echo "Must be only one process signals: "
echo $(pidof signals)

for i in $(seq 1 100); do
	echo "sending SIGINT number $i"
	kill -SIGINT  $(pidof signals)
done

