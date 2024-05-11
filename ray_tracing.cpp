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
    Vector* a = new Vector(this->x - v.x, this->y - v.y, this->z - v.z);
    return *a;
}
Vector Vector::operator-() const {
    Vector* a = new Vector((-1)*this->x,(-1)*this->y,(-1)*this->z);
    return *a;
}
Vector Vector::operator+(Vector v) const {
    Vector* a = new Vector(this->x + v.x, this->y + v.y, this->z + v.z);
    return *a;
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



//Ahoy mateys, thar be Rays!

//Constructor. The default constructor is in the header.
Ray::Ray(Vector start, Vector direct) {
    this->start = start;
    this->direct = direct;
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
    Color* a = new Color(this->red + v.red, this->green + v.green, this->blue + v.blue);
    return *a;
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
    double a = r.get_direction().dot_product(r.get_direction());
    double b = 2*(r.get_direction().dot_product(r.get_start()-this->center));
    double c = (r.get_start() - this->center).dot_product(r.get_start() - this->center) - pow(this->radius, 2);
    
    //Ensuring we don't get a negative value inside the square root.
    if ((pow(b,2) - 4*a*c) < 0) {
        return -1;
    }
    double t1 = ((-1)*b + sqrt(pow(b,2) - 4*a*c))/(2*a);
    double t2 = ((-1)*b - sqrt(pow(b,2) - 4*a*c))/(2*a);
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
    this->arr = new Shape*[100]();
    this->length = 0;
    this->l_amb = l_amb;
    this->l_spec = l_spec;
    this->l_spec_exp = l_spec_exp;
    this->max_refl = max_refl;
}
Scene::Scene(Vector cam, Vector light) {
    this->camera = cam;
    this->light = light;
    this->back = Color(135, 206, 235);
    this->arr = new Shape*[100]();
    this->length = 0;
    this->l_amb = 0.2;
    this->l_spec = 0.5;
    this->l_spec_exp = 8;
    this->max_refl = 6;
}

//Destructor
Scene::~Scene() {
    unsigned i = 0;
    while (i < this->length) {

        //This line
        delete this->arr[i];
        i++;
    }
    delete this->arr;
}

//Methods
void Scene::add_shape(Shape* s) {
    this->arr[this->length] = s;
    this->length++;
}

//This function will probably result in the most ungodly bugs ever. Watch out for it. It is dishonest
Color Scene::get_ray_color(Ray r, unsigned refl_num) {
    int i = 0;
    std::vector<std::pair<Shape*, double>> col_arr;
    while (this->arr[i] != nullptr) {

        //The ray collided with this object.
        if (this->arr[i]->get_collision_time(r) > 0) {
            std::pair<Shape*, double> pair (this->arr[i], this->arr[i]->get_collision_time(r));
            col_arr.push_back(pair);
        }
        i++;
    }

    //Check which of the objects the ray collided with was first. That will be its first color.
    int len = col_arr.size();

    //If the ray didn't intersect anything, it hit the background.
    if (len == 0){
        return this->back;
    }
    i = 0;
    double min_col = DBL_MAX;
    Shape* first_shape = nullptr;
    while (i < len) {
        if (col_arr[i].second < min_col) {
            min_col = col_arr[i].second;
            first_shape = col_arr[i].first;
        }
        i++;
    }

    //Calculating important symbols
    Vector inter = r.get_start() + first_shape->get_collision_time(r)*r.get_direction();
    inter = (1.0/(inter.magnitude()))*inter;
    Vector lt = (1.0/((this->light-inter).magnitude()))*(this->light - inter);
    Vector n = first_shape->get_normal_vector(inter);
    n = (1.0/n.magnitude())*n;

    //Ambient light.
    double ambient_factor = (this->l_amb)*(1.0-first_shape->get_reflectivity());
    Color ray_amb = ambient_factor*(first_shape->get_color());

    //Diffuse light

    //Problem: both diffuse light and specular light are really small
    double ray_dif_scalar = (1.0-ambient_factor)*(1.0-first_shape->get_reflectivity());
    ray_dif_scalar *= std::max(0.0, n.dot_product(lt));
    Color ray_dif = ray_dif_scalar*first_shape->get_color();

    //Specular light
    Vector h = lt+(1.0/(r.get_direction().magnitude()))*(-r.get_direction());
    h = (1.0/(h.magnitude()))*h;
    double ray_spec_scalar = this->l_spec * pow((std::max(0.0, h.dot_product(n))),this->l_spec_exp);
    Color ray_spec = ray_spec_scalar * Color(255,255,255);

    //Reflections

    // We have to limit the maximum number of reflections somehow.
    if (refl_num >= this->max_refl) {
        return ray_amb + ray_dif + ray_spec;
    }
    else {
        Vector v = (1.0/(r.get_direction().magnitude()))*r.get_direction();

        //I think the project method has bugs.
        Vector dir = (-v+2*((-v)+(-v.project(n))));
        Ray refl = Ray(inter, dir);
        Color ray_refl = ((1.0-ambient_factor)*first_shape->get_reflectivity())*get_ray_color(refl, refl_num+1);
        return ray_amb + ray_dif + ray_spec + ray_refl;
    }
} 

Color Scene::get_point_color(Vector p) {
    Vector dir = p - this->camera;
    Ray r = Ray(this->camera, dir);
    return this->get_ray_color(r, 0);
}
