# VM creation tutorial

Based on https://gitlab.com/adrian.stratulat/vm-scripts/

## Setting up the environment

Tested on Ubuntu 18.04.
Any reasonable recent Linux distribution should work.

    $ sudo apt-get install vagrant virtualbox virtualbox-qt python-pip python-setuptools
    $ pip install ansible
    $ vagrant plugin install vagrant-disksize
    $ vagrant plugin install vagrant-reload

## Building a VM

    $ cd vagrantfile-so
    $ vagrant up

Running the `vagrant up` command will create and configure the VM.

## Exporting the VM

Use the `playbooks-so/export.sh` script to get an `.ova` image.
