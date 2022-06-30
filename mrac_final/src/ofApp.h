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
        void on_touchosc_msg(ofxOscMessage m);
        void on_touchdesigner_msg(ofxOscMessage m);
        void on_unity_msg(ofxOscMessage m);
        void on_blender_msg(ofxOscMessage m);
        void on_gh_msg(ofxOscMessage m);
        void on_kinect_msg(ofxOscMessage m);
    
        
        // Control & Interaction
        ofxGizmo tcp_target;
        ofxGizmo look_at_target;
        AgentController agents;
        void keypressed_gizmo(int key);
        void setup_gizmos();
        
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
    
        // Sensors
        ofNode sensor;
        bool use_kinect = false;
        vector<ofNode> joints;
        ofxGizmo sensor_gizmo;
        void setup_skeleton();
        void draw_skeleton();
    
        bool use_vive = true;
        vector<ofNode> trackers;
    
        // Motion
        glm::vec3 get_closest_streamed_position();
        glm::vec3 get_closest_joint_position();
        glm::vec3 motion_lerp_pt;
        glm::vec3 motion_base_offset_pt;
        void update_motion_behaviors();
        void draw_motion_behaviors();
    
        enum K4ABT_JOINT {
            PELVIS,
            SPINE_NAVAL,
            SPINE_CHEST,
            NECK,
            CLAVICLE_LEFT,
            SHOULDER_LEFT,
            ELBOW_LEFT,
            WRIST_LEFT,
            HAND_LEFT,
            HANDTIP_LEFT,
            THUMB_LEFT,
            CLAVICLE_RIGHT,
            SHOULDER_RIGHT,
            ELBOW_RIGHT,
            WRIST_RIGHT,
            HAND_RIGHT,
            HANDTIP_RIGHT,
            THUMB_RIGHT,
            HIP_LEFT,
            KNEE_LEFT,
            ANKLE_LEFT,
            FOOT_LEFT,
            HIP_RIGHT,
            KNEE_RIGHT,
            ANKLE_RIGHT,
            FOOT_RIGHT,
            HEAD,
            NOSE,
            EYE_LEFT,
            EAR_LEFT,
            EYE_RIGHT,
            EAR_RIGHT
        };
        
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
    
        ofxPanel panel_sensor;
        ofParameter<bool> use_sensor;
        ofParameter<bool> show_sensor_gizmo;
        ofParameter<ofVec3f> sensor_position;
        ofParameter<ofVec4f> sensor_orientation;
    
        ofParameter<bool> mads_mbp;
        ofParameter<bool> unity_boids;
        ofParameter<bool> unity_leap;
        ofParameter<bool> unity_camera;
        ofParameter<bool> td_dance;
    
        void on_mads_mbp(bool &val);
        void on_unity_boids(bool &val);
        void on_unity_leap(bool &val);
        void on_unity_camera(bool &val);
        void on_td_dance(bool &val);
    
        void on_sensor_position_moved(ofVec3f & val);
        void on_sensor_orientation_moved(ofVec4f & val);
    
        ofxPanel panel_motion;
        ofParameter<bool> use_look_at;
        ofParameter<bool> use_agent;
        ofParameter<float> motion_lerp;
        ofParameter<float> motion_base_offset;
        
        ofxPanel panel_robot;
        ofParameter<bool> use_osc;
        ofParameter<bool> robot_live;
        void draw_live_robot_warning();
        void on_use_look_at(bool & val);

        ofColor background_inner = ofColor(238);
        ofColor background_outer = ofColor(118);
};
