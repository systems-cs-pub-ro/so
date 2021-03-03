#!/bin/bash

vagrant cloud publish systems-cs-pub-ro/so 2.0.0 virtualbox so-ubuntu-20-04.box -d "Ubuntu 20.04 based on bento/ubuntu-20.04 and provisioned with Ansible" --version-description "Latest stable version" --release --short-description "SO VM"
