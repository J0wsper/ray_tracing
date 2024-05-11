#ifndef _RAY_TRACING_HPP_
#define _RAY_TRACING_HPP_
#include <memory>
#include <vector>

class Vector {
private:
    double x;
    double y;
    double z;

public:

    //Constructor
    Vector(double, double, double);
    Vector() : x(0), y(0), z(0) {};

    //Getters
    double get_x() const;
    double get_y() const;
    double get_z() const;

    //Setters
    void set_x(double);
    void set_y(double);
    void set_z(double);

    //Methods
    double dot_product(Vector) const;
    double magnitude() const;
    Vector project(Vector) const;

    //Operators
    Vector operator-(Vector) const;
    Vector operator-() const;
    Vector operator+(Vector) const;
    friend Vector operator*(double, Vector);
};

class Ray {
private:
    Vector start;
    Vector direct;
public:

    //Constructor
    Ray(Vector, Vector);
    Ray() : start(Vector(0,0,0)), direct(Vector(0,0,0)) {};

    //Getters
    Vector get_start() const;
    Vector get_direction() const;
};

class Color {
private:
    double red;
    double green;
    double blue;
    
public:

    //Constructors
    Color(double, double, double);
    Color() : red(0), green(0), blue(0) {};

    //Getters
    double get_red();
    double get_green();
    double get_blue();

    //Operators
    Color operator+(Color) const;
    Color operator+=(Color);
    friend Color operator*(double, Color);
};

class Shape {
private:
    Color color;
    double ref;

public:
    virtual double get_collision_time(Ray) const = 0;
    virtual Vector get_normal_vector(Vector) const = 0;
    Shape(Color, double);
    virtual ~Shape() = default;
    Color get_color() const;
    double get_reflectivity() const;
};

class Sphere : public Shape {

private:
    Color color;
    double ref;
    Vector center;
    double radius;

public:
    Sphere(Color, double, Vector, double);
    ~Sphere() = default;
    double get_collision_time(Ray) const override;
    Vector get_normal_vector(Vector) const override;
};

class Plane : public Shape {
private:
    Vector point;
    Vector normal;

public:
    Plane(Color, double, Vector, Vector);
    ~Plane() = default;
    double get_collision_time(Ray) const override;
    Vector get_normal_vector(Vector) const override;

};

class Scene {
private:
    Vector camera;
    Vector light;
    Color back;
    unsigned length;
    Shape** arr;
    double l_amb;
    double l_spec;
    unsigned l_spec_exp;
    unsigned max_refl;
    Color get_ray_color(Ray, unsigned);
public:
    Scene(Vector, Vector, Color, double, double, double, double);
    Scene(Vector, Vector);
    ~Scene();
    void add_shape(Shape*);
    Color get_point_color(Vector);
};

//Because this is a template, it is being implemented in the header ;-;
template<typename T>
class Array2D {
private:
    unsigned width;
    unsigned height;
    std::vector<std::vector<std::unique_ptr<T>>> arr;
public:
    Array2D(unsigned width, unsigned height) {
        std::vector<std::vector<std::unique_ptr<T>>> arr(width);
        for (unsigned i = 0; i < width; i++) {
           std::vector<std::unique_ptr<T>> subarr(height);
           arr[i] = std::move(subarr);
        }
        this->arr = std::move(arr);
        this->width = width;
        this->height = height;
    }
    Array2D(const Array2D&) = delete;
    unsigned get_width() const {
        return this->width;
    }
    unsigned get_height() const {
        return this->height;
    }
    T get(unsigned x, unsigned y) const {
        return *(this->arr[x][y]);
    }
    void set(unsigned x, unsigned y, T val) {
        std::unique_ptr<T> lil_dude = std::make_unique<T>(val);
        this->arr[x][y] = std::move(lil_dude);
    }
}; 

#endif