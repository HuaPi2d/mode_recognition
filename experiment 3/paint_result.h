//
// MATLAB Compiler: 24.1 (R2024a)
// Date: Mon Jun 10 16:39:22 2024
// Arguments:
// "-B""macro_default""-W""cpplib:paint_result,all,version=1.0""-T""link:lib""-d
// ""D:\VS code\projects\mode recognition\mode recognition
// experiment\experiment 3\matlab\paint_result\for_testing""-v""D:\VS
// code\projects\mode recognition\mode recognition experiment\experiment
// 3\matlab\paint_result.m"
//

#ifndef paint_result_h
#define paint_result_h 1

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
#ifndef LIB_paint_result_C_API 
#define LIB_paint_result_C_API /* No special import/export declaration */
#endif

/* GENERAL LIBRARY FUNCTIONS -- START */

extern LIB_paint_result_C_API 
bool MW_CALL_CONV paint_resultInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_paint_result_C_API 
bool MW_CALL_CONV paint_resultInitialize(void);
extern LIB_paint_result_C_API 
void MW_CALL_CONV paint_resultTerminate(void);

extern LIB_paint_result_C_API 
void MW_CALL_CONV paint_resultPrintStackTrace(void);

/* GENERAL LIBRARY FUNCTIONS -- END */

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

extern LIB_paint_result_C_API 
bool MW_CALL_CONV mlxPaint_result(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */

#ifdef __cplusplus
}
#endif


/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__MINGW64__)

#ifdef EXPORTING_paint_result
#define PUBLIC_paint_result_CPP_API __declspec(dllexport)
#else
#define PUBLIC_paint_result_CPP_API __declspec(dllimport)
#endif

#define LIB_paint_result_CPP_API PUBLIC_paint_result_CPP_API

#else

#if !defined(LIB_paint_result_CPP_API)
#if defined(LIB_paint_result_C_API)
#define LIB_paint_result_CPP_API LIB_paint_result_C_API
#else
#define LIB_paint_result_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_paint_result_CPP_API void MW_CALL_CONV paint_result(const mwArray& filename);

/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */
#endif

#endif
