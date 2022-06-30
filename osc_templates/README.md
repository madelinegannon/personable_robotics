# osc_templates

This directory holds a series of templates in third party software that can communicate my examples examples.  how to control the `tcp_target` and `look_at_target` of the robot over [OSC](https://ccrma.stanford.edu/groups/osc/index.html) — a common, lightweight, real-time communication protocol found in _many, many_ applications. OSC is like digital duct tape for connecting separate apps together. For example 
- Use [Unity](https://thomasfredericks.github.io/UnityOSC/) for interactive or spatial projects.
- Use [Rhino/Grasshopper](https://www.grasshopper3d.com/) for geometry or fabrication based projects
- Use [Touchdesigner](https://docs.derivative.ca/) for audio / visual based projects
- Use [TouchOSC](https://hexler.net/touchosc) for controlling the robot from a touch screen.
- Use [Blender](https://www.blender.org/) for animation and timeline-based projects.

## Using Rhino & Grasshopper

To use the Rhino file `personable_robotics.3dm` and Grasshopper script `personable_robotics.gh`, please install the Grasshopper plugin [Firefly v. 1.0.0.70 ](https://www.food4rhino.com/en/app/firefly) prior to use. See [these instructions](https://www.food4rhino.com/en/faq#users-install-grasshopper-plugin) for how to install Grasshopper plugins.

> Use the `personable_robotics_osc.3dm` and `personable_robotics_osc.gh` templates as a starting point for sending data to the robot.

The Rhino file contains 2 labeled points, `tcp_target` and `look_at_target`, that send out their position to `example-osc` whenever they are moved.  

> Tested with Rhino 6 (Windows)

## Using TouchOSC

These examples are compatible with [TouchOSC](https://hexler.net/touchosc) by using the `personal_robotics_osc.tosc` template.

<img src="https://github.com/madelinegannon/personable_robotics/blob/main/assets/touch_osc_template.png" width="400">

> Use the `personal_robotics_osc.tosc` in TouchOSC as a starting point for sending data to the robot.

### Setup

#### Desktop App

Download and install the _free_ [TouchOSC Editor](https://hexler.net/touchosc#get) application. Open the `personal_robotics_osc.tosc` template, and click the _link_ icon to set the OSC properties (see below). Press the `play` icon to start streaming out inputs.

#### Mobile App

The following screen recording shows how to navigate the TouchOSC interface to setup an OSC connection on a mobile device. 

> This `Host` should be your computer's ip adress, and the `Send Port` should match the `Receiver Port` in your application.

<img src="https://github.com/madelinegannon/personable_robotics/blob/main/assets/touch_osc_multixy_pad.gif" width="400">

#### TouchOSC Quick Links

- [Download TouchOSC](https://hexler.net/touchosc#get)
- [OSC Setup Overview](https://hexler.net/touchosc/manual/getting-started-osc)
- [Build Your Own](https://hexler.net/touchosc/manual/editor) Touch Controller Interface


## Using TouchDesigner

[TouchDesigner](https://derivative.ca/feature/application-building) is a visual development platform that equips you with the tools you need to create stunning realtime projects and rich user experiences. Whether you're creating interactive media systems, architectural projections, live music visuals, or simply rapid-prototyping your latest creative impulse, TouchDesigner is the platform that can do it all.

> Use the `personable_robotics_osc.toe` template as a starting point for sending data to the robot.

#### TouchDesigner Quick Links

- [Tutorial](https://youtu.be/AcqC5rRmu3M)
- [OSC Documentation](https://docs.derivative.ca/OSC_In_CHOP)

## Using Blender

[Blender](https://www.blender.org/) is the leading free and open source 3D creation suite. It supports the entirety of the 3D pipeline—modeling, rigging, animation, simulation, rendering, compositing and motion tracking, even video editing and game creation. To use the blender file `personable_robotics_osc.blend`, please install **Blender v.2.93.3** and its OSC plugin [AddRoutes](http://www.jpfep.net/pages/addroutes/) prior to use. 

> Use the `personable_robotics_osc.blend` template as a starting point for sending data to the robot.



