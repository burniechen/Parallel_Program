#pragma once

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// #include "row.hpp"
#include "total.hpp"

template <typename T>
class Column_major_Matrix 
{
	private:
		std::vector<std::vector<T>> all_column;

	public:
		int row, col;
		
		T getter(int i, int j) const { return all_column[j][i]; }

		void setter(int i, int j, T val) { all_column[j][i] = val; }

		// non-default constructor
		Column_major_Matrix(int r = 0, int c = 0)
			:row(r), col(c) 
		{
			all_column.resize(col, std::vector<T>(row));
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
		Column_major_Matrix (const Column_major_Matrix &other)
			:row(other.row), col(other.col), all_column(other.all_column)
		{
			std::cout << "Copy Constructor is called..." << std::endl;
		}
		
		// copy assignment constructor
		Column_major_Matrix &operator = (const Column_major_Matrix &other)
		{
			row = other.row;
			col = other.col;
			all_column = other.all_column;
			std::cout << "Copy Assignment Construstor is called..." << std::endl;
			return *this;
		}
		
		// move constructor
		Column_major_Matrix(const Column_major_Matrix &&other)
			:row(other.row), col(other.col), all_column(std::move(other.all_column))
		{
			std::cout << "Move Constructor is called..." << std::endl;
		}
		
		// move assignment constructor
		Column_major_Matrix &operator = (const Column_major_Matrix &&other)
		{
			row = std::move(other.row);
			col = std::move(other.col);
			all_column = std::move(other.all_column);
			std::cout << "Move Assignment Constructor is called..." << std::endl;
			return *this;
		}

		Column_major_Matrix operator *(const Row_major_Matrix<T> &row_m)
        {
            Column_major_Matrix<T> c_tmp(row, row_m.col);
            
            for(int i=0; i<row; i++)
            {
                for(int j=0; j<row_m.col; j++)
                {
                    T tmp = 0;
                    for(int k=0; k<col; k++)
                    {
                        tmp += all_column[i][k]*row_m.getter(k, j);
                    }
                    c_tmp.setter(i, j, tmp);
                }
            }

            return c_tmp;
        }

		// implicit type conversion
		operator Row_major_Matrix<T>()
        {
			Row_major_Matrix<T> r_tmp(row, col);
            for(int i=0; i<row; i++)
                for(int j=0; j<col; j++)
                    r_tmp.setter(i, j, getter(i, j));

			std::cout << "*** Column -> Row ***\n";

            return r_tmp;
        }

		// destructor
	   ~Column_major_Matrix(){}
};