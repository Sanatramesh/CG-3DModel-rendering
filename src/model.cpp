#include "model.h"

/*
 * Point class - represents each vertex in the mesh of the 3D object
 */
// Point constructor
Point::Point(){
	x = 0.0;
	y = 0.0;
	z = 0.0;
	normx = 0.0;
	normy = 0.0;
	normz = 0.0;
}
// Point constructor
Point::Point(float x_coor, float y_coor, float z_coor){
	x = x_coor;
	y = y_coor;
	z = z_coor;
	normx = 0.0;
	normy = 0.0;
	normz = 0.0;
}
float Point::getX(){
	return x;
}
void Point::setX(float x_coor){
	x = x_coor;
}
float Point::getY(){
	return y;
}
void Point::setY(float y_coor){
	y = y_coor;
}
float Point::getZ(){
	return z;
}
void Point::setZ(float z_coor){
	z = z_coor;
}
float Point::getNormX(){
	return normx;
}
float Point::getNormY(){
	return normy;
}
float Point::getNormZ(){
	return normz;
}
void Point::print(){
	std::cout<<x<<" "<<y<<" "<<z<<std::endl;
}
void Point::setNormal(float nx, float ny, float nz){
	normx = nx;
	normy = ny;
	normz = nz;
}
void Point::addNormal(float nx, float ny, float nz){
	normx += nx;
	normy += ny;
	normz += nz;
}
void Point::normNormal(){ // Normalize the normal of this polygon 
	float n = 0;
	n = sqrt(normx*normx + normy*normy + normz*normz);
	normx /= n; normy /= n; normz /= n;
}

/*
 * Polygon class - represents each polygon of the 3D object
 */
// Polygon constructor
Polygon::Polygon(){
	no = -1;
}
void Polygon::addVertex(Point* p){
	vertices.push_back(new Point(p->getX(),p->getY(),p->getZ()));
}
Point* Polygon::getPointAt(int i){
	if (no != -1 && no != i)
		return vertices[i];
	return nullptr;
}
void Polygon::printVertices(){
	int i;
	for (i=0;i<vertices.size();i++)
		vertices[i]->print();
}
void Polygon::drawPolygon(){
	glBegin(GL_TRIANGLES);
	glColor4f(1., 1., 1., 0.);
	for (int i=0;i < vertices.size(); i++){
		glNormal3f(vertices[i]->getNormX(), vertices[i]->getNormY(), vertices[i]->getNormZ());
		glVertex3f(vertices[i]->getX(), vertices[i]->getY(), vertices[i]->getZ());
	}
	glEnd();
}
void Polygon::computeNormal(){ // compute normal to this polygon
	float n = 0;
	float v1x, v1y, v1z, v2x, v2y, v2z;
	v1x = vertices[1]->getX() - vertices[0]->getX();
	v1y = vertices[1]->getY() - vertices[0]->getY();
	v1z = vertices[1]->getZ() - vertices[0]->getZ();
	
	v2x = vertices[2]->getX() - vertices[0]->getX();
	v2y = vertices[2]->getY() - vertices[0]->getY();
	v2z = vertices[2]->getZ() - vertices[0]->getZ();
	
	normx = v1y*v2z - v1z*v2y;
	normy = v1z*v2x - v1x*v2z;
	normz = v1x*v2y - v1y*v2x;
	n = sqrt(normx*normx + normy*normy + normz*normz);
	normx /= n; normy /= n; normz /= n;
	for (int i=0;i<vertices.size(); i++)
		vertices[i]->addNormal(normx, normy, normz);
}

/*
 * Model class - represents the 3D object
 */
// Model constructor
Model::Model(){
	minx = 9999.0; miny = 9999.0; minz = 9999.0;
	maxx = -9999.0; maxy = -9999.0; maxz = -9999.0;
}
// Render the model and axis 
void Model::render(){
	drawAxis();
	drawObject();
	glPointSize(5);
	glBegin(GL_POINTS);
	glColor3f (1, 0, 0);
	glVertex3f(0, 0, 0);
	glEnd();
}
// Render x, y and z axis
void Model::drawAxis(){
	glBegin(GL_LINES);
	float axis = 2;
	//drawing x -axis
	glColor4f(0, 0, 1, 0);
	glVertex3f( axis, 0, 0);
	glVertex3f(-axis, 0, 0);
	
	//drawing y -axis
	glColor4f(0, 1, 0, 0);
	glVertex3f(0, axis, 0);
	glVertex3f(0, -axis,0);
	
	//drawing z -axis
	glColor4f(1, 0, 0, 0);
	glVertex3f(0, 0, axis);
	glVertex3f(0, 0, -axis);
	glEnd();
}
// Render Object
void Model::drawObject(){
	long int i;
	for(i=0;i < polygons.size();i++)
		polygons[i]->drawPolygon();
}
// Read object ply file
void Model::readObject(char *filename){
	std::cout<<filename<<std::endl;
	std::ifstream ifile;
	ifile.open(filename);
	std::string s;
	long int num_vertex, num_faces; long long i;
	float x, y, z, nx, ny, nz;
	int co_in = 0, norm = 0;
	int vertex_idx, j, sides = 0;
	Polygon* polygon;
	while(ifile>>s){
		if (s.compare("vertex") == 0){
			ifile>>s;
			num_vertex = std::stoi(s);
			std::cout<<num_vertex<<std::endl;
		}else if (s.compare("face") == 0){
			ifile>>s;
			num_faces = std::stoi(s);
			std::cout<<num_faces<<std::endl;
		}else if (s.compare("confidence") == 0 || s.compare("intensity") == 0){
			co_in = 1;
		}else if (s.compare("nx") == 0){
			norm = 1;
		}else if (s.compare("end_header") == 0){
			for ( i=0; i < num_vertex; i++){
				ifile>>s;
				x = std::stof(s);
				ifile>>s;
				y = std::stof(s);
				ifile>>s;
				z = std::stof(s);
				points.push_back(new Point(x, y, z));
				if (minx > x)
					minx = x;
				if (miny > y)
					miny = y;
				if (minz > z)
					minz = z;
				if (maxx < x)
					maxx = x;
				if (maxy < y)
					maxy = y;
				if (maxz < z)
					maxz = z;
				if (co_in){
					ifile>>s;
					ifile>>s;
				}else if (norm){
					ifile>>s;
					ifile>>s;
					ifile>>s;
				}
			}
			float avgx = (maxx+minx)/2,
				  avgy = (maxy+miny)/2,
				  avgz = (maxz+minz)/2;
			float maxval = -9999;
			if (abs(maxx - minx) > abs(maxy - miny) && abs(maxx - minx) > abs(maxz - minz))
				maxval = maxx - minx;
			else if (abs(maxy - miny) > abs(maxz - minz))
				maxval = maxy - miny;
			else
				maxval = maxz - minz;
			for ( i=0; i < num_vertex; i++){
				points[i]->setX((points[i]->getX() - avgx)/maxval);
				points[i]->setY((points[i]->getY() - avgy)/maxval);
				points[i]->setZ((points[i]->getZ() - avgz)/maxval);
			}
			minx = (minx - avgx)/maxval;
			miny = (miny - avgy)/maxval;
			minz = (minz - avgz)/maxval;
			
			maxx = (maxx - avgx)/maxval;
			maxy = (maxy - avgy)/maxval;
			maxz = (maxz - avgz)/maxval;
			std::cout<<minx<<" "<<maxx<<std::endl;
			std::cout<<miny<<" "<<maxy<<std::endl;
			std::cout<<minz<<" "<<maxz<<std::endl;
			std::cout<<"Maxval"<<maxval<<std::endl;
			for ( i=0; i < num_faces; i++){
				polygon = new Polygon();
				ifile>>s;
				sides = std::stoi(s);
				for ( j=0; j < sides; j++){
					ifile>>s;
					vertex_idx = std::stoi(s);
					polygon->addVertex(points[vertex_idx]);
				}
				polygons.push_back(polygon);
			}
			std::cout<<points.size()<<std::endl;
			std::cout<<polygons.size()<<std::endl;
		}
	}
	ifile.close();
}
// compute normals for each polygon of the object
void Model::computeNormals(){
	long int i;
	for(i=0;i < polygons.size();i++)
		polygons[i]->computeNormal();

	for(i=0;i < points.size();i++)
		points[i]->normNormal();
}
// Render bounding box of the object
void Model::drawBoundingBox(){
	glBegin(GL_LINES);
	glColor4f(0.4, 0.1, 0.25, 0);
	glVertex3f(minx, miny, minz);
	glVertex3f(maxx, miny, minz);
	
	glVertex3f(maxx, miny, minz);
	glVertex3f(maxx, maxy, minz);
	
	glVertex3f(maxx, maxy, minz);
	glVertex3f(minx, maxy, minz);
	
	glVertex3f(minx, maxy, minz);
	glVertex3f(minx, miny, minz);
	
	glEnd();
	
	glBegin(GL_LINES);
	glColor4f(1, 0, 0.2, 0);
	glVertex3f(minx, miny, maxz);
	glVertex3f(maxx, miny, maxz);
	
	glVertex3f(maxx, miny, maxz);
	glVertex3f(maxx, maxy, maxz);
	
	glVertex3f(maxx, maxy, maxz);
	glVertex3f(minx, maxy, maxz);
	
	glVertex3f(minx, maxy, maxz);
	glVertex3f(minx, miny, maxz);
	glEnd();
	
	glBegin(GL_LINES);
	glColor4f(1, 1, 0, 0);
	glVertex3f(minx, miny, minz);
	glVertex3f(minx, miny, maxz);
	
	glVertex3f(maxx, miny, minz);
	glVertex3f(maxx, miny, maxz);
	glEnd();
	
	glBegin(GL_LINES);
	glColor4f(1, 1, 0, 0);
	glVertex3f(minx, maxy, minz);
	glVertex3f(minx, maxy, maxz);
	
	glVertex3f(maxx, maxy, minz);
	glVertex3f(maxx, maxy, maxz);
	glEnd();
	
}
