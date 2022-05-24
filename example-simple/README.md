# example-simple

`example-simple` is a barebones app that's a useful template to build from. This minimal app is also helpful for debugging — for example, checking networking to the robot or testing joint limits and configurations.

![](https://github.com/madelinegannon/personable_robotics/blob/main/assets/example-simple.gif)

## Getting Started

1. Build and Run `example-osc`
2. Click `Robot_LIVE` when ready to move the real robot (or press 'm' to toggle on/off)

## Network Troubleshooting

If you are having issues communicating with the physical robot, there are a few common networking problems to debug:

1. Make sure your ethernet cable is securely connected between the robot and computer or switch.
2. For Universal Robots: make sure ethernet is enabled from the teach pendant. See how [here](https://wiki.ros.org/universal_robot/Tutorials/Getting%20Started%20with%20a%20Universal%20Robot%20and%20ROS-Industrial).
3. Make sure your computer has a static IP address on the same subnet of the robot. See how to set up a static IP address here ([Win](https://www.trendnet.com/press/resource-library/how-to-set-static-ip-address), [Mac](https://www.macinstruct.com/tutorials/how-to-set-a-static-ip-address-on-a-mac/), [Linux](https://linuxconfig.org/how-to-configure-static-ip-address-on-ubuntu-18-10-cosmic-cuttlefish-linux#:~:text=Ubuntu%20Desktop,-The%20simplest%20approach&text=Click%20on%20the%20top%20right,netmask%2C%20gateway%20and%20DNS%20settings.)).
4. Check that the robot's IP adress matches the `ip_addr` in the `setup()` function of  `example-simple`, and that you've set `offline` to `false`:

![](https://github.com/madelinegannon/personable_robotics/blob/main/assets/connecting_to_robot.png)

## Running the Example

`example-simple` lets you move and reorient the robot by using a [gizmo or gumball](https://github.com/NickHardeman/ofxGizmo) manipulator — like the ones commonly found in most 3D modelling software.

1. Press `w` to enable the Translate Gizmo
2. Press `e` to enable the Rotate Gizmo
3. Press `1`, `2`, `3`, or `4` to chage the viewport to preset `TOP`, `FRONT`, `SIDE`, or `PERSPECTIVE` views.
4. Press `m` to go into `LIVE` mode and move the real robot.
> WARNING: Make sure the area is clear of people and objects before going into `LIVE` mode.

> CAUTION: Large, sudden movements will throw velocity errors or joint errors that cause emergency stops.

