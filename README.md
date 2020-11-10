# AMCL localization
The package my robot lauches the acml node. After which using move_base (2d nav goal) the required localization results were acheived.
This is an excercise as a part of the Udacity Robotics Software Engineer Nano degree.

# Demo 

![ACML localization](samples/amcl.gif)

# Usage 


* Install the required dependencies as shown below :

```bash
$ sudo apt-get install ros-kinetic-navigation
$ sudo apt-get install ros-kinetic-map-server
$ sudo apt-get install ros-kinetic-move-base
$ sudo apt-get install ros-kinetic-amcl
```

* Clone the repo. Note that the repo uses git lfs to maintain the large map files. Install git lfs as shown below:
```
$ curl -s https://packagecloud.io/install/repositories/github/git-lfs/script.deb.sh | sudo bash
$ sudo apt install git-lfs
```
The above is for debian systems if you need to install in any other follow this [link](https://git-lfs.github.com/)

* Once installed get into the localization directory and follow this:
```
cd localization
git lfs install 
git lfs pull
```
* Now Catkin make the repo and launch it as shown:

```bash
roslaunch my_robot amcl.launch
```
