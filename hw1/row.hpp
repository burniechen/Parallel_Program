#pragma once

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <thread>
#include <functional>
// #include "col.hpp"
#include "total.hpp"
#include <cassert>

template <typename T>
class Row_major_Matrix
{
	private:
		std::vector<std::vector<T>> all_row;
		int thread_c = 10;

	public:
		int row, col;
		
		int getT() { return thread_c; }

		T getter(int i, int j) const { return all_row[i][j]; }

		void setter(int i, int j, T val) { all_row[i][j] = val; }

		// non-default constructor
		Row_major_Matrix(int r = 0, int c = 0)
			:row(r), col(c) 
		{
			all_row.resize(row, std::vector<T>(col));
			
			for(int i=0; i<row; i++)
			{
				for(int j=0; j<col; j++)
				{
					T tmp = rand()%10+1;
					setter(i, j, tmp);
				}
			}
			std::cout << "Non-default Constructor is called..." << std::endl;
		}

		// copy contructor
		Row_major_Matrix (const Row_major_Matrix &other)
			:row(other.row), col(other.col), all_row(other.all_row)
		{
			std::cout << "Copy Constructor is called..." << std::endl;
		}
		
		// overload copy assignment 
		Row_major_Matrix &operator = (const Row_major_Matrix &other)
		{
            row = other.row;
            col = other.col;
			all_row = other.all_row;
			std::cout << "Overload Copy Assignment is called..." << std::endl;
			return *this;
		}
		
		// move constructor
		Row_major_Matrix(const Row_major_Matrix &&other)
			:row(other.row), col(other.col), all_row(std::move(other.all_row))
		{
			std::cout << "Move Constructor is called..." << std::endl;
		}
		
		// overload move assignment 
		Row_major_Matrix &operator = (const Row_major_Matrix &&other)
		{
            row = std::move(other.row);
            col = std::move(other.col);
			all_row = std::move(other.all_row);
			std::cout << "Overload Move Assignment is called..." << std::endl;
			return *this;
		}

        Row_major_Matrix operator *(const Column_major_Matrix<T> &col_m)
        {
            Row_major_Matrix<T> r_tmp(row, col_m.col);

            for(int i=0; i<row; i++)
            {
                for(int j=0; j<col_m.col; j++)
                {
                    T tmp = 0;
                    for(int k=0; k<col; k++)
                    {
                        tmp += getter(i, k)*col_m.getter(k, j);
                    }
                    r_tmp.setter(i, j, tmp);
                }
            }

            return r_tmp;
        }

		// implicit type conversion
        operator Column_major_Matrix<T>()
        {
			Column_major_Matrix<T> c_tmp(col, row);
            for(int i=0; i<row; i++)
                for(int j=0; j<col; j++)
                    c_tmp.setter(j, i, getter(i, j));
			
			std::cout << "*** Row -> Column ***\n";

            return c_tmp;
        }

		static void multify_threading(Row_major_Matrix<T> &re, Row_major_Matrix<T> &r, const Column_major_Matrix<T> &c, int thread_idx)
		{
			int total_cal = r.row * c.col;
			int cal_per_thread = total_cal / re.getT();
			int runned = thread_idx * cal_per_thread;
			int row_start = runned / c.col;
			int col_start = runned % c.col;	
			int remainder = (thread_idx == re.getT()-1) ? total_cal - (thread_idx+1)*cal_per_thread : 0;

			if(cal_per_thread < 1)
				re = r*c;
			
			int count = cal_per_thread + remainder;
			while(count--)
			{
				T tmp = 0;
				for(int k=0; k<r.col; k++)
					tmp += r.getter(row_start, k) * c.getter(k, col_start);
				re.setter(row_start, col_start, tmp);

				col_start++;
				if(col_start == c.col) col_start = 0, row_start++;
			}

			//for(int i=row_start; i<r.row; i++)
			//{
			//	for(int j=col_start; j<c.col; j++)
			//	{
			//		T tmp = 0;
			//		for(int k=0; k<r.col; k++)
			//			tmp += r.getter(i, k) * c.getter(k, j);
			//		re.setter(i, j , tmp);
			//	}
			//}
		}

		Row_major_Matrix<T> operator %(const Column_major_Matrix<T> &col_m)
		{
			Row_major_Matrix<T> r_tmp(row, col_m.col);

			std::vector<std::thread> tt;
			for(int i=0; i<getT(); i++)
				tt.emplace_back(multify_threading, std::ref(r_tmp), std::ref(*this), std::ref(col_m), i);

			for(int i=0; i<getT(); i++)
				tt[i].join();

			return r_tmp;
		}

		// destructor
	   ~Row_major_Matrix(){}
};
