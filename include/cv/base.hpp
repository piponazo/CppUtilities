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
// Copyright (C) 2014, Itseez Inc., all rights reserved.
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

#ifndef __OPENCV_CORE_BASE_HPP__
#define __OPENCV_CORE_BASE_HPP__

#ifndef __cplusplus
#  error base.hpp header must be compiled as C++
#endif

#include <climits>

#include "cvdef.h"
#include "cvstd.hpp"

namespace minicv
{

//! @addtogroup core_utils
//! @{

namespace Error {
//! error codes
enum Code {
    StsOk=                       0,  //!< everithing is ok
    StsBackTrace=               -1,  //!< pseudo error for back trace
    StsError=                   -2,  //!< unknown /unspecified error
    StsInternal=                -3,  //!< internal error (bad state)
    StsNoMem=                   -4,  //!< insufficient memory
    StsBadArg=                  -5,  //!< function arg/param is bad
    StsBadFunc=                 -6,  //!< unsupported function
    StsNoConv=                  -7,  //!< iter. didn't converge
    StsAutoTrace=               -8,  //!< tracing
    HeaderIsNull=               -9,  //!< image header is NULL
    BadImageSize=              -10,  //!< image size is invalid
    BadOffset=                 -11,  //!< offset is invalid
    BadDataPtr=                -12,  //!<
    BadStep=                   -13,  //!<
    BadModelOrChSeq=           -14,  //!<
    BadNumChannels=            -15,  //!<
    BadNumChannel1U=           -16,  //!<
    BadDepth=                  -17,  //!<
    BadAlphaChannel=           -18,  //!<
    BadOrder=                  -19,  //!<
    BadOrigin=                 -20,  //!<
    BadAlign=                  -21,  //!<
    BadCallBack=               -22,  //!<
    BadTileSize=               -23,  //!<
    BadCOI=                    -24,  //!<
    BadROISize=                -25,  //!<
    MaskIsTiled=               -26,  //!<
    StsNullPtr=                -27,  //!< null pointer
    StsVecLengthErr=           -28,  //!< incorrect vector length
    StsFilterStructContentErr= -29,  //!< incorr. filter structure content
    StsKernelStructContentErr= -30,  //!< incorr. transform kernel content
    StsFilterOffsetErr=        -31,  //!< incorrect filter ofset value
    StsBadSize=                -201, //!< the input/output structure size is incorrect
    StsDivByZero=              -202, //!< division by zero
    StsInplaceNotSupported=    -203, //!< in-place operation is not supported
    StsObjectNotFound=         -204, //!< request can't be completed
    StsUnmatchedFormats=       -205, //!< formats of input/output arrays differ
    StsBadFlag=                -206, //!< flag is wrong or not supported
    StsBadPoint=               -207, //!< bad CvPoint
    StsBadMask=                -208, //!< bad format of mask (neither 8uC1 nor 8sC1)
    StsUnmatchedSizes=         -209, //!< sizes of input/output structures do not match
    StsUnsupportedFormat=      -210, //!< the data format/type is not supported by the function
    StsOutOfRange=             -211, //!< some of parameters are out of range
    StsParseError=             -212, //!< invalid syntax/structure of the parsed file
    StsNotImplemented=         -213, //!< the requested function/feature is not implemented
    StsBadMemBlock=            -214, //!< an allocated block has been corrupted
    StsAssert=                 -215, //!< assertion failed
    GpuNotSupported=           -216,
    GpuApiCallError=           -217,
    OpenGlNotSupported=        -218,
    OpenGlApiCallError=        -219,
    OpenCLApiCallError=        -220,
    OpenCLDoubleNotSupported=  -221,
    OpenCLInitError=           -222,
    OpenCLNoAMDBlasFft=        -223
};
} //Error

//! @} core_utils

//! @addtogroup core_array
//! @{

//! matrix decomposition types
enum DecompTypes {
    /** Gaussian elimination with the optimal pivot element chosen. */
    DECOMP_LU       = 0,
    /** singular value decomposition (SVD) method; the system can be over-defined and/or the matrix
    src1 can be singular */
    DECOMP_SVD      = 1,
    /** eigenvalue decomposition; the matrix src1 must be symmetrical */
    DECOMP_EIG      = 2,
    /** Cholesky \f$LL^T\f$ factorization; the matrix src1 must be symmetrical and positively
    defined */
    DECOMP_CHOLESKY = 3,
    /** QR factorization; the system can be over-defined and/or the matrix src1 can be singular */
    DECOMP_QR       = 4,
    /** while all the previous flags are mutually exclusive, this flag can be used together with
    any of the previous; it means that the normal equations
    \f$\texttt{src1}^T\cdot\texttt{src1}\cdot\texttt{dst}=\texttt{src1}^T\texttt{src2}\f$ are
    solved instead of the original system
    \f$\texttt{src1}\cdot\texttt{dst}=\texttt{src2}\f$ */
    DECOMP_NORMAL   = 16
};


//////////////// static assert /////////////////
#define CVAUX_CONCAT_EXP(a, b) a##b
#define CVAUX_CONCAT(a, b) CVAUX_CONCAT_EXP(a,b)

#if defined(__clang__)
#  ifndef __has_extension
#    define __has_extension __has_feature /* compatibility, for older versions of clang */
#  endif
#  if __has_extension(cxx_static_assert)
#    define CV_StaticAssert(condition, reason)    static_assert((condition), reason " " #condition)
#  endif
#elif defined(__GNUC__)
#  if (defined(__GXX_EXPERIMENTAL_CXX0X__) || __cplusplus >= 201103L)
#    define CV_StaticAssert(condition, reason)    static_assert((condition), reason " " #condition)
#  endif
#elif defined(_MSC_VER)
#  if _MSC_VER >= 1600 /* MSVC 10 */
#    define CV_StaticAssert(condition, reason)    static_assert((condition), reason " " #condition)
#  endif
#endif
#ifndef CV_StaticAssert
#  if defined(__GNUC__) && (__GNUC__ > 3) && (__GNUC_MINOR__ > 2)
#    define CV_StaticAssert(condition, reason) ({ extern int __attribute__((error("CV_StaticAssert: " reason " " #condition))) CV_StaticAssert(); ((condition) ? 0 : CV_StaticAssert()); })
#  else
     template <bool x> struct CV_StaticAssert_failed;
     template <> struct CV_StaticAssert_failed<true> { enum { val = 1 }; };
     template<int x> struct CV_StaticAssert_test {};
#    define CV_StaticAssert(condition, reason)\
       typedef minicv::CV_StaticAssert_test< sizeof(minicv::CV_StaticAssert_failed< static_cast<bool>(condition) >) > CVAUX_CONCAT(CV_StaticAssert_failed_at_, __LINE__)
#  endif
#endif

// Suppress warning "-Wdeprecated-declarations" / C4996
#if defined(_MSC_VER)
    #define CV_DO_PRAGMA(x) __pragma(x)
#elif defined(__GNUC__)
    #define CV_DO_PRAGMA(x) _Pragma (#x)
#else
    #define CV_DO_PRAGMA(x)
#endif

#ifdef _MSC_VER
#define CV_SUPPRESS_DEPRECATED_START \
    CV_DO_PRAGMA(warning(push)) \
    CV_DO_PRAGMA(warning(disable: 4996))
#define CV_SUPPRESS_DEPRECATED_END CV_DO_PRAGMA(warning(pop))
#elif defined (__clang__) || ((__GNUC__)  && ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 5))))
#define CV_SUPPRESS_DEPRECATED_START \
    CV_DO_PRAGMA(GCC diagnostic push) \
    CV_DO_PRAGMA(GCC diagnostic ignored "-Wdeprecated-declarations")
#define CV_SUPPRESS_DEPRECATED_END CV_DO_PRAGMA(GCC diagnostic pop)
#else
#define CV_SUPPRESS_DEPRECATED_START
#define CV_SUPPRESS_DEPRECATED_END
#endif
//! @endcond

/*! @brief Signals an error and raises the exception.

By default the function prints information about the error to stderr,
then it either stops if setBreakOnError() had been called before or raises the exception.
It is possible to alternate error processing by using redirectError().
@param _code - error code (Error::Code)
@param _err - error description
@param _func - function name. Available only when the compiler supports getting it
@param _file - source file name where the error has occured
@param _line - line number in the source file where the error has occured
@see CV_Error, CV_Error_, CV_ErrorNoReturn, CV_ErrorNoReturn_, CV_Assert, CV_DbgAssert
 */
CV_EXPORTS void error(int _code, const String& _err, const char* _func, const char* _file, int _line);

#ifdef __GNUC__
# if defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Winvalid-noreturn"
# endif
#endif

/** same as minicv::error, but does not return */
CV_INLINE CV_NORETURN void errorNoReturn(int _code, const String& _err, const char* _func, const char* _file, int _line)
{
    error(_code, _err, _func, _file, _line);
#ifdef __GNUC__
# if !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#endif
}
#ifdef __GNUC__
# if defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic pop
# endif
#endif

#if defined __GNUC__
#define CV_Func __func__
#elif defined _MSC_VER
#define CV_Func __FUNCTION__
#else
#define CV_Func ""
#endif

/** @brief Call the error handler.

Currently, the error handler prints the error code and the error message to the standard
error stream `stderr`. In the Debug configuration, it then provokes memory access violation, so that
the execution stack and all the parameters can be analyzed by the debugger. In the Release
configuration, the exception is thrown.

@param code one of Error::Code
@param msg error message
*/
#define CV_Error( code, msg ) minicv::error( code, msg, CV_Func, __FILE__, __LINE__ )

/**  @brief Call the error handler.

This macro can be used to construct an error message on-fly to include some dynamic information,
for example:
@code
    // note the extra parentheses around the formatted text message
    CV_Error_( CV_StsOutOfRange,
    ("the value at (%d, %d)=%g is out of range", badPt.x, badPt.y, badValue));
@endcode
@param code one of Error::Code
@param args printf-like formatted error message in parentheses
*/
#define CV_Error_( code, args ) minicv::error( code, minicv::format args, CV_Func, __FILE__, __LINE__ )

/** @brief Checks a condition at runtime and throws exception if it fails

The macros CV_Assert (and CV_DbgAssert(expr)) evaluate the specified expression. If it is 0, the macros
raise an error (see minicv::error). The macro CV_Assert checks the condition in both Debug and Release
configurations while CV_DbgAssert is only retained in the Debug configuration.
*/
#define CV_Assert( expr ) if(!!(expr)) ; else minicv::error( minicv::Error::StsAssert, #expr, CV_Func, __FILE__, __LINE__ )

/** same as CV_Error(code,msg), but does not return */
#define CV_ErrorNoReturn( code, msg ) minicv::errorNoReturn( code, msg, CV_Func, __FILE__, __LINE__ )

/** same as CV_Error_(code,args), but does not return */
#define CV_ErrorNoReturn_( code, args ) minicv::errorNoReturn( code, minicv::format args, CV_Func, __FILE__, __LINE__ )

/** replaced with CV_Assert(expr) in Debug configuration */
#ifdef _DEBUG
#  define CV_DbgAssert(expr) CV_Assert(expr)
#else
#  define CV_DbgAssert(expr)
#endif

////////////////// forward declarations for important OpenCV types //////////////////

//! @cond IGNORED

template<typename _Tp, int cn> class Vec;
template<typename _Tp, int m, int n> class Matx;

template<typename _Tp> class Complex;
template<typename _Tp> class Point_;
template<typename _Tp> class Size_;
template<typename _Tp> class Rect_;
template<typename _Tp> class Scalar_;

class CV_EXPORTS RotatedRect;
class CV_EXPORTS Range;

class CV_EXPORTS Mat;
class CV_EXPORTS MatExpr;

class CV_EXPORTS UMat;

typedef Mat MatND;

template<typename _Tp> class Mat_;

} // minicv

#endif //__OPENCV_CORE_BASE_HPP__
