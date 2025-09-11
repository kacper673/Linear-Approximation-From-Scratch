#include "aprox.h"

void data::getAndSavePositon() {
	srand(time(0));
	int number_of_points;
	std::cout << "Type the number of points: " << std::endl;
	std::cin >> number_of_points;
	size = number_of_points;
	points.clear();
	char c;
	std::cout << "Type Y to manualy type coordinates of points or type R to draw radnom points: ";
	std::cin >> c;
	for (int i = 0; i < number_of_points; i++) {
		
		if (c == 'R' || c == 'r') {
			point.x = rand() % 201 - 100;
			point.y = rand() % 201 - 100;
			points.push_back(point);
		}

		else if (c == 'Y' || c == 'y') {
			std::cout << "Type cordinates of a point (x,y)" << std::endl;
			std::cout << "x = ";
			std::cin >> point.x;
			std::cout << "y = ";
			std::cin >> point.y;
			points.push_back(point);
		}
		
			else std::cerr << "Incorrect option" << std::endl;
		
	}
	std::cout << "Would you like to correct any points? Y/N" << std::endl;
	std::cin >> c;
	
	switch (c) {
	case ('Y' | 'y') :
		
		int points_to_correct_num;
		std::cout << "How many points would you like to correct?" << std::endl;
		std::cin >> points_to_correct_num;

		for (int i = 0; i < points_to_correct_num; i++) {
			std::cout << "Point number: ";
			int n;
			std::cin >> n;
			while (n < 1 || n > points.size()) {
				std::cerr << "You are trying to access invalid point. Give proper point number: "; std::cin >> n;
			}
			std::cout << "x = ";  std::cin>>points[n - 1].x;
			std::cout << "y = "; std::cin >> points[n - 1].y;
		}

	case ('N' | 'n'):
		break;

	default:
		std::cerr << "Incorrect option" << std::endl;
	}




	

}

void data::linearAprox() {
	
	const size_t n = points.size();
	if (n < 2) { throw std::runtime_error("Need at least 2 points"); }
	double sum_x = 0.f, sum_y = 0.f, sum_x_sq = 0.f, sum_x_y = 0.f;

	for (auto it : points) {
		sum_x += it.x;
		sum_y += it.y;
		sum_x_sq += it.x * it.x;
		sum_x_y += it.x * it.y;
	}
	const double denom = ((n * sum_x_sq) - (sum_x * sum_x));

	if (std::abs(denom) < 1e-12) {
		throw std::runtime_error("All x are identical, linear regression undefined.");
	}

	a = ((n * sum_x_y) - (sum_x * sum_y)) / denom;
	b = (sum_y - (a * sum_x)) / static_cast<double>(n);

}

double data::predictedValue(double x) {
	double y = 0;
	y = a * x + b;	
	return y;
}

void data::maxMinValue() {

	for (auto it : points) {
		if (x.max < it.x) { x.max = it.x; }
		if (x.min > it.x) { x.min = it.x; }
		if (y.max < it.y) { y.max = it.y; }
		if (y.min > it.y) { y.min = it.y; }

	}
	std::cout<<"x min" << x.min << "x max " << x.max << "y min " << y.min << "y max " << y.max << std::endl;

}
float data::getX(int it){
	return points[it].x;
}
float data::getY(int it) {
	return points[it].y;
}

void data::sort(){
	std::vector<position> sortedPoints;
	std::sort(points.begin(), points.end(), [](const position& a, const position& b) {
		return a.x < b.x;
		});
}

void data::uncertainty() {
	double s = 0.f, S = 0.f, E_sq = 0.f, x_avg = 0.f, x_sum = 0.f;
	int n = size;

	for (auto it : points) {
		E_sq += (it.y - (a * it.x + b)) * (it.y - (a * it.x + b));
		x_sum += it.x;
	}
	x_avg = x_sum / n;

	s = sqrt((1.0 / (n - 2))*E_sq);

	for (auto it : points) {
		S += (it.x - x_avg) * (it.x - x_avg);
	}

	type_a = s / sqrt(S);


}




