# Personable Robotics
Breathing Life Into Machines

### Prerequisites
- ABB robot with Externally Guided Motion [3124-1] option installed.

See [`abb_egm_hello_world`](https://github.com/madelinegannon/abb_egm_hello_world) for detailed guidance on how to set up real-time control of ABB robots from an external PC.



# Server API

| Message Address | Arg Format | Description |
| --- | ------ | --- |
| /stop | —— | Trigger soft stop all koriobots |
| /move_vel | bool | Toggle velocity move on/off for all koriobots |
| /reset | —— | Trigger reset to the center of the safety bounds |
| /norm | int, float, float | Index and normalized {x,y} target coordinate. _Note: use index=-1 to move all koriobots to the normalized target._ |
| /limits/velocity | float | Sets the velocity limit for all koriobots |
| /limit/acceleration | float | Sets the acceleration limit for all koriobots |
| /bounds/pos | float, float | Sets normalized {x,y} position of the safety boundary |
| /bounds/width | float | Sets normalized width of the safety boundary |
| /bounds/height | float | Sets normalized height of the safety boundary |
