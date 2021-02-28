#!/bin/bash

vagrant cloud publish edi33416/so-ubuntu-20.04 1.3.0 virtualbox so-ubuntu-20-04.box -d "Ubuntu 20.04 based on bento/ubuntu-20.04 and provisioned with Ansible" --version-description "Latest stable version" --release --short-description "SO VM"
