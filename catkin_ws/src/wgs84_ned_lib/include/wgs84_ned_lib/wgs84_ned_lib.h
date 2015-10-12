/*
 * WGS84_NED.h
 */


/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>


/* Function Declarations */
extern void get_pos_NED_from_WGS84 (double *x, double *y, double lat, double lon, double lat_home, double lon_home);
extern void get_pos_WGS84_from_NED (double *lat, double *lon, double x, double y, double lat_home, double lon_home);

