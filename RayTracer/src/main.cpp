#include <iostream>
#include "lib.h"

template<std::size_t N>
std::ostream& operator<<(std::ostream& output, Matrix<N> mat) {
	for (std::size_t x = 0; x < N; ++x) {
		output << "[";
		for (std::size_t y = 0; y < N; ++y) {
			output << mat[x][y] << ", ";
		}
		output << "]\n";
	}
	return output;
}

int main() {
	std::cout << "What happens when you invert an identity matrix? It stays the same." << std::endl;
	Matrix<4> a = Matrix<4>::identity.inverse();
	std::cout << Matrix<4>::identity << " -> \n" << a << std::endl;

	std::cout << "What do you get when you multiply a matrix by its inverse? The identity matrix." << std::endl;
	Matrix<3> b{ {
		{1.f, 2.f, 3.f},
		{4.f, 5.f, 6.f},
		{7.f, 10.f, 9.f},
	} };

	std::cout << b << " -> \n";
	b = b * b.inverse();
	std::cout << b;

	std::cout << "Is there any difference between the inverse of the tranpose of a matrix, and the transpose of the inverse?" << std::endl;

	Matrix<3> c{ {
	{1.f, 2.f, 3.f},
	{4.f, 5.f, 6.f},
	{7.f, 10.f, 9.f},
} };

	Matrix<3> d = c.inverse().transpose();
	Matrix<3> e = c.transpose().inverse();
	
	std::cout << (d == e ? " yes\n" : " no\n");

	Matrix<4> f{ Matrix<4>::identity };
	f[1][1] = 12.f;
	Tuple t{ 1, 2, 3, 4 };
	std::cout << f * t << std::endl;
}
