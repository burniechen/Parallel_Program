The goal of Equation 1 is to impute F:
Equation 1:
	B x F(., j) = M'(., j)	1 <= j <= k, '.': m
	* B : signature matrix, m (marker gene) x c (cell types)
	* F : the fractional abundances of c cell types for each sample in M', c (cell type) x k (samples)
	* M': mixture of GEPs,  m (marker gene) x k (samples)
	B(m,c) X F(c,k) = M'(m,k) 

The goal of Equation 2 is to impute H:
Equation 2:
	H(i, .) x F = M(i, .)	1 <= i <= n, '.': c
	* H : representative cell type-specific expression signatures, n (all genes) x c (cell type)
	* F : c (cell type) x k (samples)
	* M : n (all genes) x k (samples)
	H(n,c) X F(c,k) = M(n,k)

The goal of Equation 3 is to impute G:
Equation 3:
	diag(G(i, ., .) x F) = M(i, .)	1 <= i <= n
	* G : n (genes) x k (samples) x c (cell types)
		* Gi : k (samples) x c (cell types), (k x c) expression matrix of k mixture samples by c cell types
	* F : c (cell types) x k (samples)
	* M : n (genes) x k (samples)
		* Mi : 1 x k (samples)
