#!/bin/bash

[ $# -eq 1 ] || exit

for i in $(seq 100 -1 $1); do
	[ -e "test$i".in ] || continue
	mv test$i.in test$(($i + 1)).in
	mv ../ref/test$i.ref ../ref/test$(($i + 1)).ref
done
