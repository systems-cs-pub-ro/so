# SO Assignments

Contains skeletons and tests for SO assignments

## so-create-repo.sh

This script is a wrapper over GitLab API and git. Its purpose is to export this public repo as a private repo on GitLab and to create a local git clone, where the script is run.

Installation
------------

    wget https://raw.githubusercontent.com/systems-cs-pub-ro/so/master/assignments/so-create-repo.sh

Usage
-----

Run the script from the directory where you want the local git clone to be created:

    bash so-create-repo.sh

When running for the first time, the script will:

0. check and install all dependencies: git, curl, ssh, jq
0. authenticate the user to GitLab; now the curs.cs.pub.ro credentials are requested
0. ask permission to upload a SSH public key to GitLab; if none available, it will generate a new SSH key pair
0. if the SSH authentication method was chosen, it will request you to provide a title for your public key and after that will upload that key to GitLab
0. create the private repo on GitLab; the repo URL will be printed to the output console
0. add the SO TAs as observers
0. create a local clone

Now, a new directory is available: l3-so-assignments. You can interact with your GitLab public repo through git. For example, you can update your private repo using the commands:

    cd l3-so-assignments
    git push

After the private repo on GitLab is successfully created, you can use the script to update your local clone in case the SO public repo suffers modifications or new content is provided. For these subsequent updates the script must be run from within the clone directory.

We recommend not to run directly the instance of the script available in the clone directory because, through an update, it can be the subject to modifications.

In case you already have the l3-so-assignments private repo on GitLab and you just want to create a new local clone (and not to update the current one) then run the script with --clone parameter:

    bash so-create-repo.sh --clone


Limitations
-----------
The script has support for:
- the Linux distributions which use apt or yum as the package manager
- the OS X versions which use Homebrew as the package manager

A short time is needed between creating the repo on GitLab and cloning it, in order to allow GitLab to fully update his local databases. The script is configured to wait for 30 seconds. You might try to increase this timeout if cloning still fails.

