# Personable Robotics
#### MRAC Workshop 3.2 for [IAAC](https://iaac.net)
_Barcelona, Spain — May 30 - June 3, 2022_

# About

## Tested On
- Mac 10.14.6
- Xcode 11.2.1
- openFrameworks v0.11.2


# Getting Started
1. Download and follow the setup guide for [openFrameworks](https://openframeworks.cc/download/) v.0.11.2
2. Add the `ofxRobotArm` addon into the openFramworks `/adddon` folder:

```
cd of_v0.11.2_osx_release
git lfs install 
cd addons
git clone https://github.com/CreativeInquiry/ofxRobotArm.git
git checkout personable_robotics
```
3. Run the `installAddons` script to grab all the necessary dependencies:
```
sudo ./installAddons.sh
```
4. Check out the `dev-ur10` branch of `ofxURDriver`
```
cd ../ofxURDriver
git checkout dev-ur10
```

5. Build the `example-simple` project using the [Project Generator](https://openframeworks.cc/learning/01_basics/create_a_new_project/):

6. Run the project — move the Gizmo to move the Robot:
- Press `W` to use the Translate Gizmo
- Press `E` to use the Rotate Gizmo
- When you're ready, move the simulated robot into a position near the actual robot. Then press the `Robot Live` Button to move the actual robot.

### Troubleshooting Installation

# Examples

## example-simple
`example-simple` is a barebones app that's a useful template to build from. This minimal app is also helpful for debugging — for example, checking networking to the robot or testing joint limits and configurations.

(gif)

## example-osc
`example-osc` lets you control the `tcp_target` and `look_at_target` of the robot over [OSC](https://ccrma.stanford.edu/groups/osc/index.html) — a common, lightweight, real-time communication protocol found in _many, many_ applications. OSC is like digital duct tape for connecting separate apps together. For example [Unity](https://thomasfredericks.github.io/UnityOSC/), [Grasshopper](https://www.food4rhino.com/en/app/ghowl), [MaxMSP](https://opensoundcontrol.stanford.edu/implementations/OpenSoundControl-for-MaxMSP.html), [Touchdesigner](https://docs.derivative.ca/OSC_In_CHOP), and [TouchOSC](https://hexler.net/touchosc) all have OSC plugins that can pipe information between game development, 3D modeling, audio visual, and touch screen tools.

(gif)

## example-lookat
`example-lookat` adds real-time sensing into the previous example. It uses body tracking from the [Azure Kinect](https://azure.microsoft.com/en-us/services/kinect-dk/) to get the robot to look at and track you as you move around an interaction zone. 

(gif)

## example-touch
`example-touch` uses a depth camera as a touch sensor for interacting with the robot. This is the most advanced example, and it requires the most careful calibration between sensor, robot, and environment.

(gif)
