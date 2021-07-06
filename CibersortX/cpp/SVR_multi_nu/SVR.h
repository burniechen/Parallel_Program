#include <string>
#include <armadillo>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>

#include "parser.h"
//#include "ThreadPool.h"
#include "svm.h"
#include "train.hpp"
#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

using Matrix  = arma::Mat<double>;
using namespace std;

static void SVR (string sig_file, string bulk_file, const double nu, string ID) {
	
	Matrix sig, bulk;
	vector<string> sig_gene, bulk_gene;
	vector<int> sig_gene_idx, bulk_gene_idx;
	string result_file = "SVR_"+ID+"_result.txt";

	cout << "Reading bulk and signature matrix..." << endl;

	sig = read_bulk(sig_file);
	bulk = read_bulk(bulk_file);
	sig_gene = read_gene(sig_file);
	bulk_gene = read_gene(bulk_file);

	for (int i = 0; i < sig_gene.size(); ++i) {
		for (int j = 0; j < bulk_gene.size(); ++j) {
			if (sig_gene[i] == bulk_gene[j]) {
				sig_gene_idx.push_back(i);
				bulk_gene_idx.push_back(j);
				break;
			}
		}
	}

	Matrix sig_train(sig_gene_idx.size(), sig.n_cols);
	arma::rowvec sig_as_vec = sig.row(sig_gene_idx[0]);
	for (int i = 0; i < sig_gene_idx.size(); ++i) {
		sig_train.row(i) = sig.row(sig_gene_idx[i]);
		if (i > 0) {
			sig_as_vec = arma::join_rows(sig_as_vec, sig_train.row(i));
		}
	}
	double sig_train_ave = arma::mean(arma::mean(sig_train));
	double sig_train_std = arma::stddev(sig_as_vec);
	sig_train = (sig_train - sig_train_ave) / sig_train_std;

	Matrix bulk_train(bulk_gene_idx.size(), bulk.n_cols);
	for (int i = 0; i < bulk_gene_idx.size(); ++i) {
		bulk_train.row(i) = bulk.row(bulk_gene_idx[i]);
	}

	for (int i = 0; i < bulk_train.n_cols; ++i) {
		double average = arma::mean(bulk_train.col(i));
		double std = arma::stddev(bulk_train.col(i));
		for (auto j = bulk_train.col(i).begin(); j != bulk_train.col(i).end(); ++j) {
			*j = (*j - average) / std;
		}
	}

	ofstream clean_result_file(result_file);
	clean_result_file << "";
	clean_result_file.close();

	for (int b = 0; b < bulk.n_cols; ++b) {
		Train* R = new Train();
		if (b == 600)
			break; // need to fixed
	
		R->prob.l  = sig_train.n_rows;
		R->prob.y  = Malloc(double, R->prob.l);
		R->prob.x  = Malloc(struct svm_node*, R->prob.l);
		R->x_space = Malloc(struct svm_node , R->prob.l*(sig_train.n_cols+1));

		int addr = 0;
		for (int i = 0; i < sig_train.n_rows; ++i) {
			R->prob.y[i] = bulk_train(i, b);
			R->prob.x[i] = &R->x_space[addr];
			for (int j=0; j<sig_train.n_cols+1; ++j)
			{
				R->x_space[addr].index = (j == sig_train.n_cols) ? -1 : j+1;
				R->x_space[addr].value = (j == sig_train.n_cols) ?  0 : sig_train(i, j);
				++addr;
			}
		}

		R->get_model(nu);

		arma::colvec alpha(R->sv_coefs);
		Matrix X(R->sv_coefs.size(), R->SVs[0].size());
		for (int i=0; i<R->sv_coefs.size(); ++i)
			for (int j=0; j<R->SVs[0].size(); ++j)
				X(i, j) = R->SVs[i][j];

		arma::rowvec W = alpha.t() * X;
		for (size_t i = 0; i < W.n_elem; ++i) 
			if (W(i) < 0) W(i) = 0;
		
		ofstream SVR_result;
		SVR_result.open(result_file, std::ios_base::app);
		int sum_W = arma::sum(W);
		for (size_t i = 0; i < W.n_elem; ++i) {
			if (sum_W == 0) W(i) = 0;
			else W(i) = W(i)/sum_W;
			SVR_result << W(i) << '\t';
		}
		SVR_result << '\n';
		SVR_result.close();
	}
}
