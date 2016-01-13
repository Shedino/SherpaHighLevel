/*
 * Model_GS_private.h
 *
 * Code generation for model "Model_GS".
 *
 * Model version              : 1.186
 * Simulink Coder version : 8.7 (R2014b) 08-Sep-2014
 * C++ source code generated on : Tue Jan 12 18:24:49 2016
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Generic->32-bit x86 compatible
 * Code generation objective: Execution efficiency
 * Validation result: Not run
 */
#ifndef RTW_HEADER_Model_GS_private_h_
#define RTW_HEADER_Model_GS_private_h_
#include "rtwtypes.h"
#include "multiword_types.h"

extern void Model_GS_Int32toDouble(const int32_T rtu_u[3],
  B_Int32toDouble_Model_GS_T *localB);
extern void Model_GS_ScalingWGS(const real_T rtu_WGS[3], B_ScalingWGS_Model_GS_T
  *localB);
extern void Model_GS_ECEFtoNED(const real_T rtu_WGS_home[3], const real_T
  rtu_ECEF_in[3], const real_T rtu_ECEF_home[3], B_ECEFtoNED_Model_GS_T *localB);
extern void Model_MATLABFunctionWGS84TOECEF(const real_T rtu_WGS[3],
  B_MATLABFunctionWGS84TOECEF_M_T *localB);
extern void Model_GS_singletodouble1(real32_T rtu_u,
  B_singletodouble1_Model_GS_T *localB);

#endif                                 /* RTW_HEADER_Model_GS_private_h_ */
