#!/bin/bash

#
# Script to automatically create git repo for SO assignments
#
# 2015, Operating Systems
#

# ----------------- General declarations and util functions ------------------ #
PRIVATE_TOKEN=""
REPO_ID=""
REPO_URL=""
SSH_KEY=""

check_and_install_requirements()
{
        # git, curl, ssh
        # if not present install them

        # show progress after each installed packet
        # if one packet fails to install, the script fails
        # exit if it fails
}

gitlab_authenticate()
{
        # reads user curs.cs credentials and returns PRIVATE_TOKEN
        # if something fails, exit the script with a suggestive error
}

check_ssh_key()
{
        # check if already exists a SSH key on this machine
        # if yes, obtain key and return true, else return false
}

generate_ssh_key()
{
        # function to generate SSH key if the user wants to use SSH, else
        # defaults into using HTTPS
}

setup_user_profile()
{
        # add SSH key to profile user
}

check_existing_assignment_repo()
{
        # this should check if there already is an l3-so-assignments repo
        # if there is, we only need to pull current homework
        # from github repo
        # We obtain REPO_ID, REPO_URL
        # returns true if already exists/false otherwise
}

create_so_assignment_repo()
{
        # creates entire repo
        # returns REPO_ID, REPO_URL
}

add_asist_so_assignment_group()
{
        # based on REPO_ID we add new project members
}

# -------------------------------- Run script -------------------------------- #

# First check requirements
check_and_install_requirements

PRIVATE_TOKEN="$(gitlab_authenticate)"

# check if the user wants to use SSH key
# prompt question:
# if yes:
#       check_ssh_key
#       if already exists
#               do setup_user_profile
#       else
#               do generate_ssh_key
#               do setup_user_profile
#       fi
# if no:
#       inform the user that add each push he/she will be asked for credentials
# OBS: be very verbose in the actions the script executes

check_existing_assignment_repo

# if yes:
#       check_asist_so_assignment_group
#       if no: add_asist_so_assignment_group
# if no:
#       create_so_assignment_repo
#       add_asist_so_assignment_group

# TODO: add other functionalities
# (e.g. pull from github repo, help, arguments, etc.)

exit 0
