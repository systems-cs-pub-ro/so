#
# SO, 2010 - Lab #13, Recap
# Task #1, Linux
#
# COW, page demand, os-tool

BASE_DIR=./os-tool

all: build

get:
	wget http://swarm.cs.pub.ro/~cojocar/so/os-tool.zip
	unzip os-tool.zip

build: get
	# build module
	cd $(BASE_DIR)/fault && make
	# build examples
	cd $(BASE_DIR)/examples/fault && make
	cd $(BASE_DIR)/examples/fault2 && make

run:
	-sudo apt-get remove -y libc-dbg
	-sudo insmod $(BASE_DIR)/fault/os-mon.ko
	cd $(BASE_DIR)/examples && sudo python ../src/main.py

.PHONY: clean

clean:
	rm -rf os-tool*
	-sudo rmmod os-mon
