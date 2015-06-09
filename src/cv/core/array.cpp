/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                        Intel License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
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
//   * The name of Intel Corporation may not be used to endorse or promote products
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

/* ////////////////////////////////////////////////////////////////////
//
//  CvMat, CvMatND, CvSparceMat and IplImage support functions
//  (creation, deletion, copying, retrieving and setting elements etc.)
//
// */

#include "precomp.hpp"

#define  CV_ORIGIN_TL  0
#define  CV_ORIGIN_BL  1

/* default image row align (in bytes) */
#define  CV_DEFAULT_IMAGE_ROW_ALIGN  4


//CvSparseNode*
//cvInitSparseMatIterator( const CvSparseMat* mat, CvSparseMatIterator* iterator )
//{
//    CvSparseNode* node = 0;
//    int idx;

//    if( !CV_IS_SPARSE_MAT( mat ))
//        CV_Error( CV_StsBadArg, "Invalid sparse matrix header" );

//    if( !iterator )
//        CV_Error( CV_StsNullPtr, "NULL iterator pointer" );

//    iterator->mat = (CvSparseMat*)mat;
//    iterator->node = 0;

//    for( idx = 0; idx < mat->hashsize; idx++ )
//        if( mat->hashtable[idx] )
//        {
//            node = iterator->node = (CvSparseNode*)mat->hashtable[idx];
//            break;
//        }

//    iterator->curidx = idx;
//    return node;
//}

CV_IMPL int
cvGetImageCOI( const IplImage* image )
{
    if( !image )
        CV_Error( CV_HeaderIsNull, "" );

    return image->roi ? image->roi->coi : 0;
}

/* End of file. */
