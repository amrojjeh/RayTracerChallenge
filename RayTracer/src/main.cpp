#include <iostream>
#include <fstream>
#include "lib.h"

struct Projectile {
	Point position;
	Vector velocity;
};

struct Environment {
	Vector gravity;
	Vector wind;
};

void tick(Environment e, Projectile& p) {
	p.position = p.position + p.velocity;
	p.velocity = p.velocity + e.gravity + e.wind;
}

void draw(Canvas c, Projectile p) {
	c.write_pixel(static_cast<int>(std::round(p.position.x)),
		550 - p.position.y, color(1, 0, 0));
}

int main() {
	Projectile p = Projectile{ point(0, 1, 0), vector(1.f, 1.8f, 0).normalize() * 11.25f };
	Environment e = Environment{ vector(0, -0.1, 0), vector(-0.01, 0, 0) };
	Canvas c{ 900, 550 };
	int ticks = 0;

	while (p.position.y > 0) {
		draw(c, p);
		tick(e, p);
		++ticks;
		std::cout << "After " << ticks << " ticks, the position is: " << p.position << std::endl;
	}

	CanvasToPPM ppm{ c };
	std::ofstream file;
	file.open("output.ppm");
	file << ppm.toPlainPPM();
	file.close();

	return 0;
}
