#!/bin/bash

cd ../nasty
sudo rmmod nasty.ko

make clean; make

sudo insmod nasty.ko
sudo chmod 666 /dev/nasty
