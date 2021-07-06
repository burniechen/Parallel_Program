#include <armadillo>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <time.h>

void GetTmp(
		std::string file,
		std::vector<double> &data,
		int &row)
{
	std::ifstream f(file);
	if(!f)
		std::cout << "Can't read file: " << file << "\n", exit(0);

	double tmp;
	std::string line;
	while(std::getline(f, line))
	{
		row++;
		std::stringstream ss(line);
		while(ss >> tmp)
			data.push_back(tmp);
	}
}

void GetData(
		std::string X_file,
		std::string y_file,
		arma::Mat<double> &X,
		arma::colvec &y)
{
	int X_row = 0, y_row = 0; 
	std::vector<double> data, target;
	GetTmp(X_file, data, X_row);
	GetTmp(y_file, target, y_row);

	int total = data.size();
	int col = total/X_row;
	X = arma::Mat<double> (data);
	X.reshape(col, X_row);
	X = X.t();
	// z-score
	for(size_t i=0; i<col; i++)
	{
		double m = arma::mean(arma::mean(X.col(i)));
		double v = arma::mean(arma::stddev(X.col(i)));
		X.col(i) = (X.col(i)-m)/v;
	}

	y = arma::colvec (target);
}

arma::Mat<double> Ker(
		arma::Mat<double> X,
		arma::rowvec Xi,
		char type = 'l')
{
	return X*Xi.t();
}

arma::Mat<double> UpdateX(
		arma::Mat<double> &X,
		arma::colvec alpha)
{
	arma::uvec indices = find(alpha != 0);
	int row = arma::size(indices)[0];
	int col = arma::size(X)[1];
	std::cout << "old_index: " << arma::size(X)[0] << " , new_index: " << row << "\n";
	
	arma::Mat<double> X_tmp(row, col, arma::fill::zeros);
	for(int i=0; i<row; i++)
		X_tmp.row(i) = X.row(indices[i]);

	return X_tmp;
	//std::cout << "-----------new X: \n" << X;
}

arma::colvec UpdateY(
		arma::colvec &y,
		arma::colvec alpha)
{
	arma::uvec indices = find(alpha != 0);
	int row = arma::size(indices)[0];
	
	arma::colvec y_tmp(row, arma::fill::zeros);
	for(int i=0; i<row; i++)
		y_tmp[i] = y[indices[i]];

	return y_tmp;
	//std::cout << "-----------new y: \n" << y;
}


arma::colvec UpdateAlpha(arma::colvec &alpha)
{
	arma::uvec indices = find(alpha != 0);
	int row = arma::size(indices)[0];
	
	arma::colvec alpha_tmp(row, arma::fill::zeros);
	for(int i=0; i<row; i++)
		alpha_tmp[i] = alpha[indices[i]];

	return alpha_tmp;
	//std::cout << "-----------new alpha: \n" << alpha;
}

int Random(int i, int N)
{
	int j = rand() % N;
	while(i == j)
		j = rand() % N;

	return j;
}

arma::rowvec GetWeight(
		arma::Mat<double> X,
		arma::colvec y,
		arma::colvec alpha)
{
	auto y_alpha = alpha % y;
	std::vector<double> W_tmp;
	for(size_t i=0; i<arma::size(X)[1]; i++)
		W_tmp.push_back( arma::sum(y_alpha % X.col(i)) );
	arma::rowvec W(W_tmp);

	return W;
}

int main(int argc, const char **argv)
{
	if(argc < 3) return 0;

	srand(time(NULL));

	// init
	arma::Mat<double> X;
	arma::colvec y;
	std::string data_file = argv[1];
	std::string target_file = argv[2];
	GetData(data_file, target_file, X, y);
	
	int it = 0;
	int N = y.n_elem;
	double bias =0.0, C = 0.5, tol = 1e-04;
	arma::colvec alpha(N, arma::fill::zeros);
	int pass = 0, max_pass = 100;
	while(pass < max_pass)
	{
		N = y.n_elem;
		std::cout << "-----Round: " << it << " pass: " << pass << "-----\n";
		int alpha_changed = 0;
		for(size_t i=0; i<N; i++)
		{
			arma::colvec vi = (alpha % y).t() * Ker(X, X.row(i)) + bias;
			double Ei = vi[0] - y[i];
			if( (Ei*y[i] < -tol and alpha[i] < C) or
				(Ei*y[i] >  tol and alpha[i] > 0) )
			{
				int j = Random(i, N);
				arma::colvec vj = (alpha % y).t() * Ker(X, X.row(j)) + bias;
				double Ej = vj[0] - y[j];
				double alpha_i_old = alpha[i];
				double alpha_j_old = alpha[j];
				double L, H;
				if (y(i) != y(j))
				{
					L = std::max(0.0, alpha[j]-alpha[i]);
					H = std::min(C, C+alpha[j]-alpha[i]);
				}
				else
				{
					L = std::max(0.0, alpha[i]+alpha[j]-C);
					H = std::min(C,   alpha[i]+alpha[j]);
				}

				if (L==H) continue;

				auto eta = (2*X.row(j)*X.row(i).t()
						 - X.row(i)*X.row(i).t()
						 - X.row(j)*X.row(j).t())[0];

				if (eta >= 0.0) continue;
				
				alpha[j] = alpha[j] - (y[j]*(Ei-Ej)) / eta;
				
				if (alpha[j] > H) alpha[j] = H;
				if (alpha[j] < L) alpha[j] = L;

				if (alpha[j]-alpha_j_old < tol) continue;

				alpha[i] = alpha[i] + y[i]*y[j]*(alpha_j_old - alpha[j]);

				arma::rowvec Xrowii = X.row(i)*X.row(i).t();
				arma::rowvec Xrowij = X.row(i)*X.row(j).t();
				arma::rowvec Xrowjj = X.row(j)*X.row(j).t();
				
				double b1 = bias-Ei-y[i]*(alpha[i]-alpha_i_old)*Xrowii[0]
								   -y[j]*(alpha[j]-alpha_j_old)*Xrowij[0];

				double b2 = bias-Ej-y[i]*(alpha[i]-alpha_i_old)*Xrowij[0] 
								   -y[j]*(alpha[j]-alpha_j_old)*Xrowjj[0];

				if (0.0 < alpha[i] and alpha[i] < C) bias = b1;
				else if (0.0 < alpha[j] and alpha[j] < C) bias = b2;
				else bias = (b1+b2)/2.0;

				alpha_changed += 1;
			}
		}

		it++;
		if(alpha_changed == 0) pass += 1;
		else pass = 0;
	}
	
	std::cout << "\n--------Support vectors--------\n" << arma::size(find(alpha != 0))[0];

	auto W = GetWeight(X, y, alpha);
	std::cout << "\n--------Weight--------\n" << W;
	bias = arma::mean(y - X*W.t());
	std::cout << "\n--------Bias--------\n" << bias;

	return 0;
}
