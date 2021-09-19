#include <iostream>
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

int main() {
	Projectile p = Projectile{ point(0, 1, 0), vector(1, 1, 0).normalize() };
	Environment e = Environment{ vector(0, -0.1, 0), vector(-0.01, 0, 0) };
	int ticks = 0;

	while (p.position.y > 0) {
		tick(e, p);
		++ticks;
		std::cout << "After " << ticks << " ticks, the position is: " << p.position << std::endl;
	}

	return 0;
}
