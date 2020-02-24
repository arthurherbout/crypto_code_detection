/*
 * Acoustic-MUSIC
 * Copyright (C) 2019 David Zhang
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * ================================================================== */

/** @file matrix.c
 *  @brief Functions used for real and complex matrix operations.
 *
 *  @author David Zhang (Davarco)
 */
#include <stdio.h>
#include <math.h>
#include "matrix.h"


void print(double *A, int m, int n, char *msg)
{
	printf("%s\n", msg);
	for (int r = 0; r < m; r++)
	{
		for (int c = 0; c < n; c++)
			printf("%0.15f  ", A[n*r+c]);
		printf("\n");
	}
}

void print_complex(double *Ar, double *Ai, int m, int n, char *msg)
{
	printf("%s\n", msg);
	for (int r = 0; r < m; r++)
	{
		for (int c = 0; c < n; c++)
			printf("%0.15f + %0.15fi  ", Ar[n*r+c], Ai[n*r+c]);
		printf("\n");
	}
}

void identity(double *A, int n)
{
	for (int r = 0; r < n; r++)
		for (int c = 0; c < n; c++)
			A[r*n+c] = (r == c) ? 1. : 0.;
}

void copy(double *A, int m, int n, double *B)
{
	for (int r = 0; r < n; r++)
		for (int c = 0; c < n; c++)
			B[r*n+c] = A[r*n+c];
}

void product(double *A, double *B, int m, int p, int n, double *C)
{
	for (int r = 0; r < m; r++)
	{
		for (int c = 0; c < n; c++)
		{
			C[n*r+c] = 0.;
			for (int k = 0; k < p; k++)
				C[n*r+c] += A[p*r+k] * B[n*k+c];
		}
	}
}

void product_complex(double *Ar, double *Ai, double *Br, double *Bi,
		int m, int p, int n, double *Cr, double *Ci)
{
	for (int r = 0; r < m; r++)
	{
		for (int c = 0; c < n; c++)
		{
			Cr[n*r+c] = 0.;
			Ci[n*r+c] = 0.;
			for (int k = 0; k < p; k++)
			{
				// FOIL: (a+bi) * (c+di) = (a*c)-(b*d) + (a*d)+(b*c)i
				double r1 = Ar[p*r+k];
				double i1 = Ai[p*r+k];
				double r2 = Br[n*k+c];
				double i2 = Bi[n*k+c];
				Cr[n*r+c] += r1*r2;
				Cr[n*r+c] -= i1*i2;
				Ci[n*r+c] += r1*i2;
				Ci[n*r+c] += r2*i1;
			}
		}
	}
}

void add(double *A, double *B, int m, int n, double *C)
{
	for (int r = 0; r < m; r++)
		for (int c = 0; c < n; c++)
			C[n*r+c] = A[n*r+c] + B[n*r+c];
}

void subtract(double *A, double *B, int m, int n, double *C)
{
	for (int r = 0; r < m; r++)
		for (int c = 0; c < n; c++)
			C[n*r+c] = A[n*r+c] - B[n*r+c];
}

void transpose(double *A, int m, int n, double *B)
{
	for (int r = 0; r < m; r++)
		for (int c = 0; c < n; c++)
			B[m*c+r] = A[n*r+c];
}

void transpose_complex(double *Ar, double *Ai, int m, int n, 
		double *Br, double *Bi)
{
	for (int r = 0; r < m; r++)
	{
		for (int c = 0; c < n; c++) 
		{
			// When transposing a complex matrix, the imaginary component is
			// multiplied by -1.
			Br[m*c+r] = Ar[n*r+c];
			Bi[m*c+r] = -Ai[n*r+c];
		}
	}
}

void scale(double *A, int m, int n, double a)
{
	for (int r = 0; r < m; r++)
		for (int c = 0; c < n; c++)
			A[n*r+c] = a*A[n*r+c];
}

void slice(double *A, int n, int m1, int m2, int n1, int n2, double *B)
{
	int i = 0;
	for (int r = m1; r < m2; r++)
		for (int c = n1; c < n2; c++)
			B[i++] = A[r*n+c];
}

void inverse(double *A, int n, double *B)
{
	identity(B, n);

	for (int i = 0; i < n; i++)
	{
		double k = A[i*n+i];
		for (int c = 0; c < n; c++)
		{
			A[i*n+c] /= k;
			B[i*n+c] /= k;
		}

		for (int r = 0; r < n; r++)
		{
			if (r == i) continue;

			k = A[r*n+i];
			for (int c = 0; c < n; c++)
			{
				A[r*n+c] -= k*A[i*n+c];
				B[r*n+c] -= k*B[i*n+c];
			}
		}
	}
}

void eigen(double *A, int n, double *val, double *vec)
{
	int CONVERGENCE_LIMIT = 50;

	identity(vec, n);

	double *temp_val = (double*) malloc(n*sizeof(double));
	double *dval = (double*) malloc(n*sizeof(double));
	for (int i = 0; i < n; i++)
	{
		val[i] = A[i*n+i];
		temp_val[i] = A[i*n+i];
		dval[i] = 0.;
	}

	for (int i = 0; i < CONVERGENCE_LIMIT; i++)
	{
		double sum = 0.;
		for (int r = 0; r < n-1; r++)
			for (int c = r+1; c < n; c++)
				sum += fabs(A[r*n+c]);

		if (sum == 0.) 
		{
			free(temp_val);
			free(dval);
			return;
		}

		for (int r = 0; r < n-1; r++)
		{
			for (int c = r+1; c < n; c++)
			{
				if (fabs(A[r*n+c]) > 0.)
				{
					double h = val[c]-val[r];
					double t = 0.;
					if (fabs(A[r*n+c]) == 0.)
						t = A[r*n+c]/h;
					else 
					{
						double theta = h/(2.*A[r*n+c]);
						t = 1./(fabs(theta)+sqrt(1.+theta*theta));
						if (theta < 0.) t *= -1.;
					}
					double cosrot = 1./sqrt(1.+t*t);
					double sinrot = t*cosrot;
					double tau = sinrot/(1.+cosrot);

					dval[r] -= t*A[r*n+c];
					dval[c] += t*A[r*n+c];
					val[r] -= t*A[r*n+c];
					val[c] += t*A[r*n+c];

					A[r*n+c] = 0.;
					for (int j = 0; j < r; j++)
					{
						double temp1 = A[j*n+r];
						double temp2 = A[j*n+c];
						A[j*n+r] = temp1 - sinrot*(temp2+temp1*tau);
						A[j*n+c] = temp2 + sinrot*(temp1-temp2*tau);
					}
					for (int j = r+1; j < c; j++)
					{
						double temp1 = A[r*n+j];
						double temp2 = A[j*n+c];
						A[r*n+j] = temp1 - sinrot*(temp2+temp1*tau);
						A[j*n+c] = temp2 + sinrot*(temp1-temp2*tau);
					}
					for (int j = c+1; j < n; j++)
					{
						double temp1 = A[r*n+j];
						double temp2 = A[c*n+j];
						A[r*n+j] = temp1 - sinrot*(temp2+temp1*tau);
						A[c*n+j] = temp2 + sinrot*(temp1-temp2*tau);
					}
					for (int j = 0; j < n; j++)
					{
						double temp1 = vec[j*n+r];
						double temp2 = vec[j*n+c];
						vec[j*n+r] = temp1 - sinrot*(temp2+temp1*tau);
						vec[j*n+c] = temp2 + sinrot*(temp1-temp2*tau);
					}
				}
			}
		}

		for (int i = 0; i < n; i++)
		{
			temp_val[i] += dval[i];
			val[i] = temp_val[i];
			dval[i] = 0.;
		}
	}

	free(temp_val);
	free(dval);
	printf("Eigen calculations exceeded convergence limit.\n");
}


void eigen_complex(double *Ar, double *Ai, int n, double *val,
		double *vecr, double *veci)
{
	int N = 2*n;
	double *A = (double*) malloc(N*N*sizeof(double));
	for (int r = 0; r < n; r++)
	{
		for (int c = 0; c < n; c++)
		{
			A[r*N+c] = Ar[r*n+c];
			A[(n+r)*N+c+n] = Ar[r*n+c];
			A[r*N+c+n] = -Ai[r*n+c];
			A[(n+r)*N+c] = Ai[r*n+c];
		}
	}

	double *A_val = (double*) malloc(N*sizeof(double));
	double *A_vec = (double*) malloc(N*N*sizeof(double));
	eigen(A, N, A_val, A_vec);
	free(A);

	for (int i = 0; i < n; i++)
		val[i] = A_val[i];
	for (int r = 0; r < n; r++)
	{
		for (int c = 0; c < n; c++)
		{
			vecr[r*n+c] = A_vec[r*N+c];
			veci[r*n+c] = A_vec[(n+r)*N+c];
		}
	}
	free(A_val);
	free(A_vec);
}



