/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                          License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Copyright (C) 2013, OpenCV Foundation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/


#ifndef __OPENCV_CORE_C_H__
#define __OPENCV_CORE_C_H__

#include "types_c.h"

#ifdef __cplusplus
#  ifdef _MSC_VER
/* disable warning C4190: 'function' has C-linkage specified, but returns UDT 'typename'
                          which is incompatible with C

   It is OK to disable it because we only extend few plain structures with
   C++ construrtors for simpler interoperability with C++ API of the library
*/
#    pragma warning(disable:4190)
#  elif defined __clang__ && __clang_major__ >= 3
#    pragma GCC diagnostic ignored "-Wreturn-type-c-linkage"
#  endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup core_c
    @{
*/

/****************************************************************************************\
*          Array allocation, deallocation, initialization and access to elements         *
\****************************************************************************************/

#define CV_AUTOSTEP  0x7fffffff

#define CV_MAX_ARR 10

/********************************** Error Handling **************************************/

/** Get current OpenCV error status */
CVAPI(int) cvGetErrStatus( void );

/** Sets error status silently */
CVAPI(void) cvSetErrStatus( int status );

#define CV_ErrModeLeaf     0   /* Print error and exit program */
#define CV_ErrModeParent   1   /* Print error and continue */
#define CV_ErrModeSilent   2   /* Don't print and continue */

/** Retrives current error processing mode */
CVAPI(int)  cvGetErrMode( void );

/** Sets error processing mode, returns previously used mode */
CVAPI(int) cvSetErrMode( int mode );

/** Sets error status and performs some additonal actions (displaying message box,
 writing message to stderr, terminating application etc.)
 depending on the current error mode */
CVAPI(void) cvError( int status, const char* func_name,
                    const char* err_msg, const char* file_name, int line );

/** Retrieves textual description of the error given its code */
CVAPI(const char*) cvErrorStr( int status );

/** Retrieves detailed information about the last error occured */
CVAPI(int) cvGetErrInfo( const char** errcode_desc, const char** description,
                        const char** filename, int* line );

/** Maps IPP error codes to the counterparts from OpenCV */
CVAPI(int) cvErrorFromIppStatus( int ipp_status );

typedef int (CV_CDECL *CvErrorCallback)( int status, const char* func_name,
                                        const char* err_msg, const char* file_name, int line, void* userdata );

/** Assigns a new error-handling function */
CVAPI(CvErrorCallback) cvRedirectError( CvErrorCallback error_handler,
                                       void* userdata CV_DEFAULT(NULL),
                                       void** prev_userdata CV_DEFAULT(NULL) );


#define OPENCV_ERROR(status,func,context)                           \
cvError((status),(func),(context),__FILE__,__LINE__)

#define OPENCV_ASSERT(expr,func,context)                            \
{if (! (expr))                                      \
{OPENCV_ERROR(CV_StsInternal,(func),(context));}}

#define OPENCV_CALL( Func )                                         \
{                                                                   \
Func;                                                           \
}

/**
 CV_ERROR macro unconditionally raises error with passed code and message.
 After raising error, control will be transferred to the exit label.
 */
#define CV_ERROR( Code, Msg )                                       \
{                                                                   \
    cvError( (Code), cvFuncName, Msg, __FILE__, __LINE__ );        \
    __CV_EXIT__;                                                   \
}

/**
 CV_CHECK macro checks error status after CV (or IPL)
 function call. If error detected, control will be transferred to the exit
 label.
 */
#define CV_CHECK()                                                  \
{                                                                   \
    if( cvGetErrStatus() < 0 )                                      \
        CV_ERROR( CV_StsBackTrace, "Inner function failed." );      \
}


/**
 CV_CALL macro calls CV (or IPL) function, checks error status and
 signals a error if the function failed. Useful in "parent node"
 error procesing mode
 */
#define CV_CALL( Func )                                             \
{                                                                   \
    Func;                                                           \
    CV_CHECK();                                                     \
}


/** Runtime assertion macro */
#define CV_ASSERT( Condition )                                          \
{                                                                       \
    if( !(Condition) )                                                  \
        CV_ERROR( CV_StsInternal, "Assertion: " #Condition " failed" ); \
}

#define __CV_BEGIN__       {
#define __CV_END__         goto exit; exit: ; }
#define __CV_EXIT__        goto exit

/** @} core_c */

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus


//! @}


#endif

#endif
