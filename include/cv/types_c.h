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

#ifndef __OPENCV_CORE_TYPES_H__
#define __OPENCV_CORE_TYPES_H__

#include "cvdef.h"

#ifndef SKIP_INCLUDES
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#endif // SKIP_INCLUDES

#if defined WIN32 || defined _WIN32
#  define CV_CDECL __cdecl
#  define CV_STDCALL __stdcall
#else
#  define CV_CDECL
#  define CV_STDCALL
#endif

#ifndef CV_DEFAULT
#  ifdef __cplusplus
#    define CV_DEFAULT(val) = val
#  else
#    define CV_DEFAULT(val)
#  endif
#endif

#ifndef CV_EXTERN_C_FUNCPTR
#  ifdef __cplusplus
#    define CV_EXTERN_C_FUNCPTR(x) extern "C" { typedef x; }
#  else
#    define CV_EXTERN_C_FUNCPTR(x) typedef x
#  endif
#endif

#ifndef CVAPI
#  define CVAPI(rettype) CV_EXTERN_C CV_EXPORTS rettype CV_CDECL
#endif

#ifndef CV_IMPL
#  define CV_IMPL CV_EXTERN_C
#endif

#ifdef __cplusplus
#  include "core.hpp"
#endif


/** @see cv::Error::Code */
enum {
 CV_StsOk=                       0,  /**< everything is ok                */
 CV_StsBackTrace=               -1,  /**< pseudo error for back trace     */
 CV_StsError=                   -2,  /**< unknown /unspecified error      */
 CV_StsInternal=                -3,  /**< internal error (bad state)      */
 CV_StsNoMem=                   -4,  /**< insufficient memory             */
 CV_StsBadArg=                  -5,  /**< function arg/param is bad       */
 CV_StsBadFunc=                 -6,  /**< unsupported function            */
 CV_StsNoConv=                  -7,  /**< iter. didn't converge           */
 CV_StsAutoTrace=               -8,  /**< tracing                         */
 CV_HeaderIsNull=               -9,  /**< image header is NULL            */
 CV_BadImageSize=              -10,  /**< image size is invalid           */
 CV_BadOffset=                 -11,  /**< offset is invalid               */
 CV_BadDataPtr=                -12,  /**/
 CV_BadStep=                   -13,  /**/
 CV_BadModelOrChSeq=           -14,  /**/
 CV_BadNumChannels=            -15,  /**/
 CV_BadNumChannel1U=           -16,  /**/
 CV_BadDepth=                  -17,  /**/
 CV_BadAlphaChannel=           -18,  /**/
 CV_BadOrder=                  -19,  /**/
 CV_BadOrigin=                 -20,  /**/
 CV_BadAlign=                  -21,  /**/
 CV_BadCallBack=               -22,  /**/
 CV_BadTileSize=               -23,  /**/
 CV_BadCOI=                    -24,  /**/
 CV_BadROISize=                -25,  /**/
 CV_MaskIsTiled=               -26,  /**/
 CV_StsNullPtr=                -27,  /**< null pointer */
 CV_StsVecLengthErr=           -28,  /**< incorrect vector length */
 CV_StsFilterStructContentErr= -29,  /**< incorr. filter structure content */
 CV_StsKernelStructContentErr= -30,  /**< incorr. transform kernel content */
 CV_StsFilterOffsetErr=        -31,  /**< incorrect filter offset value */
 CV_StsBadSize=                -201, /**< the input/output structure size is incorrect  */
 CV_StsDivByZero=              -202, /**< division by zero */
 CV_StsInplaceNotSupported=    -203, /**< in-place operation is not supported */
 CV_StsObjectNotFound=         -204, /**< request can't be completed */
 CV_StsUnmatchedFormats=       -205, /**< formats of input/output arrays differ */
 CV_StsBadFlag=                -206, /**< flag is wrong or not supported */
 CV_StsBadPoint=               -207, /**< bad CvPoint */
 CV_StsBadMask=                -208, /**< bad format of mask (neither 8uC1 nor 8sC1)*/
 CV_StsUnmatchedSizes=         -209, /**< sizes of input/output structures do not match */
 CV_StsUnsupportedFormat=      -210, /**< the data format/type is not supported by the function*/
 CV_StsOutOfRange=             -211, /**< some of parameters are out of range */
 CV_StsParseError=             -212, /**< invalid syntax/structure of the parsed file */
 CV_StsNotImplemented=         -213, /**< the requested function/feature is not implemented */
 CV_StsBadMemBlock=            -214, /**< an allocated block has been corrupted */
 CV_StsAssert=                 -215, /**< assertion failed */
 CV_GpuNotSupported=           -216,
 CV_GpuApiCallError=           -217,
 CV_OpenGlNotSupported=        -218,
 CV_OpenGlApiCallError=        -219,
 CV_OpenCLApiCallError=        -220,
 CV_OpenCLDoubleNotSupported=  -221,
 CV_OpenCLInitError=           -222,
 CV_OpenCLNoAMDBlasFft=        -223
};

#define CV_MAX_DIM            32
#define CV_MAX_DIM_HEAP       1024

#endif /*__OPENCV_CORE_TYPES_H__*/
