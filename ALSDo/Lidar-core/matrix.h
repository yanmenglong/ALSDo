#ifndef MATRIX_H
#define MATRIX_H

#ifdef __cplusplus
extern "C" 
{
#endif

/*******************************************************************************
* yanml (2006)
* 简单的矩阵运算，包括求逆、转置、相乘、最小二乘
* 对于方程组的求解，应考虑采用LU分解，以得到更高的运算效率和稳定性
*******************************************************************************/

int inverse(double *const m, double *const nm, const int &n);
void transpose(double *const m, double *const nm, const int &w, const int &h);
int multiply(double *const m1, double *const m2, double *const nm, const int &w1, 
	const int &h1, const int &w2, const int &h2);
int least_square(double *const a, double *const l, double *const x,const int &wa, 
	const int &ha,const int &wl,const int &hl);


#ifdef __cplusplus
}
#endif

#endif



