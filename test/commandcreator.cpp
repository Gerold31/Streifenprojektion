#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>

using namespace std;

double deg(double grad);


int main(int argc, char *argv[])
{
	cout << "b img_ref.png\n";
	for (int i = 0; i <= 65; ++i)
	{
		cout << "c " << 10 << " " << deg(45) << " " << deg(i - 41) << "\n";
		cout << "img_" << setw(2) << setfill('0') << i << ".png\n";
	}
	return EXIT_SUCCESS;
}

double deg(double grad)
{
	return grad * M_PI / 180.0;
}
