#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <math.h>

class Point{
	float x, y, z;
	float normx, normy, normz;
public:
	Point();
	Point(float x_coor, float y_coor, float z_coor);
	float getX();
	void setX(float x_coor);
	float getY();
	void setY(float y_coor);
	float getZ();
	void setZ(float z_coor);
	float getNormX();
	float getNormY();
	float getNormZ();
	void print();
	void setNormal(float normx, float normy, float normz);
	void addNormal(float normx, float normy, float normz);
	void normNormal();
};

class Polygon{
	int no;
	std::vector<Point*> vertices;
	float normx, normy, normz;
public:
	Polygon();
	void addVertex(Point *p);
	Point* getPointAt(int i);
	void printVertices();
	void drawPolygon();
	void computeNormal();
};

class Model{
	std::vector<Point*> points; // List of input points
	std::vector<Polygon*> polygons; // List of polygons
	float minx, miny, minz;
	float maxx, maxy, maxz;
public:
	Model();
	void render();
	void drawAxis();
	void drawObject();
	void readObject(char* fname);
	void computeNormals();
	void drawBoundingBox();
};

#endif
