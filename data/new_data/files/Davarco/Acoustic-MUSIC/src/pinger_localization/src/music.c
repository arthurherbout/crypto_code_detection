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

/** @file music.c
 *  @brief Functions used to compute DOA (direction-of-arrival) using MUSIC.
 *
 *  @author David Zhang (Davarco)
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "music.h"
#include "matrix.h"
#include "config.h"


void dft(double *ps)
{
	// Open named pipe to receive data from pigner simulator.
	FILE *in = fopen("datapipe", "r");
	// FILE *in = fopen("data.txt", "r");

	// Write data to file so it can be plotted.
	FILE *out = fopen("signal.txt", "w");

	// Compute real and imaginary components of the DFT.
	// TODO Add DFT explanation.
	double real[N_PHN], imag[N_PHN];
	for (int i = 0; i < N_PHN; i++)
	{
		real[i] = 0.0;
		imag[i] = 0.0;
		ps[i] = 0.0;
	}
	double t;
	double phones[4];
	for (int n = 0; n < N_DFT; n++)
	{
		// t += 1./FS;
		fscanf(in, "%lf %lf %lf %lf %lf", &t, 
				&phones[0], &phones[1], &phones[2], &phones[3]);
		fprintf(out, "%f,%f,%f,%f,%f\n", t, 
				phones[0], phones[1], phones[2], phones[3]);
		// if (n == 0) printf("\nTime: %f\n", t);
		double angle = 2*M_PI*n*F0/FS;
		for (int h = 0; h < N_PHN; h++)
		{
			real[h] += cos(angle)*phones[h];
			imag[h] -= sin(angle)*phones[h];
		}
	}

	// Compute phase shift from real and imaginary components. Since atan only
	// handles 180 degrees, we need to manually find out which quadrant it is
	// in. The extra M_PI/2 isn't necessary, but it makes the phase shifts match
	// the ones computed in the simulator.
	for (int h = 0; h < N_PHN; h++)
	{
		ps[h] = atan(imag[h]/real[h]);
		ps[h] = (real[h] > 0) ? ps[h]+M_PI/2. : ps[h]+3.*M_PI/2.;
	}

	// Clean up opened files.
	fclose(in);
	fclose(out);
}

void tdoa(double *ps, double *tdoa)
{
	// Compute the difference between the phase shifts and convert to periods,
	// NOT seconds, like usual TDOA.
	for (int i = 0; i < M; i++) 
		tdoa[i] = (ps[i]-ps[0])/2./M_PI;
}

void music(double *tdoa, double *angle)
{
	// Compute the signal model, X = A*S. "X" represents the total signal, "A"
	// is TDOA, and "S" is the source signal. I think that the source signal is
	// supoosed to be the raw readings (e.g. from pinger_simulation), but since
	// the RoboSub competition gives the pinger frequency beforehand, I can just
	// simulate the signal again to avoid signal noise.
	double *Xr = (double*) malloc(M*N*sizeof(double));
	double *Xi = (double*) malloc(M*N*sizeof(double));
	double *Ar = (double*) malloc(M*sizeof(double));
	double *Ai = (double*) malloc(M*sizeof(double));
	double *Sr = (double*) malloc(N*sizeof(double));
	double *Si = (double*) malloc(N*sizeof(double));
	for (int i = 0; i < M; i++)
	{
		double angle = 2.*M_PI*tdoa[i];
		Ar[i] = cos(angle);
		Ai[i] = -sin(angle);
	}
	// print_complex(Ar, Ai, M, 1, "\nA: ");
	for (int i = 0; i < N; i++)
	{	
		double angle = 2.*M_PI*F0*i;
		Sr[i] = cos(angle);
		Si[i] = sin(angle);
	}
	// print_complex(Sr, Si, 1, N, "\nS: ");
	product_complex(Ar, Ai, Sr, Si, M, 1, N, Xr, Xi);	
	// print_complex(Xr, Xi, M, N, "\nX: ");	
	free(Ar); 
	free(Ai);
	free(Sr); 
	free(Si);

	// Compute the covariance matrix.  
	double *XTr = (double*) malloc(N*M*sizeof(double));
	double *XTi = (double*) malloc(N*M*sizeof(double));
	double *Rr = (double*) malloc(M*M*sizeof(double));
	double *Ri = (double*) malloc(M*M*sizeof(double));
	transpose_complex(Xr, Xi, M, N, XTr, XTi);
	// print_complex(XTr, XTi, N, M, "\nX (Transpose): ");
	product_complex(Xr, Xi, XTr, XTi, M, N, M, Rr, Ri);
	free(XTr);
	free(XTi);
	free(Xr);
	free(Xi); 
	if (DEBUG) print_complex(Rr, Ri, M, M, "\nCovariance Matrix: ");

	// If the elements along the main diagonal are not exactly equal, the
	// program will compute the wrong eigenvectors and fail. 
	for (int i = 1; i < M; i++)
	{
		if (Rr[i*M+1] != Rr[0])
		{
			printf("\nError encountered. Elements along the main diagonal are "
			       "not equal, which could cause the program to crash. "
				   "Attemping manual fix now.\n");
			Rr[i*M+1] = Rr[0];
		}
	}

	// Compute the signal correlation matrix, or the eigenvectors of the
	// covariance matrix.
	double *Eval = (double*) malloc(M*sizeof(double));
	double *Evecr = (double*) malloc(M*M*sizeof(double));
	double *Eveci = (double*) malloc(M*M*sizeof(double));
	eigen_complex(Rr, Ri, M, Eval, Evecr, Eveci);
	if (DEBUG) print(Eval, M, 1, "\nEigenvalues: ");
	if (DEBUG) print_complex(Evecr, Eveci, M, M, "\nEigenvectors: ");
	free(Rr);
	free(Ri);
	free(Eval);

	// Slice the matrix to obtain the noise subspace of the correlation matrix.
	double *NSr = (double*) malloc(M*(M-1)*sizeof(double));
	double *NSi = (double*) malloc(M*(M-1)*sizeof(double));
	double *NSTr = (double*) malloc((M-1)*M*sizeof(double));
	double *NSTi = (double*) malloc((M-1)*M*sizeof(double));
	slice(Evecr, M, 0, M, 0, M-1, NSr);
	slice(Eveci, M, 0, M, 0, M-1, NSi);
	transpose_complex(NSr, NSi, M, M-1, NSTr, NSTi);
	free(Evecr);
	free(Eveci);
	if (DEBUG) print_complex(NSr, NSi, M, M-1, "\nNoise Subspace: ");

	// Iterate over possible DOAs.
	int n = 180./SEARCH_INTERVAL+1;
	double theta = -90.;
	double *Y = (double*) malloc(n*sizeof(double));
	for (int i = 0; i < n; i++)
	{
		double *SSr = (double*) malloc(M*sizeof(double));
		double *SSi = (double*) malloc(M*sizeof(double));
		for (int j = 0; j < M; j++)
		{
			SSr[j] = cos(2.*M_PI*SPACING*sin(theta*M_PI/180.)*F0/C*j);
			SSi[j] = sin(2.*M_PI*SPACING*sin(theta*M_PI/180.)*F0/C*j);
		}
		double *SSTr = (double*) malloc(M*sizeof(double));
		double *SSTi = (double*) malloc(M*sizeof(double));
		transpose_complex(SSr, SSi, 1, M, SSTr, SSTi);
		
		double *temp1r = (double*) malloc((M-1)*sizeof(double));
		double *temp1i = (double*) malloc((M-1)*sizeof(double));
		product_complex(SSr, SSi, NSr, NSi, 1, M, M-1, temp1r, temp1i);
		double *temp2r = (double*) malloc(M*sizeof(double));
		double *temp2i = (double*) malloc(M*sizeof(double));
		product_complex(temp1r, temp1i, NSTr, NSTi, 1, M-1, M, temp2r, temp2i);
		double *temp3r = (double*) malloc(1*sizeof(double));
		double *temp3i = (double*) malloc(1*sizeof(double));
		product_complex(temp2r, temp2i, SSTr, SSTi, 1, M, 1, temp3r, temp3i);
		Y[i] = fabs(1./temp3r[0]);
		free(SSr);
		free(SSi);
		free(SSTr);
		free(SSTi);
		free(temp1r);
		free(temp1i);
		free(temp2r);
		free(temp2i);
		free(temp3r);
		free(temp3i);
		
		theta += SEARCH_INTERVAL;
	}
	free(NSr);
	free(NSi);
	free(NSTr);
	free(NSTi);

	// Convert to dB so it is readable and plottable.
	FILE *out = fopen("doa.txt", "w");
	double max = -DBL_MAX;
	for (int i = 0; i < n; i++)
		max = (Y[i] > max) ? Y[i] : max;
	for (int i = 0; i < n; i++)
	{
		Y[i] = 10.*log10(Y[i]/max);
		fprintf(out, "%f,%f\n", -90+i*SEARCH_INTERVAL, Y[i]);
	}
	fclose(out);

	// Search for loudest angle to find DOA.
	max = -DBL_MAX;
	int max_idx = 0;
	for (int i = 0; i < n; i++)
	{
		if (Y[i] > max)
		{
			max = Y[i];
			max_idx = i;
		}
	}
	free(Y);

	// Write largest angle to initial address.
	double doa = -90.+max_idx*SEARCH_INTERVAL;
	*angle = doa;
}


