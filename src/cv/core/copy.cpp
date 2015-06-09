/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009-2011, Willow Garage Inc., all rights reserved.
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

/* ////////////////////////////////////////////////////////////////////
//
//  Mat basic operations: Copy, Set
//
// */

#include "precomp.hpp"
//#include "opencl_kernels_core.hpp"

namespace cv
{

template<typename T> static void
copyMask_(const uchar* _src, size_t sstep, const uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    for( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        const T* src = (const T*)_src;
        T* dst = (T*)_dst;
        int x = 0;
         #if CV_ENABLE_UNROLLED
        for( ; x <= size.width - 4; x += 4 )
        {
            if( mask[x] )
                dst[x] = src[x];
            if( mask[x+1] )
                dst[x+1] = src[x+1];
            if( mask[x+2] )
                dst[x+2] = src[x+2];
            if( mask[x+3] )
                dst[x+3] = src[x+3];
        }
        #endif
        for( ; x < size.width; x++ )
            if( mask[x] )
                dst[x] = src[x];
    }
}

template<> void
copyMask_<uchar>(const uchar* _src, size_t sstep, const uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    for( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        const uchar* src = (const uchar*)_src;
        uchar* dst = (uchar*)_dst;
        int x = 0;
        #if CV_SSE4_2
        if(USE_SSE4_2)//
        {
            __m128i zero = _mm_setzero_si128 ();

             for( ; x <= size.width - 16; x += 16 )
             {
                 const __m128i rSrc = _mm_lddqu_si128((const __m128i*)(src+x));
                 __m128i _mask = _mm_lddqu_si128((const __m128i*)(mask+x));
                 __m128i rDst = _mm_lddqu_si128((__m128i*)(dst+x));
                 __m128i _negMask = _mm_cmpeq_epi8(_mask, zero);
                 rDst = _mm_blendv_epi8(rSrc, rDst, _negMask);
                 _mm_storeu_si128((__m128i*)(dst + x), rDst);
             }
        }
        #elif CV_NEON
        uint8x16_t v_one = vdupq_n_u8(1);
        for( ; x <= size.width - 16; x += 16 )
        {
            uint8x16_t v_mask = vcgeq_u8(vld1q_u8(mask + x), v_one);
            uint8x16_t v_dst = vld1q_u8(dst + x), v_src = vld1q_u8(src + x);
            vst1q_u8(dst + x, vbslq_u8(v_mask, v_src, v_dst));
        }
        #endif
        for( ; x < size.width; x++ )
            if( mask[x] )
                dst[x] = src[x];
    }
}

template<> void
copyMask_<ushort>(const uchar* _src, size_t sstep, const uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{

    for( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        const ushort* src = (const ushort*)_src;
        ushort* dst = (ushort*)_dst;
        int x = 0;
        #if CV_SSE4_2
        if(USE_SSE4_2)//
        {
            __m128i zero = _mm_setzero_si128 ();
            for( ; x <= size.width - 8; x += 8 )
            {
                 const __m128i rSrc =_mm_lddqu_si128((const __m128i*)(src+x));
                 __m128i _mask = _mm_loadl_epi64((const __m128i*)(mask+x));
                 _mask = _mm_unpacklo_epi8(_mask, _mask);
                 __m128i rDst = _mm_lddqu_si128((const __m128i*)(dst+x));
                 __m128i _negMask = _mm_cmpeq_epi8(_mask, zero);
                 rDst = _mm_blendv_epi8(rSrc, rDst, _negMask);
                 _mm_storeu_si128((__m128i*)(dst + x), rDst);
             }
        }
        #elif CV_NEON
        uint8x8_t v_one = vdup_n_u8(1);
        for( ; x <= size.width - 8; x += 8 )
        {
            uint8x8_t v_mask = vcge_u8(vld1_u8(mask + x), v_one);
            uint8x8x2_t v_mask2 = vzip_u8(v_mask, v_mask);
            uint16x8_t v_mask_res = vreinterpretq_u16_u8(vcombine_u8(v_mask2.val[0], v_mask2.val[1]));

            uint16x8_t v_src = vld1q_u16(src + x), v_dst = vld1q_u16(dst + x);
            vst1q_u16(dst + x, vbslq_u16(v_mask_res, v_src, v_dst));
        }
        #endif
        for( ; x < size.width; x++ )
            if( mask[x] )
                dst[x] = src[x];
    }
}

static void
copyMaskGeneric(const uchar* _src, size_t sstep, const uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size, void* _esz)
{
    size_t k, esz = *(size_t*)_esz;
    for( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        const uchar* src = _src;
        uchar* dst = _dst;
        int x = 0;
        for( ; x < size.width; x++, src += esz, dst += esz )
        {
            if( !mask[x] )
                continue;
            for( k = 0; k < esz; k++ )
                dst[k] = src[k];
        }
    }
}


#define DEF_COPY_MASK(suffix, type) \
static void copyMask##suffix(const uchar* src, size_t sstep, const uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, void*) \
{ \
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}

#define DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
static void copyMask##suffix(const uchar* src, size_t sstep, const uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, void*) \
{ \
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}


DEF_COPY_MASK(8u, uchar)
DEF_COPY_MASK(16u, ushort)
DEF_COPY_MASK_F(8uC3, Vec3b, 8u_C3MR, Ipp8u)
DEF_COPY_MASK_F(32s, int, 32s_C1MR, Ipp32s)
DEF_COPY_MASK_F(16uC3, Vec3s, 16u_C3MR, Ipp16u)
DEF_COPY_MASK(32sC2, Vec2i)
DEF_COPY_MASK_F(32sC3, Vec3i, 32s_C3MR, Ipp32s)
DEF_COPY_MASK_F(32sC4, Vec4i, 32s_C4MR, Ipp32s)
DEF_COPY_MASK(32sC6, Vec6i)
DEF_COPY_MASK(32sC8, Vec8i)

BinaryFunc copyMaskTab[] =
{
    0,
    copyMask8u,
    copyMask16u,
    copyMask8uC3,
    copyMask32s,
    0,
    copyMask16uC3,
    0,
    copyMask32sC2,
    0, 0, 0,
    copyMask32sC3,
    0, 0, 0,
    copyMask32sC4,
    0, 0, 0, 0, 0, 0, 0,
    copyMask32sC6,
    0, 0, 0, 0, 0, 0, 0,
    copyMask32sC8
};

BinaryFunc getCopyMaskFunc(size_t esz)
{
    return esz <= 32 && copyMaskTab[esz] ? copyMaskTab[esz] : copyMaskGeneric;
}

/* dst = src */
void Mat::copyTo( OutputArray _dst ) const
{
    int dtype = _dst.type();
    if( _dst.fixedType() && dtype != type() )
    {
        CV_Assert( channels() == CV_MAT_CN(dtype) );
        convertTo( _dst, dtype );
        return;
    }

    if( empty() )
    {
        _dst.release();
        return;
    }

    if( _dst.isUMat() )
    {
        _dst.create( dims, size.p, type() );
        UMat dst = _dst.getUMat();

        size_t i, sz[CV_MAX_DIM], dstofs[CV_MAX_DIM], esz = elemSize();
        for( i = 0; i < (size_t)dims; i++ )
            sz[i] = size.p[i];
        sz[dims-1] *= esz;
        dst.ndoffset(dstofs);
        dstofs[dims-1] *= esz;
        dst.u->currAllocator->upload(dst.u, data, dims, sz, dstofs, dst.step.p, step.p);
        return;
    }

    if( dims <= 2 )
    {
        _dst.create( rows, cols, type() );
        Mat dst = _dst.getMat();
        if( data == dst.data )
            return;

        if( rows > 0 && cols > 0 )
        {
            const uchar* sptr = data;
            uchar* dptr = dst.data;

            Size sz = getContinuousSize(*this, dst);
            size_t len = sz.width*elemSize();

            for( ; sz.height--; sptr += step, dptr += dst.step )
                memcpy( dptr, sptr, len );
        }
        return;
    }

    _dst.create( dims, size, type() );
    Mat dst = _dst.getMat();
    if( data == dst.data )
        return;

    if( total() != 0 )
    {
        const Mat* arrays[] = { this, &dst };
        uchar* ptrs[2];
        NAryMatIterator it(arrays, ptrs, 2);
        size_t sz = it.size*elemSize();

        for( size_t i = 0; i < it.nplanes; i++, ++it )
            memcpy(ptrs[1], ptrs[0], sz);
    }
}

void Mat::copyTo( OutputArray _dst, InputArray _mask ) const
{
    Mat mask = _mask.getMat();
    if( !mask.data )
    {
        copyTo(_dst);
        return;
    }

    int cn = channels(), mcn = mask.channels();
    CV_Assert( mask.depth() == CV_8U && (mcn == 1 || mcn == cn) );
    bool colorMask = mcn > 1;

    size_t esz = colorMask ? elemSize1() : elemSize();
    BinaryFunc copymask = getCopyMaskFunc(esz);

    uchar* data0 = _dst.getMat().data;
    _dst.create( dims, size, type() );
    Mat dst = _dst.getMat();

    if( dst.data != data0 ) // do not leave dst uninitialized
        dst = Scalar(0);

    if( dims <= 2 )
    {
        CV_Assert( size() == mask.size() );
        Size sz = getContinuousSize(*this, dst, mask, mcn);
        copymask(data, step, mask.data, mask.step, dst.data, dst.step, sz, &esz);
        return;
    }

    const Mat* arrays[] = { this, &dst, &mask, 0 };
    uchar* ptrs[3];
    NAryMatIterator it(arrays, ptrs);
    Size sz((int)(it.size*mcn), 1);

    for( size_t i = 0; i < it.nplanes; i++, ++it )
        copymask(ptrs[0], 0, ptrs[2], 0, ptrs[1], 0, sz, &esz);
}

Mat& Mat::operator = (const Scalar& s)
{
    const Mat* arrays[] = { this };
    uchar* dptr;
    NAryMatIterator it(arrays, &dptr, 1);
    size_t elsize = it.size*elemSize();
    const int64* is = (const int64*)&s.val[0];

    if( is[0] == 0 && is[1] == 0 && is[2] == 0 && is[3] == 0 )
    {

        for( size_t i = 0; i < it.nplanes; i++, ++it )
            memset( dptr, 0, elsize );
    }
    else
    {
        if( it.nplanes > 0 )
        {
            double scalar[12];
            scalarToRawData(s, scalar, type(), 12);
            size_t blockSize = 12*elemSize1();

            for( size_t j = 0; j < elsize; j += blockSize )
            {
                size_t sz = MIN(blockSize, elsize - j);
                memcpy( dptr + j, scalar, sz );
            }
        }

        for( size_t i = 1; i < it.nplanes; i++ )
        {
            ++it;
            memcpy( dptr, data, elsize );
        }
    }
    return *this;
}
} // cv
