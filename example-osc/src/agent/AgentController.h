#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxEasing.h"
#include "ofxGizmo.h"
#include "Agent.h"

class AgentController
{
public:
	AgentController();
	~AgentController();

	void setup(int num_agents);
	void update();
	void draw();
    
    void set_targets(vector<ofMatrix4x4*> tgts);
    void set_targets(vector<ofNode*> tgts);
    void set_targets(vector<ofxGizmo*> tgts);
    void set_positions(vector<ofxGizmo*> tgts);
    
    void set_target(int index, ofNode tgt);
    
    ofNode get_pose(int id);
    
    ofParameterGroup params;
    int get_num(){return NUM_AGENTS;}

private:
    int NUM_AGENTS;
    
    vector<Agent*> agents;
    vector<ofNode> targets;

    void setup_gui();

    ofParameter<ofVec3f> gravity;
    ofParameter<bool> match_all;
};

