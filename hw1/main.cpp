#include "row.hpp"
#include "col.hpp"
#include <chrono>
#include <cassert>
// or only #include "all"

int main()
{
	srand(time(NULL));

	std::cout << "-----Col-----" << std::endl;

	Column_major_Matrix<int> c_test(100, 1011);

	std::cout << c_test.row << "x" << c_test.col << std::endl;

	c_test.setter(0, 0, 1000);
	std::cout << c_test.getter(0, 0) << std::endl;

	Column_major_Matrix<int> c_test2;
	c_test2 = c_test;
	std::cout << c_test2.row << "x" << c_test2.col << std::endl;

	Column_major_Matrix<int> c_test3;
	c_test3 = std::move(c_test2);
	std::cout << c_test3.row << "x" << c_test3.col << std::endl;

	std::cout << "-----Row-----" << std::endl;

	Row_major_Matrix<int> r_test(1009, 100);
	std::cout << r_test.getter(0, 0) << std::endl;
	r_test.setter(0, 0, 2500);
	std::cout << r_test.getter(0, 0) << std::endl;

	//Row_major_Matrix<int> r_test2 = r_test;
	//std::cout << r_test2.getter(0, 0) << std::endl;

	//Row_major_Matrix<int> r_test3;
	//r_test3 = r_test;
	//std::cout << r_test3.getter(0, 0) << std::endl;

	//Row_major_Matrix<int> r_test4 = std::move(r_test);
	//std::cout << r_test4.getter(0, 0) << std::endl;

	//Row_major_Matrix<int> r_test5;
	//r_test5 = std::move(r_test);
	//std::cout << r_test5.getter(0, 0) << std::endl;

	//std::cout << "-----r_test-----\n";
	//for(int i=0; i<r_test.row; i++)
	//{
	//	for(int j=0; j<r_test.col; j++)
	//	{
	//		std::cout << r_test.getter(i, j) << " ";
	//	}
	//	std::cout << "\n";
	//}

	//std::cout << "-----c_test-----\n";
	//for(int i=0; i<c_test.row; i++)
	//{
	//	for(int j=0; j<c_test.col; j++)
	//	{
	//		std::cout << c_test.getter(i, j) << " ";
	//	}
	//	std::cout << "\n";
	//}

	//std::cout << "-----result: rMc-----\n";

	//Row_major_Matrix<int> rMc;
	//rMc = r_test*c_test;
	//std::cout << rMc.row << ' ' << rMc.col << '\n';
	//for(int i=0; i<rMc.row; i++)
	//{
	//	for(int j=0; j<rMc.col; j++)
	//	{
	//		std::cout << rMc.getter(i, j) << " ";
	//	}
	//	std::cout << "\n";
	//}

	//std::cout << "-----result: cMr-----\n";
	//Column_major_Matrix<int> cMr;
	//cMr = c_test*r_test;
	//std::cout << cMr.row << ' ' << cMr.col << '\n';
	//for(int i=0; i<cMr.row; i++)
	//{
	//	for(int j=0; j<cMr.col; j++)
	//	{
	//		std::cout << cMr.getter(i, j) << " ";
	//	}
	//	std::cout << "\n";
	//}

	//std::cout << "-----conversion-----\n";
	//Row_major_Matrix<int> rr;
	//rr = c_test*r_test;
	//for(int i=0; i<rr.row; i++)
	//{
	//	for(int j=0; j<rr.col; j++)
	//	{
	//		std::cout << rr.getter(i, j) << " ";
	//	}
	//	std::cout << "\n";
	//}

	std::cout << "-----multithreading-----\n";
	std::cout << "R_major: " << r_test.row << ", " << r_test.col << "\n";
	std::cout << "C_major: " << c_test.row << ", " << c_test.col << "\n";

	auto start1 = std::chrono::steady_clock::now();
	Row_major_Matrix<int> rc = r_test*c_test;
	auto end1 = std::chrono::steady_clock::now();
	std::chrono::duration<double> diff = end1 - start1;
	std::cout << "normal: " << diff.count() << "\n";

	auto start2 = std::chrono::steady_clock::now();
	Row_major_Matrix<int> r_th = r_test%c_test;
	auto end2 = std::chrono::steady_clock::now();
	std::chrono::duration<double> thread_diff = end2 - start2;
	std::cout << "thread: " << thread_diff.count() << "\n";

	return 0;
}
