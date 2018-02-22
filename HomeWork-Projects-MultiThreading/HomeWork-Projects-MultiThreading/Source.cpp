#include "Map.h"
#include <vector>
#include "Parallel.h"

void func(int &a) {
	a++;
	std::cout << a << " ";
}

void main() {
	ThreadPool pool(10);
	std::vector<int> v(5, 0);
	v[0] = 0;
	v[1] = 1;
	v[2] = 2;
	v[3] = 3;
	v[4] = 4;
	pool.Parallel(v.begin(), v.end(), func);
	std::this_thread::sleep_for(std::chrono::seconds(3));
	std::cout << std::endl;
	for (int i : v) {
		std::cout << i << " ";
	}
	system("pause");
}