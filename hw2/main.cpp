#include <bits/stdc++.h>
#include <iomanip>

std::vector<std::vector<int>> dp(std::vector<char> &x, std::vector<char> &y)
{
	std::vector<std::vector<int>> v(x.size()+1, std::vector<int> (y.size()+1));

	v[0][0] = 0;
	for(int i=0; i<x.size(); i++)
	{
		for(int j=0; j<y.size(); j++)
		{
			int vi=i+1, vj=j+1;
			std::vector<int> tmp(4, 0);
			tmp[0] = x[i] == y[j] ? v[vi-1][vj-1]+3 : v[vi-1][vj-1]-3;
			tmp[1] = v[vi-1][vj]-2;
			tmp[2] = v[vi][vj-1]-2;
			// tmp[3] = 0;
			v[vi][vj] = *max_element(tmp.begin(), tmp.end());
		}
	}

	for(auto row:v)
	{
		for(auto ele:row)
			std::cout << std::setw(3) << ele << " |";
		std::cout << "\n";
	}

	return v;
}

std::pair<int, int> FindMax(std::vector<std::vector<int>> &score)
{
	int max = score[0][0];
	std::pair<int, int> pos = std::make_pair(0, 0);
	for(int i=0; i<score.size(); i++)
		for(int j=0; j<score[0].size(); j++)
			if(score[i][j] > max) max = score[i][j], pos = std::make_pair(i, j);

	return pos;
}

void Traceback(
		std::vector<char> x,
		std::vector<char> y,
		std::vector<std::vector<int>> score,
		std::pair<int, int> pos,
		std::vector<char> &x_,
		std::vector<char> &y_)
{
	int vi=pos.first, vj=pos.second;
	std::cout << score[vi][vj] << " -> ";

	if(score[vi][vj] == 0) return;
	if(vi-1==0 || vj-1==0) return; 

	int max = score[vi][vj];
	pos = std::make_pair(vi-1, vj-1);
	if(score[vi-1][vj] > max) 
		max = score[vi-1][vj], pos = std::make_pair(vi-1, vj);
	else if(score[vi][vj-1] > max) 
		max = score[vi][vj-1], pos = std::make_pair(vi, vj-1);

	if(vj == pos.second) 
		x_.push_back(x[pos.first]), y_.push_back('_');
	else if(vi == pos.first) 
		x_.push_back('_'), y_.push_back(y[pos.second]);
	else 
		x_.push_back(x[pos.first]), y_.push_back(y[pos.second]);

	return Traceback(x, y, score, pos, x_, y_);
}

int main()
{
	std::string s1, s2;
	while(std::cin >> s1 >> s2)
	{
		std::vector<char> x(s1.begin(), s1.end());
		std::vector<char> y(s2.begin(), s2.end());
		std::vector<std::vector<int>> score = dp(x, y);
		std::pair<int, int> pos = FindMax(score);

		std::vector<char> x_, y_;
		Traceback(x, y, score, pos, x_, y_);

		std::cout << "\n";
		for(int i=x_.size()-1; i>=0; i--)
			std::cout << x_[i] << ' ';
		std::cout << "\n";
		for(int j=y_.size()-1; j>=0; j--)
			std::cout << y_[j] << ' ';
	}

	return 0;
}
