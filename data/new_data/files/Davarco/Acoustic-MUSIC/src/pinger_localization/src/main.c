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

/** @file main.c
 *  @brief Runs MUSIC for an acoustic localization system.
 * 	
 *  @author David Zhang (Davarco)
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "music.h"
#include "config.h"
#include "matrix.h"


int main()
{
	// while (true) 
	{
		/*
		 * 2 1
		 * 4 3
		 */
		printf("Calculating DOA (direction-of-arrival) using MUSIC.\n");
		double ps[N_PHN];
		dft(ps);
		printf("\nPhase Shifts (degrees):\n%f %f %f %f\n", 
				ps[0]*180./M_PI, ps[1]*180./M_PI, 
				ps[2]*180./M_PI, ps[3]*180./M_PI);
		printf("\nPhase Shifts (seconds):\n%f %f %f %f\n", 
				ps[0]*2.*M_PI*1./F0, ps[1]*2.*M_PI*1./F0, 
				ps[2]*2.*M_PI*1./F0, ps[3]*2.*M_PI*1./F0);

		/*
		double ps1[M] = { ps[0], ps[1] };
		double tdoa1[M];
		tdoa(ps1, tdoa1);
		print(tdoa1, 1, M, "\nTDOA for NE/NW: ");

		double doa1;
		music(tdoa1, &doa1);
		printf("\nDOA for NE/NW: %f\n", doa1);
		*/
		
		double ps2[M] = { ps[2], ps[3] };
		double tdoa2[M];
		tdoa(ps2, tdoa2);
		print(tdoa2, 1, M, "\nTDOA for SE/SW: ");

		double doa2;
		music(tdoa2, &doa2);
		printf("\nDOA for SE/SW: %f\n", doa2);
		
		/*
		double ps3[M] = { ps[1], ps[3] };
		double tdoa3[M];
		tdoa(ps3, tdoa3);
		print(tdoa3, 1, M, "\nTDOA for NW/SW: ");

		double doa3;
		music(tdoa3, &doa3);
		printf("\nDOA for NW/SW: %f\n", doa3);
		*/
	}

	return 0;
}
