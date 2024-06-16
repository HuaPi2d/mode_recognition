//
// MATLAB Compiler: 24.1 (R2024a)
// Date: Sat Jun 15 15:56:27 2024
// Arguments:
// "-B""macro_default""-W""cpplib:dbscan_algorithm,all,version=1.0""-T""link:lib
// ""-d""D:\VS code\projects\mode recognition\mode recognition
// experiment\experiment 3\matlab\dbscan_algorithm\for_testing""-v""D:\VS
// code\projects\mode recognition\mode recognition experiment\experiment
// 3\matlab\dbscan_algorithm.m"
//

#ifndef dbscan_algorithm_h
#define dbscan_algorithm_h 1

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
#ifndef LIB_dbscan_algorithm_C_API 
#define LIB_dbscan_algorithm_C_API /* No special import/export declaration */
#endif

/* GENERAL LIBRARY FUNCTIONS -- START */

extern LIB_dbscan_algorithm_C_API 
bool MW_CALL_CONV dbscan_algorithmInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_dbscan_algorithm_C_API 
bool MW_CALL_CONV dbscan_algorithmInitialize(void);
extern LIB_dbscan_algorithm_C_API 
void MW_CALL_CONV dbscan_algorithmTerminate(void);

extern LIB_dbscan_algorithm_C_API 
void MW_CALL_CONV dbscan_algorithmPrintStackTrace(void);

/* GENERAL LIBRARY FUNCTIONS -- END */

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

extern LIB_dbscan_algorithm_C_API 
bool MW_CALL_CONV mlxDbscan_algorithm(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                      *prhs[]);

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */

#ifdef __cplusplus
}
#endif


/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__MINGW64__)

#ifdef EXPORTING_dbscan_algorithm
#define PUBLIC_dbscan_algorithm_CPP_API __declspec(dllexport)
#else
#define PUBLIC_dbscan_algorithm_CPP_API __declspec(dllimport)
#endif

#define LIB_dbscan_algorithm_CPP_API PUBLIC_dbscan_algorithm_CPP_API

#else

#if !defined(LIB_dbscan_algorithm_CPP_API)
#if defined(LIB_dbscan_algorithm_C_API)
#define LIB_dbscan_algorithm_CPP_API LIB_dbscan_algorithm_C_API
#else
#define LIB_dbscan_algorithm_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_dbscan_algorithm_CPP_API void MW_CALL_CONV dbscan_algorithm(int nargout, mwArray& labelArray, const mwArray& featureData, const mwArray& epsilon, const mwArray& minPts);

/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */
#endif

#endif
