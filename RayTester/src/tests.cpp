#include <gtest/gtest.h>
#include "lib.h" // includes cmath

TEST(Clamp, clamping) {
	ASSERT_EQ(25, clamp(25, 0, 30));
	ASSERT_EQ(25, clamp(30, 0, 25));
	ASSERT_EQ(25, clamp(0, 25, 30));
}

TEST(Tuples, points) {
	Tuple t1 = point(4.3f, -4.2f, 3.1f);
	ASSERT_FLOAT_EQ(t1.x, 4.3f);
	ASSERT_FLOAT_EQ(t1.y, -4.2f);
	ASSERT_FLOAT_EQ(t1.z, 3.1f);
	ASSERT_FLOAT_EQ(t1.w, 1.0f);
	ASSERT_TRUE(t1.isPoint());
	ASSERT_FALSE(t1.isVector());
}

TEST(Tuples, vectors) {
	Tuple t1 = vector(4.3f, -4.2f, 3.1f);
	ASSERT_FLOAT_EQ(t1.x, 4.3f);
	ASSERT_FLOAT_EQ(t1.y, -4.2f);
	ASSERT_FLOAT_EQ(t1.z, 3.1f);
	ASSERT_FLOAT_EQ(t1.w, 0.0f);
	ASSERT_FALSE(t1.isPoint());
	ASSERT_TRUE(t1.isVector());
}

TEST(Tuples, equality) {
	Tuple t1{ 1.f, 2.f, 3.f, 0.f };
	Tuple t2{ 1.f, 2.f, 3.f, 0.f };
	ASSERT_TRUE(t1 == t2);
	Tuple t3{ 1.f, 2.f, -3.f, 0.f};
	ASSERT_FALSE(t3 == t2);
	ASSERT_FALSE(t3 == t1);
}

TEST(Tuples, adding) {
	Tuple t1{ 3, -2, 5, 1 };
	Tuple t2{ -2, 3, 1, 0 };
	Tuple result = t1 + t2;
	Tuple expected{ 1, 1, 6, 1 };
	ASSERT_TRUE(result == expected);
}

TEST(Tuples, pSubtract) {
	Tuple p1 = point(3, 2, 1);
	Tuple p2 = point(5, 6, 7);
	Tuple result = p1 - p2;
	Tuple expected = vector(-2, -4, -6);
	ASSERT_TRUE(expected == result);
}

TEST(Tuples, pvSubtract) {
	Tuple p = point(3, 2, 1);
	Tuple v = vector(5, 6, 7);
	Tuple result = p - v;
	Tuple expected = point(-2, -4, -6);
	ASSERT_TRUE(expected == result);
}

TEST(Tuples, vSubtract) {
	Tuple v1 = vector(3, 2, 1);
	Tuple v2 = vector(5, 6, 7);
	Tuple result = v1 - v2;
	Tuple expected = vector(-2, -4, -6);
	ASSERT_TRUE(result == expected);
}

TEST(Tuples, uncivilziedNegate) {
	Tuple zero = vector(0, 0, 0);
	Tuple v = vector(1, -2, 3);
	Tuple result = zero - v;
	Tuple expected{-1, 2, -3};
	ASSERT_TRUE(expected == result);
}

TEST(Tuples, properNegate) {
	Tuple t1{1, -2, 3, -4};
	Tuple result = -t1;
	Tuple expected{-1, 2, -3, 4};
	ASSERT_TRUE(expected == result);
}

TEST(Tuples, scalarMultiplication) {
	Tuple t1{1, -2, 3, -4};
	Tuple result = 3.5f * t1;
	Tuple expected{3.5f, -7.f, 10.5f, -14.f};
	ASSERT_TRUE(result == expected);
}

TEST(Tuples, scalarDivision) {
	Tuple t{1, -2, 3, -4};
	Tuple result = t / 2.f;
	Tuple expected{0.5f, -1.f, 1.5f, -2.f};
	ASSERT_TRUE(result == expected);
}

TEST(Tuples, magnitude) {
	ASSERT_FLOAT_EQ(vector(1.f, 0.f, 0.f).magnitude(), 1.f);
	ASSERT_FLOAT_EQ(vector(0.f, 1.f, 0.f).magnitude(), 1.f);
	ASSERT_FLOAT_EQ(vector(0.f, 0.f, 1.f).magnitude(), 1.f);
	ASSERT_FLOAT_EQ(vector(1.f, 2.f, 3.f).magnitude(), std::sqrtf(14.f));
	ASSERT_FLOAT_EQ(vector(-1.f, -2.f, -3.f).magnitude(), std::sqrtf(14.f));
}

TEST(Tuples, normalizing) {
	ASSERT_TRUE(vector(4.f, 0.f, 0.f).normalize() == vector(1, 0.f, 0.f));
	ASSERT_TRUE(vector(1.f, 2.f, 3.f).normalize() == vector(1.f / sqrtf(14.f), 2.f / sqrtf(14.f), 3.f / sqrtf(14.f)));
	ASSERT_FLOAT_EQ(vector(1.f, 2.f, 3.f).normalize().magnitude(), 1.f);
}

TEST(Tuples, dotProduct) {
	ASSERT_FLOAT_EQ(vector(1.0f, 2.0f, 3.f).dot(vector(2.f, 3.f, 4.f)), 20.f);
}

TEST(Tuples, crossProduct) {
	Tuple v1{ vector(1.0f, 2.0f, 3.0f) };
	Tuple v2{ vector(2.0f, 3.0f, 4.0f) };

	Tuple result1{ v1.cross(v2) };
	Tuple result2{ v2.cross(v1) };

	Tuple expected1{ vector(-1.0f, 2.0f, -1.0f) };
	Tuple expected2{ vector(1.0f, -2.0f, 1.0f) };

	ASSERT_TRUE(result1 == expected1);
	ASSERT_TRUE(result2 == expected2);
}

TEST(Colors, creation) {
	Color c = color(-0.5f, 0.4f, 1.7f);
	ASSERT_FLOAT_EQ(-0.5f, c.getRGB().red);
	ASSERT_FLOAT_EQ(0.4f, c.getRGB().green);
	ASSERT_FLOAT_EQ(1.7f, c.getRGB().blue);
	c.getRGB().blue = 0.f;
	ASSERT_FLOAT_EQ(0.f, c.getRGB().blue);
	ASSERT_FLOAT_EQ(0.f, c.z);
}

TEST(Colors, addingColors) {
	Color c1{ color(0.9f, 0.6f, 0.75f) };
	Color c2{ color(0.7f, 0.1f, 0.25f) };
	Color result{ c1 + c2 };
	Color expected{ color(1.6f, 0.7f, 1.0f) };
	ASSERT_TRUE(result == expected);
}

TEST(Colors, subtractingColors) {
	Color c1{ color(0.9f, 0.6f, 0.75f) };
	Color c2{ color(0.7f, 0.1f, 0.25f) };
	Color result{ c1 - c2 };
	Color expected{ color(0.2f, 0.5f, 0.5f) };
	ASSERT_TRUE(result == expected);
}

TEST(Colors, multiplyingColors) {
	Color c1{ color(0.2f, 0.3f, 0.4f) };
	ASSERT_TRUE(c1 * 2 == color(0.4f, 0.6f, 0.8f));
}

TEST(Colors, HadamardProduct) {
	Color c1{ color(1.0f, 0.2f, 0.4f) };
	Color c2{ color(0.9f, 1.0f, 0.1f) };
	ASSERT_TRUE(c1.schur(c2) == color(0.9f, 0.2f, 0.04f));
}

TEST(Canvas, init) {
	Canvas c(10, 20);
	ASSERT_EQ(c.width, 10);
	ASSERT_EQ(c.height, 20);
	for (std::size_t x = 0; x < c.height * c.width; ++x)
		ASSERT_TRUE(c.canvas[x] == color(0, 0, 0));
}

TEST(Canvas, writePixel) {
	Canvas c(10, 20);
	Color red = color(1.f, 0.f, 0.f);
	c.write_pixel(2, 3, red);
	red.getRGB().red = 0.f; // Check if canvas[2][3] is copying
	ASSERT_TRUE(c.read_pixel(2, 3) == color(1.f, 0.f, 0.f));
}

std::string head(std::string str, int lines) {
	std::size_t last = 0;
	while (lines-- > 0) {
		last = str.find('\n', last + 1);
		if (last == std::string::npos) {
			return str;
		}
	}
	return str.substr(0, last);
}

TEST(util, head) {
	std::string s1 = "We\nThink\nIn\nGeneralities\n";
	std::string result1 = head(s1, 3);
	std::string result2 = head(s1, 5);
	std::string expected1 = "We\nThink\nIn";
	std::string expected2 = "We\nThink\nIn\nGeneralities\n";
	ASSERT_STREQ(result1.c_str(), expected1.c_str());
	ASSERT_STREQ(result2.c_str(), expected2.c_str());
}

TEST(PlainPPM, ppmHeader) {
	Canvas c(10, 20);
	CanvasToPPM ppm{ c, 255 };
	std::string output = ppm.toPlainPPM();
	ASSERT_STREQ("P3\n10 20\n255", head(output, 3).c_str());
}

TEST(PlainPPM, ppmPixelData) {
	Canvas c(5, 3);
	Color c1 = color(1.5f, 0.f, 0.f);
	Color c2 = color(0.f, 0.5f, 0.f);
	Color c3 = color(-0.5f, 0.f, 1.f);
	c.write_pixel(0, 0, c1);
	c.write_pixel(2, 1, c2);
	c.write_pixel(4, 2, c3);
	CanvasToPPM ppm{ c, 255 };
	std::string ppmStr = ppm.toPlainPPM();
	std::string expected =
		"P3\n"
		"5 3\n"
		"255\n"
		"255 0 0" " 0 0 0" " 0 0 0" " 0 0 0" " 0 0 0\n"
		"0 0 0" " 0 0 0" " 0 128 0" " 0 0 0" " 0 0 0\n"
		"0 0 0" " 0 0 0" " 0 0 0" " 0 0 0" " 0 0 255\n";
	ASSERT_STREQ(expected.c_str(), ppmStr.c_str());
}

TEST(PlainPPM, splittingLongLines) {
	Canvas c(10, 2);
	Color c1 = color(1.f, 0.8f, 0.6f);
	for (std::size_t x = 0; x < c.height * c.width; ++x) {
		c.canvas[x] = c1;
	}
	CanvasToPPM ppm{ c, 255 };
	std::string result = ppm.toPlainPPM();
	std::string expected =
		"P3\n"
		"10 2\n"
		"255\n"
		"255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
		"153 255 204 153 255 204 153 255 204 153 255 204 153\n"
		"255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204\n"
		"153 255 204 153 255 204 153 255 204 153 255 204 153\n";
	ASSERT_STREQ(expected.c_str(), result.c_str());
}

TEST(PlainPPM, endsWithNewLine) {
	Canvas c{ 5, 3 };
	CanvasToPPM ppm{ c, 255 };
	std::string result = ppm.toPlainPPM();
	ASSERT_EQ(result[result.length() - 1U], '\n');
}

TEST(Matrix, 4x4) {
	Matrix<4> m{ {
		{1.f, 2.f, 3.f, 4.f},
		{5.5f, 6.5f, 7.5f, 8.5f},
		{9.f, 10.f, 11.f, 12.f},
		{13.5f, 14.5f, 15.5f, 16.5f}
	} };

	ASSERT_FLOAT_EQ(m[0][0], 1.f);
	ASSERT_FLOAT_EQ(m[0][3], 4.f);
	ASSERT_FLOAT_EQ(m[1][0], 5.5f);
	ASSERT_FLOAT_EQ(m[1][2], 7.5f);
	ASSERT_FLOAT_EQ(m[2][2], 11.f);
	ASSERT_FLOAT_EQ(m[3][0], 13.5f);
	ASSERT_FLOAT_EQ(m[3][2], 15.5f);
}

TEST(Matrix, 2x2) {
	Matrix<2> m{ {
		{-3.f, 5.f},
		{1.f, -2.f},
	} };

	ASSERT_FLOAT_EQ(m[0][0], -3.f);
	ASSERT_FLOAT_EQ(m[0][1], 5.f);
	ASSERT_FLOAT_EQ(m[1][0], 1.f);
	ASSERT_FLOAT_EQ(m[1][1], -2.f);
}

TEST(Matrix, 3x3) {
	Matrix<3> m{ {
		{-3.f, 5.f, 0.f},
		{1.f, -2.f, -7.f},
		{0.f, 1.f, 1.f},
	} };

	ASSERT_FLOAT_EQ(m[0][0], -3.f);
	ASSERT_FLOAT_EQ(m[1][1], -2.f);
	ASSERT_FLOAT_EQ(m[2][2], 1.f);
}

TEST(Matrix, Equality) {
	Matrix<4> a{ {
		{1.f, 2.f, 3.f, 4.f},
		{5.f, 6.f, 7.f, 8.f},
		{9.f, 10.f, 11.f, 12.f},
		{13.f, 14.f, 15.f, 16.f},
	} };

	Matrix<4> b{ {
		{1.f, 2.f, 3.f, 4.f},
		{5.f, 6.f, 7.f, 8.f},
		{9.f, 10.f, 11.f, 12.f},
		{13.f, 14.f, 15.f, 16.f},
	} };

	ASSERT_TRUE(a == b);
	ASSERT_FALSE(a != b);
}

TEST(Matrix, Inequality) {
	Matrix<4> a{ {
		{1.f, 2.f, 3.f, 4.f},
		{5.f, 6.f, 7.f, 8.f},
		{9.f, 10.f, 11.f, 12.f},
		{13.f, 14.f, 15.f, 16.f},
	} };

	Matrix<4> b{ {
		{1.f, 2.f, 3.f, 4.f},
		{5.f, 5.f, 7.f, 8.f},
		{9.f, 10.f, 11.f, 12.f},
		{13.f, 14.f, 15.f, 16.f},
	} };

	ASSERT_FALSE(a == b);
	ASSERT_TRUE(a != b);
}

TEST(Matrix, Matrix_Multipication) {
	Matrix<4> a{ {
		{1.f, 2.f, 3.f, 4.f},
		{5.f, 6.f, 7.f, 8.f},
		{9.f, 8.f, 7.f, 6.f},
		{5.f, 4.f, 3.f, 2.f},
	} };

	Matrix<4> b{ {
		{-2.f, 1.f, 2.f, 3.f},
		{3.f, 2.f, 1.f, -1.f},
		{4.f, 3.f, 6.f, 5.f},
		{1.f, 2.f, 7.f, 8.f},
	} };

	Matrix<4> expected{ {
		{20.f, 22.f, 50.f, 48.f},
		{44.f, 54.f, 114.f, 108.f},
		{40.f, 58.f, 110.f, 102.f},
		{16.f, 26.f, 46.f, 42.f},
	} };

	Matrix<4> result = a * b;

	ASSERT_EQ(result, expected);
}

TEST(Matrix, Tuple_Multipication) {
	Matrix<4> a{ {
		{1.f, 2.f, 3.f, 4.f},
		{2.f, 4.f, 4.f, 2.f},
		{8.f, 6.f, 4.f, 1.f},
		{0.f, 0.f, 0.f, 1.f},
	} };

	Tuple b{1, 2, 3, 1};
	Tuple expected{18, 24, 33, 1};
	ASSERT_EQ(expected, a * b);
}

TEST(Matrix, Identity) {
	Matrix<4> a{ {
		{0.f, 1.f, 2.f, 4.f},
		{1.f, 2.f, 4.f, 8.f},
		{2.f, 4.f, 8.f, 16.f},
		{4.f, 8.f, 16.f, 32.f},
	} };

	Matrix<4> result = a * Matrix<4>::identity;

	ASSERT_EQ(result, a);
}

TEST(Matrix, Transposing) {
	Matrix<4> a{ {
		{0.f, 9.f, 3.f, 0.f},
		{9.f, 8.f, 0.f, 8.f},
		{1.f, 8.f, 5.f, 3.f},
		{0.f, 0.f, 5.f, 8.f},
	} };

	Matrix<4> expected{ {
		{0.f, 9.f, 1.f, 0.f},
		{9.f, 8.f, 8.f, 0.f},
		{3.f, 0.f, 5.f, 5.f},
		{0.f, 8.f, 3.f, 8.f},
	} };

	Matrix<4> result = a.transpose();

	ASSERT_TRUE(result == expected);
}

TEST(Matrix, 2x2Determinant) {
	Matrix<2> m{ {
		{1.f, 5.f},
		{-3.f, 2.f},
	} };
	float result = m.determinant();
	float expected = 17.f;
	ASSERT_FLOAT_EQ(expected, result);
}

TEST(Matrix, SubMatrix) {
	Matrix<3> a{ {
		{1.f, 5.f, 0.f},
		{-3.f, 2.f, 7.f},
		{0.f, 6.f, -3.f},
	} };

	Matrix<2> actuala = a.submatrix(0, 2);

	Matrix<2> expecteda{ {
		{-3.f, 2.f},
		{0.f, 6.f}
	} };

	ASSERT_EQ(actuala, expecteda);

	Matrix<4> b{ {
		{-6.f, 1.f, 1.f, 6.f},
		{-8.f, 5.f, 8.f, 6.f},
		{-1.f, 0.f, 8.f, 2.f},
		{-7.f, 1.f, -1.f, 1.f},
	} };

	Matrix<3> resultb = b.submatrix(2, 1);

	Matrix<3> expectedb{ {
		{-6.f, 1.f, 6.f},
		{-8.f, 8.f, 6.f},
		{-7.f, -1.f, 1.f},
	} };

	ASSERT_EQ(resultb, expectedb);
}

TEST(Matrix, 3x3Minor) {
	Matrix<3> a{ {
		{3.f, 5.f, 0.f},
		{2.f, -1.f, -7.f},
		{6.f, -1.f, 5.f},
	} };

	ASSERT_FLOAT_EQ(25.f, a.minor(1, 0));
}

TEST(Matrix, cofactor) {
	Matrix<3> a{ {
		{3.f, 5.f, 0.f},
		{2.f, -1.f, -7.f},
		{6.f, -1.f, 5.f},
	} };
	ASSERT_FLOAT_EQ(-12.f, a.minor(0, 0));
	ASSERT_FLOAT_EQ(-12.f, a.cofactor(0, 0));
	ASSERT_FLOAT_EQ(25.f, a.minor(1, 0));
	ASSERT_FLOAT_EQ(-25.f, a.cofactor(1, 0));
}

TEST(Matrix, 3x3Determinant) {
	Matrix<3> a{ {
		{1.f, 2.f, 6.f},
		{-5.f, 8.f, -4.f},
		{2.f, 6.f, 4.f},
	} };
	ASSERT_FLOAT_EQ(56.f, a.cofactor(0, 0));
	ASSERT_FLOAT_EQ(12.f, a.cofactor(0, 1));
	ASSERT_FLOAT_EQ(-46.f, a.cofactor(0, 2));
	ASSERT_FLOAT_EQ(-196.f, a.determinant());
}

TEST(Matrix, 4x4Determinant) {
	Matrix<4> a{ {
		{-2.f, -8.f, 3.f, 5.f},
		{-3.f, 1.f, 7.f, 3.f},
		{1.f, 2.f, -9.f, 6.f},
		{-6.f, 7.f, 7.f, -9.f},
	} };

	ASSERT_FLOAT_EQ(690.f, a.cofactor(0, 0));
	ASSERT_FLOAT_EQ(447.f, a.cofactor(0, 1));
	ASSERT_FLOAT_EQ(210.f, a.cofactor(0, 2));
	ASSERT_FLOAT_EQ(51.f, a.cofactor(0, 3));
	ASSERT_FLOAT_EQ(-4071.f, a.determinant());
}

TEST(Matrix, IsInvertible) {
	Matrix<4> a{ {
		{6.f, 4.f, 4.f, 4.f},
		{5.f, 5.f, 7.f, 6.f},
		{4.f, -9.f, 3.f, -7.f},
		{9.f, 1.f, 7.f, -6.f},
	} };
	ASSERT_FLOAT_EQ(a.determinant(), -2120.f);
	ASSERT_TRUE(a.invertible());

	Matrix<4> b{ {
		{-4.f, 2.f, -2.f, -3.f},
		{9.f, 6.f, 2.f, 6.f},
		{0.f, -5.f, 1.f, -5.f},
		{0.f, 0.f, 0.f, 0.f},
	} };
	ASSERT_FLOAT_EQ(b.determinant(), 0.f);
	ASSERT_FALSE(b.invertible());
}

TEST(Matrix, Inverse) {
	Matrix<4> a{ {
		{-5.f, 2.f, 6.f, -8.f},
		{1.f, -5.f, 1.f, 8.f},
		{7.f, 7.f, -6.f, -7.f},
		{1.f, -3.f, 7.f, 4.f},
	} };

	Matrix<4> b{ a.inverse() };

	ASSERT_FLOAT_EQ(532.f, a.determinant());
	ASSERT_FLOAT_EQ(-160.f, a.cofactor(2, 3));
	ASSERT_FLOAT_EQ(-160.f/532.f, b[3][2]);
	ASSERT_FLOAT_EQ(105.f, a.cofactor(3, 2));
	ASSERT_FLOAT_EQ(105.f/532.f, b[2][3]);
	Matrix<4> expected{ {
		{0.21805f, 0.45113f, 0.24060f, -0.04511f},
		{-0.80827f, -1.45677f, -0.44361f, 0.52068f},
		{-0.07895f, -0.22368f, -0.05263f, 0.19737f},
		{-0.52256f, -0.81391f, -0.30075f, 0.30639f}
	} };

	for (std::size_t x = 0; x < 4; ++x) {
		for (std::size_t y = 0; y < 4; ++y) {
			ASSERT_TRUE(std::abs(expected[x][y] - b[x][y]) <= 0.00001f);
		}
	}
}

TEST(Matrix, Inverse2) {
	Matrix<4> a{ {
		{8.f, -5.f, 9.f, 2.f},
		{7.f, 5.f, 6.f, 1.f},
		{-6.f, 0.f, 9.f, 6.f},
		{-3.f, 0.f, -9.f, -4.f}
	} };


	Matrix<4> result = a.inverse();
	Matrix<4> expected{ {
		{-0.153846f, -0.15384f, -0.28205f, -0.53846f},
		{-0.07692f,	0.12307f, 0.02564f,	0.03076f},
		{0.35897f, 0.35897f, 0.43589f, 0.92307f},
		{-0.69230f, -0.69230f, -0.76923f, -1.92307f}
	} };

	for (std::size_t x = 0; x < 4; ++x) {
		for (std::size_t y = 0; y < 4; ++y) {
			ASSERT_TRUE(std::abs(expected[x][y] - result[x][y]) <= 0.00001f);
		}
	}
}

TEST(Matrix, Inverse3) {
	Matrix<4> a{ {
		{9.f, 3.f, 0.f, 9.f},
		{-5.f, -2.f, -6.f, -3.f},
		{-4.f, 9.f, 6.f, 4.f},
		{-7.f, 6.f, 6.f, 2.f}
	} };

	Matrix<4> result = a.inverse();
	Matrix<4> expected{ {
		{-0.04074f, -0.07777f, 0.14444f, -0.22222f},
		{-0.07777f, 0.03333f, 0.36666f, -0.33333f},
		{-0.02901f, -0.14629f, -0.10925f, 0.12962f},
		{0.17777f, 0.06666f, -0.26666f, 0.33333f}
	} };

	for (std::size_t x = 0; x < 4; ++x) {
		for (std::size_t y = 0; y < 4; ++y) {
			ASSERT_TRUE(std::abs(expected[x][y] - result[x][y]) <= 0.00001f);
		}
	}
}

TEST(Matrix, Inverse4) {
	Matrix<4> a{ {
		{3.f, -9.f, 7.f, 3.f},
		{3.f, -8.f, 2.f, -9.f},
		{-4.f, 4.f, 4.f, 1.f},
		{-6.f, 5.f, -1.f, 1.f}
	} };

	Matrix<4> b{ {
		{8.f, 2.f, 2.f, 2.f},
		{3.f, -1.f, 7.f, 0.f},
		{7.f, 0.f, 5.f, 4.f},
		{6.f, -2.f, 0.f, 5.f}
	} };

	Matrix<4> c = a * b;
	Matrix<4> result = c * b.inverse();
	ASSERT_EQ(result, a);
}

TEST(MatrixTransformations, translation) {
	Matrix<4> transform = Matrix<4>::translation(5.f, -3.f, 2.f);
	Point p = point(-3, 4, 5);
	ASSERT_EQ(transform * p, point(2, 1, 7));
}

TEST(MatrixTransformations, translationInverse) {
	Matrix<4> transform = Matrix<4>::translation(5, -3, 2).inverse();
	Point p = point(-3, 4, 5);
	ASSERT_EQ(transform * p, point(-8, 7, 3));
}

TEST(MatrixTransformations, translationVector) {
	Matrix<4> transform = Matrix<4>::translation(5, -3, 2);
	Vector v = vector(-3, 4, 5);
	ASSERT_EQ(transform * v, v);
}

TEST(MatrixTransformations, scalingPoint) {
	Matrix<4> transform = Matrix<4>::scaling(2, 3, 4);
	Point p = point(-4, 6, 8);
	ASSERT_EQ(transform * p, point(-8, 18, 32));
}

TEST(MatrixTransformations, scalingVector) {
	Matrix<4> transform = Matrix<4>::scaling(2, 3, 4);
	Vector v = vector(-4, 6, 8);
	ASSERT_EQ(transform * v, vector(-8, 18, 32));
}

TEST(MatrixTransformations, scalingInverse) {
	Matrix<4> transform = Matrix<4>::scaling(2, 3, 4).inverse();
	Vector v = vector(-4, 6, 8);
	ASSERT_EQ(transform * v, vector(-2, 2, 2));
}

TEST(MatrixTransformations, reflectionAcrossX) {
	Matrix<4> transform = Matrix<4>::scaling(-1, 1, 1);
	Vector p = point(2, 3, 4);
	ASSERT_EQ(transform * p, point(-2, 3, 4));
}

TEST(MatrixTransformations, rotationXAxis) {
	Point p = point(0, 1, 0);
	Matrix<4> half_quarter = Matrix<4>::rotation_x(M_PI / 4);
	Matrix<4> full_quarter = Matrix<4>::rotation_x(M_PI / 2);
	ASSERT_EQ(half_quarter * p, point(0, std::sqrtf(2) / 2, std::sqrtf(2) / 2));
	ASSERT_EQ(full_quarter * p, point(0, 0, 1));
}

TEST(MatrixTransformations, rotationXAxisInverse) {
	Point p = point(0, 1, 0);
	Matrix<4> half_quarter = Matrix<4>::rotation_x(M_PI / 4).inverse();
	ASSERT_EQ(half_quarter * p, point(0, std::sqrtf(2) / 2.f, -std::sqrtf(2) / 2.f));
}

TEST(MatrixTransformations, rotationYAxis) {
	Point p = point(0, 0, 1);
	Matrix<4> half_quarter = Matrix<4>::rotation_y(M_PI / 4);
	Matrix<4> full_quarter = Matrix<4>::rotation_y(M_PI / 2);
	ASSERT_EQ(half_quarter * p, point(std::sqrtf(2) / 2, 0, std::sqrtf(2) / 2));
	ASSERT_EQ(full_quarter * p, point(1, 0, 0));
}

TEST(MatrixTransformations, rotationZAxis) {
	Point p = point(0, 1, 0);
	Matrix<4> half_quarter = Matrix<4>::rotation_z(M_PI / 4);
	Matrix<4> full_quarter = Matrix<4>::rotation_z(M_PI / 2);
	ASSERT_EQ(half_quarter * p, point(-std::sqrtf(2) / 2, std::sqrtf(2) / 2, 0));
	ASSERT_EQ(full_quarter * p, point(-1, 0, 0));
}

TEST(MatrixTransformation, shearingXY) {
	Matrix<4> transform = Matrix<4>::shearing(1.f, 0.f, 0.f, 0.f, 0.f, 0.f);
	Point p = point(2, 3, 4);
	ASSERT_EQ(transform * p, point(5, 3, 4));
}

TEST(MatrixTransformation, shearingXZ) {
	Matrix<4> transform = Matrix<4>::shearing(0.f, 1.f, 0.f, 0.f, 0.f, 0.f);
	Point p = point(2, 3, 4);
	ASSERT_EQ(transform * p, point(6, 3, 4));
}

TEST(MatrixTransformation, shearingYX) {
	Matrix<4> transform = Matrix<4>::shearing(0.f, 0.f, 1.f, 0.f, 0.f, 0.f);
	Point p = point(2, 3, 4);
	ASSERT_EQ(transform * p, point(2, 5, 4));
}

TEST(MatrixTransformation, shearingYZ) {
	Matrix<4> transform = Matrix<4>::shearing(0.f, 0.f, 0.f, 1.f, 0.f, 0.f);
	Point p = point(2, 3, 4);
	ASSERT_EQ(transform * p, point(2, 7, 4));
}

TEST(MatrixTransformation, shearingZX) {
	Matrix<4> transform = Matrix<4>::shearing(0.f, 0.f, 0.f, 0.f, 1.f, 0.f);
	Point p = point(2, 3, 4);
	ASSERT_EQ(transform * p, point(2, 3, 6));
}

TEST(MatrixTransformation, shearingZY) {
	Matrix<4> transform = Matrix<4>::shearing(0.f, 0.f, 0.f, 0.f, 0.f, 1.f);
	Point p = point(2, 3, 4);
	ASSERT_EQ(transform * p, point(2, 3, 7));
}

TEST(MatrixTransformation, individualTransforms) {
	Point p = point(1.f, 0.f, 1.f);
	Transform A = Transform::rotation_x(M_PI / 2);
	Transform B = Transform::scaling(5, 5, 5);
	Transform C = Transform::translation(10, 5, 7);

	// Apply rotation first
	p = A * p;
	ASSERT_EQ(p, point(1, -1, 0));
	// Apply scaling second
	p = B * p;
	ASSERT_EQ(p, point(5, -5, 0));
	// Apply translation
	p = C * p;
	ASSERT_EQ(p, point(15, 0, 7));
}

TEST(MatrixTransformation, chainTransforms) {
	Point p = point(1.f, 0.f, 1.f);
	Transform A = Transform::rotation_x(M_PI / 2);
	Transform B = Transform::scaling(5, 5, 5);
	Transform C = Transform::translation(10, 5, 7);
	Transform T = C * (B * A);
	ASSERT_EQ(T * p, point(15, 0, 7));
}

TEST(MatrixTransformation, fluentTransform) {
	Point p = point(1.f, 0.f, 1.f);
	Transform T = Transform::identity
		.rotate_x(M_PI / 2)
		.scale(5, 5, 5)
		.translate(10, 5, 7);
	ASSERT_EQ(T * p, point(15, 0, 7));
}
