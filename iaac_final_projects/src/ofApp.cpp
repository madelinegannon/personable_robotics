#include "ofApp.h"

//#define PRINT_POSE

//--------------------------------------------------------------
void ofApp::setup(){
    
    // setup scene
    setup_scene();
    
    // setup robot
    string ip_address = "192.168.125.124"; // change this to your robot's IP address
    bool offline = true; // change this to false when connected to the robot
    robot.setup(ip_address, robot_type, offline);
    
    // setup osc communications
    receiver.setup(receive_port);

    // setup gui
    setup_gui();
    
    // start robot
    robot.start();
    
    show_side.set(true);
}

//--------------------------------------------------------------
void ofApp::update(){
    
//    // receive external positions over OSC (should be in mm)
//    if (use_osc){
//        check_for_msg();
//    }
    
    // update the lookat point (TODO: should be inside check for message
    if (use_sensor){
        ofNode temp;
        if (use_kinect)
            temp.setGlobalPosition(get_closest_joint_position());
        if (use_vive)
            temp.setGlobalPosition(get_closest_streamed_position());
        // don't need to worry about orientation
        look_at_target.setNode(temp); ;
        
        // update the tcp_target based on the sensor input
        update_motion_behaviors();
    }
    
    // receive external positions over OSC (should be in mm)
    if (use_osc){
        check_for_msg();
    }
    
    // if we want to use a look_at target for the orientation
    if (use_look_at){
        ofNode temp;
        temp.setGlobalPosition(tcp_target.getTranslation());
        temp.lookAt(look_at_target.getTranslation(), ofVec3f(0,-1,0));
        temp.rotateDeg(90, temp.getYAxis());
        temp.setGlobalPosition(tcp_target.getTranslation());
        tcp_target.setNode(temp);
    }
    
    // Move the Robot
    // Use either the agent position or tcp_target pose
    ofNode tcp;
    if (use_agent){
        
        tcp.setGlobalPosition(tcp_target.getTranslation());
        tcp.setGlobalOrientation(tcp_target.getRotation());
        agents.set_target(0, tcp);
        agents.update();
        
        tcp.setGlobalPosition(agents.get_pose(0).getGlobalPosition());
        tcp.setGlobalOrientation(agents.get_pose(0).getGlobalOrientation());
        
        // Get the new orientation from the agent position to the look at target
        if (use_look_at){
            ofNode temp;
            temp.setGlobalPosition(tcp.getGlobalPosition());
            temp.lookAt(look_at_target.getTranslation(), ofVec3f(0,-1,0));
            temp.rotateDeg(90, temp.getYAxis());
            temp.setGlobalPosition(tcp.getGlobalPosition());
            
            tcp.setGlobalPosition(temp.getGlobalPosition());
            tcp.setGlobalOrientation(temp.getGlobalOrientation());
        }
    }
    else{
        tcp.setGlobalPosition(tcp_target.getTranslation());
        tcp.setGlobalOrientation(tcp_target.getRotation());
    }
    
#ifdef PRINT_POSE
    // debugging the actual vs sim arm's joint positions:
    auto pose = robot.getCurrentPose();
    cout << "Current Pose:\t[";
    for (auto p : pose){
        cout << ofRadToDeg(p) << ", ";
    }
    cout << "]" << endl;
#endif
    
    // Check if the target tcp is within the safety box
    ofVec3f pt;
    if (use_agent){
        pt = agents.get_pose(0).getGlobalPosition();
    }
    else{
        pt = toOf(tcp_target.getTranslation());
    }
    if (isInside(pt, aabb_pos.get(), aabb_bounds.get()))
    {
        // update robot
        robot.set_desired(tcp);
        robot.update();
    }
        
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient(background_inner, background_outer, OF_GRADIENT_CIRCULAR);
    
    // Draw 3D Scene
    draw_scene();
    
    // Draw 2D Information
    if (show_gui){
        draw_gui();
    }
    // if robot is LIVE, draw indicator
    if (robot_live){
        draw_live_robot_warning();
    }
}

#pragma mark - Communications
//--------------------------------------------------------------
void ofApp::check_for_msg(){
    
    // check for waiting messages
    while(receiver.hasWaitingMessages()){
        
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
//        if (m.getAddress() == "/tcp_target_unity" || m.getAddress() == "/lookat_target_unity"){
//            cout << "BOIDS!" << endl;
//        }
//        else if (m.getAddress() == "/tcp_target_leap" || m.getAddress() == "/lookat_target_leap"){
//            cout << "LEAPers!" << endl;
//        }
//        else if (m.getAddress() == "/multixy/1" || m.getAddress() == "/multixy/2" ||
//            m.getAddress() == "/tcp_target_touch_osc" || m.getAddress() == "/look_at_target_touch_osc" ||
//            m.getAddress() == "/motion_lerp_touch_osc" || m.getAddress() == "/base_offset_touch_osc" ||
//            m.getAddress() == "/tcp_target_y_touch_osc" || m.getAddress() == "/look_at_target_y_touch_osc" ||
//            m.getAddress() == "/button_0" || m.getAddress() == "/button_1" || m.getAddress() == "/button_2" || m.getAddress() == "/button_3"){
//            cout << "TOUCH_OSC!" << endl;
//        }
//        else if (m.getAddress() == "/_samplerate" || m.getAddress() == "/v1" || m.getAddress() == "/v2" || m.getAddress() == "/v3"){
//            cout << "TouchDesigner!" << endl;
//        }
//        else if (m.getAddress() == "/0/tcp_target" || m.getAddress() == "/0/look_at_target"){
//            cout << "Grasshopper!" << endl;
//        }

        // check move target messages
        if(m.getAddress() == "/tcp_target"){
            float x = m.getArgAsFloat(0);
            float y = m.getArgAsFloat(1);
            float z = m.getArgAsFloat(2);
            ofNode temp;
            temp.setGlobalPosition(x, y, z);
            temp.setGlobalOrientation(tcp_target.getRotation());
            tcp_target.setNode(temp);
        }
        else if (m.getAddress() == "/look_at_target"){
            float x = m.getArgAsFloat(0);
            float y = m.getArgAsFloat(1);
            float z = m.getArgAsFloat(2);
            ofNode temp;
            temp.setGlobalPosition(x, y, z);
            temp.setGlobalOrientation(tcp_target.getRotation());
            look_at_target.setNode(temp);
        }
        else if(m.getAddress() == "/motion_lerp"){
            float t = m.getArgAsFloat(0);   // value should be in range {0,1}
            motion_lerp.set(t);
        }
        else if(m.getAddress() == "/motion_base_offset"){
            float t = m.getArgAsFloat(0);   // value should be in range {0,1}
            t = ofMap(t, 0, 1, motion_base_offset.getMin(), motion_base_offset.getMax());
            motion_base_offset.set(t);
        }
        // Message from Blender
        else if (m.getAddress() == "/tcp_target_blender" || m.getAddress() == "/look_at_target_blender" ){
            on_blender_msg(m);
        }
        // Message from TouchOSC Example template
        else if(m.getAddress() == "/multixy/1" || m.getAddress() == "/multixy/2" ||
                m.getAddress() == "/tcp_target_touch_osc" || m.getAddress() == "/look_at_target_touch_osc" ||
                m.getAddress() == "/motion_lerp_touch_osc" || m.getAddress() == "/base_offset_touch_osc" ||
                m.getAddress() == "/tcp_target_y_touch_osc" || m.getAddress() == "/look_at_target_y_touch_osc" ||
                m.getAddress() == "/button_0" || m.getAddress() == "/button_1" || m.getAddress() == "/button_2" || m.getAddress() == "/button_3"){
            on_touchosc_msg(m);
        }
        // Message from TouchOSC Example template
        else if (m.getAddress() == "/_samplerate" || m.getAddress() == "/v1" || m.getAddress() == "/v2" || m.getAddress() == "/v3"){
            on_touchdesigner_msg(m);
        }
        // HTC Vive Puck Example
        else if (m.getAddress() == "/trackedObjects_Tracker1" || m.getAddress() == "/trackedObjects_Tracker2" || m.getAddress() == "/trackedObjects_Tracker3"|| m.getAddress() == "/tcp_target_unity" || m.getAddress() == "/lookat_target_unity" || m.getAddress() == "/tcp_target_leap" || m.getAddress() == "/lookat_target_leap"){
            on_unity_msg(m);
        }
        else if (m.getAddress() == "/0/tcp_target" || m.getAddress() == "/0/look_at_target"){
            on_gh_msg(m);
        }
        /*
        // Azure Kinect Example
        else if (m.getAddress() == "/joint" || m.getAddress() == "/skeleton"){
            on_kinect_msg(m);
        }*/
        // unrecognized message
        else{
            string msgString;
            msgString = m.getAddress();
            msgString += ":";
            for(size_t i = 0; i < m.getNumArgs(); i++){

                // get the argument type
                msgString += " ";
                msgString += m.getArgTypeName(i);
                msgString += ":";

                // display the argument - make sure we get the right type
                if(m.getArgType(i) == OFXOSC_TYPE_INT32){
                    msgString += ofToString(m.getArgAsInt32(i));
                }
                else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
                    msgString += ofToString(m.getArgAsFloat(i));
                }
                else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
                    msgString += m.getArgAsString(i);
                }
                else{
                    msgString += "unhandled argument type " + m.getArgTypeName(i);
                }
            }
            cout << msgString << endl;
        }
    }
}

void ofApp::on_touchosc_msg(ofxOscMessage m){
    
    if(m.getAddress() == "/multixy/1" || m.getAddress() == "/tcp_target_touch_osc"){
        float x = m.getArgAsFloat(0);
        float y = tcp_target.getTranslation().y;
        float z = m.getArgAsFloat(1);
        
        float min_x = aabb_pos.get().x - aabb_bounds.get().x/2;
        float max_x = aabb_pos.get().x + aabb_bounds.get().x/2;
        float min_z = aabb_pos.get().z - aabb_bounds.get().z/2;
        float max_z = aabb_pos.get().z + aabb_bounds.get().z/2;
        
        x = ofMap(x, 0, 1, min_x, max_x, true);
        z = ofMap(z, 0, 1, min_z, max_z, true);
        
        ofNode temp;
        temp.setGlobalPosition(x, y, z);
        temp.setGlobalOrientation(tcp_target.getRotation());
        tcp_target.setNode(temp);
    }
    else if(m.getAddress() == "/multixy/2" || m.getAddress() == "/look_at_target_touch_osc"){
        float x = m.getArgAsFloat(0);
        float y = look_at_target.getTranslation().y;
        float z = m.getArgAsFloat(1);
        
        float offset = 500;
        
        float min_x = aabb_pos.get().x - aabb_bounds.get().x/2;
        float max_x = aabb_pos.get().x + aabb_bounds.get().x/2;
        float min_z = aabb_pos.get().z - aabb_bounds.get().z/2;
        float max_z = aabb_pos.get().z + aabb_bounds.get().z/2;
        
        x = ofMap(x, 0, 1, min_x-offset, max_x+offset, true);
        z = ofMap(z, 0, 1, min_z-offset, max_z+offset, true);
        
        ofNode temp;
        temp.setGlobalPosition(x, y, z);
        temp.setGlobalOrientation(look_at_target.getRotation());
        look_at_target.setNode(temp);
    }
    else if(m.getAddress() == "/tcp_target_y_touch_osc"){
        float y = m.getArgAsFloat(0);
        
        float min = aabb_pos.get().y - aabb_bounds.get().y/2;
        float max = aabb_pos.get().y + aabb_bounds.get().y/2;
        
        auto val = ofMap(m.getArgAsFloat(0), 0, 1, min, max, true);
        
        ofNode temp;
        temp.setGlobalPosition(tcp_target.getTranslation().x, val, tcp_target.getTranslation().z);
        temp.setGlobalOrientation(tcp_target.getRotation());
        tcp_target.setNode(temp);
    }
    else if(m.getAddress() == "/look_at_target_y_touch_osc"){
        float y = m.getArgAsFloat(0);
        
        float min = aabb_pos.get().y - aabb_bounds.get().y/2;
        float max = aabb_pos.get().y + aabb_bounds.get().y/2;
        
        auto val = ofMap(m.getArgAsFloat(0), 0, 1, min, max, true);
        
        ofNode temp;
        temp.setGlobalPosition(look_at_target.getTranslation().x, val, look_at_target.getTranslation().z);
        temp.setGlobalOrientation(look_at_target.getRotation());
        look_at_target.setNode(temp);
    }
    else if (m.getAddress() == "/motion_lerp_touch_osc"){
        float t = m.getArgAsFloat(0);   // value should be in range {0,1}
        motion_lerp.set(t);
    }
    else if (m.getAddress() == "/base_offset_touch_osc"){
        float t = m.getArgAsFloat(0);   // incoming value is range {0,1}; make {-1,1}
        t = ofMap(t, 0, 1, motion_base_offset.getMin(), motion_base_offset.getMax());
        motion_base_offset.set(t);
    }
    else if (m.getAddress() == "/button_0"){
        float t = m.getArgAsFloat(0);
        if (t == 1.0){
            float offset = 200;
            float min = aabb_pos.get().y - aabb_bounds.get().y/2;
            float max = aabb_pos.get().y + aabb_bounds.get().y/2;
            
            auto y = tcp_target.getTranslation().y + offset;
            auto val = ofClamp(y, min, max);
            
            ofNode temp;
            temp.setGlobalPosition(tcp_target.getTranslation().x, val, tcp_target.getTranslation().z);
            temp.setGlobalOrientation(tcp_target.getRotation());
            tcp_target.setNode(temp);
        }
    }
    else if (m.getAddress() == "/button_1"){
        float t = m.getArgAsFloat(0);
        if (t == 1.0){
            float offset = 200;
            float min = aabb_pos.get().z - aabb_bounds.get().z/2;
            float max = aabb_pos.get().z + aabb_bounds.get().z/2;
            
            auto z = tcp_target.getTranslation().z - offset;
            auto val = ofClamp(z, min, max);
            
            ofNode temp;
            temp.setGlobalPosition(tcp_target.getTranslation().x,  tcp_target.getTranslation().y, val);
            temp.setGlobalOrientation(tcp_target.getRotation());
            tcp_target.setNode(temp);
        }
    }
    else if (m.getAddress() == "/button_2"){
        float t = m.getArgAsFloat(0);
        if (t == 1.0){
            float offset = 200;
            float min = aabb_pos.get().y - aabb_bounds.get().y/2;
            float max = aabb_pos.get().y + aabb_bounds.get().y/2;
            
            auto y = tcp_target.getTranslation().y - offset;
            auto val = ofClamp(y, min, max);
            
            ofNode temp;
            temp.setGlobalPosition(tcp_target.getTranslation().x, val, tcp_target.getTranslation().z);
            temp.setGlobalOrientation(tcp_target.getRotation());
            tcp_target.setNode(temp);
        }
    }
    else if (m.getAddress() == "/button_3"){
        float t = m.getArgAsFloat(0);
        if (t == 1.0){
            float offset = 200;
            float min = aabb_pos.get().z - aabb_bounds.get().z/2;
            float max = aabb_pos.get().z + aabb_bounds.get().z/2;
            
            auto z = tcp_target.getTranslation().z + offset;
            auto val = ofClamp(z, min, max);
            
            ofNode temp;
            temp.setGlobalPosition(tcp_target.getTranslation().x,  tcp_target.getTranslation().y, val);
            temp.setGlobalOrientation(tcp_target.getRotation());
            tcp_target.setNode(temp);
        }
    }
}

void ofApp::on_touchdesigner_msg(ofxOscMessage m){
    if(m.getAddress() == "/_samplerate"){
        // do nothing
    }
    else if(m.getAddress() == "/v1"){
        float min = aabb_pos.get().x - aabb_bounds.get().x/2;
        float max = aabb_pos.get().x + aabb_bounds.get().x/2;
        
        auto val = ofMap(m.getArgAsFloat(0), 0, 1, min, max, true);
        
        ofNode temp;
        temp.setGlobalPosition(val, tcp_target.getTranslation().y, tcp_target.getTranslation().z);
        temp.setGlobalOrientation(tcp_target.getRotation());
        tcp_target.setNode(temp);
    }
    else if(m.getAddress() == "/v2"){
        float min = aabb_pos.get().y - aabb_bounds.get().y/2;
        float max = aabb_pos.get().y + aabb_bounds.get().y/2;
        
        auto val = ofMap(m.getArgAsFloat(0), 0, 1, min, max, true);
        
        ofNode temp;
        temp.setGlobalPosition(tcp_target.getTranslation().x, val, tcp_target.getTranslation().z);
        temp.setGlobalOrientation(tcp_target.getRotation());
        tcp_target.setNode(temp);
    }
    else if(m.getAddress() == "/v3"){
        float min = aabb_pos.get().z - aabb_bounds.get().z/2;
        float max = aabb_pos.get().z + aabb_bounds.get().z/2;
        
        auto val = ofMap(m.getArgAsFloat(0), 0, 1, min, max, true);
        
        ofNode temp;
        temp.setGlobalPosition(tcp_target.getTranslation().x, tcp_target.getTranslation().y, val);
        temp.setGlobalOrientation(tcp_target.getRotation());
        tcp_target.setNode(temp);
    }
}

void ofApp::on_blender_msg(ofxOscMessage m){
    if (m.getAddress() == "/tcp_target_blender"){
        auto m_to_mm = 1000.0;
        float x = m.getArgAsFloat(0) * m_to_mm;
        float y = m.getArgAsFloat(1) * m_to_mm;
        float z = m.getArgAsFloat(2) * m_to_mm;
        
        ofNode temp;
        temp.setGlobalPosition(x, y, z);
        temp.setGlobalOrientation(tcp_target.getRotation());
        tcp_target.setNode(temp);
    }
    else if (m.getAddress() == "/look_at_target_blender"){
        auto m_to_mm = 1000.0;
        float x = m.getArgAsFloat(0) * m_to_mm;
        float y = m.getArgAsFloat(1) * m_to_mm;
        float z = m.getArgAsFloat(2) * m_to_mm;
        
        ofNode temp;
        temp.setGlobalPosition(x, y, z);
        temp.setGlobalOrientation(look_at_target.getRotation());
        look_at_target.setNode(temp);
    }
}

void ofApp::on_unity_msg(ofxOscMessage m){
    
    if (m.getAddress() == "/trackedObjects_Tracker1"){
        auto m_to_mm = 1000.0;
        float x = m.getArgAsFloat(0) * m_to_mm;
        float y = m.getArgAsFloat(1) * m_to_mm;
        float z = m.getArgAsFloat(2) * m_to_mm;
        
        trackers[0].setPosition(x, y, z); // <-- don't use Global Position
//            cout << "Tracker 0: " << trackers[0].getPosition() << endl;
    }
    else if (m.getAddress() == "/trackedObjects_Tracker2"){
        auto m_to_mm = 1000.0;
        float x = m.getArgAsFloat(0) * m_to_mm;
        float y = m.getArgAsFloat(1) * m_to_mm;
        float z = m.getArgAsFloat(2) * m_to_mm;
        
        trackers[1].setPosition(x, y, z); // <-- don't use Global Position
//            cout << "Tracker 1: " << trackers[1].getPosition() << endl;
    }
    else if (m.getAddress() == "/trackedObjects_Tracker3"){
        auto m_to_mm = 1000.0;
        float x = m.getArgAsFloat(0) * m_to_mm;
        float y = m.getArgAsFloat(1) * m_to_mm;
        float z = m.getArgAsFloat(2) * m_to_mm;
        
        trackers[2].setPosition(x, y, z); // <-- don't use Global Position
//            cout << "Tracker 2: " << trackers[2].getPosition() << endl;
    }
    else if (m.getAddress() == "/tcp_target_unity"){
        auto m_to_mm = 1000.0;
        float x = m.getArgAsFloat(0) * m_to_mm;
        float y = m.getArgAsFloat(1) * m_to_mm;
        float z = m.getArgAsFloat(2) * m_to_mm;
        
        ofNode temp;
        temp.setGlobalPosition(x, y, z);
        temp.setGlobalOrientation(tcp_target.getRotation());
        tcp_target.setNode(temp);
    }
    else if (m.getAddress() == "/lookat_target_unity"){
        auto m_to_mm = 1000.0;
        float x = m.getArgAsFloat(0) * m_to_mm;
        float y = m.getArgAsFloat(1) * m_to_mm;
        float z = m.getArgAsFloat(2) * m_to_mm;
        
        ofNode temp;
        temp.setGlobalPosition(x, y, z);
        temp.setGlobalOrientation(look_at_target.getRotation());
        look_at_target.setNode(temp);
    }
    else if (m.getAddress() == "/tcp_target_leap"){
        auto m_to_mm = 1000.0;
        float x = m.getArgAsFloat(0) * m_to_mm;
        float y = m.getArgAsFloat(1) * m_to_mm;
        float z = m.getArgAsFloat(2) * m_to_mm;
        
        ofNode temp;
        temp.setGlobalPosition(x, y, z);
        temp.setGlobalOrientation(tcp_target.getRotation());
        tcp_target.setNode(temp);
    }
    else if (m.getAddress() == "/lookat_target_leap"){
        auto m_to_mm = 1000.0;
        float x = m.getArgAsFloat(0) * m_to_mm;
        float y = m.getArgAsFloat(1) * m_to_mm;
        float z = m.getArgAsFloat(2) * m_to_mm;
        
        ofNode temp;
        temp.setGlobalPosition(x, y, z);
        temp.setGlobalOrientation(look_at_target.getRotation());
        look_at_target.setNode(temp);
    }
}

void ofApp::on_gh_msg(ofxOscMessage m){
    if(m.getAddress() == "/0/tcp_target"){
        float x = m.getArgAsFloat(0);
        float y = m.getArgAsFloat(1);
        float z = m.getArgAsFloat(2);
        ofNode temp;
        temp.setGlobalPosition(x, y, z);
        temp.setGlobalOrientation(tcp_target.getRotation());
        tcp_target.setNode(temp);
    }
    else if (m.getAddress() == "/0/look_at_target"){
        float x = m.getArgAsFloat(0);
        float y = m.getArgAsFloat(1);
        float z = m.getArgAsFloat(2);
        ofNode temp;
        temp.setGlobalPosition(x, y, z);
        temp.setGlobalOrientation(look_at_target.getRotation());
        look_at_target.setNode(temp);
    }
}

void ofApp::on_kinect_msg(ofxOscMessage m){
    if (m.getAddress() == "/joint" ){
        int index = m.getArgAsInt(0);
        float x = m.getArgAsFloat(1);
        float y = m.getArgAsFloat(2);
        float z = m.getArgAsFloat(3);
        float qw = m.getArgAsFloat(4);
        float qx = m.getArgAsFloat(5);
        float qy = m.getArgAsFloat(6);
        float qz = m.getArgAsFloat(7);
        joints[index].setPosition(x, y, z); // <-- don't use Global Position
        joints[index].setGlobalOrientation(glm::quat(qw, qx, qy, qz));
    }
    else if (m.getAddress() == "/skeleton"){
        int body_id = m.getArgAsInt(0);
        int msg_count = 8;
        for(int i = 1; i < m.getNumArgs()-msg_count; i+=msg_count){
            int index = m.getArgAsInt(i);
            float x = m.getArgAsFloat(i+1);
            float y = m.getArgAsFloat(i+2);
            float z = m.getArgAsFloat(i+3);
            float qw = m.getArgAsFloat(i+4);
            float qx = m.getArgAsFloat(i+5);
            float qy = m.getArgAsFloat(i+6);
            float qz = m.getArgAsFloat(i+7);
            joints[index].setPosition(x, y, z); // <-- don't use Global Position
            joints[index].setGlobalOrientation(glm::quat(qw, qx, qy, qz));
        }
    }
}


#pragma mark - Safety
//--------------------------------------------------------------
/// From https://stackoverflow.com/questions/52673935/check-if-3d-point-inside-a-box
bool ofApp::isInside(ofVec3f point, ofVec3f box_pos, ofVec3f box_bounds)
{
    /**
      vec3 center;     // Center of the box.
        vec3 dx, dy, dz; // X,Y, and Z directions, normalized.
        vec3 half;         // Box size in each dimension, divided by 2.

        vec3 point; // Point to test.
        vec3 d = point - center;
        bool inside = abs(dot(d, dx)) <= half.x &&
                  abs(dot(d, dy)) <= half.y &&
                  abs(dot(d, dz)) <= half.z;
     */

    ofVec3f center = box_pos;
    ofVec3f half = box_bounds / 2;
    ofVec3f d = point - center;
    return abs(d.dot(ofVec3f(1, 0, 0))) <= half.x &&
           abs(d.dot(ofVec3f(0, 1, 0))) <= half.y &&
           abs(d.dot(ofVec3f(0, 0, 1))) <= half.z;
}

//--------------------------------------------------------------
void ofApp::draw_safety_bounds(){
    ofPushStyle();
    // Draw the Tracking Boundary Box
    ofVec3f pt;
    if (use_agent){
        pt = agents.get_pose(0).getGlobalPosition();
    }
    else{
        pt = tcp_target.getTranslation();
    }
    if (isInside(pt, aabb_pos.get(), aabb_bounds.get()))
    {
        ofSetColor(ofColor::darkSlateGray, 100);
    }
    else
    {
        ofSetColor(ofColor::red);
    }
    ofNoFill();
    ofDrawBox(aabb_pos.get(), aabb_bounds.get().x, aabb_bounds.get().y, aabb_bounds.get().z);
    ofPopStyle();
}


#pragma mark - Scene
//--------------------------------------------------------------
void ofApp::setup_scene(){
    
    setup_gizmos();
    
    // setup agent used for smoothing tcp motion
    agents.setup(1);
    
    // setup the nodes holding the skeleton information
    setup_skeleton();
    
}

//--------------------------------------------------------------
void ofApp::draw_scene(){
    cam.begin();
    ofDrawAxis(1500);
    
    if (show_bounds){
        draw_safety_bounds();
    }
    
    // Draw Desired Robot
    robot.drawPreview();
    
    // Draw Actual Robot
    robot.draw();
    
    ofPushStyle();
    // Draw the TCP Target Gizmo
    tcp_target.draw(cam);
    // Draw the Look At Target Gizmo
    if (use_look_at){
        ofSetColor(ofColor::cyan, 100);
        ofDrawLine(tcp_target.getTranslation(), look_at_target.getTranslation());
        look_at_target.draw(cam);
    }
    // Draw the Sensor Gizmo
    if (show_sensor_gizmo){
        ofPushMatrix();
        ofTranslate(sensor_gizmo.getTranslation());
        ofDrawBitmapString("Sensor", 10, 10);
        ofPopMatrix();
        sensor_gizmo.draw(cam);
        sensor.draw(); // parent node of the sensor
    }
    
    // Draw Skeleton
    if (use_sensor){
        draw_motion_behaviors();
        draw_skeleton();
    }
    
    // Draw the Agent
    if (use_agent){
        agents.draw();
    }
    
    ofPopStyle();
    
    cam.end();
}

//--------------------------------------------------------------
void ofApp::setup_gizmos(){
    // setup default tcp target position and orientation
    ofNode temp;
    temp.setGlobalPosition(500, 0, 350);
    tcp_target.setNode(temp);
    
    // setup default look at target position and orientation
    temp = ofNode();
    temp.setGlobalPosition(tcp_target.getTranslation().x + 1000, tcp_target.getTranslation().y, tcp_target.getTranslation().z - 5);
    look_at_target.setNode(temp);
    
    // setup default sensor gizmo position and orientation
    temp = ofNode();
    temp.setGlobalPosition(0, 0, 0);                    // update with calibrated sensor position
    temp.setGlobalOrientation(glm::quat(0, 0, 0, 1));   // update with calibrated sensor position
    sensor_gizmo.setNode(temp);
}

//--------------------------------------------------------------
void ofApp::setup_skeleton(){
    
    sensor = ofNode();
    joints.resize(K4ABT_JOINT::EAR_RIGHT);
    
    int i=2;
    for (auto &j : joints){
        j = ofNode();
        j.setParent(sensor);
        j.setPosition(i*50, 0, 0);
        i++;
    }
    
    // HTC Vive Tracker (still nest under sensor node)
    trackers.resize(3);
    i=2;
    for (auto &j : trackers){
        j = ofNode();
        j.setParent(sensor);
        j.setPosition(i*50, 0, 0);
        i++;
    }
}

//--------------------------------------------------------------
void ofApp::draw_skeleton(){
    ofPushStyle();
    
    // Azure Kinect Skeleton
    if (use_kinect){
        for (int i=0; i<joints.size(); i++){
            joints[i].draw();
        }
    }
    
    // HTC Vive Trackers
    if (use_vive){
        for (int i=0; i<trackers.size(); i++){
           trackers[i].draw();
        }
    }
    ofPopStyle();
}

//--------------------------------------------------------------
glm::vec3 ofApp::get_closest_streamed_position(){
    auto t1 = &trackers[0];
    auto t2 = &trackers[1];
    auto t3 = &trackers[2];
    
       // return the streamed position that is closest to the robot's base
    
    return t1->getGlobalPosition();
}

//--------------------------------------------------------------
glm::vec3 ofApp::get_closest_joint_position(){
    
    auto neck = &joints[K4ABT_JOINT::NECK];
    auto hand_right = &joints[K4ABT_JOINT::WRIST_RIGHT];
    auto hand_left = &joints[K4ABT_JOINT::WRIST_LEFT];
    
    // return the joint that is closest to the robot's base
    auto dist_neck = glm::length2(neck->getGlobalPosition());
    auto dist_hand_right = glm::length2(hand_right->getGlobalPosition());
    auto dist_hand_left = glm::length2(hand_left->getGlobalPosition());
    
    auto min_dist = min({dist_neck, dist_hand_right, dist_hand_left});
    
    if (min_dist == dist_neck){
        return neck->getGlobalPosition();
    }
    else if (min_dist == dist_hand_right){
        return hand_right->getGlobalPosition();
    }
    else if (min_dist == dist_hand_left){
        return hand_left->getGlobalPosition();
    }
    
    return glm::vec3();
}

//--------------------------------------------------------------
void ofApp::update_motion_behaviors(){
    
    motion_base_offset_pt.z = motion_base_offset.get();
    // TODO: this is incorrect ??? not looking at look_at when base_offset is not zero
    
    ofNode temp_look_at;
    temp_look_at.setGlobalPosition(look_at_target.getTranslation());
    temp_look_at.lookAt(motion_base_offset_pt);
    temp_look_at.setGlobalPosition(motion_base_offset_pt);
    temp_look_at.dolly(1100.0 * motion_lerp);
    motion_lerp_pt = temp_look_at.getGlobalPosition();

    // update the tcp_target
    ofNode temp;
    temp.setGlobalPosition(motion_lerp_pt);
    temp.setGlobalOrientation(tcp_target.getRotation());
    tcp_target.setNode(temp);
}

//--------------------------------------------------------------
void ofApp::draw_motion_behaviors(){
    ofPushStyle();
    ofNode temp_look_at;
    temp_look_at.setGlobalPosition(look_at_target.getTranslation());
    temp_look_at.lookAt(motion_base_offset_pt);
    temp_look_at.setGlobalPosition(motion_base_offset_pt);
    temp_look_at.dolly(1100.0);
    // draw the line from the base_offset to the max end position
    ofSetColor(ofColor::mediumPurple);
    ofDrawLine(motion_base_offset_pt, temp_look_at.getGlobalPosition());
    // draw box at the motion_lerp_pt
    ofSetColor(ofColor::orange);
    ofDrawBox(motion_lerp_pt, 25);
    ofPopStyle();
}

//--------------------------------------------------------------
bool ofApp::disable_camera(){

    if (tcp_target.isInteracting() || look_at_target.isInteracting())
        return true;
    
    if (sensor_gizmo.isInteracting()){
        // update sensor node
        sensor.setGlobalPosition(sensor_gizmo.getTranslation());
        sensor.setGlobalOrientation(sensor_gizmo.getRotation());
        
        // update gui
        sensor_position.set(sensor_gizmo.getTranslation());
        ofVec4f temp;
        temp.x = sensor_gizmo.getRotation().x;
        temp.y = sensor_gizmo.getRotation().y;
        temp.z = sensor_gizmo.getRotation().z;
        temp.w = sensor_gizmo.getRotation().w;
        sensor_orientation.set(temp);
        return true;
    }
    
    ofRectangle gui_rect;
    gui_rect.setX(0);
    gui_rect.setY(0);
    gui_rect.setWidth(panel.getWidth() + 20);
    gui_rect.setHeight(panel.getHeight() + panel_sensor.getHeight() + panel_motion.getHeight() + panel_robot.getHeight() + 20);
    if (gui_rect.inside(mouseX, mouseY))
        return true;
    
    return false;
}

#pragma mark - GUI
//--------------------------------------------------------------
void ofApp::setup_gui(){
    
    params.setName("Navigation");
    params.add(show_gui.set("Show_GUI\t('h')", true));
    params.add(show_top.set("TOP\t\t('1')", true));
    params.add(show_front.set("FRONT\t\t('2')", false));
    params.add(show_side.set("SIDE\t\t('3')", false));
    params.add(show_perspective.set("PERSP\t\t('4')", false));
    
    show_top.addListener(this, &ofApp::listener_show_top);
    show_front.addListener(this, &ofApp::listener_show_front);
    show_side.addListener(this, &ofApp::listener_show_side);
    show_perspective.addListener(this, &ofApp::listener_show_perspective);
    
    panel.setup(params);
    panel.setPosition(10, 10);
    
    panel_safety.setup("Safety_Bounds");
    panel_safety.add(show_bounds.set("Show_Bounds", true));
    params_safety.setName("Safety_Bounds_Params");
    params_safety.add(aabb_pos.set("AABB_Pos", ofVec3f(0, 0, 700), ofVec3f(-2600, -2000, -2000), ofVec3f(2600, 2000, 2000)));
    params_safety.add(aabb_bounds.set("AABB_Bounds", ofVec3f(2600, 800, 1200), ofVec3f(0, 0, 0), ofVec3f(2600, 2600, 1600)));
    panel_safety.add(params_safety);
    panel_safety.setPosition(panel.getPosition().x, panel.getPosition().y + panel.getHeight() + 5);
    
    panel_sensor.setup("Sensor_Parameters");
    panel_sensor.add(use_sensor.set("Use_Sensor_Input", false));
    panel_sensor.add(mads_mbp.set("MADs_MBP", true));
    panel_sensor.add(unity_boids.set("BOIDS", false));
    panel_sensor.add(unity_leap.set("LEAP", false));
    panel_sensor.add(unity_camera.set("CAMERA", false));
    panel_sensor.add(td_dance.set("DANCE", false));
    
    mads_mbp.addListener(this, &ofApp::on_mads_mbp);
    unity_boids.addListener(this, &ofApp::on_unity_boids);
    unity_leap.addListener(this, &ofApp::on_unity_leap);
    unity_camera.addListener(this, &ofApp::on_unity_camera);
    td_dance.addListener(this, &ofApp::on_td_dance);
    
//    panel_sensor.add(
                     show_sensor_gizmo.set("Show_Sensor_Gizmo", false);
//                     );
//    panel_sensor.add(
                     sensor_position.set("Sensor_Position", ofVec3f(), ofVec3f(-2000,-2000,-2000), ofVec3f(2000,2000,2000));
//                     );
//    panel_sensor.add(
                     sensor_orientation.set("Sensor_Orientation", ofVec4f(0,0,0,1),ofVec4f(-1,-1,-1,-1),ofVec4f(1,1,1,1));
//                     );
    panel_sensor.setPosition(panel.getPosition().x, panel_safety.getPosition().y + panel_safety.getHeight() + 5);
    
    sensor_position.addListener(this, &ofApp::on_sensor_position_moved);
    sensor_orientation.addListener(this, &ofApp::on_sensor_orientation_moved);
    
    panel_motion.setup("Motion_Parameters");
    panel_motion.add(use_look_at.set("Use_Look_At\t('l')", true));
    panel_motion.add(use_agent.set("Use_Agent\t('a')", false));
    panel_motion.add(motion_lerp.set("Robot_Motion_Lerp", .5, .35, .95));
    panel_motion.add(motion_base_offset.set("Robot_Motion_Base_Off", 0, -1000, 1000));
    panel_motion.add(agents.params);
    panel_motion.setPosition(panel.getPosition().x, panel_sensor.getPosition().y + panel_sensor.getHeight() + 5);
    
    use_look_at.addListener(this, &ofApp::on_use_look_at);
    
    panel_robot.setup("Robot_Controller");
    panel_robot.add(use_osc.set("Use_OSC \t('o')", false));
    panel_robot.add(robot_live.set("Robot_LIVE\t('m')", false));
    panel_robot.setPosition(panel_motion.getPosition().x, panel_motion.getPosition().y + panel_motion.getHeight() + 5);
    
    ofSetCircleResolution(60);
}

//--------------------------------------------------------------
void ofApp::draw_gui(){
    
    panel_sensor.setPosition(panel.getPosition().x, panel_safety.getPosition().y + panel_safety.getHeight() + 5);
    panel_motion.setPosition(panel.getPosition().x, panel_sensor.getPosition().y + panel_sensor.getHeight() + 5);
    panel_robot.setPosition(panel_motion.getPosition().x, panel_motion.getPosition().y + panel_motion.getHeight() + 5);
    
    panel.draw();
    panel_safety.draw();
    panel_sensor.draw();
    panel_motion.draw();
    panel_robot.draw();
    
    ofDrawBitmapStringHighlight("FPS: "+ofToString(ofGetFrameRate()), ofGetWidth()-100, 10);
}

//--------------------------------------------------------------
void ofApp::draw_live_robot_warning(){
    float alpha = ofMap(sin(ofGetElapsedTimef()*5), -1, 1, 50, 150);
    float line_width = 25;
    ofPushStyle();
    ofSetColor(255, 0, 0, alpha);
    ofSetLineWidth(line_width);
    ofNoFill();
    ofDrawLine(0, line_width/2, ofGetWidth(), line_width/2);
    ofDrawLine(0, ofGetHeight()-line_width/2, ofGetWidth(), ofGetHeight()-line_width/2);
    ofDrawLine(line_width/2, line_width, line_width/2, ofGetHeight()-line_width);
    ofDrawLine(ofGetWidth()-line_width/2, line_width, ofGetWidth()-line_width/2, ofGetHeight()-line_width);
    ofSetColor(255,alpha+50);
    ofDrawBitmapString("WARNING: ROBOT IS LIVE!", ofGetWidth()/2-100, 2*line_width/3);
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::setup_camera(){
    cam.setFarClip(999999999);
    cam.setNearClip(0);
    cam.setDistance(4000);
    ofNode tgt;
    tgt.setGlobalPosition(0, 0, 0);
    tgt.setGlobalOrientation(ofQuaternion(0, 0, 0, 1));
    cam.setTarget(tgt);
    cam.lookAt(ofVec3f(0, 0, -1), ofVec3f(1, 0, 0));
}

//--------------------------------------------------------------
void ofApp::listener_show_top(bool & val)
{
    if (val) {
        
        int x = 0;
        int y = 0;
        int z = 4000;

        
        ofVec3f pos = ofVec3f(x, y, z);
        ofVec3f tgt = ofVec3f(pos.x, pos.y, 0);
        cam.setGlobalPosition(pos);
        cam.setTarget(tgt);
        cam.lookAt(tgt, ofVec3f(1,0,0));
        
        show_top = true;
        show_front = false;
        show_side = false;
        show_perspective = false;
    }
}

//--------------------------------------------------------------
void ofApp::listener_show_front(bool & val)
{
    if (val) {
        
        int x = 2000;
        int y = 0;
        int z = 600;
        
        ofVec3f pos = ofVec3f(x, y, z);
        ofVec3f tgt = ofVec3f(0, pos.y, pos.z);
        cam.setGlobalPosition(pos);
        cam.setTarget(tgt);
        cam.lookAt(tgt, ofVec3f(0, 0, -1));
        
        show_top = false;
        show_front = true;
        show_side = false;
        show_perspective = false;
    }
}

//--------------------------------------------------------------
void ofApp::listener_show_side(bool & val)
{
    if (val) {
        
        int x = 0;
        int y = 4000;
        int z = 600;
        
        ofVec3f pos = ofVec3f(x, y, z);
        ofVec3f tgt = ofVec3f(pos.x, 0, pos.z);
        cam.setGlobalPosition(pos);
        cam.setTarget(tgt);
        cam.lookAt(tgt, ofVec3f(0, 0, -1));
        
        show_top = false;
        show_front = false;
        show_side = true;
        show_perspective = false;
    }
}

//--------------------------------------------------------------
void ofApp::listener_show_perspective(bool & val)
{
    if (val) {
        
        int x = 3000;
        int y = -2000;
        int z = 2000;
        
        ofVec3f pos = ofVec3f(x, y, z);
        ofVec3f tgt = ofVec3f(0, 800 / 2, 0);
        cam.setGlobalPosition(pos);
        cam.setTarget(tgt);
        cam.lookAt(tgt, ofVec3f(0, 0, -1));
        cam.setGlobalPosition(pos);
        
        show_top = false;
        show_front = false;
        show_side = false;
        show_perspective = true;
    }
}

//--------------------------------------------------------------
void ofApp::on_use_look_at(bool & val)
{
    if (val) {
        keypressed_gizmo('w');
    }
}

//--------------------------------------------------------------
void ofApp::on_sensor_position_moved(ofVec3f & val)
{
    if (!sensor_gizmo.isInteracting()){
        ofNode temp;
        temp.setGlobalPosition(val);
        temp.setGlobalOrientation(sensor_gizmo.getRotation());
        sensor_gizmo.setNode(temp);
        
        // update sensor node
        sensor.setGlobalPosition(sensor_gizmo.getTranslation());
        sensor.setGlobalOrientation(sensor_gizmo.getRotation());
    }
}

//--------------------------------------------------------------
void ofApp::on_sensor_orientation_moved(ofVec4f & val)
{
    if (!sensor_gizmo.isInteracting()){
        ofNode temp;
        temp.setGlobalPosition(sensor_gizmo.getTranslation());
        temp.setGlobalOrientation(glm::quat(val.w, val.x, val.y, val.z));
        sensor_gizmo.setNode(temp);
        
        // update sensor node
        sensor.setGlobalPosition(sensor_gizmo.getTranslation());
        sensor.setGlobalOrientation(sensor_gizmo.getRotation());
    }
}

//--------------------------------------------------------------
void ofApp::on_mads_mbp(bool & val)
{
    if (val) {
        receiver.setup(55555);
        cout << "Reciever listening on: " << receiver.getPort() << endl;
//        mads_mbp.set(false);
        unity_boids.set(false);
        unity_leap.set(false);
        unity_camera.set(false);
        td_dance.set(false);
    }
}

//--------------------------------------------------------------
void ofApp::on_unity_boids(bool & val)
{
    if (val) {
        receiver.setup(55550);
        cout << "Reciever listening on: " << receiver.getPort() << endl;
        mads_mbp.set(false);
//        unity_boids.set(false);
        unity_leap.set(false);
        unity_camera.set(false);
        td_dance.set(false);
    }
}

//--------------------------------------------------------------
void ofApp::on_unity_leap(bool & val)
{
    if (val) {
        receiver.setup(55551);
        cout << "Reciever listening on: " << receiver.getPort() << endl;
        mads_mbp.set(false);
        unity_boids.set(false);
//        unity_leap.set(false);
        unity_camera.set(false);
        td_dance.set(false);
    }
}

//--------------------------------------------------------------
void ofApp::on_unity_camera(bool & val)
{
    if (val) {
        receiver.setup(55552);
        cout << "Reciever listening on: " << receiver.getPort() << endl;
        mads_mbp.set(false);
        unity_boids.set(false);
        unity_leap.set(false);
//        unity_camera.set(false);
        td_dance.set(false);
    }
}


//--------------------------------------------------------------
void ofApp::on_td_dance(bool & val)
{
    if (val) {
        receiver.setup(55553);
        cout << "Reciever listening on: " << receiver.getPort() << endl;
        mads_mbp.set(false);
        unity_boids.set(false);
        unity_leap.set(false);
        unity_camera.set(false);
//        td_dance.set(false);
    }
}

#pragma mark - App Functions

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key) {
        case 'f':
            ofToggleFullscreen();
            break;
        case 'o':
        case 'O':
            use_osc = !use_osc;
            break;
        case 'a':
        case 'A':
            use_agent = !use_agent;
            break;
        case 'l':
        case 'L':
            use_look_at = !use_look_at;
            break;
        
        // handle the sensor gizmo:
        case 'E':
            sensor_gizmo.setType(ofxGizmo::OFX_GIZMO_ROTATE);
            break;
        case 'W':
            sensor_gizmo.setType(ofxGizmo::OFX_GIZMO_MOVE);
            break;
        case 'R':{
            // reset the orientation
            sensor_gizmo.getMatrix().setRotate(ofQuaternion(0,0,0,1));
            auto temp = ofVec4f(0,0,0,1);
            on_sensor_orientation_moved(temp);
            break;
        }
            
        default:
            keypressed_robot(key);
            keypressed_camera(key);
            if (!use_look_at)
                keypressed_gizmo(key);
            break;
    }
    
}

//--------------------------------------------------------------
void ofApp::keypressed_robot(int key){
    switch (key) {
        // 'm' for MOVE!
        case 'm':
        case 'M':
            robot_live = !robot_live;
            robot.robotParams.bMove = robot_live;
            robot.set_live(robot_live);
            // TODO: move sim robot to real robot joint positions
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keypressed_camera(int key){
    bool val = true;
    switch (key) {
        case 'h':
        case 'H':
            show_gui.set(!show_gui);
            break;
        case '1':
            listener_show_top(val);
            break;
        case '2':
            listener_show_front(val);
            break;
        case '3':
            listener_show_side(val);
            break;
        case '4':
            listener_show_perspective(val);
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keypressed_gizmo(int key){
    bool val = true;
    switch (key) {
        case 'e':
//        case 'E':
            tcp_target.setType(ofxGizmo::OFX_GIZMO_ROTATE);
            break;
        case 'w':
//        case 'W':
            tcp_target.setType(ofxGizmo::OFX_GIZMO_MOVE);
            break;
        case 'r':
//        case 'R':
            // reset the orientation
            tcp_target.getMatrix().setRotate(ofQuaternion(0,0,0,1));
            break;
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    if (disable_camera()){
        cam.disableMouseInput();
    }
    else{
        cam.enableMouseInput();
    }

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if (disable_camera()){
        cam.disableMouseInput();
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    // Disable the camera if we're interacting with the GUI
    if (disable_camera()){
        cam.disableMouseInput();
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
//    if (!cam.getMouseInputEnabled())
        cam.enableMouseInput();
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    tcp_target.setViewDimensions(w, h);
    look_at_target.setViewDimensions(w, h);
    sensor_gizmo.setViewDimensions(w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
