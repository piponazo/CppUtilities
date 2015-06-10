/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
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

#include "precomp.hpp"
//#include "opencl_kernels_core.hpp"

#include "bufferpool.impl.hpp"

/****************************************************************************************\
*                           [scaled] Identity matrix initialization                      *
\****************************************************************************************/

namespace minicv {

void MatAllocator::map(UMatData*, int) const
{
}

void MatAllocator::unmap(UMatData* u) const
{
    if(u->urefcount == 0 && u->refcount == 0)
    {
        deallocate(u);
        u = NULL;
    }
}

void MatAllocator::download(UMatData* u, void* dstptr,
         int dims, const size_t sz[],
         const size_t srcofs[], const size_t srcstep[],
         const size_t dststep[]) const
{
    if(!u)
        return;
    int isz[CV_MAX_DIM];
    uchar* srcptr = u->data;
    for( int i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        if( sz[i] == 0 )
        return;
        if( srcofs )
        srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        isz[i] = (int)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    const Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t j, planesz = it.size;

    for( j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}


void MatAllocator::upload(UMatData* u, const void* srcptr, int dims, const size_t sz[],
                    const size_t dstofs[], const size_t dststep[],
                    const size_t srcstep[]) const
{
    if(!u)
        return;
    int isz[CV_MAX_DIM];
    uchar* dstptr = u->data;
    for( int i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        if( sz[i] == 0 )
        return;
        if( dstofs )
        dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (int)sz[i];
    }

    Mat src(dims, isz, CV_8U, (void*)srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    const Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t j, planesz = it.size;

    for( j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}

void MatAllocator::copy(UMatData* usrc, UMatData* udst, int dims, const size_t sz[],
                  const size_t srcofs[], const size_t srcstep[],
                  const size_t dstofs[], const size_t dststep[], bool /*sync*/) const
{
    if(!usrc || !udst)
        return;
    int isz[CV_MAX_DIM];
    uchar* srcptr = usrc->data;
    uchar* dstptr = udst->data;
    for( int i = 0; i < dims; i++ )
    {
        CV_Assert( sz[i] <= (size_t)INT_MAX );
        if( sz[i] == 0 )
            return;
        if( srcofs )
            srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
        if( dstofs )
            dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
        isz[i] = (int)sz[i];
    }

    Mat src(dims, isz, CV_8U, srcptr, srcstep);
    Mat dst(dims, isz, CV_8U, dstptr, dststep);

    const Mat* arrays[] = { &src, &dst };
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs, 2);
    size_t j, planesz = it.size;

    for( j = 0; j < it.nplanes; j++, ++it )
        memcpy(ptrs[1], ptrs[0], planesz);
}

BufferPoolController* MatAllocator::getBufferPoolController(const char* id) const
{
    (void)id;
    static DummyBufferPoolController dummy;
    return &dummy;
}

class StdMatAllocator : public MatAllocator
{
public:
    UMatData* allocate(int dims, const int* sizes, int type,
                       void* data0, size_t* step, int /*flags*/, UMatUsageFlags /*usageFlags*/) const
    {
        size_t total = CV_ELEM_SIZE(type);
        for( int i = dims-1; i >= 0; i-- )
        {
            if( step )
            {
                if( data0 && step[i] != CV_AUTOSTEP )
                {
                    CV_Assert(total <= step[i]);
                    total = step[i];
                }
                else
                    step[i] = total;
            }
            total *= sizes[i];
        }
        uchar* data = data0 ? (uchar*)data0 : (uchar*)fastMalloc(total);
        UMatData* u = new UMatData(this);
        u->data = u->origdata = data;
        u->size = total;
        if(data0)
            u->flags |= UMatData::USER_ALLOCATED;

        return u;
    }

    bool allocate(UMatData* u, int /*accessFlags*/, UMatUsageFlags /*usageFlags*/) const
    {
        if(!u) return false;
        return true;
    }

    void deallocate(UMatData* u) const
    {
        if(!u)
            return;

        CV_Assert(u->urefcount >= 0);
        CV_Assert(u->refcount >= 0);
        if(u->refcount == 0)
        {
            if( !(u->flags & UMatData::USER_ALLOCATED) )
            {
                fastFree(u->origdata);
                u->origdata = 0;
            }
            delete u;
        }
    }
};

MatAllocator* Mat::getStdAllocator()
{
    static StdMatAllocator allocator;
    return &allocator;
}

void swap( Mat& a, Mat& b )
{
    std::swap(a.flags, b.flags);
    std::swap(a.dims, b.dims);
    std::swap(a.rows, b.rows);
    std::swap(a.cols, b.cols);
    std::swap(a.data, b.data);
    std::swap(a.datastart, b.datastart);
    std::swap(a.dataend, b.dataend);
    std::swap(a.datalimit, b.datalimit);
    std::swap(a.allocator, b.allocator);
    std::swap(a.u, b.u);

    std::swap(a.size.p, b.size.p);
    std::swap(a.step.p, b.step.p);
    std::swap(a.step.buf[0], b.step.buf[0]);
    std::swap(a.step.buf[1], b.step.buf[1]);

    if( a.step.p == b.step.buf )
    {
        a.step.p = a.step.buf;
        a.size.p = &a.rows;
    }

    if( b.step.p == a.step.buf )
    {
        b.step.p = b.step.buf;
        b.size.p = &b.rows;
    }
}


static inline void setSize( Mat& m, int _dims, const int* _sz,
                            const size_t* _steps, bool autoSteps=false )
{
    CV_Assert( 0 <= _dims && _dims <= CV_MAX_DIM );
    if( m.dims != _dims )
    {
        if( m.step.p != m.step.buf )
        {
            fastFree(m.step.p);
            m.step.p = m.step.buf;
            m.size.p = &m.rows;
        }
        if( _dims > 2 )
        {
            m.step.p = (size_t*)fastMalloc(_dims*sizeof(m.step.p[0]) + (_dims+1)*sizeof(m.size.p[0]));
            m.size.p = (int*)(m.step.p + _dims) + 1;
            m.size.p[-1] = _dims;
            m.rows = m.cols = -1;
        }
    }

    m.dims = _dims;
    if( !_sz )
        return;

    size_t esz = CV_ELEM_SIZE(m.flags), esz1 = CV_ELEM_SIZE1(m.flags), total = esz;
    int i;
    for( i = _dims-1; i >= 0; i-- )
    {
        int s = _sz[i];
        CV_Assert( s >= 0 );
        m.size.p[i] = s;

        if( _steps )
        {
            if (_steps[i] % esz1 != 0)
            {
                CV_Error(Error::BadStep, "Step must be a multiple of esz1");
            }

            m.step.p[i] = i < _dims-1 ? _steps[i] : esz;
        }
        else if( autoSteps )
        {
            m.step.p[i] = total;
            int64 total1 = (int64)total*s;
            if( (uint64)total1 != (size_t)total1 )
                CV_Error( CV_StsOutOfRange, "The total matrix size does not fit to \"size_t\" type" );
            total = (size_t)total1;
        }
    }

    if( _dims == 1 )
    {
        m.dims = 2;
        m.cols = 1;
        m.step[1] = esz;
    }
}

static void updateContinuityFlag(Mat& m)
{
    int i, j;
    for( i = 0; i < m.dims; i++ )
    {
        if( m.size[i] > 1 )
            break;
    }

    for( j = m.dims-1; j > i; j-- )
    {
        if( m.step[j]*m.size[j] < m.step[j-1] )
            break;
    }

    uint64 t = (uint64)m.step[0]*m.size[0];
    if( j <= i && t == (size_t)t )
        m.flags |= Mat::CONTINUOUS_FLAG;
    else
        m.flags &= ~Mat::CONTINUOUS_FLAG;
}

static void finalizeHdr(Mat& m)
{
    updateContinuityFlag(m);
    int d = m.dims;
    if( d > 2 )
        m.rows = m.cols = -1;
    if(m.u)
        m.datastart = m.data = m.u->data;
    if( m.data )
    {
        m.datalimit = m.datastart + m.size[0]*m.step[0];
        if( m.size[0] > 0 )
        {
            m.dataend = m.ptr() + m.size[d-1]*m.step[d-1];
            for( int i = 0; i < d-1; i++ )
                m.dataend += (m.size[i] - 1)*m.step[i];
        }
        else
            m.dataend = m.datalimit;
    }
    else
        m.dataend = m.datalimit = 0;
}


void Mat::create(int d, const int* _sizes, int _type)
{
    int i;
    CV_Assert(0 <= d && d <= CV_MAX_DIM && _sizes);
    _type = CV_MAT_TYPE(_type);

    if( data && (d == dims || (d == 1 && dims <= 2)) && _type == type() )
    {
        if( d == 2 && rows == _sizes[0] && cols == _sizes[1] )
            return;
        for( i = 0; i < d; i++ )
            if( size[i] != _sizes[i] )
                break;
        if( i == d && (d > 1 || size[1] == 1))
            return;
    }

    release();
    if( d == 0 )
        return;
    flags = (_type & CV_MAT_TYPE_MASK) | MAGIC_VAL;
    setSize(*this, d, _sizes, 0, true);

    if( total() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getStdAllocator();
#ifdef HAVE_TGPU
        if( !a || a == tegra::getAllocator() )
            a = tegra::getAllocator(d, _sizes, _type);
#endif
        if(!a)
            a = a0;
        try
        {
            u = a->allocate(dims, size, _type, 0, step.p, 0, USAGE_DEFAULT);
            CV_Assert(u != 0);
        }
        catch(...)
        {
            if(a != a0)
                u = a0->allocate(dims, size, _type, 0, step.p, 0, USAGE_DEFAULT);
            CV_Assert(u != 0);
        }
        CV_Assert( step[dims-1] == (size_t)CV_ELEM_SIZE(flags) );
    }

    addref();
    finalizeHdr(*this);
}

void Mat::copySize(const Mat& m)
{
    setSize(*this, m.dims, 0, 0);
    for( int i = 0; i < dims; i++ )
    {
        size[i] = m.size[i];
        step[i] = m.step[i];
    }
}

void Mat::deallocate()
{
    if(u)
        (u->currAllocator ? u->currAllocator : allocator ? allocator : getStdAllocator())->unmap(u);
    u = NULL;
}

Mat::Mat(const Mat& m, const Range& _rowRange, const Range& _colRange)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0), size(&rows)
{
    CV_Assert( m.dims >= 2 );
    if( m.dims > 2 )
    {
        AutoBuffer<Range> rs(m.dims);
        rs[0] = _rowRange;
        rs[1] = _colRange;
        for( int i = 2; i < m.dims; i++ )
            rs[i] = Range::all();
        *this = m(rs);
        return;
    }

    *this = m;
    if( _rowRange != Range::all() && _rowRange != Range(0,rows) )
    {
        CV_Assert( 0 <= _rowRange.start && _rowRange.start <= _rowRange.end && _rowRange.end <= m.rows );
        rows = _rowRange.size();
        data += step*_rowRange.start;
        flags |= SUBMATRIX_FLAG;
    }

    if( _colRange != Range::all() && _colRange != Range(0,cols) )
    {
        CV_Assert( 0 <= _colRange.start && _colRange.start <= _colRange.end && _colRange.end <= m.cols );
        cols = _colRange.size();
        data += _colRange.start*elemSize();
        flags &= cols < m.cols ? ~CONTINUOUS_FLAG : -1;
        flags |= SUBMATRIX_FLAG;
    }

    if( rows == 1 )
        flags |= CONTINUOUS_FLAG;

    if( rows <= 0 || cols <= 0 )
    {
        release();
        rows = cols = 0;
    }
}


Mat::Mat(const Mat& m, const Rect& roi)
    : flags(m.flags), dims(2), rows(roi.height), cols(roi.width),
    data(m.data + roi.y*m.step[0]),
    datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit),
    allocator(m.allocator), u(m.u), size(&rows)
{
    CV_Assert( m.dims <= 2 );
    flags &= roi.width < m.cols ? ~CONTINUOUS_FLAG : -1;
    flags |= roi.height == 1 ? CONTINUOUS_FLAG : 0;

    size_t esz = CV_ELEM_SIZE(flags);
    data += roi.x*esz;
    CV_Assert( 0 <= roi.x && 0 <= roi.width && roi.x + roi.width <= m.cols &&
              0 <= roi.y && 0 <= roi.height && roi.y + roi.height <= m.rows );
    if( u )
        CV_XADD(&u->refcount, 1);
    if( roi.width < m.cols || roi.height < m.rows )
        flags |= SUBMATRIX_FLAG;

    step[0] = m.step[0]; step[1] = esz;

    if( rows <= 0 || cols <= 0 )
    {
        release();
        rows = cols = 0;
    }
}


Mat::Mat(int _dims, const int* _sizes, int _type, void* _data, const size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0), size(&rows)
{
    flags |= CV_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    setSize(*this, _dims, _sizes, _steps, true);
    finalizeHdr(*this);
}


Mat::Mat(const Mat& m, const Range* ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0), size(&rows)
{
    int i, d = m.dims;

    CV_Assert(ranges);
    for( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        CV_Assert( r == Range::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]) );
    }
    *this = m;
    for( i = 0; i < d; i++ )
    {
        Range r = ranges[i];
        if( r != Range::all() && r != Range(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            data += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }
    updateContinuityFlag(*this);
}

void Mat::pop_back(size_t nelems)
{
    CV_Assert( nelems <= (size_t)size.p[0] );

    if( isSubmatrix() )
        *this = rowRange(0, size.p[0] - (int)nelems);
    else
    {
        size.p[0] -= (int)nelems;
        dataend -= nelems*step.p[0];
        /*if( size.p[0] <= 1 )
        {
            if( dims <= 2 )
                flags |= CONTINUOUS_FLAG;
            else
                updateContinuityFlag(*this);
        }*/
    }
}


void Mat::push_back_(const void* elem)
{
    int r = size.p[0];
    if( isSubmatrix() || dataend + step.p[0] > datalimit )
        reserve( std::max(r + 1, (r*3+1)/2) );

    size_t esz = elemSize();
    memcpy(data + r*step.p[0], elem, esz);
    size.p[0] = r + 1;
    dataend += step.p[0];
    if( esz < step.p[0] )
        flags &= ~CONTINUOUS_FLAG;
}

void Mat::reserve(size_t nelems)
{
    const size_t MIN_SIZE = 64;

    CV_Assert( (int)nelems >= 0 );
    if( !isSubmatrix() && data + step.p[0]*nelems <= datalimit )
        return;

    int r = size.p[0];

    if( (size_t)r >= nelems )
        return;

    size.p[0] = std::max((int)nelems, 1);
    size_t newsize = total()*elemSize();

    if( newsize < MIN_SIZE )
        size.p[0] = (int)((MIN_SIZE + newsize - 1)*nelems/newsize);

    Mat m(dims, size.p, type());
    size.p[0] = r;
    if( r > 0 )
    {
        Mat mpart = m.rowRange(0, r);
        copyTo(mpart);
    }

    *this = m;
    size.p[0] = r;
    dataend = data + step.p[0]*r;
}


void Mat::resize(size_t nelems)
{
    int saveRows = size.p[0];
    if( saveRows == (int)nelems )
        return;
    CV_Assert( (int)nelems >= 0 );

    if( isSubmatrix() || data + step.p[0]*nelems > datalimit )
        reserve(nelems);

    size.p[0] = (int)nelems;
    dataend += (size.p[0] - saveRows)*step.p[0];

    //updateContinuityFlag(*this);
}


//void Mat::resize(size_t nelems, const Scalar& s)
//{
//    int saveRows = size.p[0];
//    resize(nelems);

//    if( size.p[0] > saveRows )
//    {
//        Mat part = rowRange(saveRows, size.p[0]);
//        part = s;
//    }
//}

void Mat::push_back(const Mat& elems)
{
    int r = size.p[0], delta = elems.size.p[0];
    if( delta == 0 )
        return;
    if( this == &elems )
    {
        Mat tmp = elems;
        push_back(tmp);
        return;
    }
    if( !data )
    {
        *this = elems.clone();
        return;
    }

    size.p[0] = elems.size.p[0];
    bool eq = size == elems.size;
    size.p[0] = r;
    if( !eq )
        CV_Error(CV_StsUnmatchedSizes, "");
    if( type() != elems.type() )
        CV_Error(CV_StsUnmatchedFormats, "");

    if( isSubmatrix() || dataend + step.p[0]*delta > datalimit )
        reserve( std::max(r + delta, (r*3+1)/2) );

    size.p[0] += delta;
    dataend += step.p[0]*delta;

    //updateContinuityFlag(*this);

    if( isContinuous() && elems.isContinuous() )
        memcpy(data + r*step.p[0], elems.data, elems.total()*elems.elemSize());
    else
    {
        Mat part = rowRange(r, r + delta);
        elems.copyTo(part);
    }
}

}

namespace minicv
{

void scalarToRawData(const Scalar& s, void* _buf, int type, int unroll_to)
{
    int i, depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert(cn <= 4);
    switch(depth)
    {
    case CV_8U:
        {
        uchar* buf = (uchar*)_buf;
        for(i = 0; i < cn; i++)
            buf[i] = saturate_cast<uchar>(s.val[i]);
        for(; i < unroll_to; i++)
            buf[i] = buf[i-cn];
        }
        break;
    case CV_8S:
        {
        schar* buf = (schar*)_buf;
        for(i = 0; i < cn; i++)
            buf[i] = saturate_cast<schar>(s.val[i]);
        for(; i < unroll_to; i++)
            buf[i] = buf[i-cn];
        }
        break;
    case CV_16U:
        {
        ushort* buf = (ushort*)_buf;
        for(i = 0; i < cn; i++)
            buf[i] = saturate_cast<ushort>(s.val[i]);
        for(; i < unroll_to; i++)
            buf[i] = buf[i-cn];
        }
        break;
    case CV_16S:
        {
        short* buf = (short*)_buf;
        for(i = 0; i < cn; i++)
            buf[i] = saturate_cast<short>(s.val[i]);
        for(; i < unroll_to; i++)
            buf[i] = buf[i-cn];
        }
        break;
    case CV_32S:
        {
        int* buf = (int*)_buf;
        for(i = 0; i < cn; i++)
            buf[i] = saturate_cast<int>(s.val[i]);
        for(; i < unroll_to; i++)
            buf[i] = buf[i-cn];
        }
        break;
    case CV_32F:
        {
        float* buf = (float*)_buf;
        for(i = 0; i < cn; i++)
            buf[i] = saturate_cast<float>(s.val[i]);
        for(; i < unroll_to; i++)
            buf[i] = buf[i-cn];
        }
        break;
    case CV_64F:
        {
        double* buf = (double*)_buf;
        for(i = 0; i < cn; i++)
            buf[i] = saturate_cast<double>(s.val[i]);
        for(; i < unroll_to; i++)
            buf[i] = buf[i-cn];
        break;
        }
    default:
        CV_Error(CV_StsUnsupportedFormat,"");
    }
}


/*************************************************************************************************\
                                        Input/Output Array
\*************************************************************************************************/

Mat _InputArray::getMat_(int i) const
{
    int k = kind();
//    int accessFlags = flags & ACCESS_MASK;

    if( k == MAT )
    {
        const Mat* m = (const Mat*)obj;
        if( i < 0 )
            return *m;
        return m->row(i);
    }

//    if( k == UMAT )
//    {
//        const UMat* m = (const UMat*)obj;
//        if( i < 0 )
//            return m->getMat(accessFlags);
//        return m->getMat(accessFlags).row(i);
//    }

//    if( k == EXPR )
//    {
//        CV_Assert( i < 0 );
//        return (Mat)*((const MatExpr*)obj);
//    }

    if( k == MATX )
    {
        CV_Assert( i < 0 );
        return Mat(sz, flags, obj);
    }

    if( k == STD_VECTOR )
    {
        CV_Assert( i < 0 );
        int t = CV_MAT_TYPE(flags);
        const std::vector<uchar>& v = *(const std::vector<uchar>*)obj;

        return !v.empty() ? Mat(size(), t, (void*)&v[0]) : Mat();
    }

    if( k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        int t = CV_8U;
        const std::vector<bool>& v = *(const std::vector<bool>*)obj;
        int j, n = (int)v.size();
        if( n == 0 )
            return Mat();
        Mat m(1, n, t);
        uchar* dst = m.data;
        for( j = 0; j < n; j++ )
            dst[j] = (uchar)v[j];
        return m;
    }

    if( k == NONE )
        return Mat();

    if( k == STD_VECTOR_VECTOR )
    {
        int t = type(i);
        const std::vector<std::vector<uchar> >& vv = *(const std::vector<std::vector<uchar> >*)obj;
        CV_Assert( 0 <= i && i < (int)vv.size() );
        const std::vector<uchar>& v = vv[i];

        return !v.empty() ? Mat(size(i), t, (void*)&v[0]) : Mat();
    }

    if( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        CV_Assert( 0 <= i && i < (int)v.size() );

        return v[i];
    }

//    if( k == STD_VECTOR_UMAT )
//    {
//        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
//        CV_Assert( 0 <= i && i < (int)v.size() );

//        return v[i].getMat(accessFlags);
//    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
    return Mat();
}

void _InputArray::getMatVector(std::vector<Mat>& mv) const
{
    int k = kind();
//    int accessFlags = flags & ACCESS_MASK;

    if( k == MAT )
    {
        const Mat& m = *(const Mat*)obj;
        int i, n = (int)m.size[0];
        mv.resize(n);

        for( i = 0; i < n; i++ )
            mv[i] = m.dims == 2 ? Mat(1, m.cols, m.type(), (void*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (void*)m.ptr(i), &m.step[1]);
        return;
    }

//    if( k == EXPR )
//    {
//        Mat m = *(const MatExpr*)obj;
//        int i, n = m.size[0];
//        mv.resize(n);

//        for( i = 0; i < n; i++ )
//            mv[i] = m.row(i);
//        return;
//    }

    if( k == MATX )
    {
        size_t i, n = sz.height, esz = CV_ELEM_SIZE(flags);
        mv.resize(n);

        for( i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.width, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.width*i);
        return;
    }

    if( k == STD_VECTOR )
    {
        const std::vector<uchar>& v = *(const std::vector<uchar>*)obj;

        size_t i, n = v.size(), esz = CV_ELEM_SIZE(flags);
        int t = CV_MAT_DEPTH(flags), cn = CV_MAT_CN(flags);
        mv.resize(n);

        for( i = 0; i < n; i++ )
            mv[i] = Mat(1, cn, t, (void*)(&v[0] + esz*i));
        return;
    }

    if( k == NONE )
    {
        mv.clear();
        return;
    }

    if( k == STD_VECTOR_VECTOR )
    {
        const std::vector<std::vector<uchar> >& vv = *(const std::vector<std::vector<uchar> >*)obj;
        int i, n = (int)vv.size();
        int t = CV_MAT_TYPE(flags);
        mv.resize(n);

        for( i = 0; i < n; i++ )
        {
            const std::vector<uchar>& v = vv[i];
            mv[i] = Mat(size(i), t, (void*)&v[0]);
        }
        return;
    }

    if( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        size_t i, n = v.size();
        mv.resize(n);

        for( i = 0; i < n; i++ )
            mv[i] = v[i];
        return;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

//void _InputArray::getUMatVector(std::vector<UMat>& umv) const
//{
//    int k = kind();
//    int accessFlags = flags & ACCESS_MASK;

//    if( k == NONE )
//    {
//        umv.clear();
//        return;
//    }

//    if( k == STD_VECTOR_MAT )
//    {
//        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
//        size_t i, n = v.size();
//        umv.resize(n);

//        for( i = 0; i < n; i++ )
//            umv[i] = v[i].getUMat(accessFlags);
//        return;
//    }

//    if( k == STD_VECTOR_UMAT )
//    {
//        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
//        size_t i, n = v.size();
//        umv.resize(n);

//        for( i = 0; i < n; i++ )
//            umv[i] = v[i];
//        return;
//    }

//    if( k == UMAT )
//    {
//        UMat& v = *(UMat*)obj;
//        umv.resize(1);
//        umv[0] = v;
//        return;
//    }
//    if( k == MAT )
//    {
//        Mat& v = *(Mat*)obj;
//        umv.resize(1);
//        umv[0] = v.getUMat(accessFlags);
//        return;
//    }

//    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
//}

int _InputArray::kind() const
{
    return flags & KIND_MASK;
}

int _InputArray::rows(int i) const
{
    return size(i).height;
}

int _InputArray::cols(int i) const
{
    return size(i).width;
}

Size _InputArray::size(int i) const
{
    int k = kind();

    if( k == MAT )
    {
        CV_Assert( i < 0 );
        return ((const Mat*)obj)->size();
    }

//    if( k == EXPR )
//    {
//        CV_Assert( i < 0 );
//        return ((const MatExpr*)obj)->size();
//    }

    if( k == UMAT )
    {
        CV_Assert( i < 0 );
        return ((const UMat*)obj)->size();
    }

    if( k == MATX )
    {
        CV_Assert( i < 0 );
        return sz;
    }

    if( k == STD_VECTOR )
    {
        CV_Assert( i < 0 );
        const std::vector<uchar>& v = *(const std::vector<uchar>*)obj;
        const std::vector<int>& iv = *(const std::vector<int>*)obj;
        size_t szb = v.size(), szi = iv.size();
        return szb == szi ? Size((int)szb, 1) : Size((int)(szb/CV_ELEM_SIZE(flags)), 1);
    }

    if( k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        const std::vector<bool>& v = *(const std::vector<bool>*)obj;
        return Size((int)v.size(), 1);
    }

    if( k == NONE )
        return Size();

    if( k == STD_VECTOR_VECTOR )
    {
        const std::vector<std::vector<uchar> >& vv = *(const std::vector<std::vector<uchar> >*)obj;
        if( i < 0 )
            return vv.empty() ? Size() : Size((int)vv.size(), 1);
        CV_Assert( i < (int)vv.size() );
        const std::vector<std::vector<int> >& ivv = *(const std::vector<std::vector<int> >*)obj;

        size_t szb = vv[i].size(), szi = ivv[i].size();
        return szb == szi ? Size((int)szb, 1) : Size((int)(szb/CV_ELEM_SIZE(flags)), 1);
    }

    if( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        if( i < 0 )
            return vv.empty() ? Size() : Size((int)vv.size(), 1);
        CV_Assert( i < (int)vv.size() );

        return vv[i].size();
    }

    if( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        if( i < 0 )
            return vv.empty() ? Size() : Size((int)vv.size(), 1);
        CV_Assert( i < (int)vv.size() );

        return vv[i].size();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
    return Size();
}

int _InputArray::sizend(int* arrsz, int i) const
{
    int j, d=0, k = kind();

    if( k == NONE )
        ;
    else if( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat& m = *(const Mat*)obj;
        d = m.dims;
        if(arrsz)
            for(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    else if( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat& m = *(const UMat*)obj;
        d = m.dims;
        if(arrsz)
            for(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    else if( k == STD_VECTOR_MAT && i >= 0 )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i < (int)vv.size() );
        const Mat& m = vv[i];
        d = m.dims;
        if(arrsz)
            for(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    else if( k == STD_VECTOR_UMAT && i >= 0 )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert( i < (int)vv.size() );
        const UMat& m = vv[i];
        d = m.dims;
        if(arrsz)
            for(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    else
    {
        Size sz2d = size(i);
        d = 2;
        if(arrsz)
        {
            arrsz[0] = sz2d.height;
            arrsz[1] = sz2d.width;
        }
    }

    return d;
}

bool _InputArray::sameSize(const _InputArray& arr) const
{
    int k1 = kind(), k2 = arr.kind();
    Size sz1;

    if( k1 == MAT )
    {
        const Mat* m = ((const Mat*)obj);
        if( k2 == MAT )
            return m->size == ((const Mat*)arr.obj)->size;
        if( k2 == UMAT )
            return m->size == ((const UMat*)arr.obj)->size;
        if( m->dims > 2 )
            return false;
        sz1 = m->size();
    }
    else if( k1 == UMAT )
    {
        const UMat* m = ((const UMat*)obj);
        if( k2 == MAT )
            return m->size == ((const Mat*)arr.obj)->size;
        if( k2 == UMAT )
            return m->size == ((const UMat*)arr.obj)->size;
        if( m->dims > 2 )
            return false;
        sz1 = m->size();
    }
    else
        sz1 = size();
    if( arr.dims() > 2 )
        return false;
    return sz1 == arr.size();
}

int _InputArray::dims(int i) const
{
    int k = kind();

    if( k == MAT )
    {
        CV_Assert( i < 0 );
        return ((const Mat*)obj)->dims;
    }

    if( k == EXPR )
    {
        CV_Assert( i < 0 );
        return ((const MatExpr*)obj)->a.dims;
    }

    if( k == UMAT )
    {
        CV_Assert( i < 0 );
        return ((const UMat*)obj)->dims;
    }

    if( k == MATX )
    {
        CV_Assert( i < 0 );
        return 2;
    }

    if( k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        return 2;
    }

    if( k == NONE )
        return 0;

    if( k == STD_VECTOR_VECTOR )
    {
        const std::vector<std::vector<uchar> >& vv = *(const std::vector<std::vector<uchar> >*)obj;
        if( i < 0 )
            return 1;
        CV_Assert( i < (int)vv.size() );
        return 2;
    }

    if( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        if( i < 0 )
            return 1;
        CV_Assert( i < (int)vv.size() );

        return vv[i].dims;
    }

    if( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        if( i < 0 )
            return 1;
        CV_Assert( i < (int)vv.size() );

        return vv[i].dims;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
    return 0;
}

size_t _InputArray::total(int i) const
{
    int k = kind();

    if( k == MAT )
    {
        CV_Assert( i < 0 );
        return ((const Mat*)obj)->total();
    }

    if( k == UMAT )
    {
        CV_Assert( i < 0 );
        return ((const UMat*)obj)->total();
    }

    if( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        if( i < 0 )
            return vv.size();

        CV_Assert( i < (int)vv.size() );
        return vv[i].total();
    }

    if( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        if( i < 0 )
            return vv.size();

        CV_Assert( i < (int)vv.size() );
        return vv[i].total();
    }

    return size(i).area();
}

int _InputArray::type(int i) const
{
    int k = kind();

    if( k == MAT )
        return ((const Mat*)obj)->type();

    if( k == UMAT )
        return ((const UMat*)obj)->type();

//    if( k == EXPR )
//        return ((const MatExpr*)obj)->type();

    if( k == MATX || k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        return CV_MAT_TYPE(flags);

    if( k == NONE )
        return -1;

    if( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        if( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            return CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (int)vv.size() );
        return vv[i >= 0 ? i : 0].type();
    }

    if( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        if( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            return CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (int)vv.size() );
        return vv[i >= 0 ? i : 0].type();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
    return 0;
}

int _InputArray::depth(int i) const
{
    return CV_MAT_DEPTH(type(i));
}

int _InputArray::channels(int i) const
{
    return CV_MAT_CN(type(i));
}

bool _InputArray::empty() const
{
    int k = kind();

    if( k == MAT )
        return ((const Mat*)obj)->empty();

    if( k == UMAT )
        return ((const UMat*)obj)->empty();

//    if( k == EXPR )
//        return false;

    if( k == MATX )
        return false;

    if( k == STD_VECTOR )
    {
        const std::vector<uchar>& v = *(const std::vector<uchar>*)obj;
        return v.empty();
    }

    if( k == STD_BOOL_VECTOR )
    {
        const std::vector<bool>& v = *(const std::vector<bool>*)obj;
        return v.empty();
    }

    if( k == NONE )
        return true;

    if( k == STD_VECTOR_VECTOR )
    {
        const std::vector<std::vector<uchar> >& vv = *(const std::vector<std::vector<uchar> >*)obj;
        return vv.empty();
    }

    if( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        return vv.empty();
    }

    if( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        return vv.empty();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
    return true;
}

bool _InputArray::isContinuous(int i) const
{
    int k = kind();

    if( k == MAT )
        return i < 0 ? ((const Mat*)obj)->isContinuous() : true;

    if( k == UMAT )
        return i < 0 ? ((const UMat*)obj)->isContinuous() : true;

//    if( k == EXPR || k == MATX || k == STD_VECTOR ||
//        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
//        return true;

    if( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert((size_t)i < vv.size());
        return vv[i].isContinuous();
    }

    if( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert((size_t)i < vv.size());
        return vv[i].isContinuous();
    }

    CV_Error(CV_StsNotImplemented, "Unknown/unsupported array type");
    return false;
}

bool _InputArray::isSubmatrix(int i) const
{
    int k = kind();

    if( k == MAT )
        return i < 0 ? ((const Mat*)obj)->isSubmatrix() : false;

    if( k == UMAT )
        return i < 0 ? ((const UMat*)obj)->isSubmatrix() : false;

//    if( k == EXPR || k == MATX || k == STD_VECTOR ||
//        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
//        return false;

    if( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert((size_t)i < vv.size());
        return vv[i].isSubmatrix();
    }

    if( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert((size_t)i < vv.size());
        return vv[i].isSubmatrix();
    }

    CV_Error(CV_StsNotImplemented, "");
    return false;
}

size_t _InputArray::offset(int i) const
{
    int k = kind();

    if( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat * const m = ((const Mat*)obj);
        return (size_t)(m->ptr() - m->datastart);
    }

    if( k == UMAT )
    {
        CV_Assert( i < 0 );
        return ((const UMat*)obj)->offset;
    }

//    if( k == EXPR || k == MATX || k == STD_VECTOR ||
//        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
//        return 0;

    if( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        if( i < 0 )
            return 1;
        CV_Assert( i < (int)vv.size() );

        return (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    if( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert((size_t)i < vv.size());
        return vv[i].offset;
    }

    CV_Error(Error::StsNotImplemented, "");
    return 0;
}

size_t _InputArray::step(int i) const
{
    int k = kind();

    if( k == MAT )
    {
        CV_Assert( i < 0 );
        return ((const Mat*)obj)->step;
    }

    if( k == UMAT )
    {
        CV_Assert( i < 0 );
        return ((const UMat*)obj)->step;
    }

//    if( k == EXPR || k == MATX || k == STD_VECTOR ||
//        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
//        return 0;

    if( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        if( i < 0 )
            return 1;
        CV_Assert( i < (int)vv.size() );
        return vv[i].step;
    }

    if( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert((size_t)i < vv.size());
        return vv[i].step;
    }

    CV_Error(Error::StsNotImplemented, "");
    return 0;
}

void _InputArray::copyTo(const _OutputArray& arr) const
{
    int k = kind();

    if( k == NONE )
        arr.release();
    else if( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr);
    }
//    else if( k == EXPR )
//    {
//        const MatExpr& e = *((MatExpr*)obj);
//        if( arr.kind() == MAT )
//            arr.getMatRef() = e;
//        else
//            Mat(e).copyTo(arr);
//    }
//    else if( k == UMAT )
//        ((UMat*)obj)->copyTo(arr);
    else
        CV_Error(Error::StsNotImplemented, "");
}

void _InputArray::copyTo(const _OutputArray& arr, const _InputArray & mask) const
{
    int k = kind();

    if( k == NONE )
        arr.release();
    else if( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr, mask);
    }
//    else if( k == UMAT )
//        ((UMat*)obj)->copyTo(arr, mask);
    else
        CV_Error(Error::StsNotImplemented, "");
}

bool _OutputArray::fixedSize() const
{
    return (flags & FIXED_SIZE) == FIXED_SIZE;
}

bool _OutputArray::fixedType() const
{
    return (flags & FIXED_TYPE) == FIXED_TYPE;
}

void _OutputArray::create(Size _sz, int mtype, int i, bool allowTransposed, int fixedDepthMask) const
{
    int k = kind();
    if( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        return;
    }
    if( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        return;
    }
    int sizes[] = {_sz.height, _sz.width};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::create(int _rows, int _cols, int mtype, int i, bool allowTransposed, int fixedDepthMask) const
{
    int k = kind();
    if( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_rows, _cols, mtype);
        return;
    }
    if( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_rows, _cols, mtype);
        return;
    }
    int sizes[] = {_rows, _cols};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::create(int d, const int* sizes, int mtype, int i,
                          bool allowTransposed, int fixedDepthMask) const
{
    int k = kind();
    mtype = CV_MAT_TYPE(mtype);

    if( k == MAT )
    {
        CV_Assert( i < 0 );
        Mat& m = *(Mat*)obj;
        if( allowTransposed )
        {
            if( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            if( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                return;
        }

        if(fixedType())
        {
            if(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            else
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        if(fixedSize())
        {
            CV_Assert(m.dims == d);
            for(int j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }
        m.create(d, sizes, mtype);
        return;
    }

    if( k == UMAT )
    {
        CV_Assert( i < 0 );
        UMat& m = *(UMat*)obj;
        if( allowTransposed )
        {
            if( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            if( d == 2 && m.dims == 2 && !m.empty() &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                return;
        }

        if(fixedType())
        {
            if(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            else
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        if(fixedSize())
        {
            CV_Assert(m.dims == d);
            for(int j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }
        m.create(d, sizes, mtype);
        return;
    }

    if( k == MATX )
    {
        CV_Assert( i < 0 );
        int type0 = CV_MAT_TYPE(flags);
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixedDepthMask) != 0) );
        CV_Assert( d == 2 && ((sizes[0] == sz.height && sizes[1] == sz.width) ||
                                 (allowTransposed && sizes[0] == sz.width && sizes[1] == sz.height)));
        return;
    }

    if( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
        size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0;
        std::vector<uchar>* v = (std::vector<uchar>*)obj;

        if( k == STD_VECTOR_VECTOR )
        {
            std::vector<std::vector<uchar> >& vv = *(std::vector<std::vector<uchar> >*)obj;
            if( i < 0 )
            {
                CV_Assert(!fixedSize() || len == vv.size());
                vv.resize(len);
                return;
            }
            CV_Assert( i < (int)vv.size() );
            v = &vv[i];
        }
        else
            CV_Assert( i < 0 );

        int type0 = CV_MAT_TYPE(flags);
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == CV_MAT_CN(type0) && ((1 << type0) & fixedDepthMask) != 0) );

        int esz = CV_ELEM_SIZE(type0);
        CV_Assert(!fixedSize() || len == ((std::vector<uchar>*)v)->size() / esz);
        switch( esz )
        {
        case 1:
            ((std::vector<uchar>*)v)->resize(len);
            break;
        case 2:
            ((std::vector<Vec2b>*)v)->resize(len);
            break;
        case 3:
            ((std::vector<Vec3b>*)v)->resize(len);
            break;
        case 4:
            ((std::vector<int>*)v)->resize(len);
            break;
        case 6:
            ((std::vector<Vec3s>*)v)->resize(len);
            break;
        case 8:
            ((std::vector<Vec2i>*)v)->resize(len);
            break;
        case 12:
            ((std::vector<Vec3i>*)v)->resize(len);
            break;
        case 16:
            ((std::vector<Vec4i>*)v)->resize(len);
            break;
        case 24:
            ((std::vector<Vec6i>*)v)->resize(len);
            break;
        case 32:
            ((std::vector<Vec8i>*)v)->resize(len);
            break;
        case 36:
            ((std::vector<Vec<int, 9> >*)v)->resize(len);
            break;
        case 48:
            ((std::vector<Vec<int, 12> >*)v)->resize(len);
            break;
        case 64:
            ((std::vector<Vec<int, 16> >*)v)->resize(len);
            break;
        case 128:
            ((std::vector<Vec<int, 32> >*)v)->resize(len);
            break;
        case 256:
            ((std::vector<Vec<int, 64> >*)v)->resize(len);
            break;
        case 512:
            ((std::vector<Vec<int, 128> >*)v)->resize(len);
            break;
        default:
            CV_Error_(CV_StsBadArg, ("Vectors with element size %d are not supported. Please, modify OutputArray::create()\n", esz));
        }
        return;
    }

    if( k == NONE )
    {
        CV_Error(CV_StsNullPtr, "create() called for the missing output array" );
        return;
    }

    if( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;

        if( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            if( fixedType() )
            {
                int _type = CV_MAT_TYPE(flags);
                for( size_t j = len0; j < len; j++ )
                {
                    if( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            return;
        }

        CV_Assert( i < (int)v.size() );
        Mat& m = v[i];

        if( allowTransposed )
        {
            if( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            if( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                return;
        }

        if(fixedType())
        {
            if(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            else
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        if(fixedSize())
        {
            CV_Assert(m.dims == d);
            for(int j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        return;
    }

    if( k == STD_VECTOR_UMAT )
    {
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;

        if( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            if( fixedType() )
            {
                int _type = CV_MAT_TYPE(flags);
                for( size_t j = len0; j < len; j++ )
                {
                    if( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            return;
        }

        CV_Assert( i < (int)v.size() );
        UMat& m = v[i];

        if( allowTransposed )
        {
            if( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            if( d == 2 && m.dims == 2 && m.u &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                return;
        }

        if(fixedType())
        {
            if(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            else
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        if(fixedSize())
        {
            CV_Assert(m.dims == d);
            for(int j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        return;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}


void _OutputArray::release() const
{
    CV_Assert(!fixedSize());

    int k = kind();

    if( k == MAT )
    {
        ((Mat*)obj)->release();
        return;
    }

    if( k == UMAT )
    {
        ((UMat*)obj)->release();
        return;
    }

    if( k == NONE )
        return;

    if( k == STD_VECTOR )
    {
        create(Size(), CV_MAT_TYPE(flags));
        return;
    }

    if( k == STD_VECTOR_VECTOR )
    {
        ((std::vector<std::vector<uchar> >*)obj)->clear();
        return;
    }

    if( k == STD_VECTOR_MAT )
    {
        ((std::vector<Mat>*)obj)->clear();
        return;
    }

    if( k == STD_VECTOR_UMAT )
    {
        ((std::vector<UMat>*)obj)->clear();
        return;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

bool _OutputArray::needed() const
{
    return kind() != NONE;
}

static _InputOutputArray _none;
InputOutputArray noArray() { return _none; }

}

///////////////////////////// n-dimensional matrices ////////////////////////////

namespace minicv
{

//Mat Mat::reshape(int _cn, int _newndims, const int* _newsz) const
//{
//    if(_newndims == dims)
//    {
//        if(_newsz == 0)
//            return reshape(_cn);
//        if(_newndims == 2)
//            return reshape(_cn, _newsz[0]);
//    }

//    CV_Error(CV_StsNotImplemented, "");
//    // TBD
//    return Mat();
//}

NAryMatIterator::NAryMatIterator()
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
}

NAryMatIterator::NAryMatIterator(const Mat** _arrays, Mat* _planes, int _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(const Mat** _arrays, uchar** _ptrs, int _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

void NAryMatIterator::init(const Mat** _arrays, Mat* _planes, uchar** _ptrs, int _narrays)
{
    CV_Assert( _arrays && (_ptrs || _planes) );
    int i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    if( narrays < 0 )
    {
        for( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        CV_Assert(narrays <= 1000);
    }

    iterdepth = 0;

    for( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        const Mat& A = *arrays[i];
        if( ptrs )
            ptrs[i] = A.data;

        if( !A.data )
            continue;

        if( i0 < 0 )
        {
            i0 = i;
            d = A.dims;

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            for( d1 = 0; d1 < d; d1++ )
                if( A.size[d1] > 1 )
                    break;
        }
        else
            CV_Assert( A.size == arrays[i0]->size );

        if( !A.isContinuous() )
        {
            CV_Assert( A.step[d-1] == A.elemSize() );
            for( j = d-1; j > d1; j-- )
                if( A.step[j]*A.size[j] < A.step[j-1] )
                    break;
            iterdepth = std::max(iterdepth, j);
        }
    }

    if( i0 >= 0 )
    {
        size = arrays[i0]->size[d-1];
        for( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            if( total1 != (int)total1 )
                break;
            size = (int)total1;
        }

        iterdepth = j;
        if( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        for( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    else
        iterdepth = 0;

    idx = 0;

    if( !planes )
        return;

    for( i = 0; i < narrays; i++ )
    {
        CV_Assert(arrays[i] != 0);
        const Mat& A = *arrays[i];

        if( !A.data )
        {
            planes[i] = Mat();
            continue;
        }

        planes[i] = Mat(1, (int)size, A.type(), A.data);
    }
}

NAryMatIterator& NAryMatIterator::operator ++()
{
    if( idx >= nplanes-1 )
        return *this;
    ++idx;

    if( iterdepth == 1 )
    {
        if( ptrs )
        {
            for( int i = 0; i < narrays; i++ )
            {
                if( !ptrs[i] )
                    continue;
                ptrs[i] = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
        if( planes )
        {
            for( int i = 0; i < narrays; i++ )
            {
                if( !planes[i].data )
                    continue;
                planes[i].data = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
    }
    else
    {
        for( int i = 0; i < narrays; i++ )
        {
            const Mat& A = *arrays[i];
            if( !A.data )
                continue;
            int _idx = (int)idx;
            uchar* data = A.data;
            for( int j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                int szi = A.size[j], t = _idx/szi;
                data += (_idx - t * szi)*A.step[j];
                _idx = t;
            }
            if( ptrs )
                ptrs[i] = data;
            if( planes )
                planes[i].data = data;
        }
    }

    return *this;
}

NAryMatIterator NAryMatIterator::operator ++(int)
{
    NAryMatIterator it = *this;
    ++*this;
    return it;
}

}
