#ifndef VIEW_H
#define VIEW_H

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <math.h>
#include "FastTrackball.h"

class View{
	static int w, h;
	static Trackball trackball;
public:
	static float transx, transy, transz;
	static float angle, scale;
	static void draw();
	static void initRendering();
	static void handleResize(int w, int h);
	static void rotate(float x1, float y1, float x2, float y2);
};
#endif
