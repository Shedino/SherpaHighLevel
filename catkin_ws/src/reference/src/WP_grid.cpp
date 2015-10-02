/*
 * WP_grid.cpp
 *
 * Code generation for function 'WP_grid'
 *
 * C source code generated on: Thu Oct 01 17:04:44 2015
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "WP_grid.h"

/* Type Definitions */
#ifndef struct_emxArray__common
#define struct_emxArray__common

typedef struct emxArray__common
{
  void *data;
  int32_T *size;
  int32_T allocatedSize;
  int32_T numDimensions;
  boolean_T canFreeData;
} emxArray__common;

#endif

#ifndef struct_emxArray_boolean_T
#define struct_emxArray_boolean_T

typedef struct emxArray_boolean_T
{
  boolean_T *data;
  int32_T *size;
  int32_T allocatedSize;
  int32_T numDimensions;
  boolean_T canFreeData;
} emxArray_boolean_T;

#endif

#ifndef struct_emxArray_int32_T
#define struct_emxArray_int32_T

typedef struct emxArray_int32_T
{
  int32_T *data;
  int32_T *size;
  int32_T allocatedSize;
  int32_T numDimensions;
  boolean_T canFreeData;
} emxArray_int32_T;

#endif

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
static void b_emxInit_real_T(emxArray_real_T **pEmxArray, int32_T numDimensions);
static void c_emxInit_real_T(emxArray_real_T **pEmxArray, int32_T numDimensions);
static real_T eml_xnrm2(const real_T x[3]);
static void emxEnsureCapacity(emxArray__common *emxArray, int32_T oldNumel,
  int32_T elementSize);
static void emxFree_boolean_T(emxArray_boolean_T **pEmxArray);
static void emxFree_int32_T(emxArray_int32_T **pEmxArray);
static void emxFree_real_T(emxArray_real_T **pEmxArray);
static void emxInit_boolean_T(emxArray_boolean_T **pEmxArray, int32_T
  numDimensions);
static void emxInit_int32_T(emxArray_int32_T **pEmxArray, int32_T numDimensions);
static void emxInit_real_T(emxArray_real_T **pEmxArray, int32_T numDimensions);
static real_T norm(const real_T x[3]);
static real_T rt_hypotd_snf(real_T u0, real_T u1);
static real_T rt_powd_snf(real_T u0, real_T u1);
static real_T rt_roundd_snf(real_T u);

/* Function Definitions */
static void b_emxInit_real_T(emxArray_real_T **pEmxArray, int32_T numDimensions)
{
  emxArray_real_T *emxArray;
  int32_T loop_ub;
  int32_T i;
  *pEmxArray = (emxArray_real_T *)malloc(sizeof(emxArray_real_T));
  emxArray = *pEmxArray;
  emxArray->data = (real_T *)NULL;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int32_T *)malloc((uint32_T)(sizeof(int32_T) * numDimensions));
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = TRUE;
  loop_ub = numDimensions - 1;
  for (i = 0; i <= loop_ub; i++) {
    emxArray->size[i] = 0;
  }
}

static void c_emxInit_real_T(emxArray_real_T **pEmxArray, int32_T numDimensions)
{
  emxArray_real_T *emxArray;
  int32_T loop_ub;
  int32_T i;
  *pEmxArray = (emxArray_real_T *)malloc(sizeof(emxArray_real_T));
  emxArray = *pEmxArray;
  emxArray->data = (real_T *)NULL;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int32_T *)malloc((uint32_T)(sizeof(int32_T) * numDimensions));
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = TRUE;
  loop_ub = numDimensions - 1;
  for (i = 0; i <= loop_ub; i++) {
    emxArray->size[i] = 0;
  }
}

static real_T eml_xnrm2(const real_T x[3])
{
  real_T y;
  real_T scale;
  int32_T k;
  real_T absxk;
  real_T t;
  y = 0.0;
  scale = 2.2250738585072014E-308;
  for (k = 0; k < 2; k++) {
    absxk = fabs(x[k + 1]);
    if (absxk > scale) {
      t = scale / absxk;
      y = 1.0 + y * t * t;
      scale = absxk;
    } else {
      t = absxk / scale;
      y += t * t;
    }
  }

  return scale * sqrt(y);
}

static void emxEnsureCapacity(emxArray__common *emxArray, int32_T oldNumel,
  int32_T elementSize)
{
  int32_T newNumel;
  int32_T loop_ub;
  int32_T i;
  void *newData;
  newNumel = 1;
  loop_ub = emxArray->numDimensions - 1;
  for (i = 0; i <= loop_ub; i++) {
    newNumel *= emxArray->size[i];
  }

  if (newNumel > emxArray->allocatedSize) {
    loop_ub = emxArray->allocatedSize;
    if (loop_ub < 16) {
      loop_ub = 16;
    }

    while (loop_ub < newNumel) {
      loop_ub <<= 1;
    }

    newData = calloc((uint32_T)loop_ub, (uint32_T)elementSize);
    if (emxArray->data != NULL) {
      memcpy(newData, emxArray->data, (uint32_T)(elementSize * oldNumel));
      if (emxArray->canFreeData) {
        free(emxArray->data);
      }
    }

    emxArray->data = newData;
    emxArray->allocatedSize = loop_ub;
    emxArray->canFreeData = TRUE;
  }
}

static void emxFree_boolean_T(emxArray_boolean_T **pEmxArray)
{
  if (*pEmxArray != (emxArray_boolean_T *)NULL) {
    if ((*pEmxArray)->canFreeData) {
      free((void *)(*pEmxArray)->data);
    }

    free((void *)(*pEmxArray)->size);
    free((void *)*pEmxArray);
    *pEmxArray = (emxArray_boolean_T *)NULL;
  }
}

static void emxFree_int32_T(emxArray_int32_T **pEmxArray)
{
  if (*pEmxArray != (emxArray_int32_T *)NULL) {
    if ((*pEmxArray)->canFreeData) {
      free((void *)(*pEmxArray)->data);
    }

    free((void *)(*pEmxArray)->size);
    free((void *)*pEmxArray);
    *pEmxArray = (emxArray_int32_T *)NULL;
  }
}

static void emxFree_real_T(emxArray_real_T **pEmxArray)
{
  if (*pEmxArray != (emxArray_real_T *)NULL) {
    if ((*pEmxArray)->canFreeData) {
      free((void *)(*pEmxArray)->data);
    }

    free((void *)(*pEmxArray)->size);
    free((void *)*pEmxArray);
    *pEmxArray = (emxArray_real_T *)NULL;
  }
}

static void emxInit_boolean_T(emxArray_boolean_T **pEmxArray, int32_T
  numDimensions)
{
  emxArray_boolean_T *emxArray;
  int32_T loop_ub;
  int32_T i;
  *pEmxArray = (emxArray_boolean_T *)malloc(sizeof(emxArray_boolean_T));
  emxArray = *pEmxArray;
  emxArray->data = (boolean_T *)NULL;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int32_T *)malloc((uint32_T)(sizeof(int32_T) * numDimensions));
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = TRUE;
  loop_ub = numDimensions - 1;
  for (i = 0; i <= loop_ub; i++) {
    emxArray->size[i] = 0;
  }
}

static void emxInit_int32_T(emxArray_int32_T **pEmxArray, int32_T numDimensions)
{
  emxArray_int32_T *emxArray;
  int32_T loop_ub;
  int32_T i;
  *pEmxArray = (emxArray_int32_T *)malloc(sizeof(emxArray_int32_T));
  emxArray = *pEmxArray;
  emxArray->data = (int32_T *)NULL;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int32_T *)malloc((uint32_T)(sizeof(int32_T) * numDimensions));
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = TRUE;
  loop_ub = numDimensions - 1;
  for (i = 0; i <= loop_ub; i++) {
    emxArray->size[i] = 0;
  }
}

static void emxInit_real_T(emxArray_real_T **pEmxArray, int32_T numDimensions)
{
  emxArray_real_T *emxArray;
  int32_T loop_ub;
  int32_T i;
  *pEmxArray = (emxArray_real_T *)malloc(sizeof(emxArray_real_T));
  emxArray = *pEmxArray;
  emxArray->data = (real_T *)NULL;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int32_T *)malloc((uint32_T)(sizeof(int32_T) * numDimensions));
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = TRUE;
  loop_ub = numDimensions - 1;
  for (i = 0; i <= loop_ub; i++) {
    emxArray->size[i] = 0;
  }
}

static real_T norm(const real_T x[3])
{
  real_T y;
  real_T scale;
  int32_T k;
  real_T absxk;
  real_T t;
  y = 0.0;
  scale = 2.2250738585072014E-308;
  for (k = 0; k < 3; k++) {
    absxk = fabs(x[k]);
    if (absxk > scale) {
      t = scale / absxk;
      y = 1.0 + y * t * t;
      scale = absxk;
    } else {
      t = absxk / scale;
      y += t * t;
    }
  }

  return scale * sqrt(y);
}

static real_T rt_hypotd_snf(real_T u0, real_T u1)
{
  real_T y;
  real_T a;
  a = fabs(u0);
  y = fabs(u1);
  if (a < y) {
    a /= y;
    y *= sqrt(a * a + 1.0);
  } else if (a > y) {
    y /= a;
    y = a * sqrt(y * y + 1.0);
  } else if (rtIsNaN(y)) {
  } else {
    y = a * 1.4142135623730951;
  }

  return y;
}

static real_T rt_powd_snf(real_T u0, real_T u1)
{
  real_T y;
  real_T d0;
  real_T d1;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = rtNaN;
  } else {
    d0 = fabs(u0);
    d1 = fabs(u1);
    if (rtIsInf(u1)) {
      if (d0 == 1.0) {
        y = rtNaN;
      } else if (d0 > 1.0) {
        if (u1 > 0.0) {
          y = rtInf;
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = rtInf;
      }
    } else if (d1 == 0.0) {
      y = 1.0;
    } else if (d1 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > floor(u1))) {
      y = rtNaN;
    } else {
      y = pow(u0, u1);
    }
  }

  return y;
}

static real_T rt_roundd_snf(real_T u)
{
  real_T y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

void WP_grid(const emxArray_real_T *vertex, const real_T initial_position[2],
             real_T d, real_T WP[200], boolean_T *success, real_T *number_WP)
{
  real_T temp_WP[4];
  int32_T i4;
  emxArray_real_T *vertex_out;
  real_T index_temp_WP;
  boolean_T break_flag;
  int32_T vlen;
  emxArray_int32_T *r12;
  int32_T index_WP;
  emxArray_real_T *b_vertex;
  emxArray_real_T *x1;
  emxArray_int32_T *r13;
  emxArray_real_T *c_vertex;
  emxArray_real_T *d_vertex;
  emxArray_real_T *x2;
  emxArray_int32_T *r14;
  emxArray_real_T *e_vertex;
  emxArray_real_T *b_y1;
  emxArray_int32_T *r15;
  emxArray_real_T *f_vertex;
  emxArray_real_T *g_vertex;
  emxArray_real_T *y2;
  emxArray_real_T *zcrossproduct;
  emxArray_boolean_T *x;
  real_T t1;
  real_T t2;
  boolean_T positive_intersection;
  emxArray_real_T *b_vertex_out;
  emxArray_real_T *b_zcrossproduct;
  real_T closest_point[2];
  emxArray_real_T *extended_vertex;
  real_T second_edge[2];
  real_T q_inc;
  boolean_T exitg3;
  real_T b_extended_vertex[2];
  uint8_T is_intersecting;
  boolean_T collision;
  boolean_T exitg1;
  boolean_T exitg2;
  boolean_T guard1 = FALSE;

  /*  Vertex are the edges of the polygone. The given polygone can be */
  /*  non-convex but it needs to be non-complex. Vector Nx2 where N is the */
  /*  number of vertexes. */
  /*  */
  /*  initial_position --> position of UAV [x y]. Vector 1x2 */
  /*  d --> scalar distance between parallel lines */
  memset(&WP[0], 0, 200U * sizeof(real_T));

  /* to initialize */
  /* %coder.varsize('WP', [MAX_WP 2], [1 0]);     %%max dimension MAX_WP x 2 */
  for (i4 = 0; i4 < 4; i4++) {
    temp_WP[i4] = 0.0;
  }

  emxInit_real_T(&vertex_out, 2);
  index_temp_WP = 1.0;
  *success = FALSE;
  break_flag = FALSE;
  *number_WP = 0.0;

  /* vertex = [ x1 y1 */
  /*            x2 y2 */
  /*            .. .. */
  /*            xn yn]; */
  /* %% TODO check if complex or not  %%% we suppose that points are given */
  /* %% not complex --> clockwise or counter-clockwise */
  i4 = vertex_out->size[0] * vertex_out->size[1];
  vertex_out->size[0] = vertex->size[0];
  vertex_out->size[1] = 2;
  emxEnsureCapacity((emxArray__common *)vertex_out, i4, (int32_T)sizeof(real_T));
  vlen = vertex->size[0] * vertex->size[1] - 1;
  for (i4 = 0; i4 <= vlen; i4++) {
    vertex_out->data[i4] = vertex->data[i4];
  }

  if (vertex->size[0] < 3) {
    /* %not a polygon */
  } else {
    emxInit_int32_T(&r12, 1);
    i4 = vertex->size[0];
    index_WP = r12->size[0];
    r12->size[0] = i4;
    emxEnsureCapacity((emxArray__common *)r12, index_WP, (int32_T)sizeof(int32_T));
    vlen = i4 - 1;
    for (i4 = 0; i4 <= vlen; i4++) {
      r12->data[i4] = 1 + i4;
    }

    b_emxInit_real_T(&b_vertex, 1);
    i4 = r12->size[0];
    index_WP = vertex->size[0];
    vlen = b_vertex->size[0];
    b_vertex->size[0] = index_WP;
    emxEnsureCapacity((emxArray__common *)b_vertex, vlen, (int32_T)sizeof(real_T));
    emxFree_int32_T(&r12);
    vlen = index_WP - 1;
    for (index_WP = 0; index_WP <= vlen; index_WP++) {
      b_vertex->data[index_WP] = vertex->data[index_WP];
    }

    b_emxInit_real_T(&x1, 1);
    index_WP = x1->size[0];
    x1->size[0] = i4;
    emxEnsureCapacity((emxArray__common *)x1, index_WP, (int32_T)sizeof(real_T));
    vlen = i4 - 2;
    for (index_WP = 0; index_WP <= vlen; index_WP++) {
      x1->data[index_WP] = b_vertex->data[1 + index_WP];
    }

    emxFree_real_T(&b_vertex);
    emxInit_int32_T(&r13, 1);
    x1->data[i4 - 1] = vertex->data[0];
    i4 = vertex->size[0];
    index_WP = r13->size[0];
    r13->size[0] = i4;
    emxEnsureCapacity((emxArray__common *)r13, index_WP, (int32_T)sizeof(int32_T));
    vlen = i4 - 1;
    for (i4 = 0; i4 <= vlen; i4++) {
      r13->data[i4] = 1 + i4;
    }

    b_emxInit_real_T(&c_vertex, 1);
    i4 = r13->size[0];
    index_WP = vertex->size[0];
    vlen = c_vertex->size[0];
    c_vertex->size[0] = index_WP;
    emxEnsureCapacity((emxArray__common *)c_vertex, vlen, (int32_T)sizeof(real_T));
    emxFree_int32_T(&r13);
    vlen = index_WP - 1;
    for (index_WP = 0; index_WP <= vlen; index_WP++) {
      c_vertex->data[index_WP] = vertex->data[index_WP];
    }

    b_emxInit_real_T(&d_vertex, 1);
    index_WP = vertex->size[0];
    vlen = d_vertex->size[0];
    d_vertex->size[0] = index_WP;
    emxEnsureCapacity((emxArray__common *)d_vertex, vlen, (int32_T)sizeof(real_T));
    vlen = index_WP - 1;
    for (index_WP = 0; index_WP <= vlen; index_WP++) {
      d_vertex->data[index_WP] = vertex->data[index_WP];
    }

    b_emxInit_real_T(&x2, 1);
    index_WP = x2->size[0];
    x2->size[0] = i4;
    emxEnsureCapacity((emxArray__common *)x2, index_WP, (int32_T)sizeof(real_T));
    vlen = i4 - 3;
    for (index_WP = 0; index_WP <= vlen; index_WP++) {
      x2->data[index_WP] = c_vertex->data[2 + index_WP];
    }

    emxFree_real_T(&c_vertex);
    for (index_WP = 0; index_WP < 2; index_WP++) {
      x2->data[(index_WP + i4) - 2] = d_vertex->data[index_WP];
    }

    emxFree_real_T(&d_vertex);
    emxInit_int32_T(&r14, 1);
    i4 = vertex->size[0];
    index_WP = r14->size[0];
    r14->size[0] = i4;
    emxEnsureCapacity((emxArray__common *)r14, index_WP, (int32_T)sizeof(int32_T));
    vlen = i4 - 1;
    for (i4 = 0; i4 <= vlen; i4++) {
      r14->data[i4] = 1 + i4;
    }

    b_emxInit_real_T(&e_vertex, 1);
    i4 = r14->size[0];
    index_WP = vertex->size[0];
    vlen = e_vertex->size[0];
    e_vertex->size[0] = index_WP;
    emxEnsureCapacity((emxArray__common *)e_vertex, vlen, (int32_T)sizeof(real_T));
    emxFree_int32_T(&r14);
    vlen = index_WP - 1;
    for (index_WP = 0; index_WP <= vlen; index_WP++) {
      e_vertex->data[index_WP] = vertex->data[index_WP + vertex->size[0]];
    }

    b_emxInit_real_T(&b_y1, 1);
    index_WP = b_y1->size[0];
    b_y1->size[0] = i4;
    emxEnsureCapacity((emxArray__common *)b_y1, index_WP, (int32_T)sizeof(real_T));
    vlen = i4 - 2;
    for (index_WP = 0; index_WP <= vlen; index_WP++) {
      b_y1->data[index_WP] = e_vertex->data[1 + index_WP];
    }

    emxFree_real_T(&e_vertex);
    emxInit_int32_T(&r15, 1);
    b_y1->data[i4 - 1] = vertex->data[vertex->size[0]];
    i4 = vertex->size[0];
    index_WP = r15->size[0];
    r15->size[0] = i4;
    emxEnsureCapacity((emxArray__common *)r15, index_WP, (int32_T)sizeof(int32_T));
    vlen = i4 - 1;
    for (i4 = 0; i4 <= vlen; i4++) {
      r15->data[i4] = 1 + i4;
    }

    b_emxInit_real_T(&f_vertex, 1);
    i4 = r15->size[0];
    index_WP = vertex->size[0];
    vlen = f_vertex->size[0];
    f_vertex->size[0] = index_WP;
    emxEnsureCapacity((emxArray__common *)f_vertex, vlen, (int32_T)sizeof(real_T));
    emxFree_int32_T(&r15);
    vlen = index_WP - 1;
    for (index_WP = 0; index_WP <= vlen; index_WP++) {
      f_vertex->data[index_WP] = vertex->data[index_WP + vertex->size[0]];
    }

    b_emxInit_real_T(&g_vertex, 1);
    index_WP = vertex->size[0];
    vlen = g_vertex->size[0];
    g_vertex->size[0] = index_WP;
    emxEnsureCapacity((emxArray__common *)g_vertex, vlen, (int32_T)sizeof(real_T));
    vlen = index_WP - 1;
    for (index_WP = 0; index_WP <= vlen; index_WP++) {
      g_vertex->data[index_WP] = vertex->data[index_WP + vertex->size[0]];
    }

    b_emxInit_real_T(&y2, 1);
    index_WP = y2->size[0];
    y2->size[0] = i4;
    emxEnsureCapacity((emxArray__common *)y2, index_WP, (int32_T)sizeof(real_T));
    vlen = i4 - 3;
    for (index_WP = 0; index_WP <= vlen; index_WP++) {
      y2->data[index_WP] = f_vertex->data[2 + index_WP];
    }

    emxFree_real_T(&f_vertex);
    for (index_WP = 0; index_WP < 2; index_WP++) {
      y2->data[(index_WP + i4) - 2] = g_vertex->data[index_WP];
    }

    emxFree_real_T(&g_vertex);
    b_emxInit_real_T(&zcrossproduct, 1);

    /*  --> see http://stackoverflow.com/questions/471962/how-do-determine-if-a-polygon-is-complex-convex-nonconvex */
    i4 = zcrossproduct->size[0];
    zcrossproduct->size[0] = x2->size[0];
    emxEnsureCapacity((emxArray__common *)zcrossproduct, i4, (int32_T)sizeof
                      (real_T));
    vlen = x2->size[0] - 1;
    for (i4 = 0; i4 <= vlen; i4++) {
      zcrossproduct->data[i4] = (x2->data[i4] - x1->data[i4]) * (vertex->data[i4
        + vertex->size[0]] - b_y1->data[i4]) - (y2->data[i4] - b_y1->data[i4]) *
        (vertex->data[i4] - x1->data[i4]);
    }

    emxFree_real_T(&y2);
    emxFree_real_T(&b_y1);
    emxFree_real_T(&x2);
    emxFree_real_T(&x1);
    emxInit_boolean_T(&x, 1);
    i4 = x->size[0];
    x->size[0] = zcrossproduct->size[0];
    emxEnsureCapacity((emxArray__common *)x, i4, (int32_T)sizeof(boolean_T));
    vlen = zcrossproduct->size[0] - 1;
    for (i4 = 0; i4 <= vlen; i4++) {
      x->data[i4] = (zcrossproduct->data[i4] >= 0.0);
    }

    vlen = x->size[0];
    t1 = (real_T)x->data[0];
    for (index_WP = 2; index_WP <= vlen; index_WP++) {
      t1 += (real_T)x->data[index_WP - 1];
    }

    i4 = x->size[0];
    x->size[0] = zcrossproduct->size[0];
    emxEnsureCapacity((emxArray__common *)x, i4, (int32_T)sizeof(boolean_T));
    vlen = zcrossproduct->size[0] - 1;
    for (i4 = 0; i4 <= vlen; i4++) {
      x->data[i4] = (zcrossproduct->data[i4] <= 0.0);
    }

    vlen = x->size[0];
    t2 = (real_T)x->data[0];
    for (index_WP = 2; index_WP <= vlen; index_WP++) {
      t2 += (real_T)x->data[index_WP - 1];
    }

    emxFree_boolean_T(&x);
    if ((t1 == (real_T)vertex->size[0]) || (t2 == (real_T)vertex->size[0])) {
      positive_intersection = TRUE;
    } else {
      positive_intersection = FALSE;
    }

    if (positive_intersection) {
    } else {
      /*  not convex and need to convexify recursively */
      emxInit_real_T(&b_vertex_out, 2);
      b_emxInit_real_T(&b_zcrossproduct, 1);
      while (!positive_intersection) {
        i4 = b_vertex_out->size[0] * b_vertex_out->size[1];
        b_vertex_out->size[0] = vertex_out->size[0];
        b_vertex_out->size[1] = 2;
        emxEnsureCapacity((emxArray__common *)b_vertex_out, i4, (int32_T)sizeof
                          (real_T));
        vlen = vertex_out->size[0] * vertex_out->size[1] - 1;
        for (i4 = 0; i4 <= vlen; i4++) {
          b_vertex_out->data[i4] = vertex_out->data[i4];
        }

        i4 = b_zcrossproduct->size[0];
        b_zcrossproduct->size[0] = zcrossproduct->size[0];
        emxEnsureCapacity((emxArray__common *)b_zcrossproduct, i4, (int32_T)
                          sizeof(real_T));
        vlen = zcrossproduct->size[0] - 1;
        for (i4 = 0; i4 <= vlen; i4++) {
          b_zcrossproduct->data[i4] = zcrossproduct->data[i4];
        }

        convexify_polygon(b_vertex_out, b_zcrossproduct, &positive_intersection,
                          vertex_out, zcrossproduct);

        /* %recursive */
      }

      emxFree_real_T(&b_zcrossproduct);
      emxFree_real_T(&b_vertex_out);

      /*  here it should have find the convex polygon */
    }

    emxFree_real_T(&zcrossproduct);
  }

  /*      if (convex)                                     */
  /*          convex_vertex = vertex; */
  /*          return;                                    % END: found convexity */
  /*      else if (convexify)                            % not convex and need to convexify recursively */
  /*              sum_clock = 0; */
  /*              index_removed = 0; */
  /*              vertex_extended = [vertex ; [vertex(1,1),vertex(1,2)]]; */
  /*              for i=1:N */
  /*                  sum_clock = sum_clock + ((vertex_extended(i,1)-vertex_extended(i+1,1))*(vertex_extended(i,2)+vertex_extended(i+1,2))); */
  /*              end */
  /*              clockwise = sum_clock <= 0;           % see --> http://stackoverflow.com/questions/1165647/how-to-determine-if-a-list-of-polygon-points-are-in-clockwise-order */
  /*              for i=1:N */
  /*                  if ( (clockwise && zcrossproduct(i)>0)   ||  (~clockwise && zcrossproduct(i)<0) ) */
  /*                      convex_vertex(i+1-index_removed,:) = []; */
  /*                      index_removed = index_removed+1; */
  /*                  end */
  /*              end */
  /*              [convex_not_used,convex_vertex] = is_convex(convex_vertex,true);     %%recursive */
  /*          else */
  /*              return             % END: not convex and no need to convexify */
  /*          end */
  /*      end */
  /* %finding if convex and convexifying inf not */
  find_closest_2D(vertex_out, initial_position, closest_point, &t1);
  for (i4 = 0; i4 < 2; i4++) {
    WP[100 * i4] = closest_point[i4];
  }

  emxInit_real_T(&extended_vertex, 2);
  i4 = extended_vertex->size[0] * extended_vertex->size[1];
  extended_vertex->size[0] = vertex_out->size[0] + 1;
  extended_vertex->size[1] = 2;
  emxEnsureCapacity((emxArray__common *)extended_vertex, i4, (int32_T)sizeof
                    (real_T));
  for (i4 = 0; i4 < 2; i4++) {
    vlen = vertex_out->size[0] - 1;
    for (index_WP = 0; index_WP <= vlen; index_WP++) {
      extended_vertex->data[index_WP + extended_vertex->size[0] * i4] =
        vertex_out->data[index_WP + vertex_out->size[0] * i4];
    }
  }

  for (i4 = 0; i4 < 2; i4++) {
    extended_vertex->data[vertex_out->size[0] + extended_vertex->size[0] * i4] =
      vertex_out->data[vertex_out->size[0] * i4];
  }

  /*  circular so that last vertex is the first */
  if (t1 == (real_T)vertex_out->size[0]) {
    for (i4 = 0; i4 < 2; i4++) {
      second_edge[i4] = vertex_out->data[vertex_out->size[0] * i4];
    }
  } else {
    for (i4 = 0; i4 < 2; i4++) {
      second_edge[i4] = vertex_out->data[((int32_T)(t1 + 1.0) + vertex_out->
        size[0] * i4) - 1];
    }

    /* second edge is used to build the parallel lines to the line passing for closest_point and second_edge */
  }

  for (i4 = 0; i4 < 2; i4++) {
    WP[1 + 100 * i4] = second_edge[i4];
  }

  index_WP = 1;

  /* ---------------------- JUST DEBUG PLOT TO REMOVE */
  /*  figure; */
  /*  hold on; */
  /*  N = size(new_vertex,1); */
  /*  for i=1:N */
  /*      if (i~=N) */
  /*          line([new_vertex(i,1),new_vertex(i+1,1)],[new_vertex(i,2),new_vertex(i+1,2)]); */
  /*      else */
  /*          line([new_vertex(i,1),new_vertex(1,1)],[new_vertex(i,2),new_vertex(1,2)]); */
  /*      end */
  /*  end */
  /*  axis equal */
  /*  plot(closest_point(1),closest_point(2),'bx'); */
  /*  plot(initial_position(1),initial_position(2),'gx'); */
  /* ---------------------------- */
  if (closest_point[0] != second_edge[0]) {
    t1 = (closest_point[1] - second_edge[1]) / (closest_point[0] - second_edge[0]);

    /* line slope */
    t2 = closest_point[1] - t1 * closest_point[0];

    /* line coefficient */
    q_inc = d / sin(1.5707963267948966 - atan(t1));

    /* increments of q for the lines bundle to reflect the distance d */
  } else {
    t1 = rtInf;
    t2 = closest_point[0];
    q_inc = d;
  }

  positive_intersection = FALSE;
  vlen = 0;
  exitg3 = FALSE;
  while ((exitg3 == 0U) && (vlen <= vertex_out->size[0] - 1)) {
    for (i4 = 0; i4 < 2; i4++) {
      second_edge[i4] = extended_vertex->data[vlen + extended_vertex->size[0] *
        i4];
    }

    for (i4 = 0; i4 < 2; i4++) {
      b_extended_vertex[i4] = extended_vertex->data[(vlen +
        extended_vertex->size[0] * i4) + 1];
    }

    intersection_line_segment(second_edge, b_extended_vertex, t1, t2 + q_inc,
      &is_intersecting, closest_point);
    if (is_intersecting != 0) {
      positive_intersection = TRUE;

      /*  the polygon is at the "right" of the first line, left otherwise */
      exitg3 = TRUE;
    } else {
      vlen++;
    }
  }

  collision = TRUE;
  exitg1 = FALSE;
  while ((exitg1 == 0U) && collision) {
    collision = FALSE;
    if (positive_intersection) {
      /* USE POSITIVE INCREMENTS */
      t2 += q_inc;

      /* display('Found Positive inc') */
    } else {
      /* %USE NEGATIVE INCREMENTS */
      t2 -= q_inc;

      /* display('Negative inc') */
    }

    /*      x = -4:0.1:4; */
    /*      y = m*x+q; */
    /*      plot(x,y,'r-'); */
    vlen = 0;
    exitg2 = FALSE;
    while ((exitg2 == 0U) && (vlen <= vertex_out->size[0] - 1)) {
      for (i4 = 0; i4 < 2; i4++) {
        second_edge[i4] = extended_vertex->data[vlen + extended_vertex->size[0] *
          i4];
      }

      for (i4 = 0; i4 < 2; i4++) {
        b_extended_vertex[i4] = extended_vertex->data[(vlen +
          extended_vertex->size[0] * i4) + 1];
      }

      intersection_line_segment(second_edge, b_extended_vertex, t1, t2,
        &is_intersecting, closest_point);
      guard1 = FALSE;
      if (is_intersecting == 1) {
        /* one intersection */
        if (index_WP + 2 > 100) {
          break_flag = TRUE;
          exitg2 = TRUE;
        } else {
          collision = TRUE;

          /* display('Found intersection') */
          for (i4 = 0; i4 < 2; i4++) {
            temp_WP[((int32_T)index_temp_WP + (i4 << 1)) - 1] = closest_point[i4];
          }

          index_temp_WP++;
          guard1 = TRUE;
        }
      } else if (is_intersecting == 2) {
        /* collinear */
        if (index_WP + 2 > 100) {
          break_flag = TRUE;
          exitg2 = TRUE;
        } else {
          collision = TRUE;

          /* display('Found collinear') */
          for (i4 = 0; i4 < 2; i4++) {
            temp_WP[((int32_T)index_temp_WP + (i4 << 1)) - 1] =
              extended_vertex->data[vlen + extended_vertex->size[0] * i4];
          }

          /* if collinear the 2 waypoints are given by the edges of the segment */
          for (i4 = 0; i4 < 2; i4++) {
            temp_WP[1 + (i4 << 1)] = extended_vertex->data[(vlen +
              extended_vertex->size[0] * i4) + 1];
          }

          index_temp_WP = 3.0;
          guard1 = TRUE;
        }
      } else {
        /* display('No intersections this segment') */
        guard1 = TRUE;
      }

      if (guard1 == TRUE) {
        if (index_temp_WP == 3.0) {
          /* found 2 WP. For convex polygon it is the maximum */
          index_temp_WP = 1.0;
          vlen = 1;

          /* to initialize */
          /* to initialize */
          for (i4 = 0; i4 < 2; i4++) {
            closest_point[i4] = temp_WP[i4 << 1];
          }

          /* to initialize */
          /* starting from second point in the list */
          if (sqrt(rt_powd_snf(temp_WP[1] - WP[index_WP], 2.0) + rt_powd_snf
                   (temp_WP[3] - WP[100 + index_WP], 2.0)) < sqrt(rt_powd_snf
               (temp_WP[0] - WP[index_WP], 2.0) + rt_powd_snf(temp_WP[2] - WP
                [100 + index_WP], 2.0))) {
            vlen = 2;
            for (i4 = 0; i4 < 2; i4++) {
              closest_point[i4] = temp_WP[1 + (i4 << 1)];
            }
          }

          /* the closest temp_WP to the last WP is the next going into WP */
          for (i4 = 0; i4 < 2; i4++) {
            WP[(index_WP + 100 * i4) + 1] = closest_point[i4];
          }

          index_WP += 2;
          if (vlen == 1) {
            for (i4 = 0; i4 < 2; i4++) {
              WP[index_WP + 100 * i4] = temp_WP[1 + (i4 << 1)];
            }
          } else {
            for (i4 = 0; i4 < 2; i4++) {
              WP[index_WP + 100 * i4] = temp_WP[i4 << 1];
            }
          }

          exitg2 = TRUE;
        } else {
          vlen++;
        }
      }
    }

    if ((!collision) && (!break_flag)) {
      *success = TRUE;
      *number_WP = (real_T)(index_WP + 2) - 1.0;
      exitg1 = TRUE;
    } else {
      if (break_flag) {
        *number_WP = 100.0;
        exitg1 = TRUE;
      }
    }
  }

  emxFree_real_T(&vertex_out);
  emxFree_real_T(&extended_vertex);

  /* ----------------REMOVE ABOVE, JUST PLOTS */
  /*  for i=1:size(WP,1) */
  /*      plot(WP(i,1),WP(i,2),'rx'); */
  /*      if (i~=size(WP,1)) */
  /*          h = line([WP(i,1) WP(i+1,1)],[WP(i,2) WP(i+1,2)]); */
  /*          set(h,'Color',[0 1 0]);   */
  /*      end */
  /*  end */
  /*   */
  /*   */
  /*  figure; */
  /*  hold on; */
  /*  N = size(vertex,1); */
  /*  for i=1:N */
  /*      if (i~=N) */
  /*          line([vertex(i,1),vertex(i+1,1)],[vertex(i,2),vertex(i+1,2)]); */
  /*      else */
  /*          line([vertex(i,1),vertex(1,1)],[vertex(i,2),vertex(1,2)]); */
  /*      end */
  /*  end */
  /*  axis equal */
}

void WP_grid_initialize(void)
{
  rt_InitInfAndNaN(8U);
}

void WP_grid_terminate(void)
{
  /* (no terminate code required) */
}

void convexify_polygon(const emxArray_real_T *vertex, const emxArray_real_T
  *zcrossproduct, boolean_T *convex, emxArray_real_T *convex_vertex,
  emxArray_real_T *zcrossproduct_out)
{
  int32_T i0;
  int32_T vlen;
  emxArray_real_T *vertex_extended;
  real_T sum_clock;
  real_T index_removed;
  int32_T i1;
  int32_T i;
  real_T t2;
  real_T b;
  boolean_T clockwise;
  emxArray_real_T *b_convex_vertex;
  emxArray_real_T *c_convex_vertex;
  int32_T nrows;
  int32_T k;
  emxArray_int32_T *r0;
  emxArray_int32_T *r1;
  emxArray_real_T *d_convex_vertex;
  emxArray_real_T *x1;
  emxArray_int32_T *r2;
  emxArray_int32_T *r3;
  emxArray_real_T *e_convex_vertex;
  emxArray_real_T *f_convex_vertex;
  emxArray_real_T *x2;
  emxArray_int32_T *r4;
  emxArray_int32_T *r5;
  emxArray_real_T *g_convex_vertex;
  emxArray_real_T *b_y1;
  emxArray_int32_T *r6;
  emxArray_int32_T *r7;
  emxArray_real_T *h_convex_vertex;
  emxArray_real_T *i_convex_vertex;
  emxArray_real_T *y2;
  emxArray_boolean_T *x;

  /* vertex = [ x1 y1 */
  /*            x2 y2 */
  /*            .. .. */
  /*            xn yn]; */
  /* %% TODO check if complex or not  %%% we suppose that points are given */
  /* %% not complex --> clockwise or counter-clockwise */
  /* zcrossproduct in output should be given of the convexified polygon, */
  /* while in the input are the ones of the input polygon */
  i0 = convex_vertex->size[0] * convex_vertex->size[1];
  convex_vertex->size[0] = vertex->size[0];
  convex_vertex->size[1] = 2;
  emxEnsureCapacity((emxArray__common *)convex_vertex, i0, (int32_T)sizeof
                    (real_T));
  vlen = vertex->size[0] * vertex->size[1] - 1;
  for (i0 = 0; i0 <= vlen; i0++) {
    convex_vertex->data[i0] = vertex->data[i0];
  }

  /* initialize */
  i0 = zcrossproduct_out->size[0];
  zcrossproduct_out->size[0] = zcrossproduct->size[0];
  emxEnsureCapacity((emxArray__common *)zcrossproduct_out, i0, (int32_T)sizeof
                    (real_T));
  vlen = zcrossproduct->size[0] - 1;
  for (i0 = 0; i0 <= vlen; i0++) {
    zcrossproduct_out->data[i0] = zcrossproduct->data[i0];
  }

  /* initialize */
  if (vertex->size[0] < 3) {
    /* %not a polygon */
    *convex = FALSE;
  } else {
    emxInit_real_T(&vertex_extended, 2);
    sum_clock = 0.0;
    index_removed = 0.0;
    i0 = vertex_extended->size[0] * vertex_extended->size[1];
    vertex_extended->size[0] = vertex->size[0] + 1;
    vertex_extended->size[1] = 2;
    emxEnsureCapacity((emxArray__common *)vertex_extended, i0, (int32_T)sizeof
                      (real_T));
    for (i0 = 0; i0 < 2; i0++) {
      vlen = vertex->size[0] - 1;
      for (i1 = 0; i1 <= vlen; i1++) {
        vertex_extended->data[i1 + vertex_extended->size[0] * i0] = vertex->
          data[i1 + vertex->size[0] * i0];
      }
    }

    vertex_extended->data[vertex->size[0]] = vertex->data[0];
    vertex_extended->data[vertex->size[0] + vertex_extended->size[0]] =
      vertex->data[vertex->size[0]];
    for (i = 0; i <= vertex->size[0] - 1; i++) {
      t2 = vertex_extended->data[i] - vertex_extended->data[(int32_T)(uint32_T)
        (i + 1)];
      b = vertex_extended->data[i + vertex_extended->size[0]] +
        vertex_extended->data[(i + vertex_extended->size[0]) + 1];
      sum_clock += t2 * b;
    }

    emxFree_real_T(&vertex_extended);
    clockwise = (sum_clock <= 0.0);

    /*  see --> http://stackoverflow.com/questions/1165647/how-to-determine-if-a-list-of-polygon-points-are-in-clockwise-order */
    i = 0;
    emxInit_real_T(&b_convex_vertex, 2);
    emxInit_real_T(&c_convex_vertex, 2);
    while (i <= vertex->size[0] - 1) {
      if ((clockwise && (zcrossproduct->data[i] > 0.0)) || ((!clockwise) &&
           (zcrossproduct->data[i] < 0.0))) {
        if (i + 1 == vertex->size[0]) {
          nrows = convex_vertex->size[0] - 1;
          for (vlen = 0; vlen < 2; vlen++) {
            for (k = 1; k <= nrows; k++) {
              convex_vertex->data[(k + convex_vertex->size[0] * vlen) - 1] =
                convex_vertex->data[k + convex_vertex->size[0] * vlen];
            }
          }

          if (1 > nrows) {
            nrows = 0;
          }

          i0 = c_convex_vertex->size[0] * c_convex_vertex->size[1];
          c_convex_vertex->size[0] = nrows;
          c_convex_vertex->size[1] = 2;
          emxEnsureCapacity((emxArray__common *)c_convex_vertex, i0, (int32_T)
                            sizeof(real_T));
          for (i0 = 0; i0 < 2; i0++) {
            vlen = nrows - 1;
            for (i1 = 0; i1 <= vlen; i1++) {
              c_convex_vertex->data[i1 + c_convex_vertex->size[0] * i0] =
                convex_vertex->data[i1 + convex_vertex->size[0] * i0];
            }
          }

          i0 = convex_vertex->size[0] * convex_vertex->size[1];
          convex_vertex->size[0] = c_convex_vertex->size[0];
          convex_vertex->size[1] = 2;
          emxEnsureCapacity((emxArray__common *)convex_vertex, i0, (int32_T)
                            sizeof(real_T));
          for (i0 = 0; i0 < 2; i0++) {
            vlen = c_convex_vertex->size[0] - 1;
            for (i1 = 0; i1 <= vlen; i1++) {
              convex_vertex->data[i1 + convex_vertex->size[0] * i0] =
                c_convex_vertex->data[i1 + c_convex_vertex->size[0] * i0];
            }
          }

          index_removed++;
        } else {
          sum_clock = ((1.0 + (real_T)i) + 1.0) - index_removed;
          nrows = convex_vertex->size[0] - 1;
          for (vlen = 0; vlen < 2; vlen++) {
            t2 = rt_roundd_snf(sum_clock);
            if (t2 < 2.147483648E+9) {
              if (t2 >= -2.147483648E+9) {
                k = (int32_T)t2;
              } else {
                k = MIN_int32_T;
              }
            } else {
              k = MAX_int32_T;
            }

            while (k <= nrows) {
              convex_vertex->data[(k + convex_vertex->size[0] * vlen) - 1] =
                convex_vertex->data[k + convex_vertex->size[0] * vlen];
              k++;
            }
          }

          if (1 > nrows) {
            nrows = 0;
          }

          i0 = b_convex_vertex->size[0] * b_convex_vertex->size[1];
          b_convex_vertex->size[0] = nrows;
          b_convex_vertex->size[1] = 2;
          emxEnsureCapacity((emxArray__common *)b_convex_vertex, i0, (int32_T)
                            sizeof(real_T));
          for (i0 = 0; i0 < 2; i0++) {
            vlen = nrows - 1;
            for (i1 = 0; i1 <= vlen; i1++) {
              b_convex_vertex->data[i1 + b_convex_vertex->size[0] * i0] =
                convex_vertex->data[i1 + convex_vertex->size[0] * i0];
            }
          }

          i0 = convex_vertex->size[0] * convex_vertex->size[1];
          convex_vertex->size[0] = b_convex_vertex->size[0];
          convex_vertex->size[1] = 2;
          emxEnsureCapacity((emxArray__common *)convex_vertex, i0, (int32_T)
                            sizeof(real_T));
          for (i0 = 0; i0 < 2; i0++) {
            vlen = b_convex_vertex->size[0] - 1;
            for (i1 = 0; i1 <= vlen; i1++) {
              convex_vertex->data[i1 + convex_vertex->size[0] * i0] =
                b_convex_vertex->data[i1 + b_convex_vertex->size[0] * i0];
            }
          }

          index_removed++;
        }
      }

      i++;
    }

    emxFree_real_T(&c_convex_vertex);
    emxFree_real_T(&b_convex_vertex);
    emxInit_int32_T(&r0, 1);

    /* chef if now is convex */
    i0 = convex_vertex->size[0];
    i1 = r0->size[0];
    r0->size[0] = i0;
    emxEnsureCapacity((emxArray__common *)r0, i1, (int32_T)sizeof(int32_T));
    vlen = i0 - 1;
    for (i0 = 0; i0 <= vlen; i0++) {
      r0->data[i0] = 1 + i0;
    }

    if (2 > r0->size[0]) {
      i0 = 0;
      i1 = 0;
    } else {
      emxInit_int32_T(&r1, 1);
      i0 = 1;
      i1 = convex_vertex->size[0];
      k = r1->size[0];
      r1->size[0] = i1;
      emxEnsureCapacity((emxArray__common *)r1, k, (int32_T)sizeof(int32_T));
      vlen = i1 - 1;
      for (i1 = 0; i1 <= vlen; i1++) {
        r1->data[i1] = 1 + i1;
      }

      i1 = r1->size[0];
      emxFree_int32_T(&r1);
    }

    emxFree_int32_T(&r0);
    b_emxInit_real_T(&d_convex_vertex, 1);
    k = convex_vertex->size[0];
    vlen = d_convex_vertex->size[0];
    d_convex_vertex->size[0] = k;
    emxEnsureCapacity((emxArray__common *)d_convex_vertex, vlen, (int32_T)sizeof
                      (real_T));
    vlen = k - 1;
    for (k = 0; k <= vlen; k++) {
      d_convex_vertex->data[k] = convex_vertex->data[k];
    }

    b_emxInit_real_T(&x1, 1);
    k = x1->size[0];
    x1->size[0] = (i1 - i0) + 1;
    emxEnsureCapacity((emxArray__common *)x1, k, (int32_T)sizeof(real_T));
    vlen = (i1 - i0) - 1;
    for (k = 0; k <= vlen; k++) {
      x1->data[k] = d_convex_vertex->data[i0 + k];
    }

    emxFree_real_T(&d_convex_vertex);
    emxInit_int32_T(&r2, 1);
    x1->data[i1 - i0] = convex_vertex->data[0];
    i0 = convex_vertex->size[0];
    i1 = r2->size[0];
    r2->size[0] = i0;
    emxEnsureCapacity((emxArray__common *)r2, i1, (int32_T)sizeof(int32_T));
    vlen = i0 - 1;
    for (i0 = 0; i0 <= vlen; i0++) {
      r2->data[i0] = 1 + i0;
    }

    if (3 > r2->size[0]) {
      i0 = 0;
      i1 = 0;
    } else {
      emxInit_int32_T(&r3, 1);
      i0 = 2;
      i1 = convex_vertex->size[0];
      k = r3->size[0];
      r3->size[0] = i1;
      emxEnsureCapacity((emxArray__common *)r3, k, (int32_T)sizeof(int32_T));
      vlen = i1 - 1;
      for (i1 = 0; i1 <= vlen; i1++) {
        r3->data[i1] = 1 + i1;
      }

      i1 = r3->size[0];
      emxFree_int32_T(&r3);
    }

    emxFree_int32_T(&r2);
    b_emxInit_real_T(&e_convex_vertex, 1);
    k = convex_vertex->size[0];
    vlen = e_convex_vertex->size[0];
    e_convex_vertex->size[0] = k;
    emxEnsureCapacity((emxArray__common *)e_convex_vertex, vlen, (int32_T)sizeof
                      (real_T));
    vlen = k - 1;
    for (k = 0; k <= vlen; k++) {
      e_convex_vertex->data[k] = convex_vertex->data[k];
    }

    b_emxInit_real_T(&f_convex_vertex, 1);
    k = convex_vertex->size[0];
    vlen = f_convex_vertex->size[0];
    f_convex_vertex->size[0] = k;
    emxEnsureCapacity((emxArray__common *)f_convex_vertex, vlen, (int32_T)sizeof
                      (real_T));
    vlen = k - 1;
    for (k = 0; k <= vlen; k++) {
      f_convex_vertex->data[k] = convex_vertex->data[k];
    }

    b_emxInit_real_T(&x2, 1);
    k = x2->size[0];
    x2->size[0] = (i1 - i0) + 2;
    emxEnsureCapacity((emxArray__common *)x2, k, (int32_T)sizeof(real_T));
    vlen = (i1 - i0) - 1;
    for (k = 0; k <= vlen; k++) {
      x2->data[k] = e_convex_vertex->data[i0 + k];
    }

    emxFree_real_T(&e_convex_vertex);
    for (k = 0; k < 2; k++) {
      x2->data[(k + i1) - i0] = f_convex_vertex->data[k];
    }

    emxFree_real_T(&f_convex_vertex);
    emxInit_int32_T(&r4, 1);
    i0 = convex_vertex->size[0];
    i1 = r4->size[0];
    r4->size[0] = i0;
    emxEnsureCapacity((emxArray__common *)r4, i1, (int32_T)sizeof(int32_T));
    vlen = i0 - 1;
    for (i0 = 0; i0 <= vlen; i0++) {
      r4->data[i0] = 1 + i0;
    }

    if (2 > r4->size[0]) {
      i0 = 0;
      i1 = 0;
    } else {
      emxInit_int32_T(&r5, 1);
      i0 = 1;
      i1 = convex_vertex->size[0];
      k = r5->size[0];
      r5->size[0] = i1;
      emxEnsureCapacity((emxArray__common *)r5, k, (int32_T)sizeof(int32_T));
      vlen = i1 - 1;
      for (i1 = 0; i1 <= vlen; i1++) {
        r5->data[i1] = 1 + i1;
      }

      i1 = r5->size[0];
      emxFree_int32_T(&r5);
    }

    emxFree_int32_T(&r4);
    b_emxInit_real_T(&g_convex_vertex, 1);
    k = convex_vertex->size[0];
    vlen = g_convex_vertex->size[0];
    g_convex_vertex->size[0] = k;
    emxEnsureCapacity((emxArray__common *)g_convex_vertex, vlen, (int32_T)sizeof
                      (real_T));
    vlen = k - 1;
    for (k = 0; k <= vlen; k++) {
      g_convex_vertex->data[k] = convex_vertex->data[k + convex_vertex->size[0]];
    }

    b_emxInit_real_T(&b_y1, 1);
    k = b_y1->size[0];
    b_y1->size[0] = (i1 - i0) + 1;
    emxEnsureCapacity((emxArray__common *)b_y1, k, (int32_T)sizeof(real_T));
    vlen = (i1 - i0) - 1;
    for (k = 0; k <= vlen; k++) {
      b_y1->data[k] = g_convex_vertex->data[i0 + k];
    }

    emxFree_real_T(&g_convex_vertex);
    emxInit_int32_T(&r6, 1);
    b_y1->data[i1 - i0] = convex_vertex->data[convex_vertex->size[0]];
    i0 = convex_vertex->size[0];
    i1 = r6->size[0];
    r6->size[0] = i0;
    emxEnsureCapacity((emxArray__common *)r6, i1, (int32_T)sizeof(int32_T));
    vlen = i0 - 1;
    for (i0 = 0; i0 <= vlen; i0++) {
      r6->data[i0] = 1 + i0;
    }

    if (3 > r6->size[0]) {
      i0 = 0;
      i1 = 0;
    } else {
      emxInit_int32_T(&r7, 1);
      i0 = 2;
      i1 = convex_vertex->size[0];
      k = r7->size[0];
      r7->size[0] = i1;
      emxEnsureCapacity((emxArray__common *)r7, k, (int32_T)sizeof(int32_T));
      vlen = i1 - 1;
      for (i1 = 0; i1 <= vlen; i1++) {
        r7->data[i1] = 1 + i1;
      }

      i1 = r7->size[0];
      emxFree_int32_T(&r7);
    }

    emxFree_int32_T(&r6);
    b_emxInit_real_T(&h_convex_vertex, 1);
    k = convex_vertex->size[0];
    vlen = h_convex_vertex->size[0];
    h_convex_vertex->size[0] = k;
    emxEnsureCapacity((emxArray__common *)h_convex_vertex, vlen, (int32_T)sizeof
                      (real_T));
    vlen = k - 1;
    for (k = 0; k <= vlen; k++) {
      h_convex_vertex->data[k] = convex_vertex->data[k + convex_vertex->size[0]];
    }

    b_emxInit_real_T(&i_convex_vertex, 1);
    k = convex_vertex->size[0];
    vlen = i_convex_vertex->size[0];
    i_convex_vertex->size[0] = k;
    emxEnsureCapacity((emxArray__common *)i_convex_vertex, vlen, (int32_T)sizeof
                      (real_T));
    vlen = k - 1;
    for (k = 0; k <= vlen; k++) {
      i_convex_vertex->data[k] = convex_vertex->data[k + convex_vertex->size[0]];
    }

    b_emxInit_real_T(&y2, 1);
    k = y2->size[0];
    y2->size[0] = (i1 - i0) + 2;
    emxEnsureCapacity((emxArray__common *)y2, k, (int32_T)sizeof(real_T));
    vlen = (i1 - i0) - 1;
    for (k = 0; k <= vlen; k++) {
      y2->data[k] = h_convex_vertex->data[i0 + k];
    }

    emxFree_real_T(&h_convex_vertex);
    for (k = 0; k < 2; k++) {
      y2->data[(k + i1) - i0] = i_convex_vertex->data[k];
    }

    emxFree_real_T(&i_convex_vertex);

    /*  --> see http://stackoverflow.com/questions/471962/how-do-determine-if-a-polygon-is-complex-convex-nonconvex */
    i0 = zcrossproduct_out->size[0];
    zcrossproduct_out->size[0] = x2->size[0];
    emxEnsureCapacity((emxArray__common *)zcrossproduct_out, i0, (int32_T)sizeof
                      (real_T));
    vlen = x2->size[0] - 1;
    for (i0 = 0; i0 <= vlen; i0++) {
      zcrossproduct_out->data[i0] = (x2->data[i0] - x1->data[i0]) *
        (convex_vertex->data[i0 + convex_vertex->size[0]] - b_y1->data[i0]) -
        (y2->data[i0] - b_y1->data[i0]) * (convex_vertex->data[i0] - x1->data[i0]);
    }

    emxFree_real_T(&y2);
    emxFree_real_T(&b_y1);
    emxFree_real_T(&x2);
    emxFree_real_T(&x1);
    emxInit_boolean_T(&x, 1);
    i0 = x->size[0];
    x->size[0] = zcrossproduct_out->size[0];
    emxEnsureCapacity((emxArray__common *)x, i0, (int32_T)sizeof(boolean_T));
    vlen = zcrossproduct_out->size[0] - 1;
    for (i0 = 0; i0 <= vlen; i0++) {
      x->data[i0] = (zcrossproduct_out->data[i0] >= 0.0);
    }

    vlen = x->size[0];
    sum_clock = (real_T)x->data[0];
    for (k = 2; k <= vlen; k++) {
      sum_clock += (real_T)x->data[k - 1];
    }

    i0 = x->size[0];
    x->size[0] = zcrossproduct_out->size[0];
    emxEnsureCapacity((emxArray__common *)x, i0, (int32_T)sizeof(boolean_T));
    vlen = zcrossproduct_out->size[0] - 1;
    for (i0 = 0; i0 <= vlen; i0++) {
      x->data[i0] = (zcrossproduct_out->data[i0] <= 0.0);
    }

    vlen = x->size[0];
    t2 = (real_T)x->data[0];
    for (k = 2; k <= vlen; k++) {
      t2 += (real_T)x->data[k - 1];
    }

    emxFree_boolean_T(&x);
    if ((sum_clock == (real_T)vertex->size[0]) || (t2 == (real_T)vertex->size[0]))
    {
      clockwise = TRUE;
    } else {
      clockwise = FALSE;
    }

    *convex = clockwise;
  }
}

emxArray_real_T *emxCreateND_real_T(int32_T numDimensions, int32_T *size)
{
  emxArray_real_T *emx;
  int32_T numEl;
  int32_T loop_ub;
  int32_T i;
  c_emxInit_real_T(&emx, numDimensions);
  numEl = 1;
  loop_ub = numDimensions - 1;
  for (i = 0; i <= loop_ub; i++) {
    numEl *= size[i];
    emx->size[i] = size[i];
  }

  emx->data = (real_T *)calloc((uint32_T)numEl, sizeof(real_T));
  emx->numDimensions = numDimensions;
  emx->allocatedSize = numEl;
  return emx;
}

emxArray_real_T *emxCreateWrapperND_real_T(real_T *data, int32_T numDimensions,
  int32_T *size)
{
  emxArray_real_T *emx;
  int32_T numEl;
  int32_T loop_ub;
  int32_T i;
  c_emxInit_real_T(&emx, numDimensions);
  numEl = 1;
  loop_ub = numDimensions - 1;
  for (i = 0; i <= loop_ub; i++) {
    numEl *= size[i];
    emx->size[i] = size[i];
  }

  emx->data = data;
  emx->numDimensions = numDimensions;
  emx->allocatedSize = numEl;
  emx->canFreeData = FALSE;
  return emx;
}

emxArray_real_T *emxCreateWrapper_real_T(real_T *data, int32_T rows, int32_T
  cols)
{
  emxArray_real_T *emx;
  int32_T size[2];
  int32_T numEl;
  int32_T i;
  size[0] = rows;
  size[1] = cols;
  c_emxInit_real_T(&emx, 2);
  numEl = 1;
  for (i = 0; i < 2; i++) {
    numEl *= size[i];
    emx->size[i] = size[i];
  }

  emx->data = data;
  emx->numDimensions = 2;
  emx->allocatedSize = numEl;
  emx->canFreeData = FALSE;
  return emx;
}

emxArray_real_T *emxCreate_real_T(int32_T rows, int32_T cols)
{
  emxArray_real_T *emx;
  int32_T size[2];
  int32_T numEl;
  int32_T i;
  size[0] = rows;
  size[1] = cols;
  c_emxInit_real_T(&emx, 2);
  numEl = 1;
  for (i = 0; i < 2; i++) {
    numEl *= size[i];
    emx->size[i] = size[i];
  }

  emx->data = (real_T *)calloc((uint32_T)numEl, sizeof(real_T));
  emx->numDimensions = 2;
  emx->allocatedSize = numEl;
  return emx;
}

void emxDestroyArray_real_T(emxArray_real_T *emxArray)
{
  emxFree_real_T(&emxArray);
}

void find_closest_2D(const emxArray_real_T *points, const real_T initial_point[2],
                     real_T closest_point[2], real_T *b_index)
{
  real_T distance;
  int32_T i2;
  int32_T i;
  real_T temp_distance;
  real_T a;
  *b_index = 1.0;

  /* to initialize */
  distance = sqrt(rt_powd_snf(points->data[0] - initial_point[0], 2.0) +
                  rt_powd_snf(points->data[points->size[0]] - initial_point[1],
    2.0));

  /* to initialize */
  for (i2 = 0; i2 < 2; i2++) {
    closest_point[i2] = points->data[points->size[0] * i2];
  }

  /* to initialize */
  for (i = 1; i - 1 <= points->size[0] - 2; i++) {
    /* starting from second point in the list */
    temp_distance = points->data[i] - initial_point[0];
    a = points->data[i + points->size[0]] - initial_point[1];
    temp_distance = sqrt(rt_powd_snf(temp_distance, 2.0) + rt_powd_snf(a, 2.0));
    if (temp_distance < distance) {
      distance = temp_distance;
      *b_index = 2.0 + (real_T)(i - 1);
      for (i2 = 0; i2 < 2; i2++) {
        closest_point[i2] = points->data[i + points->size[0] * i2];
      }
    }
  }
}

void intersection_line_segment(const real_T s1[2], const real_T s2[2], real_T m,
  real_T q, uint8_T *is_intersecting, real_T intersection_point[2])
{
  int32_T k;
  real_T p[2];
  real_T r[2];
  real_T xnorm;
  real_T a[3];
  real_T b[3];
  real_T s[2];
  real_T r_cross_s[3];
  real_T q_minus_p_cross_r[3];
  real_T wj;
  real_T tau;
  int32_T knt;

  /*  see --> http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect/565282#565282 */
  /*  is_intersecting = 0 --> no collision */
  /*  is_intersecting = 1 --> 1 point collision */
  /*  is_intersecting = 2 --> collinear */
  /*  */
  /*   CAN BE EXTENDED EASILY TO LINE-LINE or SEGMENT-SEGMENT */
  /* initialization */
  *is_intersecting = 0;
  for (k = 0; k < 2; k++) {
    intersection_point[k] = 0.0;
  }

  if ((m != rtInf) && (m != 0.0)) {
    p[0] = -q / m;
    p[1] = 0.0;
    r[0] = 1.0;
    r[1] = m;
  } else if (m == rtInf) {
    p[0] = q;
    p[1] = 0.0;
    for (k = 0; k < 2; k++) {
      r[k] = (real_T)k;
    }
  } else {
    p[0] = 0.0;
    p[1] = q;
    for (k = 0; k < 2; k++) {
      r[k] = 1.0 - (real_T)k;
    }
  }

  for (k = 0; k < 2; k++) {
    xnorm = s2[k] - s1[k];
    a[k] = r[k];
    b[k] = xnorm;
    s[k] = xnorm;
  }

  a[2] = 0.0;
  b[2] = 0.0;
  r_cross_s[0] = a[1] * 0.0 - 0.0 * b[1];
  r_cross_s[1] = 0.0 * b[0];
  r_cross_s[2] = a[0] * b[1] - a[1] * b[0];

  /* extending to 3D */
  for (k = 0; k < 2; k++) {
    a[k] = s1[k] - p[k];
    b[k] = r[k];
  }

  a[2] = 0.0;
  b[2] = 0.0;
  q_minus_p_cross_r[0] = a[1] * 0.0 - 0.0 * b[1];
  q_minus_p_cross_r[1] = 0.0 - a[0] * 0.0;
  q_minus_p_cross_r[2] = a[0] * b[1] - a[1] * b[0];
  if ((norm(r_cross_s) == 0.0) && (norm(q_minus_p_cross_r) == 0.0)) {
    /* display('Collinear'); */
    *is_intersecting = 2;
  } else if (((norm(r_cross_s) == 0.0) && (norm(q_minus_p_cross_r) != 0.0)) || (
              !(norm(r_cross_s) != 0.0))) {
    /* display('Parallel') */
  } else {
    for (k = 0; k < 3; k++) {
      a[k] = q_minus_p_cross_r[k];
      b[k] = r_cross_s[k];
    }

    wj = r_cross_s[0];
    tau = 0.0;
    xnorm = eml_xnrm2(r_cross_s);
    if (xnorm != 0.0) {
      xnorm = rt_hypotd_snf(r_cross_s[0], xnorm);
      if (r_cross_s[0] >= 0.0) {
        xnorm = -xnorm;
      }

      if (fabs(xnorm) < 1.0020841800044864E-292) {
        knt = 0;
        do {
          knt++;
          for (k = 0; k < 2; k++) {
            b[k + 1] *= 9.9792015476736E+291;
          }

          xnorm *= 9.9792015476736E+291;
          wj *= 9.9792015476736E+291;
        } while (!(fabs(xnorm) >= 1.0020841800044864E-292));

        xnorm = rt_hypotd_snf(wj, eml_xnrm2(b));
        if (wj >= 0.0) {
          xnorm = -xnorm;
        }

        tau = (xnorm - wj) / xnorm;
        wj = 1.0 / (wj - xnorm);
        for (k = 0; k < 2; k++) {
          b[k + 1] *= wj;
        }

        for (k = 1; k <= knt; k++) {
          xnorm *= 1.0020841800044864E-292;
        }

        wj = xnorm;
      } else {
        tau = (xnorm - r_cross_s[0]) / xnorm;
        wj = 1.0 / (r_cross_s[0] - xnorm);
        for (k = 0; k < 2; k++) {
          b[k + 1] *= wj;
        }

        wj = xnorm;
      }
    }

    b[0] = wj;
    knt = -1;
    if (fabs(b[0]) <= 3.0 * fabs(b[0]) * 2.2204460492503131E-16) {
    } else {
      knt = 0;
    }

    xnorm = 0.0;
    if (tau != 0.0) {
      wj = q_minus_p_cross_r[0];
      for (k = 0; k < 2; k++) {
        wj += b[1 + k] * q_minus_p_cross_r[1 + k];
      }

      wj *= tau;
      if (wj != 0.0) {
        a[0] = q_minus_p_cross_r[0] - wj;
        for (k = 0; k < 2; k++) {
          a[1 + k] -= b[1 + k] * wj;
        }
      }
    }

    k = 0;
    while (k <= knt) {
      xnorm = a[0];
      k = 1;
    }

    k = 0;
    while (k <= knt) {
      xnorm /= b[0];
      k = 1;
    }

    if ((xnorm >= 0.0) && (xnorm <= 1.0)) {
      /* display('Intersect') */
      *is_intersecting = 1;
      for (k = 0; k < 2; k++) {
        intersection_point[k] = s1[k] + xnorm * s[k];
      }
    }
  }
}

void is_convex(const emxArray_real_T *vertex, boolean_T convexify, boolean_T
               *convex, emxArray_real_T *convex_vertex)
{
  int32_T k;
  int32_T vlen;
  emxArray_int32_T *r8;
  int32_T i3;
  emxArray_real_T *b_vertex;
  emxArray_real_T *x1;
  emxArray_int32_T *r9;
  emxArray_real_T *c_vertex;
  emxArray_real_T *d_vertex;
  emxArray_real_T *x2;
  emxArray_int32_T *r10;
  emxArray_real_T *e_vertex;
  emxArray_real_T *b_y1;
  emxArray_int32_T *r11;
  emxArray_real_T *f_vertex;
  emxArray_real_T *g_vertex;
  emxArray_real_T *y2;
  emxArray_real_T *zcrossproduct;
  emxArray_boolean_T *x;
  real_T t1;
  real_T t2;
  boolean_T b0;
  emxArray_real_T *b_convex_vertex;
  emxArray_real_T *b_zcrossproduct;

  /* vertex = [ x1 y1 */
  /*            x2 y2 */
  /*            .. .. */
  /*            xn yn]; */
  /* %% TODO check if complex or not  %%% we suppose that points are given */
  /* %% not complex --> clockwise or counter-clockwise */
  k = convex_vertex->size[0] * convex_vertex->size[1];
  convex_vertex->size[0] = vertex->size[0];
  convex_vertex->size[1] = 2;
  emxEnsureCapacity((emxArray__common *)convex_vertex, k, (int32_T)sizeof(real_T));
  vlen = vertex->size[0] * vertex->size[1] - 1;
  for (k = 0; k <= vlen; k++) {
    convex_vertex->data[k] = vertex->data[k];
  }

  if (vertex->size[0] < 3) {
    /* %not a polygon */
    *convex = FALSE;
  } else {
    emxInit_int32_T(&r8, 1);
    k = vertex->size[0];
    i3 = r8->size[0];
    r8->size[0] = k;
    emxEnsureCapacity((emxArray__common *)r8, i3, (int32_T)sizeof(int32_T));
    vlen = k - 1;
    for (k = 0; k <= vlen; k++) {
      r8->data[k] = 1 + k;
    }

    b_emxInit_real_T(&b_vertex, 1);
    k = r8->size[0];
    i3 = vertex->size[0];
    vlen = b_vertex->size[0];
    b_vertex->size[0] = i3;
    emxEnsureCapacity((emxArray__common *)b_vertex, vlen, (int32_T)sizeof(real_T));
    emxFree_int32_T(&r8);
    vlen = i3 - 1;
    for (i3 = 0; i3 <= vlen; i3++) {
      b_vertex->data[i3] = vertex->data[i3];
    }

    b_emxInit_real_T(&x1, 1);
    i3 = x1->size[0];
    x1->size[0] = k;
    emxEnsureCapacity((emxArray__common *)x1, i3, (int32_T)sizeof(real_T));
    vlen = k - 2;
    for (i3 = 0; i3 <= vlen; i3++) {
      x1->data[i3] = b_vertex->data[1 + i3];
    }

    emxFree_real_T(&b_vertex);
    emxInit_int32_T(&r9, 1);
    x1->data[k - 1] = vertex->data[0];
    k = vertex->size[0];
    i3 = r9->size[0];
    r9->size[0] = k;
    emxEnsureCapacity((emxArray__common *)r9, i3, (int32_T)sizeof(int32_T));
    vlen = k - 1;
    for (k = 0; k <= vlen; k++) {
      r9->data[k] = 1 + k;
    }

    b_emxInit_real_T(&c_vertex, 1);
    k = r9->size[0];
    i3 = vertex->size[0];
    vlen = c_vertex->size[0];
    c_vertex->size[0] = i3;
    emxEnsureCapacity((emxArray__common *)c_vertex, vlen, (int32_T)sizeof(real_T));
    emxFree_int32_T(&r9);
    vlen = i3 - 1;
    for (i3 = 0; i3 <= vlen; i3++) {
      c_vertex->data[i3] = vertex->data[i3];
    }

    b_emxInit_real_T(&d_vertex, 1);
    i3 = vertex->size[0];
    vlen = d_vertex->size[0];
    d_vertex->size[0] = i3;
    emxEnsureCapacity((emxArray__common *)d_vertex, vlen, (int32_T)sizeof(real_T));
    vlen = i3 - 1;
    for (i3 = 0; i3 <= vlen; i3++) {
      d_vertex->data[i3] = vertex->data[i3];
    }

    b_emxInit_real_T(&x2, 1);
    i3 = x2->size[0];
    x2->size[0] = k;
    emxEnsureCapacity((emxArray__common *)x2, i3, (int32_T)sizeof(real_T));
    vlen = k - 3;
    for (i3 = 0; i3 <= vlen; i3++) {
      x2->data[i3] = c_vertex->data[2 + i3];
    }

    emxFree_real_T(&c_vertex);
    for (i3 = 0; i3 < 2; i3++) {
      x2->data[(i3 + k) - 2] = d_vertex->data[i3];
    }

    emxFree_real_T(&d_vertex);
    emxInit_int32_T(&r10, 1);
    k = vertex->size[0];
    i3 = r10->size[0];
    r10->size[0] = k;
    emxEnsureCapacity((emxArray__common *)r10, i3, (int32_T)sizeof(int32_T));
    vlen = k - 1;
    for (k = 0; k <= vlen; k++) {
      r10->data[k] = 1 + k;
    }

    b_emxInit_real_T(&e_vertex, 1);
    k = r10->size[0];
    i3 = vertex->size[0];
    vlen = e_vertex->size[0];
    e_vertex->size[0] = i3;
    emxEnsureCapacity((emxArray__common *)e_vertex, vlen, (int32_T)sizeof(real_T));
    emxFree_int32_T(&r10);
    vlen = i3 - 1;
    for (i3 = 0; i3 <= vlen; i3++) {
      e_vertex->data[i3] = vertex->data[i3 + vertex->size[0]];
    }

    b_emxInit_real_T(&b_y1, 1);
    i3 = b_y1->size[0];
    b_y1->size[0] = k;
    emxEnsureCapacity((emxArray__common *)b_y1, i3, (int32_T)sizeof(real_T));
    vlen = k - 2;
    for (i3 = 0; i3 <= vlen; i3++) {
      b_y1->data[i3] = e_vertex->data[1 + i3];
    }

    emxFree_real_T(&e_vertex);
    emxInit_int32_T(&r11, 1);
    b_y1->data[k - 1] = vertex->data[vertex->size[0]];
    k = vertex->size[0];
    i3 = r11->size[0];
    r11->size[0] = k;
    emxEnsureCapacity((emxArray__common *)r11, i3, (int32_T)sizeof(int32_T));
    vlen = k - 1;
    for (k = 0; k <= vlen; k++) {
      r11->data[k] = 1 + k;
    }

    b_emxInit_real_T(&f_vertex, 1);
    k = r11->size[0];
    i3 = vertex->size[0];
    vlen = f_vertex->size[0];
    f_vertex->size[0] = i3;
    emxEnsureCapacity((emxArray__common *)f_vertex, vlen, (int32_T)sizeof(real_T));
    emxFree_int32_T(&r11);
    vlen = i3 - 1;
    for (i3 = 0; i3 <= vlen; i3++) {
      f_vertex->data[i3] = vertex->data[i3 + vertex->size[0]];
    }

    b_emxInit_real_T(&g_vertex, 1);
    i3 = vertex->size[0];
    vlen = g_vertex->size[0];
    g_vertex->size[0] = i3;
    emxEnsureCapacity((emxArray__common *)g_vertex, vlen, (int32_T)sizeof(real_T));
    vlen = i3 - 1;
    for (i3 = 0; i3 <= vlen; i3++) {
      g_vertex->data[i3] = vertex->data[i3 + vertex->size[0]];
    }

    b_emxInit_real_T(&y2, 1);
    i3 = y2->size[0];
    y2->size[0] = k;
    emxEnsureCapacity((emxArray__common *)y2, i3, (int32_T)sizeof(real_T));
    vlen = k - 3;
    for (i3 = 0; i3 <= vlen; i3++) {
      y2->data[i3] = f_vertex->data[2 + i3];
    }

    emxFree_real_T(&f_vertex);
    for (i3 = 0; i3 < 2; i3++) {
      y2->data[(i3 + k) - 2] = g_vertex->data[i3];
    }

    emxFree_real_T(&g_vertex);
    b_emxInit_real_T(&zcrossproduct, 1);

    /*  --> see http://stackoverflow.com/questions/471962/how-do-determine-if-a-polygon-is-complex-convex-nonconvex */
    k = zcrossproduct->size[0];
    zcrossproduct->size[0] = x2->size[0];
    emxEnsureCapacity((emxArray__common *)zcrossproduct, k, (int32_T)sizeof
                      (real_T));
    vlen = x2->size[0] - 1;
    for (k = 0; k <= vlen; k++) {
      zcrossproduct->data[k] = (x2->data[k] - x1->data[k]) * (vertex->data[k +
        vertex->size[0]] - b_y1->data[k]) - (y2->data[k] - b_y1->data[k]) *
        (vertex->data[k] - x1->data[k]);
    }

    emxFree_real_T(&y2);
    emxFree_real_T(&b_y1);
    emxFree_real_T(&x2);
    emxFree_real_T(&x1);
    emxInit_boolean_T(&x, 1);
    k = x->size[0];
    x->size[0] = zcrossproduct->size[0];
    emxEnsureCapacity((emxArray__common *)x, k, (int32_T)sizeof(boolean_T));
    vlen = zcrossproduct->size[0] - 1;
    for (k = 0; k <= vlen; k++) {
      x->data[k] = (zcrossproduct->data[k] >= 0.0);
    }

    vlen = x->size[0];
    t1 = (real_T)x->data[0];
    for (k = 2; k <= vlen; k++) {
      t1 += (real_T)x->data[k - 1];
    }

    k = x->size[0];
    x->size[0] = zcrossproduct->size[0];
    emxEnsureCapacity((emxArray__common *)x, k, (int32_T)sizeof(boolean_T));
    vlen = zcrossproduct->size[0] - 1;
    for (k = 0; k <= vlen; k++) {
      x->data[k] = (zcrossproduct->data[k] <= 0.0);
    }

    vlen = x->size[0];
    t2 = (real_T)x->data[0];
    for (k = 2; k <= vlen; k++) {
      t2 += (real_T)x->data[k - 1];
    }

    emxFree_boolean_T(&x);
    if ((t1 == (real_T)vertex->size[0]) || (t2 == (real_T)vertex->size[0])) {
      b0 = TRUE;
    } else {
      b0 = FALSE;
    }

    *convex = b0;
    if (b0 || (!convexify)) {
    } else {
      /*  not convex and need to convexify recursively */
      emxInit_real_T(&b_convex_vertex, 2);
      b_emxInit_real_T(&b_zcrossproduct, 1);
      while (!*convex) {
        k = b_convex_vertex->size[0] * b_convex_vertex->size[1];
        b_convex_vertex->size[0] = convex_vertex->size[0];
        b_convex_vertex->size[1] = 2;
        emxEnsureCapacity((emxArray__common *)b_convex_vertex, k, (int32_T)
                          sizeof(real_T));
        vlen = convex_vertex->size[0] * convex_vertex->size[1] - 1;
        for (k = 0; k <= vlen; k++) {
          b_convex_vertex->data[k] = convex_vertex->data[k];
        }

        k = b_zcrossproduct->size[0];
        b_zcrossproduct->size[0] = zcrossproduct->size[0];
        emxEnsureCapacity((emxArray__common *)b_zcrossproduct, k, (int32_T)
                          sizeof(real_T));
        vlen = zcrossproduct->size[0] - 1;
        for (k = 0; k <= vlen; k++) {
          b_zcrossproduct->data[k] = zcrossproduct->data[k];
        }

        convexify_polygon(b_convex_vertex, b_zcrossproduct, convex,
                          convex_vertex, zcrossproduct);

        /* %recursive */
      }

      emxFree_real_T(&b_zcrossproduct);
      emxFree_real_T(&b_convex_vertex);

      /*  here it should have find the convex polygon */
    }

    emxFree_real_T(&zcrossproduct);
  }

  /*      if (convex)                                     */
  /*          convex_vertex = vertex; */
  /*          return;                                    % END: found convexity */
  /*      else if (convexify)                            % not convex and need to convexify recursively */
  /*              sum_clock = 0; */
  /*              index_removed = 0; */
  /*              vertex_extended = [vertex ; [vertex(1,1),vertex(1,2)]]; */
  /*              for i=1:N */
  /*                  sum_clock = sum_clock + ((vertex_extended(i,1)-vertex_extended(i+1,1))*(vertex_extended(i,2)+vertex_extended(i+1,2))); */
  /*              end */
  /*              clockwise = sum_clock <= 0;           % see --> http://stackoverflow.com/questions/1165647/how-to-determine-if-a-list-of-polygon-points-are-in-clockwise-order */
  /*              for i=1:N */
  /*                  if ( (clockwise && zcrossproduct(i)>0)   ||  (~clockwise && zcrossproduct(i)<0) ) */
  /*                      convex_vertex(i+1-index_removed,:) = []; */
  /*                      index_removed = index_removed+1; */
  /*                  end */
  /*              end */
  /*              [convex_not_used,convex_vertex] = is_convex(convex_vertex,true);     %%recursive */
  /*          else */
  /*              return             % END: not convex and no need to convexify */
  /*          end */
  /*      end */
}

/* End of code generation (WP_grid.cpp) */
