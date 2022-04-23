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

6. Build the `example-simple` project using the [Project Generator](https://openframeworks.cc/learning/01_basics/create_a_new_project/):

(gif)

5. Run the project — move the Gizmo to move the Robot:
- Press `W` to use the Translate Gizmo
- Press `E` to use the Rotate Gizmo
- When you're ready, press the `Robot Live` Button to move the actual robot.

## Troubleshooting Installation
