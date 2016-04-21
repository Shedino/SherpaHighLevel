/*
 * Model_GS.cpp
 *
 * Code generation for model "Model_GS".
 *
 * Model version              : 1.199
 * Simulink Coder version : 8.7 (R2014b) 08-Sep-2014
 * C++ source code generated on : Wed Feb 03 10:38:35 2016
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Generic->32-bit x86 compatible
 * Code generation objective: Execution efficiency
 * Validation result: Not run
 */
#include "Model_GS.h"
#include "Model_GS_private.h"

/*
 * Output and update for atomic system:
 *    '<Root>/Any2[-pi;pi]'
 *    '<Root>/Any2[-pi;pi]1'
 */
void Model_GS_Any2pipi(real_T rtu_u, B_Any2pipi_Model_GS_T *localB)
{
  real_T u;

  /* MATLAB Function 'Any2[-pi;pi]': '<S1>:1' */
  /* '<S1>:1:4' */
  /* x are the extra 2pi */
  /* subtract the extra 2pi */
  /* '<S1>:1:8' */
  u = rtu_u / 6.2831853071795862;
  if (u < 0.0) {
    u = ceil(u);
  } else {
    u = floor(u);
  }

  localB->output = rtu_u - u * 2.0 * 3.1415926535897931;

  /* now y is in the ranges [0;2pi] or [0;-2pi] */
  if (localB->output > 3.1415926535897931) {
    /* '<S1>:1:11' */
    /* '<S1>:1:12' */
    localB->output -= 6.2831853071795862;
  }

  if (localB->output < -3.1415926535897931) {
    /* '<S1>:1:14' */
    /* '<S1>:1:15' */
    localB->output += 6.2831853071795862;
  }

  /* now y is in ranges [0;pi] and [0;-pi] */
  /* '<S1>:1:20' */
}

/*
 * Output and update for atomic system:
 *    '<Root>/Int32 to Double'
 *    '<Root>/Int32 to Double6'
 */
void Model_GS_Int32toDouble(const int32_T rtu_u[3], B_Int32toDouble_Model_GS_T
  *localB)
{
  /* MATLAB Function 'Int32 to Double': '<S8>:1' */
  /* '<S8>:1:4' */
  localB->y[0] = rtu_u[0];
  localB->y[1] = rtu_u[1];
  localB->y[2] = rtu_u[2];
}

/*
 * Output and update for atomic system:
 *    '<Root>/Scaling WGS'
 *    '<Root>/Scaling WGS1'
 */
void Model_GS_ScalingWGS(const real_T rtu_WGS[3], B_ScalingWGS_Model_GS_T
  *localB)
{
  /* MATLAB Function 'Scaling WGS': '<S11>:1' */
  /* '<S11>:1:4' */
  /* to rad */
  /* '<S11>:1:5' */
  /* to rad */
  /* '<S11>:1:6' */
  /* mm to meters */
  /* '<S11>:1:8' */
  localB->WGS_scaled[0] = rtu_WGS[0] * 1.0E-7 * 3.1415926535897931 / 180.0;
  localB->WGS_scaled[1] = rtu_WGS[1] * 1.0E-7 * 3.1415926535897931 / 180.0;
  localB->WGS_scaled[2] = rtu_WGS[2] * 0.001;
}

/*
 * Output and update for atomic system:
 *    '<S22>/ ECEF to NED'
 *    '<S26>/ ECEF to NED'
 */
void Model_GS_ECEFtoNED(const real_T rtu_WGS_home[3], const real_T rtu_ECEF_in[3],
  const real_T rtu_ECEF_home[3], B_ECEFtoNED_Model_GS_T *localB)
{
  real_T dX;
  real_T dY;
  real_T dZ;

  /* MATLAB Function 'WGS2NED1/ECEF2NED/ ECEF to NED': '<S24>:1' */
  /* '<S24>:1:4' */
  /* lat */
  /* '<S24>:1:5' */
  /* lon */
  /* '<S24>:1:7' */
  dX = rtu_ECEF_in[0] - rtu_ECEF_home[0];

  /* '<S24>:1:8' */
  dY = rtu_ECEF_in[1] - rtu_ECEF_home[1];

  /* '<S24>:1:9' */
  dZ = rtu_ECEF_in[2] - rtu_ECEF_home[2];

  /* '<S24>:1:11' */
  localB->N = (-cos(rtu_WGS_home[1]) * sin(rtu_WGS_home[0]) * dX - sin
               (rtu_WGS_home[1]) * sin(rtu_WGS_home[0]) * dY) + cos
    (rtu_WGS_home[0]) * dZ;

  /* '<S24>:1:12' */
  localB->E = -sin(rtu_WGS_home[1]) * dX + cos(rtu_WGS_home[1]) * dY;

  /* '<S24>:1:13' */
  localB->D = (-cos(rtu_WGS_home[1]) * cos(rtu_WGS_home[0]) * dX - sin
               (rtu_WGS_home[1]) * cos(rtu_WGS_home[0]) * dY) - sin
    (rtu_WGS_home[0]) * dZ;
}

/*
 * Output and update for atomic system:
 *    '<S22>/MATLAB Function WGS84 TO ECEF'
 *    '<S13>/MATLAB Function WGS84 TO ECEF'
 *    '<S26>/MATLAB Function WGS84 TO ECEF'
 *    '<S14>/MATLAB Function WGS84 TO ECEF'
 */
void Model_MATLABFunctionWGS84TOECEF(const real_T rtu_WGS[3],
  B_MATLABFunctionWGS84TOECEF_M_T *localB)
{
  real_T Ne;

  /* MATLAB Function 'WGS2NED1/ECEF2NED/MATLAB Function WGS84 TO ECEF': '<S25>:1' */
  /* '<S25>:1:4' */
  /* '<S25>:1:5' */
  /* '<S25>:1:6' */
  /* '<S25>:1:8' */
  Ne = sin(rtu_WGS[0]);
  Ne = 6.378137E+6 / sqrt(1.0 - Ne * Ne * 0.0066943798522561);

  /* '<S25>:1:9' */
  /* '<S25>:1:10' */
  /* '<S25>:1:11' */
  /* '<S25>:1:13' */
  localB->ECEF[0] = (Ne + rtu_WGS[2]) * cos(rtu_WGS[0]) * cos(rtu_WGS[1]);
  localB->ECEF[1] = (Ne + rtu_WGS[2]) * cos(rtu_WGS[0]) * sin(rtu_WGS[1]);
  localB->ECEF[2] = (Ne * 0.99330562014774393 + rtu_WGS[2]) * sin(rtu_WGS[0]);
}

/*
 * Output and update for atomic system:
 *    '<Root>/single to double1'
 *    '<Root>/uint16 to double1'
 *    '<Root>/uint16 to double2'
 */
void Model_GS_singletodouble1(real32_T rtu_u, B_singletodouble1_Model_GS_T
  *localB)
{
  /* MATLAB Function 'single to double1': '<S15>:1' */
  /* '<S15>:1:4' */
  localB->y = rtu_u;
}

/* Model step function */
void Model_GSModelClass::step()
{
  real_T N_err;
  real_T E_err;
  real_T D_err;
  real_T YAW_err;
  real_T u_N;
  real_T u_D;
  real_T rtb_y_a[9];
  int32_T i;

  /* MATLAB Function: '<Root>/uint16 to double1' incorporates:
   *  Inport: '<Root>/Actual_Yaw'
   */
  Model_GS_singletodouble1(Model_GS_U.Actual_Yaw, &Model_GS_B.sf_uint16todouble1);

  /* MATLAB Function: '<Root>/Any2[-pi;pi]1' */
  Model_GS_Any2pipi(Model_GS_B.sf_uint16todouble1.y, &Model_GS_B.sf_Any2pipi1);

  /* MATLAB Function: '<Root>/Int32 to Double6' incorporates:
   *  Inport: '<Root>/Actual_Pos '
   */
  Model_GS_Int32toDouble(Model_GS_U.Actual_Pos, &Model_GS_B.sf_Int32toDouble6);

  /* MATLAB Function: '<Root>/Scaling WGS' */
  Model_GS_ScalingWGS(Model_GS_B.sf_Int32toDouble6.y, &Model_GS_B.sf_ScalingWGS);

  /* MATLAB Function: '<S26>/MATLAB Function WGS84 TO ECEF' */
  Model_MATLABFunctionWGS84TOECEF(Model_GS_B.sf_ScalingWGS.WGS_scaled,
    &Model_GS_B.sf_MATLABFunctionWGS84TOECEF);

  /* MATLAB Function: '<S14>/MATLAB Function WGS84 TO ECEF' */
  Model_MATLABFunctionWGS84TOECEF(Model_GS_DW.Memory_PreviousInput,
    &Model_GS_B.sf_MATLABFunctionWGS84TOECEF_d);

  /* MATLAB Function: '<S26>/ ECEF to NED' */
  Model_GS_ECEFtoNED(Model_GS_DW.Memory_PreviousInput,
                     Model_GS_B.sf_MATLABFunctionWGS84TOECEF.ECEF,
                     Model_GS_B.sf_MATLABFunctionWGS84TOECEF_d.ECEF,
                     &Model_GS_B.sf_ECEFtoNED);

  /* MATLAB Function: '<Root>/Int32 to Double' incorporates:
   *  Inport: '<Root>/Reference_Pos '
   */
  Model_GS_Int32toDouble(Model_GS_U.Reference_Pos, &Model_GS_B.sf_Int32toDouble);

  /* MATLAB Function: '<Root>/Scaling WGS1' */
  Model_GS_ScalingWGS(Model_GS_B.sf_Int32toDouble.y, &Model_GS_B.sf_ScalingWGS1);

  /* MATLAB Function: '<S22>/MATLAB Function WGS84 TO ECEF' */
  Model_MATLABFunctionWGS84TOECEF(Model_GS_B.sf_ScalingWGS1.WGS_scaled,
    &Model_GS_B.sf_MATLABFunctionWGS84TOECEF_e);

  /* MATLAB Function: '<S13>/MATLAB Function WGS84 TO ECEF' */
  Model_MATLABFunctionWGS84TOECEF(Model_GS_DW.Memory_PreviousInput,
    &Model_GS_B.sf_MATLABFunctionWGS84TOECEF_j);

  /* MATLAB Function: '<S22>/ ECEF to NED' */
  Model_GS_ECEFtoNED(Model_GS_DW.Memory_PreviousInput,
                     Model_GS_B.sf_MATLABFunctionWGS84TOECEF_e.ECEF,
                     Model_GS_B.sf_MATLABFunctionWGS84TOECEF_j.ECEF,
                     &Model_GS_B.sf_ECEFtoNED_e);

  /* MATLAB Function: '<Root>/uint16 to double2' incorporates:
   *  Inport: '<Root>/Reference_Yaw'
   */
  Model_GS_singletodouble1(Model_GS_U.Reference_Yaw,
    &Model_GS_B.sf_uint16todouble2);

  /* MATLAB Function: '<Root>/Any2[-pi;pi]' */
  Model_GS_Any2pipi(Model_GS_B.sf_uint16todouble2.y, &Model_GS_B.sf_Any2pipi);

  /* MATLAB Function: '<Root>/single to double7' incorporates:
   *  Inport: '<Root>/Control_Param'
   */
  /* MATLAB Function 'single to double2': '<S16>:1' */
  /* '<S16>:1:4' */
  /* MATLAB Function 'single to double7': '<S17>:1' */
  /* '<S17>:1:4' */
  for (i = 0; i < 9; i++) {
    rtb_y_a[i] = Model_GS_U.Control_Param[i];
  }

  /* End of MATLAB Function: '<Root>/single to double7' */

  /* MATLAB Function: '<Root>/single to double1' incorporates:
   *  Inport: '<Root>/dt'
   */
  Model_GS_singletodouble1(Model_GS_U.dt, &Model_GS_B.sf_singletodouble1);

  /* MATLAB Function: '<Root>/Edge detector' incorporates:
   *  Inport: '<Root>/Trigger'
   */
  /* MATLAB Function 'Edge detector': '<S6>:1' */
  /* '<S6>:1:3' */
  i = 0;
  if ((Model_GS_DW.k == 0.0) && (Model_GS_U.Trigger == 1)) {
    /* '<S6>:1:9' */
    /* '<S6>:1:10' */
    Model_GS_DW.k = 1.0;

    /* '<S6>:1:11' */
    i = 1;
  }

  if ((Model_GS_DW.k == 1.0) && (Model_GS_U.Trigger == 0)) {
    /* '<S6>:1:13' */
    /* '<S6>:1:14' */
    Model_GS_DW.k = 0.0;
  }

  /* End of MATLAB Function: '<Root>/Edge detector' */

  /* MATLAB Function: '<Root>/Control Law NED' incorporates:
   *  Inport: '<Root>/Reference_Speed'
   *  MATLAB Function: '<Root>/single to double2'
   *  SignalConversion: '<S4>/TmpSignal ConversionAt SFunction Inport1'
   *  SignalConversion: '<S4>/TmpSignal ConversionAt SFunction Inport2'
   */
  /* MATLAB Function 'Control Law NED': '<S4>:1' */
  /* speed_ref is vx,vy,vz,vyaw */
  /* PARAMETERS */
  /* '<S4>:1:7' */
  /* '<S4>:1:8' */
  /* '<S4>:1:9' */
  /* '<S4>:1:10' */
  /* '<S4>:1:11' */
  /* '<S4>:1:12' */
  /* '<S4>:1:13' */
  /* '<S4>:1:14' */
  /* ERROR */
  /* '<S4>:1:17' */
  /* '<S4>:1:18' */
  /* '<S4>:1:19' */
  /* '<S4>:1:20' */
  /* '<S4>:1:21' */
  /* '<S4>:1:22' */
  /* '<S4>:1:23' */
  N_err = Model_GS_B.sf_ECEFtoNED_e.N - Model_GS_B.sf_ECEFtoNED.N;

  /* '<S4>:1:24' */
  E_err = Model_GS_B.sf_ECEFtoNED_e.E - Model_GS_B.sf_ECEFtoNED.E;

  /* '<S4>:1:25' */
  D_err = Model_GS_B.sf_ECEFtoNED_e.D - Model_GS_B.sf_ECEFtoNED.D;

  /* '<S4>:1:26' */
  YAW_err = Model_GS_B.sf_Any2pipi.output - Model_GS_B.sf_Any2pipi1.output;

  /* angles are in ranges [-pi;pi] */
  if (YAW_err > 3.1415926535897931) {
    /* '<S4>:1:27' */
    /* to avoid unwinding, we switch to closest error */
    /* '<S4>:1:28' */
    YAW_err -= 6.2831853071795862;
  }

  if (YAW_err < -3.1415926535897931) {
    /* '<S4>:1:30' */
    /* '<S4>:1:31' */
    YAW_err += 6.2831853071795862;
  }

  /* STATE */
  /* integral for Down to compensate midpoint RC_3 not 1500 in ardupilot */
  if (i != 0) {
    /* '<S4>:1:42' */
    Model_GS_DW.u_D_I = 0.0;

    /* reset integral */
  }

  if (fabs(N_err) > rtb_y_a[6]) {
    /* '<S4>:1:45' */
    /* '<S4>:1:46' */
    u_N = rtb_y_a[3] * N_err + Model_GS_U.Reference_Speed[0];

    /*  proportional and feedforward */
  } else {
    /* '<S4>:1:48' */
    u_N = 0.0;
  }

  if (fabs(E_err) > rtb_y_a[6]) {
    /* '<S4>:1:50' */
    /* '<S4>:1:51' */
    N_err = rtb_y_a[3] * E_err + Model_GS_U.Reference_Speed[1];

    /*  proportional and feedforward */
  } else {
    /* '<S4>:1:53' */
    N_err = 0.0;
  }

  if (fabs(D_err) > rtb_y_a[7]) {
    /* '<S4>:1:55' */
    /* '<S4>:1:56' */
    Model_GS_DW.u_D_I += D_err * rtb_y_a[8] * Model_GS_B.sf_singletodouble1.y;

    /* integral */
    if (Model_GS_DW.u_D_I > rtb_y_a[1] / 2.0) {
      /* '<S4>:1:57' */
      /* antiwindup */
      /* '<S4>:1:58' */
      Model_GS_DW.u_D_I = rtb_y_a[1] / 2.0;
    } else {
      if (Model_GS_DW.u_D_I < -rtb_y_a[1] / 2.0) {
        /* '<S4>:1:59' */
        /* '<S4>:1:60' */
        Model_GS_DW.u_D_I = -rtb_y_a[1] / 2.0;
      }
    }

    /* '<S4>:1:62' */
    u_D = (rtb_y_a[4] * D_err + Model_GS_DW.u_D_I) + Model_GS_U.Reference_Speed
      [2];

    /*  proportional and integral and feedforward */
  } else {
    /* '<S4>:1:64' */
    u_D = Model_GS_DW.u_D_I;
  }

  /* '<S4>:1:68' */
  E_err = rtb_y_a[5] * YAW_err + Model_GS_U.Reference_Speed[3];

  /*  proportional and feedforward */
  if (E_err > rtb_y_a[2]) {
    /* '<S4>:1:69' */
    /* '<S4>:1:70' */
    E_err = rtb_y_a[2];
  }

  if (E_err < -rtb_y_a[2]) {
    /* '<S4>:1:72' */
    /* '<S4>:1:73' */
    E_err = -rtb_y_a[2];
  }

  /* MATLAB Function: '<Root>/NED2BODY' incorporates:
   *  MATLAB Function: '<Root>/Control Law NED'
   */
  /* '<S4>:1:76' */
  /* MATLAB Function 'NED2BODY': '<S10>:1' */
  /* '<S10>:1:4' */
  /* '<S10>:1:5' */
  /* '<S10>:1:6' */
  /* '<S10>:1:8' */
  /* '<S10>:1:9' */
  /* '<S10>:1:12' */
  D_err = cos(Model_GS_B.sf_Any2pipi1.output) * u_N + sin
    (Model_GS_B.sf_Any2pipi1.output) * N_err;
  N_err = -sin(Model_GS_B.sf_Any2pipi1.output) * u_N + cos
    (Model_GS_B.sf_Any2pipi1.output) * N_err;

  /* MATLAB Function: '<Root>/Body Saturation' incorporates:
   *  MATLAB Function: '<Root>/Control Law NED'
   */
  /* MATLAB Function 'Body Saturation': '<S3>:1' */
  /* '<S3>:1:4' */
  /* '<S3>:1:5' */
  if (D_err > rtb_y_a[0]) {
    /* '<S3>:1:7' */
    /* '<S3>:1:8' */
    D_err = rtb_y_a[0];
  } else if (D_err < -rtb_y_a[0]) {
    /* '<S3>:1:9' */
    /* '<S3>:1:10' */
    D_err = -rtb_y_a[0];
  } else {
    /* '<S3>:1:12' */
  }

  if (N_err > rtb_y_a[0]) {
    /* '<S3>:1:15' */
    /* '<S3>:1:16' */
    N_err = rtb_y_a[0];
  } else if (N_err < -rtb_y_a[0]) {
    /* '<S3>:1:17' */
    /* '<S3>:1:18' */
    N_err = -rtb_y_a[0];
  } else {
    /* '<S3>:1:20' */
  }

  if (u_D > rtb_y_a[1]) {
    /* '<S3>:1:23' */
    /* '<S3>:1:24' */
    u_D = rtb_y_a[1];
  } else if (u_D < -rtb_y_a[1]) {
    /* '<S3>:1:25' */
    /* '<S3>:1:26' */
    u_D = -rtb_y_a[1];
  } else {
    /* '<S3>:1:28' */
  }

  /* Outport: '<Root>/BodySpeedInput' incorporates:
   *  MATLAB Function: '<Root>/Body Saturation'
   *  MATLAB Function: '<Root>/Control Law NED'
   *  MATLAB Function: '<Root>/Double to Single'
   */
  /* '<S3>:1:31' */
  /* MATLAB Function 'Double to Single': '<S5>:1' */
  /* '<S5>:1:4' */
  Model_GS_Y.BodySpeedInput[0] = (real32_T)D_err;
  Model_GS_Y.BodySpeedInput[1] = (real32_T)N_err;
  Model_GS_Y.BodySpeedInput[2] = (real32_T)u_D;
  Model_GS_Y.BodySpeedInput[3] = (real32_T)E_err;

  /* MATLAB Function: '<S7>/trigger' */
  /* MATLAB Function 'Home/trigger': '<S21>:1' */
  if (i != 0) {
    /* Update for Memory: '<S7>/Memory' */
    /* '<S21>:1:6' */
    Model_GS_DW.Memory_PreviousInput[0] = Model_GS_B.sf_ScalingWGS.WGS_scaled[0];
    Model_GS_DW.Memory_PreviousInput[1] = Model_GS_B.sf_ScalingWGS.WGS_scaled[1];
    Model_GS_DW.Memory_PreviousInput[2] = Model_GS_B.sf_ScalingWGS.WGS_scaled[2];
  } else {
    /* '<S21>:1:8' */
  }

  /* End of MATLAB Function: '<S7>/trigger' */
  /* MATLAB Function 'uint32 to double': '<S20>:1' */
  /* '<S20>:1:4' */
}

/* Model initialize function */
void Model_GSModelClass::initialize()
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus((&Model_GS_M), (NULL));

  /* block I/O */
  (void) memset(((void *) &Model_GS_B), 0,
                sizeof(B_Model_GS_T));

  /* states (dwork) */
  (void) memset((void *)&Model_GS_DW, 0,
                sizeof(DW_Model_GS_T));

  /* external inputs */
  (void) memset((void *)&Model_GS_U, 0,
                sizeof(ExtU_Model_GS_T));

  /* external outputs */
  (void) memset(&Model_GS_Y.BodySpeedInput[0], 0,
                4U*sizeof(real32_T));

  /* InitializeConditions for Memory: '<S7>/Memory' */
  Model_GS_DW.Memory_PreviousInput[0] = Model_GS_P.Memory_X0[0];
  Model_GS_DW.Memory_PreviousInput[1] = Model_GS_P.Memory_X0[1];
  Model_GS_DW.Memory_PreviousInput[2] = Model_GS_P.Memory_X0[2];

  /* InitializeConditions for MATLAB Function: '<Root>/Edge detector' */
  Model_GS_DW.k = 0.0;

  /* InitializeConditions for MATLAB Function: '<Root>/Control Law NED' */
  Model_GS_DW.u_D_I = 0.0;
}

/* Model terminate function */
void Model_GSModelClass::terminate()
{
  /* (no terminate code required) */
}

/* Constructor */
Model_GSModelClass::Model_GSModelClass()
{
  static const P_Model_GS_T Model_GS_P_temp = {
    /*  Expression: [0 0 0]
     * Referenced by: '<S7>/Memory'
     */
    { 0.0, 0.0, 0.0 }
  };                                   /* Modifiable parameters */

  /* Initialize tunable parameters */
  Model_GS_P = Model_GS_P_temp;
}

/* Destructor */
Model_GSModelClass::~Model_GSModelClass()
{
  /* Currently there is no destructor body generated.*/
}

/* Real-Time Model get method */
RT_MODEL_Model_GS_T * Model_GSModelClass::getRTM()
{
  return (&Model_GS_M);
}
