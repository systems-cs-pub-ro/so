#!/bin/bash

vagrant cloud publish edi33416/so-ubuntu-18.04 1.1.0 virtualbox so-ubuntu-18-04.box -d "Ubuntu 18.04 based on bento/ubuntu-18.04 and provisioned with Ansible" --version-description "Latest stable version" --release --short-description "SO VM"
