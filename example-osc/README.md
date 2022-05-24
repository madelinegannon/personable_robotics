# example-osc

`example-osc` shows how to control the `tcp_target` and `look_at_target` of the robot over [OSC](https://ccrma.stanford.edu/groups/osc/index.html) — a common, lightweight, real-time communication protocol found in _many, many_ applications. OSC is like digital duct tape for connecting separate apps together. For example [Unity](https://thomasfredericks.github.io/UnityOSC/), [Grasshopper](https://www.food4rhino.com/en/app/ghowl), [MaxMSP](https://opensoundcontrol.stanford.edu/implementations/OpenSoundControl-for-MaxMSP.html), [Touchdesigner](https://docs.derivative.ca/OSC_In_CHOP), and [TouchOSC](https://hexler.net/touchosc) all have OSC plugins that can pipe information between game development, 3D modeling, audio visual, and touch screen tools.

## Getting Started

## 

This example also introduces techniques to smoothly interpolate between large changes in movements. 

## Using TouchOSC

This example is compatible with TouchOSC's [MultiXY Pad example](http://library.isr.ist.utl.pt/docs/roswiki/touchosc_bridge(2f)Controls.html#Multi_XY_Pad). The first touch controls the `tcp_target` position within a min/max range, and the second touch controls the `look_at_target` position within a min/max range.

### Setup

The following screen recording shows how to navigate the TouchOSC interface to setup an OSC connection on a mobile device. 

> This `ip address` should be your computer's ip adress, and the `port` should match the `osc_receiver_port` in your application.

#### TouchOSC Quick Links

- [Download TouchOSC](https://hexler.net/touchosc#get)
- [OSC Setup Overview](https://hexler.net/touchosc/manual/getting-started-osc)
- [Build Your Own](https://hexler.net/touchosc/manual/editor) Touch Controller Interface

See [this overview] if you would like to build your own touch controller interface.



