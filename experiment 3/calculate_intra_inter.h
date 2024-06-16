//
// MATLAB Compiler: 24.1 (R2024a)
// Date: Tue Jun 11 15:37:23 2024
// Arguments:
// "-B""macro_default""-W""cpplib:calculate_intra_inter,all,version=1.0""-T""lin
// k:lib""-d""D:\VS code\projects\mode recognition\mode recognition
// experiment\experiment 3\matlab\calculate_intra_inter\for_testing""-v""D:\VS
// code\projects\mode recognition\mode recognition experiment\experiment
// 3\matlab\calculate_intra_inter.m"
//

#ifndef calculate_intra_inter_h
#define calculate_intra_inter_h 1

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
#ifndef LIB_calculate_intra_inter_C_API 
#define LIB_calculate_intra_inter_C_API /* No special import/export declaration */
#endif

/* GENERAL LIBRARY FUNCTIONS -- START */

extern LIB_calculate_intra_inter_C_API 
bool MW_CALL_CONV calculate_intra_interInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_calculate_intra_inter_C_API 
bool MW_CALL_CONV calculate_intra_interInitialize(void);
extern LIB_calculate_intra_inter_C_API 
void MW_CALL_CONV calculate_intra_interTerminate(void);

extern LIB_calculate_intra_inter_C_API 
void MW_CALL_CONV calculate_intra_interPrintStackTrace(void);

/* GENERAL LIBRARY FUNCTIONS -- END */

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

extern LIB_calculate_intra_inter_C_API 
bool MW_CALL_CONV mlxCalculate_intra_inter(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                           *prhs[]);

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */

#ifdef __cplusplus
}
#endif


/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__MINGW64__)

#ifdef EXPORTING_calculate_intra_inter
#define PUBLIC_calculate_intra_inter_CPP_API __declspec(dllexport)
#else
#define PUBLIC_calculate_intra_inter_CPP_API __declspec(dllimport)
#endif

#define LIB_calculate_intra_inter_CPP_API PUBLIC_calculate_intra_inter_CPP_API

#else

#if !defined(LIB_calculate_intra_inter_CPP_API)
#if defined(LIB_calculate_intra_inter_C_API)
#define LIB_calculate_intra_inter_CPP_API LIB_calculate_intra_inter_C_API
#else
#define LIB_calculate_intra_inter_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_calculate_intra_inter_CPP_API void MW_CALL_CONV calculate_intra_inter(int nargout, mwArray& intra_class_scatter, mwArray& inter_class_scatter, const mwArray& data, const mwArray& labelArray);

/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */
#endif

#endif
