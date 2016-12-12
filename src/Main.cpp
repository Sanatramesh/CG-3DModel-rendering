#include <iostream>
#include "model.h"
#include "view.h"
#include "controller.h"

using namespace std;

View view;
Model model;

/*
 * main: initialize glut window
 */
int main(int argc,char* argv[])
{
	if (argc != 2){
		std::cout<<"Usage: ./assign2 <ply filename>"<<std::endl;
		exit(-1);
	}
	Controller controller;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	glutInitWindowSize(400, 400); //Set the window size
	glutInitWindowPosition(50,50);
	//Create the window
	glutCreateWindow("3D Object Rendering");
	controller.run(argv);
	
	return 0;	
}

