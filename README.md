# Personable Robotics
Breathing Life Into Machines

### Prerequisites
- ABB robot with Externally Guided Motion [3124-1] option installed.
- See [`abb_egm_hello_world`](https://github.com/madelinegannon/abb_egm_hello_world) for detailed guidance on how to set up real-time control of ABB robots from an external PC.

![](https://github.com/madelinegannon/personable_robotics/blob/main/assets/ov_egm_working_2.gif)

# EGM Server API

| Message Address | Arg Format | Description |
| --- | ------ | --- |
| /enable | bool | Toggle **follow motion target** on/off for all robots. |
| /motion/reset | int | Motion target **reset** for Robot {index}. _Reset defaults to a normalized coordinate of (0.5, 0.8, 0,5) within the safety bounds._ |
| /motion/pos | int, float, float, float | Motion target **position** {x, y} for Robot {index} (normalized coordinate). _World target position is calculated based on the safety bounds._ |
| /motion/rot | int, float, float, float | Motion target **orientation** {rx, ry, rz} for Robot {index} (in degrees). |
| /motion/rot/rx | int, float | Motion target **x-axis rotation** {rx} for Robot {index} (normalized range from [0, 1] to [-180, 180] degrees). _Other rotation axes set to 0._ |
| /motion/rot/ry | int, float | Motion target **y-axis rotation** {ry} for Robot {index} (normalized range from [0, 1] to [-180, 180] degrees). _Other rotation axes set to 0._ |
| /motion/rot/rz | int, float | Motion target **z-axis rotation** {rz} for Robot {index} (normalized range from [0, 1] to [-180, 180] degrees). _Other rotation axes set to 0._ |
| /bounds/pos | int, float, float, float | Safety bounds **position** {x, y} of the center of the safety boundary (in meters), relative to the base of Robot {index}. _`bounds.pos.z` value is auto-calculated based on `bounds.height` to maintain minimum distance from the ground._ |
| /bounds/width | int, float | Safety bounds **width** of Robot {index} safety boundary (in meters). |
| /bounds/height | int, float | Safety bounds **height** of Robot {index} safety boundary (in meters). |
| /bounds/depth | int, float | Safety bounds **depth** of Robot {index} safety boundary (in meters). |


# Templates
The [`templates`](https://github.com/madelinegannon/personable_robotics/tree/main/templates) folder has basic templates for communicating over OSC to the `EGM Server`, including Processing, Grasshopper, and TouchOSC.

## TouchOSC

[TouchOSC](https://hexler.net/touchosc) is an easy-to-use interface builder that can send and receive [OSC](https://ccrma.stanford.edu/groups/osc/index.html) messages from multiple sources. It's the quickest way to build your own interactive touch interface on your desktop, phone, or tablet — _plus_ it's supported on nearly every operating system: Windows / macOS / Linux / iOS / Android / RasPi.

We can use TouchOSC to talk to the EGM Sever:
![](https://github.com/madelinegannon/personable_robotics/blob/main/assets/touchosc/egm_server_touchosc.gif)

### Setup

1. Download TouchOSC for your platform [here](https://hexler.net/touchosc#get).
> The desktop version is free.
2. Open TouchOSC and configure your network settings.

![](https://github.com/madelinegannon/koriobots/blob/main/assets/touchosc/touchosc_network_settings.gif)

3. Open a template file in TouchOSC and press the play button to start running the file.
4. Press the dot in the upper right corner to close the file.

![](https://github.com/madelinegannon/koriobots/blob/main/assets/touchosc/touchosc_play_stop.gif)

### Notes

- Tested on TouchOSC 1.2.7.190
- [TouchOSC User Manual](https://hexler.net/touchosc/manual/introduction)


## Processing
[Processing](https://processing.org/) is a creative coding platform for sketching with code. 

We can use Processing and the third-party library [oscP5](https://sojamo.de/libraries/oscP5/) to talk to the EGM Sever:

![](https://github.com/madelinegannon/personable_robotics/blob/main/assets/processing/controller_template_processing.gif)

### Setup

1. Download and install [Processing](https://processing.org/download).
2. Download and install [oscP5](https://sojamo.de/libraries/oscP5/) following these [instructions](https://sojamo.de/libraries/oscP5/#installation).
3. Copy `controller_template.pde` into your Processing Sketchbook.
4. Configure your network settings:

![](https://github.com/madelinegannon/personable_robotics/blob/main/assets/processing/processing_network_settings.png)

6. Press play and run.

### Examples

#### egm_example_mouse2D
This example updates the position of one robot's motion target when hovering, and enables/disables motion on `mousePressed`.
![](https://github.com/madelinegannon/personable_robotics/blob/main/assets/processing/example_mouse2D_processing.gif)

#### egm_example_physics2D
This example updates the position of one robot's motion target based on the position of a physics-enabled box that you can throw around the canvas.
![](https://github.com/madelinegannon/personable_robotics/blob/main/assets/processing/example_physics2D_processing.gif)

### Notes

- Tested on Processing 4.3, oscP5 0.9.8, and controlP5 2.2.5
