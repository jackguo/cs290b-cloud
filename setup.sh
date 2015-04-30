#! /usr/bin/env bash

# vm setup script: install package

sudo apt-get update
sudo apt-get -y install emacs24-nox vim git g++
git clone https://github.com/jackguo/emacs-profile.git
mv emacs-profile .emacs.d
