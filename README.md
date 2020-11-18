doublescara-ros


Instruction:
1) run roscore in the terminal
	roscore
2) run the publisher node
	rosrun position_pub/talker
3) print the topic to make sure that it is working
	rostopic echo /position_pub
4) Install rosserial (replace "melodic" through your ros distrubution)
http://wiki.ros.org/rosserial_server
	sudo apt-get install ros-melodic-rosserial-arduino
	sudo apt-get install ros-melodic-rosserial
	sudo apt-get install ros-melodic-rosserial-server
5) start roscore in a separate terminal tab
	roscore
6) launch rosserial_server on the serial port used by the microcontroller
	rosrun rosserial_python serial_node.py /dev/ttyACM0
7) you should see the topic published by the microcontroller now
	rostopic list
.
.
/megaChatter
.
.

