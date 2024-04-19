/**
 *  EGM Server Example Physics2D
 *
 *  Processing for moving ABB robo based on bouncing ball.
 *  See documentation here: https://github.com/madelinegannon/personable_robotics
 *
 *  Library Dependencies:
 *    - oscP5 (https://sojamo.de/libraries/oscP5)
 *    - controlP5 (https://sojamo.de/libraries/controlP5)
 *    - box2D (https://github.com/shiffman/Box2D-for-Processing)
 *
 *  Madeline Gannon | atonaton.com
 *  04.17.2024
 */

import netP5.*;
import oscP5.*;
import controlP5.*;
import shiffman.box2d.*;
import org.jbox2d.collision.shapes.*;
import org.jbox2d.common.*;
import org.jbox2d.dynamics.*;
import org.jbox2d.dynamics.joints.*;

OscP5 oscP5;
NetAddress server_addr;

ControlP5 cp5;

// Change these to match the server osc settings
String host = "127.0.0.1";   // ip address of the server
int port_send = 55556;       // receiving port of the server
int port_receive = 55555;    // sending port of the server

// Setup the bounds
float bounds_x = 0;
float bounds_y = 0;
float bounds_width = 0;
float bounds_height = 0;

boolean is_inside = false;

// Physics variables
Box2DProcessing box2d;
ArrayList<Boundary> boundaries = new ArrayList<Boundary>();
ArrayList<Spring> springs = new ArrayList<Spring>();
Box box;
boolean play_physics = true;

void setup() {
  size(800, 1000);
  oscP5 = new OscP5(this, port_receive);
  server_addr = new NetAddress(host, port_send);
  setup_gui();

  // Create the safety boundary rectangle
  bounds_width = width/2;
  bounds_height = bounds_width;
  bounds_x = width/2 - bounds_width/2;
  bounds_y = height/2 - bounds_width/3;

  setup_physics();
}

void draw() {
  background(0);
  float x = mouseX;
  float y = mouseY;

  if (play_physics){
    box2d.step();
    x = box2d.getBodyPixelCoord(box.body).x;
    y = box2d.getBodyPixelCoord(box.body).y;
  }

  box.display();
  for (Boundary boundary : boundaries) {
    boundary.display();
  }
  
  for (Spring spring : springs) {
    spring.update(mouseX,mouseY);
    spring.display();
  }


  // move the koriobot's target with the mouse if we are inside the safety bounds
  if (is_inside(x, y)) {
    // normalize the mouse position based on the safety bounds
    float _x = map(x, bounds_x, bounds_x+bounds_width, 1, 0);
    float _y = map(y, bounds_y, bounds_y+bounds_height, 1, 0);
    OscMessage msg = new OscMessage("/motion/pos");
    msg.add(0);    // use the -1 index to move all koriobots
    msg.add(_x);
    msg.add(_y);
    oscP5.send(msg, server_addr);

    // highlight the bounds when the mouse is inside
    fill(250, 0, 250, 80);
    rect(bounds_x, bounds_y, bounds_width, bounds_height);
    fill(255, 0, 255);
    ellipse(x, y, 10, 10);
    line(x, bounds_y, x, bounds_y+bounds_height);
    line(bounds_x, y, bounds_x+bounds_width, y);
    if (mousePressed) {
      fill(255, 60);
      ellipse(x, y, 25, 25);
    }
  }

  // draw the bounds
  noFill();
  stroke(250, 0, 250);
  rect(bounds_x, bounds_y, bounds_width, bounds_height);

}

void setup_physics() {
  // Initialize box2d physics and create the world
  box2d = new Box2DProcessing(this);
  box2d.createWorld();
  box2d.setGravity(0, -20);

  int wall_width = 10;
  boundaries.add(new Boundary(width/2, bounds_y+bounds_height + wall_width/2, width/2, wall_width)); // bottom
  boundaries.add(new Boundary(width/2, bounds_y+bounds_height + wall_width/2, width/2, wall_width)); // top
  boundaries.add(new Boundary(bounds_x - wall_width/2, bounds_y+bounds_height/2 + wall_width/2, wall_width, bounds_height + wall_width)); // left
  boundaries.add(new Boundary(bounds_x+bounds_width + wall_width/2, bounds_y+bounds_height/2 + wall_width/2, wall_width, bounds_height + wall_width)); // left
  
  box = new Box(width/2,height/2);
  springs.add(new Spring());
}

/**
 *  Check if the mouse is inside the bounds rectangle.
 */
void mouseMoved() {
  is_inside = (mouseX > bounds_x) & (mouseX < bounds_x+bounds_width) & (mouseY > bounds_y) & (mouseY < bounds_y+bounds_height);
}

boolean is_inside(float x, float y){
  return (x > bounds_x) & (x < bounds_x+bounds_width) & (y > bounds_y) & (y < bounds_y+bounds_height);
}

/**
 *  Move the koriobots if we are pressing the mouse button.
 */
void mousePressed() {
  if (is_inside) {
    OscMessage msg = new OscMessage("/motion/enable");
    msg.add(true);
    //oscP5.send(msg, server_addr);
  }
  
  springs.get(0).bind(mouseX,mouseY, box);//bounds_x, bounds_y);
}

/**
 *  Stop moving the koriobots if we released the mouse button.
 */
void mouseReleased() {
  if (is_inside) {
    OscMessage msg = new OscMessage("/motion/enable");
    msg.add(false);
    //oscP5.send(msg, server_addr);
  }
  springs.get(0).destroy();
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
  int x_offset = 25;//int(width / 2);
  int y_offset = 30;

  Group params = cp5.addGroup("params")
    .setPosition(x_offset, y + 10)
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
    .setPosition(x_offset+160, y + 10)
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
    .setPosition(x_offset+380, y + 10)
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
    .setPosition(x_offset+160, y + 10 + 300)
    .setWidth(210)
    .setHeight(15)
    .activateEvent(true)
    .setBackgroundColor(color(255, 120))
    .setBackgroundHeight(280)
    .setLabel("Bounds_1")
    .setOpen(false)
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
    .setPosition(x_offset+380, y + 10 + 300)
    .setWidth(210)
    .setHeight(15)
    .activateEvent(true)
    .setBackgroundColor(color(255, 120))
    .setBackgroundHeight(280)
    .setLabel("Bounds_0")
    .setOpen(false)
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
