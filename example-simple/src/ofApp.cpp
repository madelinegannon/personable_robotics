#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // setup scene
    setup_scene();
    
    // setup robot
    string ip_address = "192.168.1.100"; // change this to your robot's IP address
    bool offline = false; // change this to false when connected to the robot
    robot.setup(ip_address, robot_type, offline);

    // setup gui
    setup_gui();
    
    // start robot
    robot.start();
    
    show_perspective.set(true);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // do movements
    ofNode tcp;
    tcp.setGlobalPosition(tcp_target.getTranslation());
    tcp.setGlobalOrientation(tcp_target.getRotation());
    robot.set_desired(tcp);
    
    // Check if the target tcp is within the safety box
    ofVec3f pt = toOf(tcp_target.getTranslation());
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
    ofVec3f pt = tcp_target.getTranslation();
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
    
    tcp_target.draw(cam);
    
    cam.end();
}

//--------------------------------------------------------------
bool ofApp::disable_camera(){

    if (tcp_target.isInteracting())
        return true;
    
    ofRectangle gui_rect;
    gui_rect.setX(panel.getPosition().x);
    gui_rect.setY(panel.getPosition().y);
    gui_rect.setWidth(panel.getWidth());
    gui_rect.setHeight(panel.getHeight() + panel_robot.getHeight() + 5);
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
    params_safety.add(aabb_pos.set("AABB_Pos", ofVec3f(625, 0, 300), ofVec3f(-1000, -1000, -1000), ofVec3f(1000, 1000, 1000)));
    params_safety.add(aabb_bounds.set("AABB_Bounds", ofVec3f(650, 1200, 1200), ofVec3f(0, 0, 0), ofVec3f(1500, 1500, 1500)));
    panel_safety.add(params_safety);
    panel_safety.setPosition(panel.getPosition().x, panel.getPosition().y + panel.getHeight() + 5);
    
    panel_robot.setup("Robot_Controller");
    panel_robot.add(robot_live.set("Robot_LIVE\t('m')", false));
    panel_robot.setPosition(panel.getPosition().x, panel_safety.getPosition().y + panel_safety.getHeight() + 5);
    
    ofSetCircleResolution(60);
}

//--------------------------------------------------------------
void ofApp::draw_gui(){
    panel_robot.setPosition(panel.getPosition().x, panel_safety.getPosition().y + panel_safety.getHeight() + 5);
    
    panel.draw();
    panel_safety.draw();
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
    cam.setFarClip(9999999);
    cam.setDistance(5000);
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
        int y = 400;
        int z = 600;
        
        ofVec3f pos = ofVec3f(x, y, z);
        ofVec3f tgt = ofVec3f(0, pos.y, pos.z);
        cam.setGlobalPosition(pos);
        cam.setTarget(tgt);
        cam.lookAt(tgt, ofVec3f(0, 0, 1));
        
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
        
        int x = 900;
        int y = -2000;
        int z = 600;
        
        ofVec3f pos = ofVec3f(x, y, z);
        ofVec3f tgt = ofVec3f(pos.x, 0, pos.z);
        cam.setGlobalPosition(pos);
        cam.setTarget(tgt);
        cam.lookAt(tgt, ofVec3f(0, 0, 1));
        
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
        cam.lookAt(tgt, ofVec3f(0, 0, 1));
        cam.setGlobalPosition(pos);
        
        show_top = false;
        show_front = false;
        show_side = false;
        show_perspective = true;
    }
}

#pragma mark - App Functions

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key) {
        case 'f':
            ofToggleFullscreen();
            break;
            
        default:
            keypressed_robot(key);
            keypressed_camera(key);
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
            robot.set_live(robot_live);
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
        case 'E':
            tcp_target.setType(ofxGizmo::OFX_GIZMO_ROTATE);
            break;
        case 'w':
        case 'W':
            tcp_target.setType(ofxGizmo::OFX_GIZMO_MOVE);
            break;
        case 'r':
        case 'R':
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
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
