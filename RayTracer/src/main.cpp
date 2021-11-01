#include <iostream>
#include <fstream>
#include "lib.h"

int main() {
	int height, width;
	height = width = 600;
	Canvas c(width, height);
	float radius = 250.f;
	Point p = point(0.f, -radius, 0.f); // Starts at the top of the clock
	Transform T = Transform::identity.rotate_z(M_PI / 6);
	for (int x = 0; x < 12; ++x) {
		std::cout << p << std::endl;
		for (int w = -5; w < 5; ++w)
			for (int h = -5; h < 5; ++h)
				c.write_pixel(static_cast<int>(p.x + width / 2) + w, static_cast<int>(p.y + height / 2) + h, color(255, 255, 255));
		p = T * p;
	}
	CanvasToPPM c2ppm(c);
	std::ofstream file;
	file.open("output.ppm");
	file << c2ppm.toPlainPPM();
	file.close();
}
