#include <gtest/gtest.h>
#include "lib.h" // includes cmath

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
	ASSERT_FLOAT_EQ(vector(1, 0, 0).magnitude(), 1.f);
	ASSERT_FLOAT_EQ(vector(0, 1, 0).magnitude(), 1.f);
	ASSERT_FLOAT_EQ(vector(0, 0, 1).magnitude(), 1.f);
	ASSERT_FLOAT_EQ(vector(1, 2, 3).magnitude(), std::sqrtf(14));
	ASSERT_FLOAT_EQ(vector(-1, -2, -3).magnitude(), std::sqrtf(14));
}

TEST(Tuples, normalizing) {
	ASSERT_TRUE(vector(4, 0, 0).normalize() == vector(1, 0, 0));
	ASSERT_TRUE(vector(1, 2, 3).normalize() == vector(1 / sqrtf(14), 2 / sqrtf(14), 3 / sqrtf(14)));
	ASSERT_FLOAT_EQ(vector(1, 2, 3).normalize().magnitude(), 1.f);
}

TEST(Tuples, dotProduct) {
	ASSERT_FLOAT_EQ(vector(1, 2, 3).dot(vector(2, 3, 4)), 20.f);
}
