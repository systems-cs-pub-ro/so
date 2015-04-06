#!/bin/bash

#
# Script to automatically create git repo for SO assignments
#
# 2015, Operating Systems
#

# ----------------- General declarations and util functions ------------------ #
USER=""
PRIVATE_TOKEN=""
REPO_NAME="l3-so-assignments"
REPO_ID=""
REPO_URL=""
REPO_SSH_URL=""
REPO_HTTPS_URL=""
SSH_KEY_PATH="${HOME}/.ssh/id_rsa.pub"
SSH_KEY=""
GITLAB_API_URL="https://gitlab.cs.pub.ro/api/v3"
SO_ASSIGNMENTS_URL="https://github.com/systems-cs-pub-ro/so-assignments.git"
TIMEOUT=30

check_and_install_requirements()
{
        # Packets required: git, curl, ssh, jq

        # if not present install them
        # show progress after each installed packet
        # if one packet fails to install, the script fails
        # exit if it fails

        INSTALLER=$(which apt-get)
        if [ -z $INSTALLER ]; then
                INSTALLER=$(which yum)
        fi

        if [ -z $INSTALLER ]; then
                echo -e "error: Distribution not supported. Please contribute to
                this script in order to add support for your distribution\n"
                exit 1
        fi

        echo -e "Checking system requirements ...\n"
        PACKAGES=( git curl ssh jq )

        for package in ${PACKAGES[@]}; do
                which $package > /dev/null
                if [ $? -ne 0 ]; then
                        echo -e "info: $package not available on the system. Installing...\n"
                        sudo $INSTALLER install $package -y
                        if [ $? -ne 0 ]; then
                                echo -e "error: Failed installing $package. Aborting ...\n"
                                exit 1
                        fi
                else
                        echo "info: $package already available on the system"
                fi
        done
        echo ""
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
                echo -e "internal error: Could not create session ... exiting\n"
                exit 1
        fi

        # get private token
        PRIVATE_TOKEN=$(echo $session | jq -r '.private_token')
        if [ "$PRIVATE_TOKEN" == "null" ]; then
                echo -e "error: Authentication failed ... exiting\n"
                exit 1
        fi

        # get name
        USER=$(echo $session | jq -r '.name')

        echo -e "Hi, $USER! You have successfully authenticated!\n"
}

check_ssh_key()
{
        # check if already exists a SSH key on this machine. We take the first
        # available SSH key. If there are more than one, it will only take the first one
        # if yes, obtain key and return true, else return false
        # We assume a public SSH key has name.pub
        SSH_KEY_NAME=$(ls -lh ~/.ssh/ | grep ".pub" | tr -s " " | cut -d ' ' -f 9 | head -n 1)
        SSH_KEY_PATH="${HOME}/.ssh/$SSH_KEY_NAME"
        SSH_KEY=$(cat $SSH_KEY_PATH 2> /dev/null)
        return $?
}

generate_ssh_key()
{
        # function to generate SSH key if the user wants to use SSH, else
        # defaults into using HTTPS

        echo -e "No SSH keys were found! Generating a new pair ...\n"

        ssh-keygen -t rsa -C "$USER"
        ssh-add
        SSH_KEY_PATH="${HOME}/.ssh/id_rsa.pub"
        SSH_KEY=$(cat $SSH_KEY_PATH 2> /dev/null)
}

setup_user_profile()
{
        echo "Adding SSH public key ($SSH_KEY_PATH) to GitLab ..."

        # read SSH key title
        read -p "Please provide a title for your SSH public key: " title

        # add SSH key to user profile
        res=$(curl -sS -H "PRIVATE-TOKEN: $PRIVATE_TOKEN"   \
                "$GITLAB_API_URL/user/keys"                 \
                --data-urlencode "title=$title"             \
                --data-urlencode "key=$SSH_KEY")
        if [ $? -ne 0 ]; then
                echo -e "internal error: Could not add SSH public key ... exiting\n"
                exit 1
        fi

        # get key ID
        id=$(echo $res | jq -r '.id')
        if [ "$id" == "null" ]; then
                echo -e "error: Adding SSH public key failed ... exiting\n"
                exit 1
        else
                echo -e "Your SSH public key was successfully added to GitLab!\n"
        fi
}

check_existing_assignment_repo()
{
        # checks if there already is an "l3-so-assignments" repo
        # TODO: if there is, we only need to pull current homework
        # returns true if already exists/false otherwise
        echo "Checking $REPO_NAME repo on GitLab ..."

        # get all repos
        repos=$(curl -sS -H "PRIVATE-TOKEN: $PRIVATE_TOKEN"     \
               "$GITLAB_API_URL/projects/owned")
        if [ $? -ne 0 ]; then
                echo -e "internal error: Could not get all repos ... exiting\n"
                exit 1
        fi

        # compute number of repos
        num_repos=$(echo $repos | jq -r '. | length')

        # search "l3-so-assignments" repo
        for i in $(seq 0 $num_repos); do
                name=$(echo $repos | jq -r ".[$i] | .name")
                if [ "$name" == "$REPO_NAME" ]; then
                        REPO_ID=$(echo $repos | jq -r ".[$i] | .id")
                        REPO_URL=$(echo $repos | jq -r ".[$i] | .ssh_url_to_repo")
                        echo "$REPO_NAME repo was found!"
                        echo -e "Your $REPO_NAME repo URL is $REPO_URL\n"
                        return 0
                fi
        done

        echo -e "$REPO_NAME repo was not found!\n"
        return 1
}

create_so_assignment_repo()
{
        echo "Creating $REPO_NAME repo on GitLab ..."

        # create "l3-so-assignments" repo
        res=$(curl -sS -H "PRIVATE-TOKEN: $PRIVATE_TOKEN"           \
                "$GITLAB_API_URL/projects"                          \
                --data-urlencode "name=$REPO_NAME"                  \
                --data-urlencode "visibility_level=0"               \
                --data-urlencode "import_url=$SO_ASSIGNMENTS_URL")
        if [ $? -ne 0 ]; then
                echo -e "internal error: Could not create repo ... exiting\n"
                exit 1
        fi

        # get repo ID
        REPO_ID=$(echo $res | jq -r '.id')
        if [ "$REPO_ID" == "null" ]; then
                echo -e "error: Creating repo failed ... exiting\n"
                exit 1
        else
                echo "Your $REPO_NAME repo was successfully created!"
        fi

        # get SSH repo URL
        REPO_SSH_URL=$(echo $res | jq -r '.ssh_url_to_repo')
        # get HTTPS repo URL
        REPO_HTTPS_URL=$(echo $res | jq -r '.http_url_to_repo')
        USE_SSH="$1"

        if [ $USE_SSH = "yes" ]; then
                REPO_URL=$REPO_SSH_URL
        else
                REPO_URL=$REPO_HTTPS_URL
        fi

        echo -e "Your $REPO_NAME repo URL is $REPO_URL\n"
}

update_so_assignment_repo()
{
        echo "Updating $REPO_NAME repo on GitLab ..."

        git push origin master

        echo -e "Your $REPO_NAME repo is up-to-date!\n"
}

create_so_assignment_clone()
{
        echo "Creating $REPO_NAME local clone ..."

        git clone $REPO_URL

        cd $REPO_NAME
        git remote add upstream $SO_ASSIGNMENTS_URL
        cd -

        echo -e "Your $REPO_NAME local clone was successfully created!\n"
}

update_so_assignment_clone()
{
        echo "Updating $REPO_NAME local clone ..."

        git fetch upstream
        git checkout master
        git merge upstream/master

        echo -e "Your $REPO_NAME local clone is up-to-date!\n"
}

add_asist_so_assignment_group()
{
        # based on REPO_ID we add new project members
        :
}

# -------------------------------- Run script -------------------------------- #

# First check requirements
check_and_install_requirements

# authenticate
gitlab_authenticate

# check if the user has the "l3-so-assignments" repo on GitLab
check_existing_assignment_repo

if [ $? -ne 0 ]; then
        # check if the user wants to use SSH key
        read -p "Would you like to use SSH with git? [yes/no] " use_ssh

        if [ $use_ssh = "yes" ]; then
               check_ssh_key
               if [ $? -ne 0 ]; then
                       generate_ssh_key
               fi
                setup_user_profile
        else
                echo -ne "\ninfo: Not using an SSH key. All git actions will be done through HTTPS."
                echo -e " This means each time you will be asked for your curs.cs.pub.ro credentials\n"
        fi

        create_so_assignment_repo $use_ssh

        echo "Waiting $TIMEOUT seconds ..."
        sleep $TIMEOUT

        create_so_assignment_clone
fi

# TODO: add other functionalities
# (e.g. pull from github repo, help, arguments, etc.)

exit 0

