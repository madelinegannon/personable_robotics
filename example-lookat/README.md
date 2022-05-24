# example-lookat

`example-lookat` adds real-time sensing into the previous example, `example-osc`. It uses body tracking from the [Azure Kinect](https://azure.microsoft.com/en-us/services/kinect-dk/) to get the robot to look at and track you as you move around a predefined interaction zone. 

![](https://github.com/madelinegannon/personable_robotics/blob/main/assets/example-lookat.gif)

## Getting Started

1. Build and Run `example-lookat`
2. Calibrate the sensor by manually moving the `sensor_gizmo` into the correct position and orientation.
3. Click `Use OSC` in the GUI to enable listening for OSC messages (or press 'o' to toggle on/off)
4. Click `Use Agent` to interpolate between the incoming values (or press 'a' to toggle on/off)
5. Build and Run `azure_kinect_skeleton_streaming`
> Ensure that the IP Address and Port are correct before running.
6. Click `Robot_LIVE` in `example-lookat` when ready to move the real robot (or press 'm' to toggle on/off)

## Sensor Calibration

This example introduces core concepts in how to calibrate sensors to a robot. Sensors have their own coordinate systems for measuring the world — for example, the Azure Kinect [coordinate system]() uses Y-Up and Z-Forward and is in meters, whereas Z-Up is the norm in robotics and this example uses millimeters. In order for the robot to accurately see and understand the world, we need to get the data from the sensor into the robot's coordinate system. While there are automated tools and techniques for automating sensor calibration, for simplicity's sake  `example-lookat` provides manual tools for positioning and orientating a sensor relative to the robot.

![](https://github.com/madelinegannon/personable_robotics/blob/main/assets/sensor_parameters_gui.png)

- Move and rotate the the `sensor_gizmo` through the gui or with the viewport gizmo into the correct position and orientation.
- Record the position and orientation quaternion, and update the default pose in the `setup_gizmos()` function: 

![](https://github.com/madelinegannon/personable_robotics/blob/main/assets/sensor_pose_calibration.png)

## Body Tracking & Motion Parameters

This example also introduces how to build spatial relationships between the robot and a person. By default, `example-lookat` attaches the robot's `look_at_target`to a detected skeleton's head or hands (whichever is closest). Although the Azure Kinect can see many different skeletons at one time, for simplicity's sake the  `azure_kinect_skeleton_streaming` app only streams the closest skeleton over OSC.

The robot's `tcp_target` then gets updated based on the position of the `look_at_target` and the `Robot_Motion_Lerp` and `Robot_Motion_Base_Offset` values.

![](https://github.com/madelinegannon/personable_robotics/blob/main/assets/motion_parameters_gui.png)

1. The robot's `tcp_target` position is determined by the vector from the robot's base to the `look_at_target`.
2. Raise or lower the `Robot_Motion_Base_Offset` to have the robot look from above or below.
3. Increase or decrease the `Robot_Motion_Lerp` to move the robot closer or further away from the `look_at_target`.

Begin by playing with the `Robot_Motion_Lerp` and `Robot_Motion_Base_Offset` values through the GUI. Once you have an intuition for how the parameters effect motions, begin automating, parametricizing, or triggering different value pairs. You can also do this over OSC:

![](https://github.com/madelinegannon/personable_robotics/blob/main/assets/motion_parameters_osc.png)

Some things to consider when designing these motion behaviors:

1. What should the robot do as I move closer? as I move further away?
2. What should the robot do when my hand is in a given region?
3. What should the robot do when my hands are together? above my head? 
4. What should the robot do when there's no one around?
