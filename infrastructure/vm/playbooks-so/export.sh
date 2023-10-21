#!/bin/bash

vboxmanage modifyvm  'so-vm' --uart1 off
vboxmanage export 'so-vm' --ovf10 -o so-ubuntu-22-04.ova
