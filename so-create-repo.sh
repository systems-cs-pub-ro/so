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
GITLAB_API_URL="https://gitlab.cs.pub.ro/api/v3"
SO_ASSIGNMENTS_URL="https://github.com/systems-cs-pub-ro/so-assignments.git"

check_and_install_requirements()
{
        # git, curl, ssh, jq
        # if not present install them

        # show progress after each installed packet
        # if one packet fails to install, the script fails
        # exit if it fails
        :
}

gitlab_authenticate()
{
        echo "Connecting to GitLab ..."

        # read user curs.cs credentials
        read -p "username: " username
        read -s -p "password: " password; echo

        # create session
        session=$(curl -sS "$GITLAB_API_URL/session"    \
                --data-urlencode "login=$username"      \
                --data-urlencode "password=$password")
        if [ $? -ne 0 ]; then
                echo "internal error: Could not create session ... exiting"
                exit 1
        fi

        # get private token
        PRIVATE_TOKEN=$(echo $session | jq -r '.private_token')
        if [ "$PRIVATE_TOKEN" == "null" ]; then
                echo "error: Authentication failed ... exiting"
                exit 1
        fi

        # get name
        name=$(echo $session | jq -r '.name')

        echo "Hi, $name! You have successfully authenticated!"
}

check_ssh_key()
{
        # check if already exists a SSH key on this machine
        # if yes, obtain key and return true, else return false
        :
}

generate_ssh_key()
{
        # function to generate SSH key if the user wants to use SSH, else
        # defaults into using HTTPS
        :
}

setup_user_profile()
{
        echo "Adding SSH key ..."

        # read SSH key title
        read -p "Please provide a title for the SSH key: " title

        # add SSH key to user profile
        res=$(curl -sS -H "PRIVATE-TOKEN: $PRIVATE_TOKEN"   \
                "$GITLAB_API_URL/user/keys"                 \
                --data-urlencode "title=$title"             \
                --data-urlencode "key=$SSH_KEY")
        if [ $? -ne 0 ]; then
                echo "internal error: Could not add SSH key ... exiting"
                exit 1
        fi

        # get key ID
        id=$(echo $res | jq -r '.id')
        if [ "$id" == "null" ]; then
                echo "error: Adding SSH key failed ... exiting"
                exit 1
        else
                echo "You have successfully added your SSH key!"
        fi
}

check_existing_assignment_repo()
{
        # this should check if there already is an l3-so-assignments repo
        # if there is, we only need to pull current homework
        # from github repo
        # We obtain REPO_ID, REPO_URL
        # returns true if already exists/false otherwise
        :
}

create_so_assignment_repo()
{
        echo "Creating GitLab repo ..."

        # create "l3-so-assignments" repo
        res=$(curl -sS -H "PRIVATE-TOKEN: $PRIVATE_TOKEN"           \
                "$GITLAB_API_URL/projects"                          \
                --data-urlencode "name=l3-so-assignments"           \
                --data-urlencode "visibility_level=0"               \
                --data-urlencode "import_url=$SO_ASSIGNMENTS_URL")
        if [ $? -ne 0 ]; then
                echo "internal error: Could not create repo ... exiting"
                exit 1
        fi

        # get repo ID
        REPO_ID=$(echo $res | jq -r '.id')
        if [ "$REPO_ID" == "null" ]; then
                echo "error: Creating repo failed ... exiting"
                exit 1
        else
                echo "You have successfully created your GitLab repo!"
        fi

        # get repo URL
        REPO_URL=$(echo $res | jq -r '.ssh_url_to_repo')

        echo "Your GitLab repo URL is $REPO_URL"
}

add_asist_so_assignment_group()
{
        # based on REPO_ID we add new project members
        :
}

# -------------------------------- Run script -------------------------------- #

# First check requirements
check_and_install_requirements

# TODO: the following lines are only for testing purposes - need refactoring
gitlab_authenticate
SSH_KEY=$(cat ~/.ssh/id_rsa.pub)
setup_user_profile
create_so_assignment_repo
# end-of-testing-section

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

