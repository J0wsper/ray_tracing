#include "ray_tracing.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include "ray_tracing.hpp"

/*

NOTES:

PROBLEM 1: Okay for some reason everything is really fucking dark. I don't know why. Here are a few things I've tried:

- Disabling reflections
- Disabling the segment where I turn off specular light if an object is in shadow

FIX: my types for l_amb and l_spec were unsigned instead of double.

PROBLEM 2: All of my shapes look flat as shit. Here are a few things I've tried:

- 

FIX: Not discovered

PROBLEM 3: Reflections are broken as hell. Here are a few things I've tried:

- 

FIX: Not discovered.

*/


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
    Plane test2 = Plane(Color(255,255,255), 0, Vector(0,0,0), Vector(0,0,1));
    Sphere test3 = Sphere(Color(255,0,0), 0.3, Vector(0.25, 0.45, 0.4), 0.4);
    Sphere test4 = Sphere(Color(0,255,0), 0, Vector(1,1,0.25), 0.25);
    Sphere test5 = Sphere(Color(0,0,255), 0.7, Vector(0.8,0.3,0.15), 0.15);
    test1.add_shape(&test2);
    test1.add_shape(&test3);
    test1.add_shape(&test4);
    test1.add_shape(&test5);

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

    Color top_left = array.get(0,0);
    std::cout << top_left.get_red() << std::endl;
    std::cout << top_left.get_green() << std::endl;
    std::cout << top_left.get_blue() << std::endl;
    
    return 0;

    /*
    TODO: One of my destructors causes a segmentation fault. Figure out which one.

    A: It's the fucking Scene destructor.
    */
}