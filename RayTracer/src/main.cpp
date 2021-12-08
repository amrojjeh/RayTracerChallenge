#include <iostream>
#include <fstream>
#include "lib.h"

int main() {
	int height, width;
	height = width = 600;
	Canvas c(width, height);
	Sphere sphere;
	sphere.transform = Transform::scaling(width / 2.f, width / 2.f, width / 2.f);
	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			Ray ray{ point(x - width / 2, y - height / 2, -5), vector(0, 0, 1) };
			std::vector<Intersection> inters = intersections(intersect(sphere, ray));
			Intersection* h = nullptr;
			hit(inters, &h);
			if (h != nullptr) {
				c.write_pixel(x, y, color(255, 0, 0));
			}
			else {
				c.write_pixel(x, y, color(0, 0, 0));
			}
		}
	}
	CanvasToPPM c2ppm(c);
	std::ofstream file;
	file.open("output.ppm");
	file << c2ppm.toPlainPPM();
	file.close();
}
