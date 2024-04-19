/**
 *  EGM Controller Template
 *
 *  Processing template for communicating with ABB EGM Server (https://github.com/madelinegannon/personable_robotics)
 *
 *  Library Dependencies:
 *    - oscP5 (https://sojamo.de/libraries/oscP5)
 *    - controlP5 (https://sojamo.de/libraries/controlP5)
 *
 *  Madeline Gannon | atonaton.com
 *  04.17.2024
 */

import netP5.*;
import oscP5.*;
import controlP5.*;

OscP5 oscP5;
NetAddress server_addr;

ControlP5 cp5;

// Change these to match the server osc settings
String host = "127.0.0.1";   // ip address of the server
int port_send = 55556;       // receiving port of the server
int port_receive = 55555;    // sending port of the server

void setup() {
  size(615, 640);
  oscP5 = new OscP5(this, port_receive);
  server_addr = new NetAddress(host, port_send);
  setup_gui();
}

void draw() {
  background(0);
}

/**
 *  Incoming OSC message are forwarded to the oscEvent method.
 */
void oscEvent(OscMessage theOscMessage) {
  // print the address pattern and the typetag of the received OscMessage
  print("### received an osc message.");
  print(" addrpattern: "+theOscMessage.addrPattern());
  println(" typetag: "+theOscMessage.typetag());
}

/**
 *  GUI Event handler
 */
void controlEvent(ControlEvent theEvent) {
  if (theEvent.isController() && millis() > 1500) {          // ignore triggers as the program loads
    OscMessage msg = new OscMessage("/test");
    if (theEvent.getController().getName() == "enable_move") {
      msg = new OscMessage("/motion/enable");
      int val = int(theEvent.getController().getValue());
      msg.add(val == 1);
    } else if (theEvent.getController().getName() == "XZ_Plane_0") {
      // normalize the {X,Y} value before sending
      float x = theEvent.getController().getArrayValue()[0] / 100.0;
      float y = theEvent.getController().getArrayValue()[1] / 100.0;
      msg = new OscMessage("/motion/pos");
      msg.add(0);
      msg.add(x);
      msg.add(y);
    } else if (theEvent.getController().getName() == "Y_0") {
      msg = new OscMessage("/motion/depth");
      msg.add(0);
      msg.add(theEvent.getController().getValue());
    } else if (theEvent.getController().getName() == "reset_0") {
      msg = new OscMessage("/motion/reset");
      msg.add(0);
    } else if (theEvent.getController().getName() == "XZ_Plane_1") {
      // normalize the {X,Y} value before sending
      float x = theEvent.getController().getArrayValue()[0] / 100.0;
      float y = theEvent.getController().getArrayValue()[1] / 100.0;
      msg = new OscMessage("/motion/pos");
      msg.add(1);
      msg.add(x);
      msg.add(y);
    } else if (theEvent.getController().getName() == "Y_1") {
      msg = new OscMessage("/motion/depth");
      msg.add(1);
      msg.add(theEvent.getController().getValue());
    } else if (theEvent.getController().getName() == "reset_1") {
      msg = new OscMessage("/motion/reset");
      msg.add(1);
    } else if (theEvent.getController().getName() == "position_0") {
      // normalize the {X,Y} value before sending
      float x = theEvent.getController().getArrayValue()[0] / 100.0;
      float y = theEvent.getController().getArrayValue()[1] / 100.0;
      msg = new OscMessage("/bounds/pos");
      msg.add(0);
      msg.add(x);
      msg.add(y);
    } else if (theEvent.getController().getName() == "width_0") {
      msg = new OscMessage("/bounds/width");
      msg.add(0);
      msg.add(theEvent.getController().getValue());
    } else if (theEvent.getController().getName() == "height_0") {
      msg = new OscMessage("/bounds/height");
      msg.add(0);
      msg.add(theEvent.getController().getValue());
    } else if (theEvent.getController().getName() == "depth_0") {
      msg = new OscMessage("/bounds/depth");
      msg.add(0);
      msg.add(theEvent.getController().getValue());
    } else if (theEvent.getController().getName() == "position_1") {
      // normalize the {X,Y} value before sending
      float x = theEvent.getController().getArrayValue()[0] / 100.0;
      float y = theEvent.getController().getArrayValue()[1] / 100.0;
      msg = new OscMessage("/bounds/pos");
      msg.add(1);
      msg.add(x);
      msg.add(y);
    } else if (theEvent.getController().getName() == "width_1") {
      msg = new OscMessage("/bounds/width");
      msg.add(1);
      msg.add(theEvent.getController().getValue());
    } else if (theEvent.getController().getName() == "height_1") {
      msg = new OscMessage("/bounds/height");
      msg.add(1);
      msg.add(theEvent.getController().getValue());
    } else if (theEvent.getController().getName() == "depth_1") {
      msg = new OscMessage("/bounds/depth");
      msg.add(1);
      msg.add(theEvent.getController().getValue());
    }
    oscP5.send(msg, server_addr);
  }
}


void setup_gui() {
  cp5 = new ControlP5(this);

  int x = 10;
  int y = 25;
  int h = 25;
  int w = 150;
  int y_offset = 30;

  Group params = cp5.addGroup("params")
    .setPosition(x, y + 10)
    .setWidth(150)
    .setHeight(15)
    .activateEvent(true)
    .setBackgroundColor(color(255, 120))
    .setBackgroundHeight(50)
    .setLabel("EGM Controller")
    ;


  cp5.addToggle("enable_move")
    .setPosition(x, x)
    .setSize(h, h)
    .setGroup(params)
    .setColorBackground(color(0, 120, 0))
    .setColorForeground(color(0, 180, 0))
    .setColorActive(color(0, 250, 0))
    .setLabelVisible(false)
    ;

  cp5.addLabel("ENABLE_MOVE")
    .setPosition(x+h, x + 8)
    .setGroup(params)
    ;

  // Set up MOTION parameters
  Group params_motion = cp5.addGroup("params_motion")
    .setPosition(x+160, y + 10)
    .setWidth(210)
    .setHeight(15)
    .activateEvent(true)
    .setBackgroundColor(color(255, 120))
    .setBackgroundHeight(280)
    .setLabel("Motion_1")
    ;

  cp5.addSlider2D("XZ_Plane_1")
    .setPosition(x, x)
    .setSize(w, w)
    .setMinMax(0.0, 100.0, 100, 0)
    .setValue(50, 50)
    .setGroup(params_motion)
    ;

  cp5.addSlider("Y_1")
    .setPosition(x, y + w + 5 + 0*y_offset)
    .setSize(w, h)
    .setMin(0)
    .setMax(1)
    .setGroup(params_motion)
    ;

  cp5.addButton("reset_1")
    .setPosition(x, y + w + 5 + 1*y_offset)
    .setSize(h, h)
    .setGroup(params_motion)
    .setLabelVisible(false)
    ;

  cp5.addLabel("RESET ")
    .setPosition(x+h, y + w + 5 + 8+ 1*y_offset)
    .setGroup(params_motion)
    ;

  Group params_motion_0 = cp5.addGroup("params_motion_0")
    .setPosition(x+380, y + 10)
    .setWidth(210)
    .setHeight(15)
    .activateEvent(true)
    .setBackgroundColor(color(255, 120))
    .setBackgroundHeight(280)
    .setLabel("Motion_0")
    ;

  cp5.addSlider2D("XZ_Plane_0")
    .setPosition(x, x)
    .setSize(w, w)
    .setMinMax(0.0, 100.0, 100, 0)
    .setValue(50, 50)
    .setGroup(params_motion_0)
    ;

  cp5.addSlider("Y_0")
    .setPosition(x, y + w + 5 + 0*y_offset)
    .setSize(w, h)
    .setMin(0)
    .setMax(1)
    .setGroup(params_motion_0)
    ;

  cp5.addButton("reset_0")
    .setPosition(x, y + w + 5 + 1*y_offset)
    .setSize(h, h)
    .setGroup(params_motion_0)
    .setLabelVisible(false)
    ;

  cp5.addLabel("RESET")
    .setPosition(x+h, y + w + 5 + 8+ 1*y_offset)
    .setGroup(params_motion_0)
    ;

  // Set up BOUNDS parameters
  Group params_bounds = cp5.addGroup("params_bounds")
    .setPosition(x+160, y + 10 + 300)
    .setWidth(210)
    .setHeight(15)
    .activateEvent(true)
    .setBackgroundColor(color(255, 120))
    .setBackgroundHeight(280)
    .setLabel("Bounds_1")
    ;

  cp5.addSlider2D("position_1")
    .setPosition(x, x)
    .setSize(w, w)
    .setMinMax(0.0, 0, 100.0, 100.0)
    .setValue(50, 50)
    .setGroup(params_bounds)
    ;

  cp5.addSlider("width_1")
    .setPosition(x, y+w+5 + 0*y_offset)
    .setSize(w, h)
    .setMin(0)
    .setMax(1)
    .setGroup(params_bounds)
    ;

  cp5.addSlider("height_1")
    .setPosition(x, y+w+5 + 1*y_offset)
    .setSize(w, h)
    .setMin(0)
    .setMax(1)
    .setGroup(params_bounds)
    ;

  cp5.addSlider("depth_1")
    .setPosition(x, y+w+5 + 2*y_offset)
    .setSize(w, h)
    .setMin(0)
    .setMax(1)
    .setGroup(params_bounds)
    ;

  Group params_bounds_0 = cp5.addGroup("params_bounds_0")
    .setPosition(x+380, y + 10 + 300)
    .setWidth(210)
    .setHeight(15)
    .activateEvent(true)
    .setBackgroundColor(color(255, 120))
    .setBackgroundHeight(280)
    .setLabel("Bounds_0")
    ;

  cp5.addSlider2D("position_0")
    .setPosition(x, x)
    .setSize(w, w)
    .setMinMax(0.0, 0, 100.0, 100.0)
    .setValue(50, 50)
    .setGroup(params_bounds_0)
    ;

  cp5.addSlider("width_0")
    .setPosition(x, y+w+5 + 0*y_offset)
    .setSize(w, h)
    .setMin(0)
    .setMax(1)
    .setGroup(params_bounds_0)
    ;

  cp5.addSlider("height_0")
    .setPosition(x, y+w+5 + 1*y_offset)
    .setSize(w, h)
    .setMin(0)
    .setMax(1)
    .setGroup(params_bounds_0)
    ;

  cp5.addSlider("depth_0")
    .setPosition(x, y+w+5 + 2*y_offset)
    .setSize(w, h)
    .setMin(0)
    .setMax(1)
    .setGroup(params_bounds_0)
    ;
}
