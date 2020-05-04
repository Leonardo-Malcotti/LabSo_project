#!bin/bash

sudo docker run -it --rm --name UBUNTU --hostname LABSO ubuntu:bionic
apt update
apt upgrade
