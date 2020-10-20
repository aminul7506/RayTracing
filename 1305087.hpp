#include<bits/stdc++.h>
#define pi (2*acos(0.0))
using namespace std;
class Vector3 {
public:
    double x, y, z;
};

class Ray{
    public:
    Vector3 start;
    Vector3 dir;
    Ray(Vector3 start, Vector3 dir) {
        this->start = start;
        this->dir = dir;
    }

};

class Object {

public:

    Vector3 reference_point;
    double height, width, length;
    int shine;
    double color[3];
    double co_efficients[4];
    double source_factor = 1.0;
    double eta = 1.5;
    bitmap_image texture;
    double textureHeight, textureWidth;
    Object(){ }
    virtual void draw(){}
    virtual void setTexture(){}
    void setColor(double r, double g, double b)
    {
        color[0] = r;
        color[1] = g;
        color[2] = b;
    }
    void setShine(int shine)
    {
        this->shine = shine;
    }
    void setCoEfficients(double p, double q, double r, double s)
    {
        co_efficients[0] = p;
        co_efficients[1] = q;
        co_efficients[2] = r;
        co_efficients[3] = s;
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
    virtual double intersect(Ray *r, double *current_color, int level){
        return -1;
    }
    virtual Vector3 getNormal(Vector3 intersection) {};
    virtual double getIntersectingT(Ray *r) {return -1;};
    Vector3 getReflection(Ray* r, Vector3 normal)
    {
        Vector3 reflection;
        reflection.x = r->dir.x - normal.x * 2 * (r->dir.x*normal.x + r->dir.y*normal.y + r->dir.z*normal.z);
        reflection.y = r->dir.y - normal.y * 2 * (r->dir.x*normal.x + r->dir.y*normal.y + r->dir.z*normal.z);
        reflection.z = r->dir.z - normal.z * 2 * (r->dir.x*normal.x + r->dir.y*normal.y + r->dir.z*normal.z);
        Vector3 reflection1 = normalize(reflection);
        return reflection1;
    }
    Vector3 getRefraction(Ray *r, Vector3 normal)
    {
        Vector3 v;
        double product = r->dir.x * normal.x + r->dir.y * normal.y + r->dir.z * normal.z;
        double k = 1.0 - eta * eta * (1.0 - product * product);
        if( k < 0){
            v.x = 0;
            v.y = 0;
            v.z = 0;
        }
        else{
            v.x = eta * r->dir.x - (eta * product + pow(k, 0.5)) * normal.x;
            v.y = eta * r->dir.y - (eta * product + pow(k, 0.5)) * normal.y;
            v.z = eta * r->dir.z - (eta * product + pow(k, 0.5)) * normal.z;
            v = normalize(v);
        }
        return v;
    }
    Vector3 textureSetting(Vector3 intersectionPoint)
    {
        unsigned char red, green, blue;
        int value1 = (intersectionPoint.x + abs(reference_point.x)) * textureWidth;
        int value2 = (intersectionPoint.y + abs(reference_point.y)) * textureHeight;
        texture.get_pixel(value1, value2, red, green, blue);
        Vector3 v;
        v.x = red;
        v.y = green;
        v.z =  blue;
        return v;
    }
};

vector <Object*> objects;
vector <Vector3> lights;
int recursion_level;

class Sphere: public Object {

public:
    Sphere(Vector3 center, double radius){
        reference_point = center;
        length = radius;
    }
    void draw(){
        Vector3 points[100][100];
        int i,j;
        double h,r;
        int slices = 24;
        int stacks = 20;
        glColor3f(color[0], color[1], color[2]);
        //generate points
        for(i=0;i<=stacks;i++)
        {
            h=length*sin(((double)i/(double)stacks)*(pi/2));
            r=length*cos(((double)i/(double)stacks)*(pi/2));
            for(j=0;j<=slices;j++){
                points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
                points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
                points[i][j].z=h;
            }
        }
	//draw quads using generated points
        for(i=0;i<stacks;i++)
        {
            //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
            for(j=0;j<slices;j++)
            {
                glBegin(GL_QUADS);{
			    //upper hemisphere
                    glVertex3f(points[i][j].x+reference_point.x,points[i][j].y+reference_point.y,points[i][j].z+reference_point.z);
                    glVertex3f(points[i][j+1].x+reference_point.x,points[i][j+1].y+reference_point.y,points[i][j+1].z+reference_point.z);
                    glVertex3f(points[i+1][j+1].x+reference_point.x,points[i+1][j+1].y+reference_point.y,points[i+1][j+1].z+reference_point.z);
                    glVertex3f(points[i+1][j].x+reference_point.x,points[i+1][j].y+reference_point.y,points[i+1][j].z+reference_point.z);
                    //lower hemisphere
                    glVertex3f(points[i][j].x+reference_point.x,points[i][j].y+reference_point.y,-points[i][j].z+reference_point.z);
                    glVertex3f(points[i][j+1].x+reference_point.x,points[i][j+1].y+reference_point.y,-points[i][j+1].z+reference_point.z);
                    glVertex3f(points[i+1][j+1].x+reference_point.x,points[i+1][j+1].y+reference_point.y,-points[i+1][j+1].z+reference_point.z);
                    glVertex3f(points[i+1][j].x+reference_point.x,points[i+1][j].y+reference_point.y,-points[i+1][j].z+reference_point.z);
                }glEnd();
            }
        }
    }

    Vector3 getNormal(Vector3 intersectionPoint)
    {
        Vector3 temp;
        temp.x = intersectionPoint.x - reference_point.x;
        temp.y = intersectionPoint.y - reference_point.y;
        temp.z = intersectionPoint.z - reference_point.z;
        Vector3 temp1 = normalize(temp);
        return temp1;

    }

    double getIntersectingT(Ray *r)
    {
        Vector3 r_0;
        r_0.x = r->start.x - reference_point.x;
        r_0.y = r->start.y - reference_point.y;
        r_0.z = r->start.z - reference_point.z;
        double a = r->dir.x * r->dir.x + r->dir.y * r->dir.y + r->dir.z * r->dir.z;
        double b = 2 * (r->dir.x * r_0.x + r->dir.y * r_0.y + r->dir.z * r_0.z);
        double c = (r_0.x * r_0.x + r_0.y * r_0.y + r_0.z * r_0.z) - (length * length);
        double d = b * b - 4 * a * c;
        if(d < 0) return -1;
        double t1 = (-b + pow(d, 0.5))/(2 * a);
        double t2 = (-b - pow(d, 0.5))/(2 * a);
        double min_t;
        if(t1 >= t2)min_t = t2;
        else min_t = t1;
        return min_t;
    }

    double intersect(Ray *r, double current_color[3], int level)
    {
        double t = getIntersectingT(r);
        if(t <= 0)  return -1;

        if(level == 0)return t;
        for (int i = 0; i < 3; i++) {
            current_color[i] = color[i] * co_efficients[0];//ambient
        }

        Vector3 intersectionPoint;
        intersectionPoint.x = r->start.x + r->dir.x * t;
        intersectionPoint.y = r->start.y + r->dir.y * t;
        intersectionPoint.z = r->start.z + r->dir.z * t;
        Vector3 normal = getNormal(intersectionPoint);
        Vector3 reflection = getReflection(r, normal);
        Vector3 refraction = getRefraction(r, normal);
        int light_size = lights.size();

        for(int i = 0; i < light_size; i++){
            Vector3 direction;
            direction.x = lights[i].x - intersectionPoint.x;
            direction.y = lights[i].y - intersectionPoint.y;
            direction.z = lights[i].z - intersectionPoint.z;
            direction = normalize(direction);

            Vector3 start;
            start.x = intersectionPoint.x + direction.x * 1.0;
            start.y = intersectionPoint.y + direction.y * 1.0;
            start.z = intersectionPoint.z + direction.z * 1.0;

            Ray L(start, direction);

            int object_size = objects.size();
            int indicator = 1;
            for(int j = 0; j < object_size; j++){
                double t_temp = objects[j]->getIntersectingT(&L);
                if(t_temp > 0){
                    indicator = 0;
                    break;
                }
            }

            if (indicator == 1){
                double lambert = L.dir.x * normal.x + L.dir.y * normal.y + L.dir.z * normal.z;
                double phong = reflection.x * r->dir.x + reflection.y * r->dir.y + reflection.z * r->dir.z;
                if(lambert <= 0) lambert = 0;
                if(phong <= 0) phong = 0;
                for (int k = 0; k < 3; k++) {
                    current_color[k] += source_factor * lambert * co_efficients[1] * color[k];//diffuse
                    current_color[k] += source_factor * pow(phong, shine) * co_efficients[2] * color[k];//specular
                }
            }

            if(level < recursion_level){
                start.x = intersectionPoint.x + reflection.x * 1.0;
                start.y = intersectionPoint.y + reflection.y * 1.0;
                start.z = intersectionPoint.z + reflection.z * 1.0;

                Ray reflectionRay(start, reflection);
                int nearest = -1;
                double color_ref[3];
                double t_min = 100000;
                int object_size = objects.size();
                for(int k = 0; k < object_size; k++){
                    double t = objects[k]->getIntersectingT(&reflectionRay);
                    if(t <= 0) continue;
                    if(t < t_min){
                        nearest = k;
                        t_min = t;
                    }
                }

                if(nearest != -1){
                    double t = objects[nearest]->intersect(&reflectionRay, color_ref, level + 1);
                    //cout << t << endl;
                    if( t != -1){
                    for(int k = 0; k < 3; k++){
                        current_color[k] += color_ref[k] * co_efficients[3];//reflection
                    }}
                }

                //for refraction

                start.x = intersectionPoint.x + refraction.x * 1.0;
                start.y = intersectionPoint.y + refraction.y * 1.0;
                start.z = intersectionPoint.z + refraction.z * 1.0;

                Ray refractionRay(start, refraction);
                nearest = -1;
                color_ref[3];
                t_min = 100000;
                object_size = objects.size();
                for(int k = 0; k < object_size; k++){
                    double t = objects[k]->getIntersectingT(&refractionRay);
                    if(t <= 0) continue;
                    if(t < t_min){
                        nearest = k;
                        t_min = t;
                    }
                }

                if(nearest != -1){
                    double t = objects[nearest]->intersect(&refractionRay, color_ref, level + 1);
                    //cout << t << endl;
                    if(t != -1){
                    for(int k = 0; k < 3; k++){
                        current_color[k] += color_ref[k] * eta;//refraction

                    }}
                }
            }

            for (int k = 0; k < 3; k++) {
                if (current_color[k] > 1) current_color[k] = 1;
                else if (current_color[k] < 0) current_color[k] = 0;
            }
        }
        //cout << t << endl;
        return t;
    }

};

class Floor: public Object{
public:
    Floor(double floorWidth, double tileWidth){
        Vector3 temp;
        temp.x = -floorWidth/2, temp.y = -floorWidth/2, temp.z = 0;
        reference_point = temp;
        length = tileWidth;
    }

    void setTexture()
    {
        texture = bitmap_image("tex.bmp");
        textureHeight = texture.height()/1000.0;
        textureWidth = texture.width()/1000.0;
    }

    Vector3 getNormal(Vector3 intersectionPoint)
    {
        Vector3 v;
        v.x = 0;
        v.y = 0;
        v.z = 1;
        return v;
    }

    double getIntersectingT(Ray *r)
    {
        Vector3 normal = getNormal(reference_point);
        double nom = -(normal.x * r->start.x + normal.y * r->start.y + normal.z * r->start.z);
        double denom = normal.x * r->dir.x + normal.y * r->dir.y + normal.z * r->dir.z;
        double t = nom/denom;
        return t;
    }

    void draw(){
        int tilesNo = abs((reference_point.x * 2)/length);
        for(int i = 0; i < tilesNo; i++){
            for (int j = 0; j < tilesNo; j++){
                if((i+j) % 2 == 0)glColor3f(1.0, 1.0, 1.0);
                else glColor3f(0.0, 0.0, 0.0);
                glBegin(GL_QUADS);
                {
                    glVertex3f(reference_point.x + length * i, reference_point.y + length * j, reference_point.z);
                    glVertex3f(reference_point.x + length * i, reference_point.y + length * (j+1), reference_point.z);
                    glVertex3f(reference_point.x + length * (i+1), reference_point.y + length * (j+1), reference_point.z);
                    glVertex3f(reference_point.x + length * (i+1), reference_point.y + length * j, reference_point.z);
                }
                glEnd();
            }
        }
    }


    double intersect(Ray* r, double current_color[3], int level) {
        double min_x = reference_point.x;
        double min_y = reference_point.y;
        double max_x = -min_x;
        double max_y = -min_y;

        double t = getIntersectingT(r);
        Vector3 intersectionPoint;
        intersectionPoint.x = r->start.x + r->dir.x * t;
        intersectionPoint.y = r->start.y + r->dir.y * t;
        intersectionPoint.z = r->start.z + r->dir.z * t;

        if(min_x > intersectionPoint.x ||  max_x < intersectionPoint.x) return -1;
        if(min_y > intersectionPoint.y ||  max_y < intersectionPoint.y) return -1;

        int x_val = intersectionPoint.x / length;
        int y_val = intersectionPoint.y / length;

        if ((x_val + y_val)%2 == 0) {
            color[0] = 1;
            color[1] = 1;
            color[2] = 1;
        }
        else{
            color[0] = 0;
            color[1] = 0;
            color[2] = 0;
        }

        //texture

        Vector3 temp = textureSetting(intersectionPoint);
        double temp_a[3];
        temp_a[0] = temp.x;
        temp_a[1] = temp.y;
        temp_a[2] = temp.z;

        for (int i = 0; i < 3; i++) {
            current_color[i] = color[i] * co_efficients[0] * temp_a[i] / 255.0;//ambient
        }

        Vector3 normal = getNormal(intersectionPoint);
        Vector3 reflection = getReflection(r, normal);
        int light_size = lights.size();

        for(int i = 0; i < light_size; i++){
            Vector3 direction;
            direction.x = lights[i].x - intersectionPoint.x;
            direction.y = lights[i].y - intersectionPoint.y;
            direction.z = lights[i].z - intersectionPoint.z;
            direction = normalize(direction);

            Vector3 start;
            start.x = intersectionPoint.x + direction.x * 1.0;
            start.y = intersectionPoint.y + direction.y * 1.0;
            start.z = intersectionPoint.z + direction.z * 1.0;

            Ray L(start, direction);

            int object_size = objects.size();
            int indicator = 1;
            for(int j = 0; j < object_size; j++){
                double t_temp = objects[j]->getIntersectingT(&L);
                if(t_temp > 0){
                    indicator = 0;
                    break;
                }
            }

            if (indicator == 1){
                double lambert = L.dir.x * normal.x + L.dir.y * normal.y + L.dir.z * normal.z;
                double phong = reflection.x * r->dir.x + reflection.y * r->dir.y + reflection.z * r->dir.z;
                if(lambert <= 0) lambert = 0;
                if(phong <= 0) phong = 0;
                for (int k = 0; k < 3; k++) {
                    current_color[k] += source_factor * lambert * co_efficients[1] * color[k];//diffuse
                    current_color[k] += source_factor * pow(phong, shine) * co_efficients[2] * color[k];//specular
                }
            }

            if(level < recursion_level){
                start.x = intersectionPoint.x + reflection.x * 1.0;
                start.y = intersectionPoint.y + reflection.y * 1.0;
                start.z = intersectionPoint.z + reflection.z * 1.0;

                Ray reflectionRay(start, reflection);
                int nearest = -1;
                double color_ref[3];
                double t_min = 100000;
                int object_size = objects.size();
                for(int k = 0; k < object_size; k++){
                    double t = objects[k]->getIntersectingT(&reflectionRay);
                    if(t <= 0) continue;
                    if(t < t_min){
                        nearest = k;
                        t_min = t;
                    }
                }

                if(nearest != -1){
                    double t = objects[nearest]->intersect(&reflectionRay, color_ref, level + 1);
                    //cout << t << endl;
                    if( t != -1){
                    for(int k = 0; k < 3; k++){
                        current_color[k] += color_ref[k] * co_efficients[3];//reflection
                    }}
                }

            }

            for (int k = 0; k < 3; k++) {
                if (current_color[k] > 1) current_color[k] = 1;
                else if (current_color[k] < 0) current_color[k] = 0;
            }
        }
        //cout << t << endl;
        return t;
    }
};

class Triangle: public Object {
public:
    Vector3 a, b, c;
    Triangle(Vector3 a, Vector3 b, Vector3 c) {
        this->a = a;
        this->b = b;
        this->c = c;
    }

    void draw() {
        glColor3f(color[0],color[1],color[2]);
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(a.x, a.y, a.z);
            glVertex3f(b.x, b.y, b.z);
            glVertex3f(c.x, c.y, c.z);
        }
        glEnd();
    }

    Vector3 getNormal(Vector3 intersectionPoint) {
        Vector3 p, q;
        p.x = b.x - a.x;
        p.y = b.y - a.y;
        p.z = b.z - a.z;
        q.x = c.x - a.x;
        q.y = c.y - a.y;
        q.z = c.z - a.z;
        Vector3 product;
        product.x = p.y * q.z - p.z * q.y;
        product.y = p.z * q.x - p.x * q.z;
        product.z = p.x * q.y - p.y * q.x;
        Vector3 normal = normalize(product);
        return normal;
    }

    double getIntersectingT(Ray* r)
    {
        double EPSILON = 0.0000001;
        Vector3 p, q, h, s, q1;
        double a1,f,u,v;
        p.x = b.x - a.x;
        p.y = b.y - a.y;
        p.z = b.z - a.z;
        q.x = c.x - a.x;
        q.y = c.y - a.y;
        q.z = c.z - a.z;
        h.x = r->dir.y * q.z - r->dir.z * q.y;
        h.y = r->dir.z * q.x - r->dir.x * q.z;
        h.z = r->dir.x * q.y - r->dir.y * q.x;
        a1 = p.x * h.x + p.y * h.y + p.z * h.z;
        //cout << a1 << endl;
        //cout << r->dir.x << " " << r->dir.y << " " << r->dir.z << endl;
        if (a1 > -EPSILON && a1 < EPSILON)return -1;
        f = 1.0/a1;
        //cout << f << endl;
        s.x = r->start.x - a.x;
        s.y = r->start.y - a.y;
        s.z = r->start.z - a.z;
        u = f * (s.x * h.x + s.y * h.y + s.z * h.z);
        if (u < 0.0 || u > 1.0) return -1;
        //cout << u << endl;
        q1.x = s.y * p.z - s.z * p.y;
        q1.y = s.z * p.x - s.x * p.z;
        q1.z = s.x * p.y - s.y * p.x;
        //cout << q1.x << " " << q1.y << " " << q1.z << endl;
        v = f * (r->dir.x * q1.x + r->dir.y * q1.y + r->dir.z * q1.z);
        if (v < 0.0 || u + v > 1.0)return -1;
        double t = f * (q.x * q1.x + q.y * q1.y + q.z * q1.z);
        if (t > EPSILON) {
            return t;
        }
        else return -1;
    }


    double intersect(Ray *r, double current_color[3], int level)
    {
        double t = getIntersectingT(r);
        //cout << t << endl;
        if(t <= 0)  return -1;

        if(level == 0)return t;
        for (int i = 0; i < 3; i++) {
            current_color[i] = color[i] * co_efficients[0];//ambient
        }

        Vector3 intersectionPoint;
        intersectionPoint.x = r->start.x + r->dir.x * t;
        intersectionPoint.y = r->start.y + r->dir.y * t;
        intersectionPoint.z = r->start.z + r->dir.z * t;

        int light_size = lights.size();

        for(int i = 0; i < light_size; i++){
            Vector3 normal = getNormal(intersectionPoint);
            Vector3 direction1;
            direction1.x = lights[i].x - intersectionPoint.x;
            direction1.y = lights[i].y - intersectionPoint.y;
            direction1.z = lights[i].z - intersectionPoint.z;
            Vector3 direction = normalize(direction1);

            if((direction.x * normal.x + direction.y * normal.y + direction.z * normal.z) > 0) {
                normal.x = - normal.x;
                normal.y = - normal.y;
                normal.z = - normal.z;
            }
            //cout << normal.x << " " << normal.y << " " << normal.z << endl;
            Vector3 reflection = getReflection(r, normal);
            Vector3 start;
            start.x = intersectionPoint.x + direction.x * 1.0;
            start.y = intersectionPoint.y + direction.y * 1.0;
            start.z = intersectionPoint.z + direction.z * 1.0;
            //cout << intersectionPoint.x << " " << intersectionPoint.y << " " << intersectionPoint.z << endl;
            Ray L(start, direction);

            int object_size = objects.size();
            int indicator = 1;
            for(int j = 0; j < object_size; j++){
                double t_temp = objects[j]->getIntersectingT(&L);
                if(t_temp > 0){
                    indicator = 0;
                    break;
                }
            }

            if (indicator == 1){
                double lambert = L.dir.x * normal.x + L.dir.y * normal.y + L.dir.z * normal.z;
                double phong = reflection.x * r->dir.x + reflection.y * r->dir.y + reflection.z * r->dir.z;
                if(lambert <= 0) lambert = 0;
                if(phong <= 0) phong = 0;
                for (int k = 0; k < 3; k++) {
                    current_color[k] += source_factor * lambert * co_efficients[1] * color[k];//diffuse
                    current_color[k] += source_factor * pow(phong, shine) * co_efficients[2] * color[k];//specular
                }
            }

            if(level < recursion_level){
                start.x = intersectionPoint.x + reflection.x * 1.0;
                start.y = intersectionPoint.y + reflection.y * 1.0;
                start.z = intersectionPoint.z + reflection.z * 1.0;

                Ray reflectionRay(start, reflection);
                int nearest = -1;
                double color_ref[3];
                double t_min = 100000;
                int object_size = objects.size();
                for(int k = 0; k < object_size; k++){
                    double t = objects[k]->getIntersectingT(&reflectionRay);
                    if(t <= 0) continue;
                    if(t < t_min){
                        nearest = k;
                        t_min = t;
                    }
                }

                if(nearest != -1){
                    double t = objects[nearest]->intersect(&reflectionRay, color_ref, level + 1);
                    //cout << t << endl;
                    if(t != -1){
                    for(int k = 0; k < 3; k++){
                        current_color[k] += color_ref[k] * co_efficients[3];//reflection
                    }}
                }

            }

            for (int k = 0; k < 3; k++) {
                if (current_color[k] > 1) current_color[k] = 1;
                else if (current_color[k] < 0) current_color[k] = 0;
            }
        }
        return t;
    }

};

class GeneralQuadratic: public Object {
public:
    double A,B,C,D,E,F,G,H,I,J;

    GeneralQuadratic(double A,double B, double C,double D,double E,
                     double F,double G,double H,double I,double J,Vector3 reference_point, double length, double width, double height)
{
        this->A = A;
        this->B = B;
        this->C = C;
        this->D = D;
        this->E = E;
        this->F = F;
        this->G = G;
        this->H = H;
        this->I = I;
        this->J = J;
        this->length = length;
        this->height = height;
        this->width = width;
        this->reference_point = reference_point;
    }

    void draw()
    {

    }

    double calculateA(Ray *r, double A, double B, double C, double D, double E, double F)
    {
        return A * r->dir.x * r->dir.x + B * r->dir.y * r->dir.y + C * r->dir.z * r->dir.z + D * r->dir.x * r->dir.y +
         E * r->dir.y * r->dir.z + F * r->dir.z * r->dir.x;
    }

    double calculateB(Ray *r, double A, double B, double C, double D, double E, double F, double G, double H, double I)
    {
        return (2 * (A * r->start.x * r->dir.x + B * r->start.y * r->dir.y + C * r->start.z * r->dir.z)) +
            D * (r->start.x * r->dir.y + r->dir.x * r->start.y) + E * (r->start.y * r->dir.z +
            r->dir.y * r->start.z) + F * (r->start.z * r->dir.x + r->dir.z * r->start.x) + G * r->dir.x +
            H * r->dir.y + I * r->dir.z;
    }

    double calculateC(Ray *r ,double A,double B,double C, double D, double E, double F, double G, double H, double I, double J)
    {
        return A * r->start.x * r->start.x + B * r->start.y * r->start.y + C * r->start.z * r->start.z +
                D * r->start.x * r->start.y + E * r->start.y * r->start.z
                + F * r->start.z * r->start.x +G * r->start.x + H * r->start.y + I * r->start.z + J;
    }

    Vector3 getNormal(Vector3 intersectionPoint)
    {
        Vector3 v;
        v.x = G + 2 * intersectionPoint.x * A + D * intersectionPoint.y + F * intersectionPoint.z;
        v.y = H + 2 * intersectionPoint.y * B + D * intersectionPoint.x + E * intersectionPoint.z;
        v.z = I + 2 * intersectionPoint.z * C + E * intersectionPoint.y + F * intersectionPoint.x;
        Vector3 normal = normalize(v);
        return normal;
    }


    double getIntersectingT(Ray* r)
    {
        double a, b, c, d;
        a = calculateA(r, A, B, C, D, E, F);
        b = calculateB(r, A, B, C, D, E, F, G, H, I);
        c = calculateC(r, A, B, C, D, E, F, G, H, I, J);
        d = b * b - 4 * a * c;
        if(d < 0) return -1;
        //if(!isnan(d) and d != 0) cout << d << endl;
        double t1 = (-b + pow(d, 0.5))/(2 * a);
        double t2 = (-b - pow(d, 0.5))/(2 * a);
        //if(!isnan(t1)) cout << t1 << endl;
        //cout << t1 << " " << t2 << endl;
        //if(!isnan(t1) and !isnan(t2)) cout << t1 << " " << t2 << endl;

        double min_x, min_y, min_z;
        min_x = reference_point.x;
        min_y = reference_point.y;
        min_z = reference_point.z;

        double max_x, max_y, max_z;
        max_x = reference_point.x + length;
        max_y = reference_point.x + width;
        max_z = reference_point.x + height;

        Vector3 intersectionPoint1, intersectionPoint2;
        intersectionPoint1.x = r->start.x + r->dir.x * t1;
        intersectionPoint1.y = r->start.y + r->dir.y * t1;
        intersectionPoint1.z = r->start.z + r->dir.z * t1;
        intersectionPoint2.x = r->start.x + r->dir.x * t2;
        intersectionPoint2.y = r->start.y + r->dir.y * t2;
        intersectionPoint2.z = r->start.z + r->dir.z * t2;

        int indicator1 = 0, indicator2 = 0;
        if((length > 0 and (min_x > intersectionPoint1.x || max_x < intersectionPoint1.x)) ||
           (width > 0 and   (min_y > intersectionPoint1.y || max_y < intersectionPoint1.y)) ||
           (height > 0 and  (min_z > intersectionPoint1.z || max_z < intersectionPoint1.z))) indicator1 = 1;
        if((length > 0 and (min_x > intersectionPoint2.x || max_x < intersectionPoint2.x)) ||
           (width > 0 and   (min_y > intersectionPoint2.y || max_y < intersectionPoint2.y)) ||
           (height > 0 and  (min_z > intersectionPoint2.z || max_z < intersectionPoint2.z))) indicator2 = 1;

        double t;
        if(indicator1 == 1 and indicator2 == 1)return -1;
        else if(indicator1 == 1)t = t2;
        else if(indicator2 == 1)t = t1;
        else{
            if(t1 >= t2)t = t2;
            else t = t1;
        }
        return t;

    }

    double intersect(Ray *r, double current_color[3], int level)
    {
        double t = getIntersectingT(r);
        if(t <= 0)  return -1;

        if(level == 0)return t;
        for (int i = 0; i < 3; i++) {
            current_color[i] = color[i] * co_efficients[0];//ambient
        }

        Vector3 intersectionPoint;
        intersectionPoint.x = r->start.x + r->dir.x * t;
        intersectionPoint.y = r->start.y + r->dir.y * t;
        intersectionPoint.z = r->start.z + r->dir.z * t;
        Vector3 normal = getNormal(intersectionPoint);
        Vector3 reflection = getReflection(r, normal);
        int light_size = lights.size();

        for(int i = 0; i < light_size; i++){
            Vector3 direction;
            direction.x = lights[i].x - intersectionPoint.x;
            direction.y = lights[i].y - intersectionPoint.y;
            direction.z = lights[i].z - intersectionPoint.z;
            direction = normalize(direction);

            Vector3 start;
            start.x = intersectionPoint.x + direction.x * 1.0;
            start.y = intersectionPoint.y + direction.y * 1.0;
            start.z = intersectionPoint.z + direction.z * 1.0;

            Ray L(start, direction);

            int object_size = objects.size();
            int indicator = 1;
            for(int j = 0; j < object_size; j++){
                double t_temp = objects[j]->getIntersectingT(&L);
                if(t_temp > 0){
                    indicator = 0;
                    break;
                }
            }

            if (indicator == 1){
                double lambert = L.dir.x * normal.x + L.dir.y * normal.y + L.dir.z * normal.z;
                double phong = reflection.x * r->dir.x + reflection.y * r->dir.y + reflection.z * r->dir.z;
                if(lambert <= 0) lambert = 0;
                if(phong <= 0) phong = 0;
                for (int k = 0; k < 3; k++) {
                    current_color[k] += source_factor * lambert * co_efficients[1] * color[k];//diffuse
                    current_color[k] += source_factor * pow(phong, shine) * co_efficients[2] * color[k];//specular
                }
            }

            if(level < recursion_level){
                start.x = intersectionPoint.x + reflection.x * 1.0;
                start.y = intersectionPoint.y + reflection.y * 1.0;
                start.z = intersectionPoint.z + reflection.z * 1.0;

                Ray reflectionRay(start, reflection);
                int nearest = -1;
                double color_ref[3];
                double t_min = 100000;
                int object_size = objects.size();
                for(int k = 0; k < object_size; k++){
                    double t = objects[k]->getIntersectingT(&reflectionRay);
                    if(t <= 0) continue;
                    if(t < t_min){
                        nearest = k;
                        t_min = t;
                    }
                }

                if(nearest != -1){
                    double t = objects[nearest]->intersect(&reflectionRay, color_ref, level + 1);
                    //cout << t << endl;
                    if( t != -1){
                    for(int k = 0; k < 3; k++){
                        current_color[k] += color_ref[k] * co_efficients[3];//reflection
                    }}
                }
            }

            for (int k = 0; k < 3; k++) {
                if (current_color[k] > 1) current_color[k] = 1;
                else if (current_color[k] < 0) current_color[k] = 0;
            }
        }
        //if(t != -1)cout << t << endl;
        return t;
    }
};
