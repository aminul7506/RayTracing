#include<bits/stdc++.h>
#include<cstdlib>
#include<cmath>
#include "bitmap_image.hpp"
#include <windows.h>
#include <glut.h>
#include"1305087.hpp"
#include<vector>

using namespace std;

#define pi (2*acos(0.0))
#define infinity 1000000




Vector3 u;
Vector3 r;
Vector3 l;
Vector3 pos;
int imageWidth, imageHeight;
int window_width, window_height;
double lightthick = 1.0;
Vector3 **image;
double fovY;



void drawPoint(Vector3 point) {

    glColor3f(1.0, 0.0, 1.0);

    glBegin(GL_QUADS);
    {
        glVertex3f(point.x + lightthick, point.y, point.z + lightthick);
        glVertex3f(point.x + lightthick, point.y, point.z - lightthick);
        glVertex3f(point.x - lightthick, point.y, point.z - lightthick);
        glVertex3f(point.x - lightthick, point.y, point.z + lightthick);
    }
    glEnd();

}

Vector3 normalize(Vector3 point)
{
    double value = pow(point.x*point.x + point.y*point.y + point.z*point.z,0.5);
    Vector3 nor;
    nor.x = point.x/value;
    nor.y = point.y/value;
    nor.z = point.z/value;
    return nor;
}



void clearMemory()
{
    vector<Object*>().swap(objects);
    vector<Vector3>().swap(lights);
    delete[] image;
}

void capture()
{
    image = new Vector3 *[imageWidth];
     for(int i = 0; i < imageWidth; i++){
        image[i] = new Vector3 [imageWidth];
    }
    Vector3 c;
    c.x = 0.0;
    c.y = 0.0;
    c.z = 0.0;
    for(int i = 0; i < imageWidth; i++){
        for(int j = 0; j < imageWidth; j++){
            image[i][j] = c;
        }
    }

    double planeDistance = (window_height/2.0)/tan((fovY*pi)/360);
    Vector3 topLeft;

    topLeft.x = pos.x + (l.x * planeDistance - r.x * (window_width/2.0) + u.x * (window_height/2.0));
    topLeft.y = pos.y + (l.y * planeDistance - r.y * (window_width/2.0) + u.y * (window_height/2.0));
    topLeft.z = pos.z + (l.z * planeDistance - r.z * (window_width/2.0) + u.z * (window_height/2.0));

    double du = (window_width * 1.0)/imageWidth;
    double dv = (window_height * 1.0)/imageHeight;

    for(int i = 0; i < imageWidth; i++){
        for(int j = 0; j < imageHeight; j++){
            Vector3 corner;
            corner.x = topLeft.x + r.x * j * du - u.x * i *dv;
            corner.y = topLeft.y + r.y * j * du - u.y * i *dv;
            corner.z = topLeft.z + r.z * j * du - u.z * i *dv;

            Vector3 temp;
            temp.x = corner.x - pos.x;
            temp.y = corner.y - pos.y;
            temp.z = corner.z - pos.z;
            Vector3 temp_norm = normalize(temp);

            Ray ray(pos, temp_norm);
            int nearest = -1;
            double color[3];
            double t_min = infinity;
            int object_size = objects.size();
            for(int k = 0; k < object_size; k++){
                double t = objects[k]->intersect(&ray, color, 0);
                if(t <= 0) continue;
                if(t < t_min){
                    nearest = k;
                    t_min = t;
                }
            }
            if(nearest != -1){
                double t = objects[nearest]->intersect(&ray, color, 1);
                image[i][j].x = color[0];
                image[i][j].y = color[1];
                image[i][j].z = color[2];
                //cout << "updating";
            }
        }
    }

    bitmap_image image1(imageWidth,imageWidth);
    for(int i = 0; i < imageWidth; i++){
        for(int j = 0; j < imageHeight; j++){
            image1.set_pixel(j,i,image[i][j].x * 255,image[i][j].y * 255,image[i][j].z * 255);
        }
    }
    image1.save_image("1305087.bmp");
    cout << "Saved Output Image\n";
}


void keyboardListener(unsigned char key, int x,int y){
	switch(key){

	        case '1':{
            double cosVal = cos(3*pi/180);
		    double sinVal = sin(3*pi/180);

		    r.x = r.x*cosVal + l.x*sinVal;
		    r.y = r.y*cosVal + l.y*sinVal;
		    r.z = r.z*cosVal + l.z*sinVal;

			double s1 = pow(r.x,2) + pow(r.y,2) + pow(r.z,2);
			r.x = r.x/s1;
			r.y = r.y/s1;
			r.z = r.z/s1;

		    l.x = l.x*cosVal - r.x*sinVal;
		    l.y = l.y*cosVal - r.y*sinVal;
		    l.z = l.z*cosVal - r.z*sinVal;

            double s = pow(l.x,2) + pow(l.y,2) + pow(l.z,2);
			l.x = l.x/s;
			l.y = l.y/s;
			l.z = l.z/s;

			break;
        }

           case '2':{
            double cosVal = cos(-3*pi/180);
		    double sinVal = sin(-3*pi/180);

		    r.x = r.x*cosVal + l.x*sinVal;
		    r.y = r.y*cosVal + l.y*sinVal;
		    r.z = r.z*cosVal + l.z*sinVal;

			double s1 = pow(r.x,2) + pow(r.y,2) + pow(r.z,2);
			r.x = r.x/s1;
			r.y = r.y/s1;
			r.z = r.z/s1;

		    l.x = l.x*cosVal - r.x*sinVal;
		    l.y = l.y*cosVal - r.y*sinVal;
		    l.z = l.z*cosVal - r.z*sinVal;

			double s = pow(l.x,2) + pow(l.y,2) + pow(l.z,2);
			l.x = l.x/s;
			l.y = l.y/s;
			l.z = l.z/s;

			break;
        }

         case '3':
        {
            double cosVal = cos(-3*pi/180);
		    double sinVal = sin(-3*pi/180);

		    l.x = l.x*cosVal + u.x*sinVal;
		    l.y = l.y*cosVal + u.y*sinVal;
		    l.z = l.z*cosVal + u.z*sinVal;

			double s = pow(l.x,2) + pow(l.y,2) + pow(l.z,2);
			l.x = l.x/s;
			l.y = l.y/s;
			l.z = l.z/s;

            u.x = u.x*cosVal - l.x*sinVal;
		    u.y = u.y*cosVal - l.y*sinVal;
		    u.z = u.z*cosVal - l.z*sinVal;

			double s1 = pow(u.x,2) + pow(u.y,2) + pow(u.z,2);
			u.x = u.x/s1;
			u.y = u.y/s1;
			u.z = u.z/s1;

			break;
        }


        case '4':
        {
            double cosVal = cos(3*pi/180);
		    double sinVal = sin(3*pi/180);

            l.x = l.x*cosVal + u.x*sinVal;
		    l.y = l.y*cosVal + u.y*sinVal;
		    l.z = l.z*cosVal + u.z*sinVal;

			double s = pow(l.x,2) + pow(l.y,2) + pow(l.z,2);
			l.x = l.x/s;
			l.y = l.y/s;
			l.z = l.z/s;

		    u.x = u.x*cosVal - l.x*sinVal;
		    u.y = u.y*cosVal - l.y*sinVal;
		    u.z = u.z*cosVal - l.z*sinVal;

			double s1 = pow(u.x,2) + pow(u.y,2) + pow(u.z,2);
			u.x = u.x/s1;
			u.y = u.y/s1;
			u.z = u.z/s1;
			break;
        }



		case '5':
        {
            double cosVal = cos(-3*pi/180);
		    double sinVal = sin(-3*pi/180);

            r.x = r.x*cosVal + u.x*sinVal;
		    r.y = r.y*cosVal + u.y*sinVal;
		    r.z = r.z*cosVal + u.z*sinVal;

			double s = pow(r.x,2) + pow(r.y,2) + pow(r.z,2);
			r.x = r.x/s;
			r.y = r.y/s;
			r.z = r.z/s;

            u.x = u.x*cosVal - r.x*sinVal;
		    u.y = u.y*cosVal - r.y*sinVal;
		    u.z = u.z*cosVal - r.z*sinVal;

			double s1 = pow(u.x,2) + pow(u.y,2) + pow(u.z,2);
			u.x = u.x/s1;
			u.y = u.y/s1;
			u.z = u.z/s1;


			break;
        }

        case '6':
        {
            double cosVal = cos(3*pi/180);
		    double sinVal = sin(3*pi/180);

            r.x = r.x*cosVal + u.x*sinVal;
		    r.y = r.y*cosVal + u.y*sinVal;
		    r.z = r.z*cosVal + u.z*sinVal;

			double s1 = pow(r.x,2) + pow(r.y,2) + pow(r.z,2);
			r.x = r.x/s1;
			r.y = r.y/s1;
			r.z = r.z/s1;

		    u.x = u.x*cosVal - r.x*sinVal;
		    u.y = u.y*cosVal - r.y*sinVal;
		    u.z = u.z*cosVal - r.z*sinVal;

			double s = pow(u.x,2) + pow(u.y,2) + pow(u.z,2);
			u.x = u.x/s;
			u.y = u.y/s;
			u.z = u.z/s;

			break;
        }

        case 'u':		// up arrow key
            pos.y -= 2;
			pos.x -= 2;
			break;

        case 'd':		//down arrow key as my down arrow key is damaged
			//cameraHeight -= 3.0;
			pos.y += 2;
            pos.x += 2;
			break;

        case '0':
            capture();
            break;
        case 'e':
            exit(0);
            break;

        default:
			break;
	}

}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			//cameraHeight -= 3.0;
			pos.y += 2;
            pos.x += 2;
			break;
		case GLUT_KEY_UP:		// up arrow key
            pos.y -= 2;
			pos.x -= 2;
			break;

		case GLUT_KEY_RIGHT:
			//cameraAngle += 0.03;
			pos.x -= 2;
			pos.y += 2;//cout << pos.x << " " << pos.y << " " <<  pos.z << endl;
			break;
		case GLUT_KEY_LEFT:
		    pos.x += 2;
		    pos.y -= 2;//cout << pos.x << " " << pos.y << " " <<  pos.z << endl;
			break;

		case GLUT_KEY_PAGE_UP:{
		    pos.z += 2;
			break;
        }
		case GLUT_KEY_PAGE_DOWN:{
		    pos.z -= 2;
			break;
		}
		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
		    pos.z += 2;
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}




void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

    gluLookAt(pos.x, pos.y, pos.z,
    pos.x + l.x, pos.y + l.y, pos.z + l.z,
    u.x, u.y, u.z);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

    int object_size = objects.size();
	for (int i = 0; i < object_size; i++) {
        objects[i]->draw();
    }
    int light_size = lights.size();
    for (int i = 0; i < light_size; i++) {
        drawPoint(lights[i]);
    }


	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	window_height = 500;
	window_width = 500;
	fovY = 80;

	/*u.x = 0;
	u.y = 0;
	u.z = 1;
	r.x = -1/(pow(2,.5));
	r.y = 1/pow(2,.5);
	r.z = 0;
	l.x = -1/(pow(2,.5));
	l.y = -1/pow(2,.5);
	l.z = 0;
	pos.x = 100;
	pos.y = 100;
	pos.z = 10;*/

	u.x = 0;
	u.y = 0;
	u.z = 1;
	r.x = 1;
	r.y = 0;
	r.z = 0;
	l.x = 0;
	l.y = 1;
	l.z = 0;
	pos.x = 0;
	pos.y = -150;
	pos.z = 60;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(fovY,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

void setUpFloor()
{
    Object *temp;
    temp = new Floor(1000, 20);
    temp->setShine(1);
    temp->setCoEfficients(0.4,0.2,0.2,0.2);
    temp->setTexture();
    objects.push_back(temp);
}

void loadTestData()
{
    recursion_level = 3;
    imageWidth = 768;
    imageHeight = imageWidth;
    Object *temp;
    Vector3 c;
    c.x = 20, c.y = 20, c.z = 10;
    double radius = 10;
    temp = new Sphere(c, radius);
    temp->setColor(1,0,0);
    temp->setCoEfficients(0.4,0.2,0.2,0.2);
    temp->setShine(1);
    objects.push_back(temp);

    c.x = 0, c.y = 5, c.z = 15;
    radius = 10;

    temp = new Sphere(c, radius);
    temp->setColor(1,1,1);
    temp->setCoEfficients(0.4,0.2,0.2,0.2);
    temp->setShine(1);
    objects.push_back(temp);

    Vector3 light;
    light.x = -50, light.y = 50, light.z = 50;
    lights.push_back(light);
    setUpFloor();

    Vector3 a;
    a.x = 20.0, a.y = 20.0, a.z = 20.0;
    Vector3 b;
    b.x = 40.0, b.y = 30.0, b.z = 10.0;
    Vector3 c1;
    c1.x = 50.0, c1.y = 40.0, c1.z = 0.0;

    temp = new Triangle(a, b, c1);
    temp->setColor(0, 1, 0);
    temp->setCoEfficients(0.4, 0.2, 0.1, 0.3);
    temp->setShine(5);
    objects.push_back(temp);

    Vector3 reference_point;
    reference_point.x = 0;
    reference_point.y = 0;
    reference_point.z = 0;
    double A, B, C, D, E, F, G, H, I, J;
    A = B = C = 1, D = E = F = 0, G = H = I = -20, J = 200;
    temp = new GeneralQuadratic(A, B, C, D, E, F, G, H, I, J, reference_point, 0, 0, 5);
    temp->setColor(1, 0, 1);
    temp->setCoEfficients(0.4, 0.2, 0.1, 0.3);
    temp->setShine(4);
    objects.push_back(temp);
}


void loadActualData()
{
    ifstream myfile ("scene.txt");
    if (myfile.is_open()){
        myfile >> recursion_level;
        cout << recursion_level << endl;
        myfile >> imageWidth;
        cout << imageWidth << endl;
        imageHeight = imageWidth;
        int num_of_obj;
        myfile >> num_of_obj;
        cout << num_of_obj << endl;
        cout << endl;
        for(int i = 0; i < num_of_obj; i++){
            string shape;
            myfile >> shape;
            cout << shape << endl;
            if(shape == "sphere"){
                Object *temp;
                Vector3 v;
                myfile >> v.x >> v.y >> v.z;
                cout << v.x << " " << v.y << " " << v.z << endl;
                double radius;
                myfile >> radius;
                cout << radius << endl;
                temp = new Sphere(v, radius);
                myfile >> v.x >> v.y >> v.z;
                cout << v.x << " " << v.y << " " << v.z << endl;
                temp->setColor(v.x, v.y, v.z);
                double a,b,c,d;
                int s;
                myfile >> a >> b >> c >> d;
                cout << a << " " << b << " "<< c << " " << d << endl;
                temp->setCoEfficients(a,b,c,d);
                myfile >> s;
                cout << s << endl;
                temp->setShine(s);
                objects.push_back(temp);
                cout << endl;
            }
            else if(shape == "triangle"){
                Object *temp;
                Vector3 a,b,c,v;
                myfile >> a.x >> a.y >> a.z;
                cout << a.x << " " << a.y << " " << a.z << endl;
                myfile >> b.x >> b.y >> b.z;
                cout << b.x << " " << b.y << " " << b.z << endl;
                myfile >> c.x >> c.y >> c.z;
                cout << c.x << " " << c.y << " " << c.z << endl;
                temp = new Triangle(a,b,c);
                myfile >> v.x >> v.y >> v.z;
                temp->setColor(v.x, v.y, v.z);
                cout << v.x << " " << v.y << " " << v.z << endl;
                double a1,b1,c1,d1;
                int s;
                myfile >> a1 >> b1 >> c1 >> d1;
                cout << a1 << " " << b1 << " " << c1 << " " << d1 << endl;
                temp->setCoEfficients(a1,b1,c1,d1);
                myfile >> s;
                cout << s << endl;
                temp->setShine(s);
                objects.push_back(temp);
                cout << endl;
            }
            else if(shape == "general"){
                Object *temp;
                double A,B,C,D,E,F,G,H,I,J;
                myfile >> A >> B >> C >> D >> E >> F >> G >> H >> I >> J;
                cout << A << " " << B << " " << C << " " << D << " " <<  E << " "  << F << " "  << G << " "  << H << " "
                 << I << " "  << J << endl;
                Vector3 v;
                double length, width, height;
                myfile >> v.x >> v.y >> v.z >> length >> width >> height;
                cout << v.x << " " << v.y << " " << v.z << " " << length << " " << width << " " << height<< endl;
                temp = new GeneralQuadratic(A,B,C,D,E,F,G,H,I,J, v, length, width, height);
                myfile >> v.x >> v.y >> v.z;
                cout << v.x << " " << v.y << " " << v.z << endl;
                temp->setColor(v.x, v.y, v.z);
                double a1,b1,c1,d1;
                int s;
                myfile >> a1 >> b1 >> c1 >> d1;
                cout << a1 << " " << b1 << " " << c1 << " "<< d1<<endl;
                temp->setCoEfficients(a1,b1,c1,d1);
                myfile >> s;
                cout << s << endl;
                temp->setShine(s);
                objects.push_back(temp);
                cout << endl;
            }
        }

        int num_of_light;
        myfile >> num_of_light;
        cout << num_of_light << endl;
        Vector3 v;
        cout << endl;
        for(int i = 0; i < num_of_light; i++){
            myfile >> v.x >> v.y >> v.z;
            cout << v.x << " " << v.y << " " << v.z << endl;
            lights.push_back(v);
        }
        myfile.close();
        cout << endl;
    }
    setUpFloor();
}


int main(int argc, char **argv){
    //loadTestData();
    loadActualData();
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Ray Tracing:1305087");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);

	glutMainLoop();		//The main loop of OpenGL
    clearMemory();
	return 0;
}

