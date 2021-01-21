#!/bin/bash

FILE_AGG="my_big_file_agg.txt"
FILE="my_big_file.txt"
i=0

while [ $i -lt 10000 ];
do
    echo 'My cool text' >> $FILE_AGG
    i=$(expr $i + 1)
done

i=0

while [ $i -lt 100 ];
do
    cat $FILE_AGG >> $FILE
    i=$(expr $i + 1)
done

