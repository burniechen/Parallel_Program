#pragma once

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// #include "col.hpp"
#include "total.hpp"

template <typename T>
class Row_major_Matrix
{
	private:
		std::vector<std::vector<T>> all_row;

	public:
		int row, col;
		
		T getter(int i, int j) const { return all_row[i][j]; }

		void setter(int i, int j, T val) { all_row[i][j] = val; }

		// non-default constructor
		Row_major_Matrix(int r = 0, int c = 0)
			:row(r), col(c) 
		{
			all_row.resize(row, std::vector<T>(col));
			srand(time(NULL));
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
                        tmp += all_row[i][k]*col_m.getter(k, j);
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

		// destructor
	   ~Row_major_Matrix(){}
};