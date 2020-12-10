**********************
Note!! if you get some python errors indicating that some libraries are missing: do the following:
1) install pip
sudo apt install python-pip
2) install shapely (python library)
sudo pip install shapely
3) if further libraries are missing, install them in the same way
sudo pip install (library name)



**********************
A simple test method
1)
roscore

2)
rosrun collision_checker collision_checker.py

3)
rostopic echo /checked_angles_array

4)
rostopic echo /pose_ok


5)
rostopic pub /set_angle_array std_msgs/Float32MultiArray "layout:
  dim:
  - label: ''
    size: 0
    stride: 0
  data_offset: 0
data:
 [0.0,0.20]" --once


6) Check the results of the topics


