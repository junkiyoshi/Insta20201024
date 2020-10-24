#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(0);
	ofSetColor(255);
	ofSetLineWidth(0.5);
	ofEnableDepthTest();

	for (int i = 0; i < 5; i++) {
		
		this->noise_seed_list.push_back(glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000)));
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	for (auto& noise_seed : this->noise_seed_list) {

		auto location = glm::vec3(
			ofMap(ofNoise(noise_seed.x, ofGetFrameNum() * 0.003), 0, 1, -300, 300),
			ofMap(ofNoise(noise_seed.y, ofGetFrameNum() * 0.003), 0, 1, -300, 300),
			ofMap(ofNoise(noise_seed.z, ofGetFrameNum() * 0.003), 0, 1, -300, 300)
		);

		auto next = glm::vec3(
			ofMap(ofNoise(noise_seed.x, (ofGetFrameNum() + 1) * 0.003), 0, 1, -300, 300),
			ofMap(ofNoise(noise_seed.y, (ofGetFrameNum() + 1) * 0.003), 0, 1, -300, 300),
			ofMap(ofNoise(noise_seed.z, (ofGetFrameNum() + 1) * 0.003), 0, 1, -300, 300)
		);
		auto distance = location - next;
		distance *= 2;

		auto future = location + distance * 30;
		auto random_deg_1 = ofRandom(360);
		auto random_deg_2 = ofRandom(360);
		future += glm::vec3(
			30 * cos(random_deg_1 * DEG_TO_RAD) * sin(random_deg_2 * DEG_TO_RAD),
			30 * sin(random_deg_1 * DEG_TO_RAD) * sin(random_deg_2 * DEG_TO_RAD),
			30 * cos(random_deg_2 * DEG_TO_RAD)
		);

		auto future_distance = future - location;

		this->location_list.push_back(location);
		this->velocity_list.push_back(glm::normalize(future_distance) * glm::length(distance));
		vector<glm::vec3> log;
		this->log_list.push_back(log);
	}

	for (int i = this->location_list.size() - 1; i > -1; i--) {

		auto tmp = this->location_list[i] + this->velocity_list[i];
		this->location_list[i] = tmp;
		this->velocity_list[i] *= 1.01;
		this->log_list[i].push_back(tmp);

		if (glm::length(this->location_list[i]) > 720) {

			this->location_list.erase(this->location_list.begin() + i);
			this->velocity_list.erase(this->velocity_list.begin() + i);
			this->log_list.erase(this->log_list.begin() + i);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 0.5);

	for (int i = 0; i < this->location_list.size(); i++) {

		for (int k = 0; k < this->log_list[i].size() - 1; k++) {

			ofDrawLine(this->log_list[i][k], this->log_list[i][k + 1]);
		}
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}