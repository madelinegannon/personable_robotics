#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxGizmo.h"
#include "RobotController.h"
#include "ofxOsc.h"
#include "agent/AgentController.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
        // Robot
        ofxRobotArm::RobotController robot;
        RobotType robot_type = RobotType::UR10;
        void keypressed_robot(int key);
    
        // Communications
        ofxOscReceiver receiver;
        int receive_port = 55555;
        void check_for_msg();
        
        // Control & Interaction
        ofxGizmo tcp_target;
        ofxGizmo look_at_target;
        AgentController agents;
        void keypressed_gizmo(int key);
    
        // Safety bounds
        ofxPanel panel_safety;
        ofParameter<bool> show_bounds;
        ofParameterGroup params_safety;
        ofParameter<ofVec3f> aabb_pos, aabb_bounds;
        void setup_bounds();
        bool isInside(ofVec3f point, ofVec3f box_pos, ofVec3f box_bounds);
        void draw_safety_bounds();
        
        // Scene
        void setup_scene();
        void update_scene();
        void draw_scene();
        
        ofEasyCam cam;
        void setup_camera();
        void keypressed_camera(int key);
        bool disable_camera();
        
        // GUI
        void setup_gui();
        void draw_gui();
        
        ofxPanel panel;
        ofParameterGroup params;
        ofParameter<bool> show_gui, show_top, show_front, show_side, show_perspective;
        void listener_show_top(bool & val);
        void listener_show_front(bool & val);
        void listener_show_side(bool & val);
        void listener_show_perspective(bool & val);
    
        ofxPanel panel_motion;
        ofParameter<bool> use_look_at;
        ofParameter<bool> use_agent;
        
        ofxPanel panel_robot;
        ofParameter<bool> use_osc;
        ofParameter<bool> teleport;
        ofParameter<bool> robot_live;
        void draw_live_robot_warning();
        void on_use_look_at(bool & val);
        void on_teleport(bool & val);

        ofColor background_inner = ofColor(238);
        ofColor background_outer = ofColor(118);
};
