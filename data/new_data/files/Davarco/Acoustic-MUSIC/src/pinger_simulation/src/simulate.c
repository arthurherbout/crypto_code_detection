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

/** @file simulate.c
 *  @brief Simulates sampling a pinger in an underwater environment.
 *
 *  @author David Zhang (davarco)
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "config.h"


double randn(double, double);

int main()
{
	// Open named pipe to localization package.
	FILE *out = fopen("datapipe", "w");

	// Calculate pinger offsets from spacing.
	double OFFSET[NUM_PHONES][2] = {
		{  SPACING/2.,  SPACING/2. },
		{  SPACING/2., -SPACING/2. },
		{ -SPACING/2.,  SPACING/2. },
		{ -SPACING/2., -SPACING/2. }
	};

	// Find distances to pinger and theoretical phase shifts.
	double d[NUM_PHONES];
	double ps[NUM_PHONES];
	for (int i = 0; i < NUM_PHONES; i++) 
	{
		double dx = PINGER_X_LOC - OFFSET[i][0];
		double dy = PINGER_Y_LOC - OFFSET[i][1];
		d[i] = sqrt(dx*dx + dy*dy)/C*F0*2.*M_PI;
		ps[i] = fmod(d[i], 2.*M_PI)*180./M_PI;
	}
	// printf("Distances (radians): %f %f %f %f\n", d[0], d[1], d[2], d[3]);
	printf("Phase Shifts (degrees): %f %f %f %f\n", ps[0], ps[1], ps[2], ps[3]);
	printf("Phase Shifts (degrees): %f %f %f %f\n", 
			ps[0]*M_PI/180., ps[1]*M_PI/180., 
			ps[2]*M_PI/180., ps[3]*M_PI/180.);

	// Calculate amount of points where pinger is not pinging.
	// Sidenote: Don't do /TS in place of *FS. Since PING_DURATION and TS are
	// macros, the compiler will attempt to optimize and set SIGNAL to 0. In the
	// future, it's a good idea to move off macros for const, but that makes
	// some of the other constants a pain to declare.
	int EMPTY = (TIME_BETWEEN_PINGS-PING_DURATION)*FS;
	int SIGNAL = PING_DURATION*FS;

	// Write data to pipe from equation calculations.
	double v[NUM_PHONES];
	double i = 0; 
	while (1)
	{
		// No signal, pure noise (1% of signal).
		/*
		for (int n = 0; n < EMPTY; n++) 
		{
			v[0] = randn(0.0f, 0.01f);
			v[1] = randn(0.0f, 0.01f); 
			v[2] = randn(0.0f, 0.01f); 
			v[3] = randn(0.0f, 0.01f); 
			fprintf(out, "%f %f %f %f %f\n", i*TS, v[0], v[1], v[2], v[3]);
			i += 1;
		}
		*/

		// Contains signal, some noise (user-defined).
		for (int n = 0; n < SIGNAL; n++)
		{
			v[0] = sin(2.*M_PI*F0*i*TS+d[0]) + randn(0.0f, NOISE);
			v[1] = sin(2.*M_PI*F0*i*TS+d[1]) + randn(0.0f, NOISE);
			v[2] = sin(2.*M_PI*F0*i*TS+d[2]) + randn(0.0f, NOISE);
			v[3] = sin(2.*M_PI*F0*i*TS+d[3]) + randn(0.0f, NOISE);
			fprintf(out, "%f %f %f %f %f\n", i*TS, v[0], v[1], v[2], v[3]);
			i += 1;
		}
	}

}

// Phoxis.org/2013/05/04/generating-random-numbers-from-normal-distribution-in-c
// Generates data around a normal distribution in C.
double randn(double mu, double sigma)
{
	double U1, U2, W, mult;
	static double X1, X2;
	static int call = 0;

	if (call == 1) {
		call = !call;
		return (mu + sigma * (double) X2);
	}

	do {
		U1 = -1 + ((double) rand() / RAND_MAX) * 2;
		U2 = -1 + ((double) rand() / RAND_MAX) * 2;
		W = pow (U1, 2) + pow (U2, 2);
	} while (W >= 1 || W == 0);

	mult = sqrt ((-2 * log (W)) / W);
	X1 = U1 * mult;
	X2 = U2 * mult;

	call = !call;

	return (mu + sigma * (double) X1);
}

