/*
 * WP_grid.h
 *
 * Code generation for function 'WP_grid'
 *
 * C source code generated on: Thu Oct 01 17:04:44 2015
 *
 */

#ifndef __WP_GRID_H__
#define __WP_GRID_H__
/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"

#include "rtwtypes.h"
#include "WP_grid_types.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
extern void WP_grid(const emxArray_real_T *vertex, const real_T initial_position[2], real_T d, real_T WP[200], boolean_T *success, real_T *number_WP);
extern void WP_grid_initialize(void);
extern void WP_grid_terminate(void);
extern void convexify_polygon(const emxArray_real_T *vertex, const emxArray_real_T *zcrossproduct, boolean_T *convex, emxArray_real_T *convex_vertex, emxArray_real_T *zcrossproduct_out);
extern emxArray_real_T *emxCreateND_real_T(int32_T numDimensions, int32_T *size);
extern emxArray_real_T *emxCreateWrapperND_real_T(real_T *data, int32_T numDimensions, int32_T *size);
extern emxArray_real_T *emxCreateWrapper_real_T(real_T *data, int32_T rows, int32_T cols);
extern emxArray_real_T *emxCreate_real_T(int32_T rows, int32_T cols);
extern void emxDestroyArray_real_T(emxArray_real_T *emxArray);
extern void find_closest_2D(const emxArray_real_T *points, const real_T initial_point[2], real_T closest_point[2], real_T *b_index);
extern void intersection_line_segment(const real_T s1[2], const real_T s2[2], real_T m, real_T q, uint8_T *is_intersecting, real_T intersection_point[2]);
extern void is_convex(const emxArray_real_T *vertex, boolean_T convexify, boolean_T *convex, emxArray_real_T *convex_vertex);
#endif
/* End of code generation (WP_grid.h) */
