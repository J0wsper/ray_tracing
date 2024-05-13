#include "ray_tracing.hpp"
#include <string>
#include <math.h>
#include <utility>
#include <float.h>



//Lo, here be Vectors!

//Constructor. The default constructor is in the header.
Vector::Vector(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

//Operators
Vector operator*(double c, Vector v) {
    v.x = c*v.x;
    v.y = c*v.y;
    v.z = c*v.z;
    return v;
}
Vector Vector::operator-(Vector v) const {
    Vector a = Vector(this->x - v.x, this->y - v.y, this->z - v.z);
    return a;
}
Vector Vector::operator-() const {
    Vector a = Vector((-1)*this->x,(-1)*this->y,(-1)*this->z);
    return a;
}
Vector Vector::operator+(Vector v) const {
    Vector a = Vector(this->x + v.x, this->y + v.y, this->z + v.z);
    return a;
}

//Getters
double Vector::get_x() const {
    return this->x;
}
double Vector::get_y() const {
    return this->y;
}
double Vector::get_z() const {
    return this->z;
}

//Setters
void Vector::set_x(double new_x) {
    this->x = new_x;
}
void Vector::set_y(double new_y) {
    this->y = new_y;
}
void Vector::set_z(double new_z) {
    this->z = new_z;
}

//Methods
double Vector::dot_product(Vector v) const {
    double x_dot = v.get_x()*this->x;
    double y_dot = v.get_y()*this->y;
    double z_dot = v.get_z()*this->z;
    return x_dot + y_dot + z_dot;
}
double Vector::magnitude() const {
    double square_mag = pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2);
    return sqrt(square_mag);
}
Vector Vector::project(Vector v) const {
    double scalar = (this->dot_product(v))/(v.dot_product(v));
    return scalar*v;
}
void Vector::normalize() {
    Vector v = Vector(this->x, this->y, this->z);
    v = (1.0/v.magnitude())*v;
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
}



//Ahoy mateys, thar be Rays!

//Constructor. The default constructor is in the header.
Ray::Ray(Vector start, Vector direct) {
    this->start = start;

    //Rays have normalized direction.
    this->direct = (1.0/direct.magnitude())*direct;
}

//Getters
Vector Ray::get_start() const {
    return this->start;
}
Vector Ray::get_direction() const {
    return this->direct;
}



//Arghh!!! Colors ho!

//Constructor. The default constructor is in the header.
Color::Color(double red, double green, double blue) {
    this->red = red;
    this->green = green;
    this->blue = blue;
}

//Getters
double Color::get_red() {
    if (this->red > 255) {
        this->red = 255;
    } 
    return this->red;
}
double Color::get_green() {
    if (this->green > 255) {
        this->green = 255;
    }
    return this->green;
}
double Color::get_blue() {
    if (this->blue > 255) {
        this->blue = 255;
    } 
    return this->blue;
}

//Operators
Color Color::operator+(Color v) const {
    Color a = Color(this->red + v.red, this->green + v.green, this->blue + v.blue);
    return a;
}
Color Color::operator+=(Color v) {
    this->red = this->red + v.red;
    this->green = this->green + v.green;
    this->blue = this->blue + v.blue;
    return *this;
}
Color operator*(double c, Color v) {
    v.red = c*v.red;
    v.green = c*v.green;
    v.blue = c*v.blue;
    return v;
}



//Shapes!

//Constructor
Shape::Shape(Color c, double ref) {
    this->color = c;
    this->ref = ref;
}


//Getters
Color Shape::get_color() const {
    return this->color;
}
double Shape::get_reflectivity() const {
    return this->ref;
}



//Sphere moment

//Constructor
Sphere::Sphere(Color c, double ref, Vector v, double r) : Shape(c, ref) {
    this->center = v;
    this->radius = r;
}

//Redefined virtual methods
double Sphere::get_collision_time(Ray r) const {
    Vector m = r.get_start() - this->center;
    double a = r.get_direction().dot_product(r.get_direction());
    double b = 2*(r.get_direction().dot_product(m));
    double c = m.dot_product(m) - pow(this->radius, 2);
    double discr = pow(b,2)-4*a*c;

    //Ensuring we don't get a negative value inside the square root.
    if (discr < 0) {
        return -1;
    }

    //Finding the smaller value.
    double t1 = ((-1)*b + sqrt(discr))/(2*a);
    double t2 = ((-1)*b - sqrt(discr))/(2*a);
    if (t1 == -0 || t2 == -0) {
        return 0;
    }
    if (t1 <= 0) {
        return t2;
    }
    else if (t2 <= 0) {
        return t1;
    }
    else if (t1 < t2) {
        return t1;
    }
    else {
        return t2;
    }
}
Vector Sphere::get_normal_vector(Vector v) const {
    return v - this->center;
}


//Can we pretend that airplanes in the night sky are like shooting stars

//Constructor
Plane::Plane(Color c, double ref, Vector point, Vector normal) : Shape(c, ref) {
    this->point = point;
    this->normal = normal;
}

//Methods
double Plane::get_collision_time(Ray r) const {
    double num = (this->point - r.get_start()).dot_product(this->normal);
    double dem = r.get_direction().dot_product(this->normal);

    //Ensuring we don't get a divide by 0 error
    if (dem == 0) {
        return -1;
    }
    return num/dem;
}
Vector Plane::get_normal_vector(Vector v) const {
    return this->normal;
}



//Scenes!

//Constructors
Scene::Scene(Vector cam, Vector light, Color back, double l_amb, double l_spec, double l_spec_exp, double max_refl) {
    this->camera = cam;
    this->light = light;
    this->back = back;
    std::vector<std::shared_ptr<Shape>> shape_arr;
    this->arr = std::move(shape_arr);
    this->l_amb = l_amb;
    this->l_spec = l_spec;
    this->l_spec_exp = l_spec_exp;
    this->max_refl = max_refl;
}
Scene::Scene(Vector cam, Vector light) {
    this->camera = cam;
    this->light = light;
    this->back = Color(135, 206, 235);
    std::vector<std::shared_ptr<Shape>> shape_arr;
    this->arr = std::move(shape_arr);
    this->l_amb = 0.2;
    this->l_spec = 0.5;
    this->l_spec_exp = 8;
    this->max_refl = 6;
}
Scene::~Scene() {
    this->arr.clear();
}

//Methods
void Scene::add_shape(std::shared_ptr<Shape> s) {
    this->arr.push_back(s);
}

//Primarily used for recursion purposes in reflections with get_point_color().
Color Scene::get_ray_color(Ray r, unsigned refl_num) {
    std::vector<std::pair<std::shared_ptr<Shape>, double>> col_arr;
    for (unsigned i = 0; i < this->arr.size(); i++) {

        //The ray collided with this object.
        if (this->arr[i]->get_collision_time(r) > 0) {
            std::pair<std::shared_ptr<Shape>, double> pair (this->arr[i], this->arr[i]->get_collision_time(r));
            col_arr.push_back(pair);
        }
    }

    //Check which of the objects the ray collided with was first. That will be its first color.
    unsigned len = col_arr.size();

    //If the ray didn't intersect anything, it hit the background.
    if (len == 0){
        return this->back;
    }
    double min_col = DBL_MAX;
    std::shared_ptr<Shape> first_shape = nullptr;
    for (unsigned i = 0; i < len; i++) {
        if (col_arr[i].second < min_col) {
            min_col = col_arr[i].second;
            first_shape = col_arr[i].first;
        }
    }


    //calculating important symbols

    //Intersection point.
    Vector inter = r.get_start() + first_shape->get_collision_time(r)*r.get_direction();
    
    //Surface normal.
    Vector N = first_shape->get_normal_vector(inter);
    N.normalize();

    //Unit vector pointing from intersection point to light.
    Vector L = this->light - inter;
    L.normalize();

    //Unit vector pointing from POV to intersection point.
    Vector V = inter - this->camera;
    V.normalize();

    //Light reflection vector.
    Vector R = r.get_direction() - 2*(r.get_direction().dot_product(N))*N;
    R.normalize();


    //Detects if the point is in shadow.
    Ray S = Ray(inter+1e-6*L, L);
    bool is_shadow = false;
    std::vector<std::shared_ptr<Shape>>::iterator ptr;
    for (ptr = this->arr.begin(); ptr < this->arr.end(); ptr++) {
        if ((*ptr)->get_collision_time(S) > 1e-6) {
            is_shadow = true;
            break;
        }
    }

    //Ambient light
    Color r_amb = this->l_amb*first_shape->get_color();

    //Diffuse and specular light are not calculated if the point is in shadow
    Color r_dif = Color();
    Color r_spec = Color();
    if (is_shadow == false) {

        //Diffuse light
        r_dif = (1-first_shape->get_reflectivity())*(std::max(0.0,N.dot_product(L)))*first_shape->get_color();

        //Specular light
        r_spec = this->l_spec*pow(std::max(0.0,R.dot_product(V)), this->l_spec_exp)*Color(255,255,255);
    }

    //TODO: Reflections have some strange noise. Fix it?

    //Checks to see whether reflections should be applied.
    if (refl_num >= this->max_refl || first_shape->get_reflectivity() == 0) {
        return r_amb + r_dif + r_spec;
    }
    else {

        //Reflections
        Ray refl = Ray(inter+1e-6*R, R);
        Color r_refl = first_shape->get_reflectivity()*get_ray_color(refl, refl_num+1);
        return r_amb + r_dif + r_spec + r_refl;
    }
} 

//Primarily a wrapper function for get_ray_color().
Color Scene::get_point_color(Vector p) {
    Vector dir = p - this->camera;
    Ray r = Ray(this->camera, dir);
    return this->get_ray_color(r, 0);
}
