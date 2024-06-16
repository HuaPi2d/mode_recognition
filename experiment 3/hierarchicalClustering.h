//
// MATLAB Compiler: 24.1 (R2024a)
// Date: Tue Jun 11 14:07:34 2024
// Arguments:
// "-B""macro_default""-W""cpplib:hierarchicalClustering,all,version=1.0""-T""li
// nk:lib""-d""D:\VS code\projects\mode recognition\mode recognition
// experiment\experiment 3\matlab\hierarchicalClustering\for_testing""-v""D:\VS
// code\projects\mode recognition\mode recognition experiment\experiment
// 3\matlab\hierarchicalClustering.m"
//

#ifndef hierarchicalClustering_h
#define hierarchicalClustering_h 1

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
#ifndef LIB_hierarchicalClustering_C_API 
#define LIB_hierarchicalClustering_C_API /* No special import/export declaration */
#endif

/* GENERAL LIBRARY FUNCTIONS -- START */

extern LIB_hierarchicalClustering_C_API 
bool MW_CALL_CONV hierarchicalClusteringInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_hierarchicalClustering_C_API 
bool MW_CALL_CONV hierarchicalClusteringInitialize(void);
extern LIB_hierarchicalClustering_C_API 
void MW_CALL_CONV hierarchicalClusteringTerminate(void);

extern LIB_hierarchicalClustering_C_API 
void MW_CALL_CONV hierarchicalClusteringPrintStackTrace(void);

/* GENERAL LIBRARY FUNCTIONS -- END */

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

extern LIB_hierarchicalClustering_C_API 
bool MW_CALL_CONV mlxHierarchicalClustering(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                            *prhs[]);

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */

#ifdef __cplusplus
}
#endif


/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__MINGW64__)

#ifdef EXPORTING_hierarchicalClustering
#define PUBLIC_hierarchicalClustering_CPP_API __declspec(dllexport)
#else
#define PUBLIC_hierarchicalClustering_CPP_API __declspec(dllimport)
#endif

#define LIB_hierarchicalClustering_CPP_API PUBLIC_hierarchicalClustering_CPP_API

#else

#if !defined(LIB_hierarchicalClustering_CPP_API)
#if defined(LIB_hierarchicalClustering_C_API)
#define LIB_hierarchicalClustering_CPP_API LIB_hierarchicalClustering_C_API
#else
#define LIB_hierarchicalClustering_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_hierarchicalClustering_CPP_API void MW_CALL_CONV hierarchicalClustering(int nargout, mwArray& clusters, const mwArray& data, const mwArray& num_clusters);

/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */
#endif

#endif
