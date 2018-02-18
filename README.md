# LuaCad - Cad library for cgal (and opencsg)

## Description

* lua based cad language
* tcl interactive editor
* relies on cgal for accurate rendering
* relies on opencsg for fast preview
* similar concept as OpenSCAD

## Installing

### On Linux

sudo apt-get install git cmake build-essential

sudo apt-get install libcgal-dev libcgal-qt5-dev libeigen3-dev libglfw3-dev libglew-dev lua5.3 liblua5.3-dev libopencsg-dev

sudo apt-get install tcl8.6-dev tk8.6-dev

cd luacad/src;mkdir build;cd build;cmake ..;make && sudo make install

cd tclua;mkdir build;cd build;cmake ..;make && sudo make install

...

### On MacOS
...

### On windows
...

## extra ###

* [Learn Markdown](https://bitbucket.org/tutorials/markdowndemo)

## rappel sur git ##

#### pour creer une nouvelle branche toto

git checkout -b toto

(si on veut aussi que la branche soit sur bitbucket)

git push -u origin toto

#### quand on fait des changements

git commit -a -m 'ce que jai fait dans ce commit'

(et pour envoyer le tout sur bitbucket)

git push

#### quand on veut merger notre branche dans master

git checkout master

git pull

git merge toto

git push

#### pour eliminer une branche toto

git branch -d toto

(et la branche sur bitbucket)

git push origin --delete toto
