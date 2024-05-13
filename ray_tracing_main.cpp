#include "ray_tracing.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include "ray_tracing.hpp"


void write_ppm(const Array2D<Color>& pixels, const std::string& filename) {
    std::ofstream out(filename, std::ios::binary | std::ios::out);
    out.write("P6\n", 3);
    std::string s = std::to_string(pixels.get_width());
    out.write(s.c_str(), s.length());
    out.write(" ", 1);
    s = std::to_string(pixels.get_height());
    out.write(s.c_str(), s.length());
    out.write("\n255\n", 5);
    char color[3];
    for (unsigned y = 0; y < pixels.get_height(); y++) {
        for (unsigned x = 0; x < pixels.get_width(); x++) {
        Color c = pixels.get(x, y);
        color[0] = c.get_red();
        color[1] = c.get_green();
        color[2] = c.get_blue();
        out.write(color, 3);
        }
    }
    out.close();
}

int main() {

    //Creating the scene.
    Vector cam = Vector(0.5,-1.0,0.5);
    Scene test1 = Scene(cam, Vector(0.0, -0.5, 1.0));
    std::shared_ptr<Plane> test2 = std::make_shared<Plane>(Plane(Color(255,255,255), 0, Vector(0,0,0), Vector(0,0,1)));
    std::shared_ptr<Sphere> test3 = std::make_shared<Sphere>(Sphere(Color(255,0,0), 0.3, Vector(0.25, 0.45, 0.4), 0.4));
    std::shared_ptr<Sphere> test4 = std::make_shared<Sphere>(Sphere(Color(0,255,0), 0, Vector(1,1,0.25), 0.25));
    std::shared_ptr<Sphere> test5 = std::make_shared<Sphere>(Sphere(Color(0,0,255), 0.7, Vector(0.8,0.3,0.15), 0.15));
    test1.add_shape(test2);
    test1.add_shape(test3);
    test1.add_shape(test4);
    test1.add_shape(test5);

    //Creating the Array2D and looping over the pixels.
    Array2D<Color> array = Array2D<Color>(512, 512);
    for (int i = 0; i < 512; i++) {
        for (int j = 0; j < 512; j++) {
            Vector screen_point = Vector(i/512.0, 0, (512-j)/512.0);
            Color p = test1.get_point_color(screen_point);
            array.set(i,j,p);
        }
    }

    write_ppm(array, "test.ppm");

    return 0;

    /*
    TODO: 

    - Fix reflections
        - Reflections have strange noise that still needs to be fixed but reflections appear to be in the right place.

    - Add rectangles

    - Add cylinders

    */
}