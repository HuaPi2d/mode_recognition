//
// MATLAB Compiler: 24.1 (R2024a)
// Date: Mon Jun 10 19:01:58 2024
// Arguments:
// "-B""macro_default""-W""cpplib:paint_point,all,version=1.0""-T""link:lib""-d"
// "D:\VS code\projects\mode recognition\mode recognition experiment\experiment
// 3\matlab\paint_point\for_testing""-v""D:\VS code\projects\mode
// recognition\mode recognition experiment\experiment 3\matlab\paint_point.m"
//

#ifndef paint_point_h
#define paint_point_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" { // sbcheck:ok:extern_c
#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_paint_point_C_API 
#define LIB_paint_point_C_API /* No special import/export declaration */
#endif

/* GENERAL LIBRARY FUNCTIONS -- START */

extern LIB_paint_point_C_API 
bool MW_CALL_CONV paint_pointInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_paint_point_C_API 
bool MW_CALL_CONV paint_pointInitialize(void);
extern LIB_paint_point_C_API 
void MW_CALL_CONV paint_pointTerminate(void);

extern LIB_paint_point_C_API 
void MW_CALL_CONV paint_pointPrintStackTrace(void);

/* GENERAL LIBRARY FUNCTIONS -- END */

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

extern LIB_paint_point_C_API 
bool MW_CALL_CONV mlxPaint_point(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */

#ifdef __cplusplus
}
#endif


/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__MINGW64__)

#ifdef EXPORTING_paint_point
#define PUBLIC_paint_point_CPP_API __declspec(dllexport)
#else
#define PUBLIC_paint_point_CPP_API __declspec(dllimport)
#endif

#define LIB_paint_point_CPP_API PUBLIC_paint_point_CPP_API

#else

#if !defined(LIB_paint_point_CPP_API)
#if defined(LIB_paint_point_C_API)
#define LIB_paint_point_CPP_API LIB_paint_point_C_API
#else
#define LIB_paint_point_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_paint_point_CPP_API void MW_CALL_CONV paint_point(const mwArray& featureData, const mwArray& labelArray, const mwArray& title_name);

/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */
#endif

#endif
