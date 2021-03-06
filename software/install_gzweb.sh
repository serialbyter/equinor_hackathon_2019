#!/bin/bash

shdir=$(realpath $(dirname $0))
modeldir=$(realpath $shdir/../catkin_ws/src/simulator/models)

sudo apt-get update && sudo apt-get install -q -y \
  build-essential \
  cmake \
  curl \
  imagemagick \
  libalsaplayer-dev \
  libboost-all-dev \
  libgazebo9-dev \
  libjansson-dev \
  libtinyxml-dev \
  mercurial \
  pkg-config \
  psmisc \
  xvfb

pushd $HOME
hg clone https://bitbucket.org/osrf/gzweb

curl -sL https://deb.nodesource.com/setup_8.x >> setup_8.sh
chmod +x setup_8.sh
sudo ./setup_8.sh
sudo apt-get install -y nodejs
rm setup_8.sh

export GAZEBO_MODEL_PATH=$modeldir
cd gzweb
source /usr/share/gazebo/setup.sh
hg up gzweb_1.4.0
npm run deploy --- -t -m local

popd
