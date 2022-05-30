# example-osc

`example-osc` shows how to control the `tcp_target` and `look_at_target` of the robot over [OSC](https://ccrma.stanford.edu/groups/osc/index.html) — a common, lightweight, real-time communication protocol found in _many, many_ applications. OSC is like digital duct tape for connecting separate apps together. For example [Unity](https://thomasfredericks.github.io/UnityOSC/), [Grasshopper](https://www.food4rhino.com/en/app/firefly), [MaxMSP](https://opensoundcontrol.stanford.edu/implementations/OpenSoundControl-for-MaxMSP.html), [Touchdesigner](https://docs.derivative.ca/OSC_In_CHOP), and [TouchOSC](https://hexler.net/touchosc) all have OSC plugins that can pipe information between game development, 3D modeling, audio visual, and touch screen tools.

![](https://github.com/madelinegannon/personable_robotics/blob/main/assets/example-osc.gif)

## Getting Started

1. Build and Run `example-osc`
2. Click `Use OSC` in the GUI to enable listening for OSC messages (or press 'o' to toggle on/off)
3. Click `Use Agent` to interpolate between the incoming values (or press 'a' to toggle on/off)
4. Click `Robot_LIVE` when ready to move the real robot (or press 'm' to toggle on/off)

## Agent Motion

This example also introduces techniques to smoothly interpolate between in incoming values. Some kind of interpolation is necessary when using live data that can vary wildly — some [IK solvers](https://en.wikipedia.org/wiki/Inverse_kinematics) handle this, but the one we're using does not. We need to feed the robot a set of points to get it from where it is, to where we want it to be.

Here we use an `agent` that moves towards the `target_tcp` position. The `agent` is based off of a simple PD controller, and has tunable that effect its precision, latency, and responsiveness. You can think of it like a three-dimensional spring:

![](https://github.com/madelinegannon/personable_robotics/blob/main/assets/agent_controller_gui.png)

- `Proportional Gains` controls the _stiffness_
- `Derivative Gains` controls the _damping_
- `Steering Scalar` adds an overall scalar to exaggerate or mitigate motion

## Using Rhino & Grasshopper

This example is compatible with the included Rhino file `personable_robotics.3dm` and Grasshopper script `personable_robotics.gh`.  You must install the Grasshopper plugin [Firefly v. 1.0.0.70 ](https://www.food4rhino.com/en/app/firefly) prior to use. See [these instructions](https://www.food4rhino.com/en/faq#users-install-grasshopper-plugin) for how to install Grasshopper plugins.

> Use the `personable_robotics.3dm` and `personable_robotics.gh` templates as a starting point for sending data to the robot.

The Rhino file contains 2 labeled points, `tcp_target` and `look_at_target`, that send out their position to `example-osc` whenever they are moved.  

> Tested with Rhino 6 (Windows)

## Using TouchOSC

This example is compatible with TouchOSC's [MultiXY Pad example](http://library.isr.ist.utl.pt/docs/roswiki/touchosc_bridge(2f)Controls.html#Multi_XY_Pad). The first touch controls the `tcp_target` position within a min/max range, and the second touch controls the `look_at_target` position within a min/max range.

![](https://github.com/madelinegannon/personable_robotics/blob/main/assets/touch_osc_messages.png)

### Setup

The following screen recording shows how to navigate the TouchOSC interface to setup an OSC connection on a mobile device. 

> This `Host` should be your computer's ip adress, and the `Send Port` should match the `Receiver Port` in your application.

![](https://github.com/madelinegannon/personable_robotics/blob/main/assets/touch_osc_multixy_pad.gif)

#### TouchOSC Quick Links

- [Download TouchOSC](https://hexler.net/touchosc#get)
- [OSC Setup Overview](https://hexler.net/touchosc/manual/getting-started-osc)
- [Build Your Own](https://hexler.net/touchosc/manual/editor) Touch Controller Interface


## Using TouchDesigner

[TouchDesigner](https://derivative.ca/feature/application-building) is a visual development platform that equips you with the tools you need to create stunning realtime projects and rich user experiences. Whether you're creating interactive media systems, architectural projections, live music visuals, or simply rapid-prototyping your latest creative impulse, TouchDesigner is the platform that can do it all.

> Use the `personable_robotics.toe` template as a starting point for sending data to the robot.

#### TouchDesigner Quick Links

- [Tutorial](https://youtu.be/AcqC5rRmu3M)



