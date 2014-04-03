#include <math.h>

#include "matrix.h"

/***********************************************************
* 求逆矩阵
***********************************************************/
int inverse(double *const m, double *const nm, const int &n) { 
	int i=0, j=0, k=0;
	double base=0.0;
    double *tm=new double[2*n*n];
    for (i=0; i<n; i++)
	{
        for (int j=0; j<n; j++)
            tm[i*n*2+j]=m[i*n+j];        
    }
    for (i=0; i<n; i++)
	{
        for (int j=n; j<n*2; j++)
            tm[i*n*2+j]=0.0;
        tm[i*n*2+n+i]=1.0;        
    }

    //initialization over!
    for (i=0; i<n; i++)//process cols
    {
        base=tm[i*n*2+i];
        if (fabs(base) < 1E-300)
		{
			return 1;
        }
        for (j=0; j<n; j++)//row
        {
            if (j==i) 
				continue;
            double times = tm[j*n*2+i]/base;
            for (k=0; k<n*2; k++)//col
            {        
                tm[j*n*2+k]=tm[j*n*2+k]-times*tm[i*n*2+k];
            }
        }        
        for (k=0; k<n*2; k++)
		{
            tm[i*n*2+k] /= base;
        }
    }
    for (i=0; i<n; i++)
    {
        for (j=0; j<n; j++)
            nm[i*n+j] = tm[i*n*2+j+n];        
    }    
    delete[] tm;
	return 0;
}

/**********************************************************
* 转置
***********************************************************/
void transpose(double *const m, double *const nm, const int &w, const int &h) {
	int i=0, j=0;
	for(i = 0; i<h;i++)
	{
		for(j=0; j<w;j++)
		{
			nm[j*h+i]  = m[i*w+j];
 		}
	}
}

/**********************************************************
* 相乘
***********************************************************/
int multiply(double *const m1, double *const m2, double *const nm, const int &w1, 
	const int &h1, const int &w2, const int &h2) {
	int i=0, j=0, p1=0, p2=0;
	double tmp = 0;
	if(w1 != h2)
	{
		return 1;
	}
	
	for(i=0; i<h1; i++)
	{
		for(j=0; j<w2; j++)
		{
			tmp=0;
			p2=j;
			for(p1=i*w1; p1<i*w1+w1; p1++)
			{
				tmp=tmp+m1[p1]*m2[p2];
				p2=p2+w2;
			}
			nm[i*w2+j] = tmp;
		}
	}
	return 0;
}

/******************************************************
* 最小二乘
******************************************************/
int least_square(double *const a, double *const l, double *const x,const int &wa, 
	const int &ha,const int &wl,const int &hl) {
	if(ha != hl)
	{
		return -1;
	}
	double *at = new double[wa*ha];
	double *ata = new double[wa*wa];
	double *rata = new double[wa*wa];
	double *rataat = new double[wa*ha];
	transpose(a, at, wa, ha);
	multiply(at, a, ata, ha, wa, wa, ha);
	inverse(ata, rata, wa);
	multiply(rata, at, rataat, wa, wa, ha, wa);
	multiply(rataat, l, x, ha, wa, 1, ha);
	delete []at;
	delete []ata;
	delete []rata;
	delete []rataat;
	return 0;
}

