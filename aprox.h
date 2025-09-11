#ifndef APROX_H
#define APROX_H
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <algorithm>

class data {
private:
	struct position{
		float x;
		float y;
	}point;

	int size = 0;
	double a = 0.f, b = 0.f;
	double type_a = 0.f, type_b = 0.f;
public:
	std::vector <position> points;

	struct max_value {
		double max = 0;
		double min = 0;
	}x;

	struct min_value {
		double max = 0;
		double min = 0;
	}y;

	void getAndSavePositon();
	void linearAprox();
	double predictedValue(double x);
	void maxMinValue();
	void test() {
		std::cout << "Linear funcion aproximating the data set" << std::endl;
		std::cout << "y = " << a<<"x + " << b << std::endl;
		std::cout << "u(a)" << std::endl;
		std::cout << type_a << std::endl;
	}
	void uncertainty();
	float getX(int it);
	float getY(int it);
	int pointsSize() {
		return points.size();
	}
	void sort();

};


#endif 
