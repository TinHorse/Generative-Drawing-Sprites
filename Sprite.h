#pragma once
#include "ofMain.h"

class Building {
private:
	vector<ofVec3f> corners;
	ofMesh mesh;
	int stage;

protected:
	string type;
	float timer = 0;
public:
	Building(vector<ofVec3f> _corners, int _stage) : corners(_corners), stage(_stage) {}

	void setup(ofColor col1, ofColor col2) {
		if (col1.a > 30) {
			col1.a -= (stage * 10);
			col2.a -= (stage * 10);
		}

		mesh.enableColors();
		mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
		for (int c = 0; c < corners.size(); c++) {
			mesh.addVertex(ofVec3f(corners[c].x, corners[c].y, 0));
			mesh.addColor(col1.lerp(col2, ofRandom(0.3)));
		}
	}

	virtual void draw() {
		timer++;
		mesh.draw();
	}
	string getType() {
		return type;
	}

};

class Manufacture : public Building {
private:
	ofColor col1 = { 0,0,ofRandom(50,200), ofRandom(150,200) };

public:
	Manufacture(vector<ofVec3f> corners, int stage) : Building(corners, stage) {
		setup(col1, col1);
		ofVec3f centre = corners[3] - ((corners[3] - corners[0]) / 2);
		centre.z = corners[3].z;
		type = "Manufacture";
	}

	virtual void draw() {
		Building::draw();
	}

};

class House : public Building {
private:
	ofColor col1 = { ofRandom(100,200),100,0,ofRandom(30,100) };
	ofVec3f centre;

public:
	House(vector<ofVec3f> corners, int stage) : Building(corners, stage) {
		setup(col1, col1);
		type = "House";
	}



};


class Space {
private:
	vector<ofVec3f> corners;
	ofVec3f centre;
	bool occupied;
	int stage;
	float alpha = 255;
public:
	Space(vector<ofVec3f> cor, ofVec3f _centre, int _stage) : corners(cor), centre(_centre), occupied(false), stage(_stage) {
		alpha -= (stage * 10);
	}

	void draw() {
		if (occupied) {
			ofSetLineWidth(2);
			ofNoFill();
			ofSetColor(0,0, 255 - alpha,255-alpha);
			ofDrawCircle(corners[0], 3);
			ofDrawCircle(corners[1], 3);
			ofDrawCircle(corners[2], 3);
			ofDrawCircle(corners[3], 3);
			ofSetLineWidth(1);
			ofFill();
			ofSetColor(200, 150, 60, alpha-100);
			ofDrawCircle(corners[0], 3);
			ofDrawCircle(corners[1], 3);
			ofDrawCircle(corners[2], 3);
			ofDrawCircle(corners[3], 3);

			ofDrawLine(corners[0], corners[1]);
			ofDrawLine(corners[0], corners[2]);
			ofDrawLine(corners[2], corners[3]);
			ofDrawLine(corners[3], corners[1]);

			ofDrawLine(centre, corners[2]);
			ofDrawLine(centre, corners[3]);
		}
	}

	vector<ofVec3f>& getCorners() {
		return corners;
	}

	bool getOccupied() {
		return occupied;
	}
	void setOccupied(bool occ){
		occupied = occ;
	}
	int getStage() {
		return stage;
	}

};

class Spaces {
private:
	float size;
	ofVec3f centre;
	vector<Space> spaces;
	vector<ofVec3f> pathways;
public:
	Spaces(float x, float y, float _size) : centre(x,y), size(_size) {}

	vector<ofVec3f> calculateSpaces(int _amount, int _stages, int num_pathways) {
		vector<ofVec3f> centre_points;
		float amount = _amount;
		int stages = _stages;
		float _size = size;


		//paths

		_size = size;
		float rot = ofRandom(10,360);
		float max_rot = amount / 2.6;
		int iterations = stages + int(stages / 2);
		if (num_pathways < 3) {
			iterations += int(stages / 2);
		}
		for (int i = 1; i < num_pathways + 1; i++) {
			rot += max_rot / num_pathways;
			for (int s = 1; s <= iterations + 1; s++) {
				ofVec3f point = { centre.x + (((size * (s)) + _size)*float(cos(rot))), centre.y + ((( size * (s + 2)) + _size) *float(sin(rot))), 0 };
				pathways.push_back(point);
				if (s == iterations) {
					centre_points.push_back(point);
				}
			}
			float rot2 = rot + (max_rot/60);
			for (int s = 1; s <= iterations + 1; s++) {
				ofVec3f point = { centre.x + (((size * (s)) + _size)*float(cos(rot2))), centre.y + (((size * (s + 2)) + _size) *float(sin(rot2))), 0 };
				pathways.push_back(point);
			}
		}


		//stages

		vector<ofVec3f> line_points;
		for (int s = 0; s < stages; s++) {
			float offset = ofRandom(10);
			float rot_step = ((1 / amount) * amount/2.6);
			float rotation = 0;
			
			_size += s * 2;
			float size_increase = 10;
			for (int j = 0; j < amount; j++) {
				float height = 0;
				vector <ofVec3f> points;
				for (int i = 0; i < 2; i++) {
					float rot = offset + rotation + (i*rot_step);
					ofVec3f point = { centre.x + ((_size)*float(cos(rot))), centre.y + ((_size)*float(sin(rot))), height };
					points.push_back(point + offset);
				}
				for (int i = 0; i < 2; i++) {
					float rot = offset + rotation + (i*rot_step);
					ofVec3f point = { 1.4f*size_increase + s + centre.x + ((size_increase + size + (_size))*float(cos(offset + rotation + (i*rot_step)))), 1.4f*size_increase + s + centre.y + ((size_increase + size + (_size))*float(sin(offset + rotation + (i*rot_step)))), height };
					points.push_back(point);
				}
				

				bool del = false;
				for (auto p : points) {
					for (auto p2 : pathways) {
						if (p.distance(p2) < (size/1.1)) {
							del = true;
							break;
						}
					}
				}

				if (!del) {
					spaces.push_back(Space(points, centre, s));
				}
				rotation += rot_step;
			}
			_size += size;
			if (s % 2 == 0) {
				amount--;
			}
		}
		
		return centre_points;
	}

	void draw() {
		for (int i = 0; i < spaces.size(); i++) {
			spaces[i].draw();
		}
		for (int i = 0; i < pathways.size(); i++) {
			ofNoFill();
			ofSetColor(255, 100);
			ofDrawCircle(pathways[i], 3);
			ofSetColor(0, 100);
			ofDrawCircle(pathways[i], 1.5);
			ofFill();
		}
	}

	int getNumSpaces() {
		return spaces.size();
	}
	vector<Space>& getSpaces() {
		return spaces;
	}
};


class Light {
private:
	ofVec3f loc;
	ofVec3f target;
	float timer;
	float size;

public:
	Light(ofVec3f location, float _size) : loc(location), timer(0), size(_size) {}

	void draw() {
		ofPushMatrix();
		ofSetColor(200, 200, 0, 30);
		ofTranslate(loc);
		ofRotateDeg(timer / 4);
		ofDrawTriangle(0, 0, size, -40, size, 40);
		ofSetColor(0);
		ofDrawCircle(10, 0, 5);
		ofSetColor(255);
		ofDrawCircle(10, 0, 2);
		ofPopMatrix();
	}

	void run() {
		timer++;
		draw();
	}
};

class CentrePoint {
private:
	ofVec3f loc;
	bool occupied;
public:
	CentrePoint(ofVec3f _loc) : loc(_loc), occupied(false) {}
	ofVec3f getLoc() {
		return loc;
	}
	bool getOccupied() {
		return occupied;
	}
	void setOccupied(bool occ) {
		occupied = occ;
	}
	void draw() {
		ofSetColor(0);
		ofDrawCircle(loc, 8);
		ofSetColor(255);
		ofDrawCircle(loc, 2);
	}
	ofVec3f getPoint() {
		return loc;
	}
};



class City {
private:
	int capital;
	float locX, locY;
	float population;
	float timer{ 0 };
	Spaces space{locX, locY, 30};
	int stages;
	int amount = 10;
	vector<CentrePoint> centre_points;
	bool out_of_space = false;
	int centre_spaces;

	vector<unique_ptr<Building>> buildings;
	ofMesh town_centre;
	Light town_light{ ofVec3f(locX,locY), 600 };
public:
	City(float x, float y, int _stages, int _capital) : locX(x), locY(y), population(20), stages(_stages), capital(_capital) {
		auto cent = space.calculateSpaces(amount,stages, _capital);
		for (int i = 0; i < cent.size(); i++) {
			centre_points.push_back(CentrePoint(cent[i]));
		}
		makeTownCentre();
	}

	void addBuilding(string type) {
		bool taken = true;
		int index = 0;
		int max_index = space.getSpaces().size();
		while (taken && index <max_index) {
			if (!space.getSpaces()[index].getOccupied()) {
				if (type == "Manufacture") {
					buildings.push_back(make_unique<Manufacture>(space.getSpaces()[index].getCorners(),space.getSpaces()[index].getStage()));
				}
				else if (type == "House") {
					buildings.push_back(make_unique<House>(space.getSpaces()[index].getCorners(), space.getSpaces()[index].getStage()));
				}
				space.getSpaces()[index].setOccupied(true);
				taken = false;
				break;
			}
			index++;
		}
	}

	void makeTownCentre() {
		town_centre.enableColors();
		town_centre.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
		for (int i = 0; i < 10; i++) {
			float rot = i;
			ofVec3f point = { locX + (population*float(cos(rot))), locY + (population*float(sin(rot))), 0 };
			town_centre.addVertex(point);
			town_centre.addColor(ofColor(ofRandom(50,100),0,0,100));
		}
	}

	void draw() {
		space.draw();
		town_centre.draw();
		for (int i = 0; i < buildings.size(); i++) {
			if (buildings[i]->getType() == "House") {
				buildings[i]->draw();
			}
		}
		for (int i = 0; i < buildings.size(); i++) {
			if (buildings[i]->getType() == "Manufacture") {
				buildings[i]->draw();
			}
		}

		//lighthouse
		ofSetColor(0);
		ofDrawCircle(locX, locY, population);
		ofSetColor(255);
		ofDrawCircle(locX, locY, 10);
		ofSetColor(0);
		ofDrawCircle(locX, locY, 8);
		ofSetColor(255);
		ofDrawCircle(locX, locY, 4);
		for (int i = 0; i < centre_points.size(); i++) {
			centre_points[i].draw();
		}
		town_light.run();
	}

	void run(ofVec3f target) {
		//gun.run();
		//gun.updateTarget(target);
		timer++;
		int t = timer;
		draw();
		if (t % 50 == 0) {
			addBuilding("Manufacture");
		}
		if (t % 100 == 0) {
			addBuilding("House");
		}
		if (capital && t % 500 == 0) {
			centre_spaces = 0;
			for (int i = 0; i < centre_points.size(); i++) {
				if (!centre_points[i].getOccupied()) {
					centre_spaces++;
				}
			}
			if (buildings.size() >= space.getSpaces().size() / 2 && centre_spaces > 0) {
				out_of_space = true;
			}
		}
	}

	bool getOutOfSpace() {
		return out_of_space;
	}
	void setOutOfSpace(bool o) {
		out_of_space = o;
	}

	vector<CentrePoint>& getCentrePoints()
	{
		return centre_points;
	}

	ofVec3f getLoc() {
		return ofVec3f(locX, locY);
	}


};


class Metro {
private:
	vector<unique_ptr<City>> centres;
	ofVec3f location;
	float timer = 0;
	int num_pathways = 2;
public:
	Metro(ofVec3f loc) : location(loc){
		centres.push_back(make_unique<City>(loc.x,loc.y, 8, 3));
	}

	void run(ofVec3f target) {
		for (int i = 0; i < centres.size(); i++) {
			centres[i]->run(target);
			
			if (centres[i]->getOutOfSpace()) {
				for (int j = 0; j < centres[i]->getCentrePoints().size(); j++) {
					if (!centres[i]->getCentrePoints()[j].getOccupied()) {
						centres.push_back(make_unique<City>(centres[i]->getCentrePoints()[j].getLoc().x, centres[i]->getCentrePoints()[j].getLoc().y, num_pathways+2, num_pathways));
						centres[i]->getCentrePoints()[j].setOccupied(true);
						centres[i]->setOutOfSpace(false);
						if (num_pathways > 0) {
							num_pathways--;
						}
						break;
					}
				}
			}
			
		}
		timer++;
	}

	vector<ofVec3f> getCentrePoints() {
		vector<ofVec3f> cen;
		for (int i = 0; i < centres[0]->getCentrePoints().size(); i++) {
			cen.push_back(centres[0]->getCentrePoints()[i].getPoint());
		}
		return cen;
	}

};