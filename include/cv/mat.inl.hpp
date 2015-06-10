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
// Copyright (C) 2015, Itseez Inc., all rights reserved.
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

#ifndef __OPENCV_CORE_MATRIX_OPERATIONS_HPP__
#define __OPENCV_CORE_MATRIX_OPERATIONS_HPP__

#ifndef __cplusplus
#  error mat.inl.hpp header must be compiled as C++
#endif

namespace minicv
{

//! @cond IGNORED

//////////////////////// Input/Output Arrays ////////////////////////

inline void _InputArray::init(int _flags, const void* _obj)
{ flags = _flags; obj = (void*)_obj; }

inline void _InputArray::init(int _flags, const void* _obj, Size _sz)
{ flags = _flags; obj = (void*)_obj; sz = _sz; }

inline void* _InputArray::getObj() const { return obj; }
inline int _InputArray::getFlags() const { return flags; }
inline Size _InputArray::getSz() const { return sz; }

inline _InputArray::_InputArray() { init(NONE, 0); }
inline _InputArray::_InputArray(int _flags, void* _obj) { init(_flags, _obj); }
inline _InputArray::_InputArray(const Mat& m) { init(MAT+ACCESS_READ, &m); }
inline _InputArray::_InputArray(const UMat& m) { init(UMAT+ACCESS_READ, &m); }


template<typename _Tp, int m, int n> inline
_InputArray::_InputArray(const Matx<_Tp, m, n>& mtx)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + DataType<_Tp>::type + ACCESS_READ, &mtx, Size(n, m)); }

template<typename _Tp> inline
_InputArray::_InputArray(const _Tp* vec, int n)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + DataType<_Tp>::type + ACCESS_READ, vec, Size(n, 1)); }


inline _InputArray::_InputArray(const double& val)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + CV_64F + ACCESS_READ, &val, Size(1,1)); }


inline _InputArray::~_InputArray() {}

inline Mat _InputArray::getMat(int i) const
{
    if( kind() == MAT && i < 0 )
        return *(const Mat*)obj;
    return getMat_(i);
}

inline bool _InputArray::isMat() const { return kind() == _InputArray::MAT; }
inline bool _InputArray::isUMat() const  { return kind() == _InputArray::UMAT; }
inline bool _InputArray::isMatVector() const { return kind() == _InputArray::STD_VECTOR_MAT; }
inline bool _InputArray::isUMatVector() const  { return kind() == _InputArray::STD_VECTOR_UMAT; }
inline bool _InputArray::isMatx() const { return kind() == _InputArray::MATX; }

////////////////////////////////////////////////////////////////////////////////////////

inline _OutputArray::_OutputArray() { init(ACCESS_WRITE, 0); }
inline _OutputArray::_OutputArray(int _flags, void* _obj) { init(_flags|ACCESS_WRITE, _obj); }
inline _OutputArray::_OutputArray(Mat& m) { init(MAT+ACCESS_WRITE, &m); }
inline _OutputArray::_OutputArray(UMat& m) { init(UMAT+ACCESS_WRITE, &m); }


template<typename _Tp, int m, int n> inline
_OutputArray::_OutputArray(Matx<_Tp, m, n>& mtx)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + DataType<_Tp>::type + ACCESS_WRITE, &mtx, Size(n, m)); }

template<typename _Tp> inline
_OutputArray::_OutputArray(_Tp* vec, int n)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + DataType<_Tp>::type + ACCESS_WRITE, vec, Size(n, 1)); }


template<typename _Tp, int m, int n> inline
_OutputArray::_OutputArray(const Matx<_Tp, m, n>& mtx)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + DataType<_Tp>::type + ACCESS_WRITE, &mtx, Size(n, m)); }

template<typename _Tp> inline
_OutputArray::_OutputArray(const _Tp* vec, int n)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + DataType<_Tp>::type + ACCESS_WRITE, vec, Size(n, 1)); }


inline _OutputArray::_OutputArray(const Mat& m)
{ init(FIXED_TYPE + FIXED_SIZE + MAT + ACCESS_WRITE, &m); }


inline _OutputArray::_OutputArray(const UMat& m)
{ init(FIXED_TYPE + FIXED_SIZE + UMAT + ACCESS_WRITE, &m); }



///////////////////////////////////////////////////////////////////////////////////////////

inline _InputOutputArray::_InputOutputArray() { init(ACCESS_RW, 0); }
inline _InputOutputArray::_InputOutputArray(int _flags, void* _obj) { init(_flags|ACCESS_RW, _obj); }
inline _InputOutputArray::_InputOutputArray(Mat& m) { init(MAT+ACCESS_RW, &m); }
inline _InputOutputArray::_InputOutputArray(UMat& m) { init(UMAT+ACCESS_RW, &m); }


template<typename _Tp, int m, int n> inline
_InputOutputArray::_InputOutputArray(Matx<_Tp, m, n>& mtx)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + DataType<_Tp>::type + ACCESS_RW, &mtx, Size(n, m)); }

template<typename _Tp> inline
_InputOutputArray::_InputOutputArray(_Tp* vec, int n)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + DataType<_Tp>::type + ACCESS_RW, vec, Size(n, 1)); }


template<typename _Tp, int m, int n> inline
_InputOutputArray::_InputOutputArray(const Matx<_Tp, m, n>& mtx)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + DataType<_Tp>::type + ACCESS_RW, &mtx, Size(n, m)); }

template<typename _Tp> inline
_InputOutputArray::_InputOutputArray(const _Tp* vec, int n)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + DataType<_Tp>::type + ACCESS_RW, vec, Size(n, 1)); }


inline _InputOutputArray::_InputOutputArray(const Mat& m)
{ init(FIXED_TYPE + FIXED_SIZE + MAT + ACCESS_RW, &m); }


inline _InputOutputArray::_InputOutputArray(const UMat& m)
{ init(FIXED_TYPE + FIXED_SIZE + UMAT + ACCESS_RW, &m); }



//////////////////////////////////////////// Mat //////////////////////////////////////////

inline
Mat::Mat()
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0), size(&rows)
{}

inline
Mat::Mat(int _rows, int _cols, int _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0), size(&rows)
{
    create(_rows, _cols, _type);
}

inline
Mat::Mat(int _rows, int _cols, int _type, const Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0), size(&rows)
{
    create(_rows, _cols, _type);
    *this = _s;
}

inline
Mat::Mat(Size _sz, int _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0), size(&rows)
{
    create( _sz.height, _sz.width, _type );
}

inline
Mat::Mat(Size _sz, int _type, const Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0), size(&rows)
{
    create(_sz.height, _sz.width, _type);
    *this = _s;
}

inline
Mat::Mat(int _dims, const int* _sz, int _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0), size(&rows)
{
    create(_dims, _sz, _type);
}

inline
Mat::Mat(int _dims, const int* _sz, int _type, const Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0), size(&rows)
{
    create(_dims, _sz, _type);
    *this = _s;
}

inline
Mat::Mat(const Mat& m)
    : flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), data(m.data),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(&rows)
{
    if( u )
        CV_XADD(&u->refcount, 1);
    if( m.dims <= 2 )
    {
        step[0] = m.step[0]; step[1] = m.step[1];
    }
    else
    {
        dims = 0;
        copySize(m);
    }
}

inline
Mat::Mat(int _rows, int _cols, int _type, void* _data, size_t _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_rows), cols(_cols),
      data((uchar*)_data), datastart((uchar*)_data), dataend(0), datalimit(0),
      allocator(0), u(0), size(&rows)
{
    size_t esz = CV_ELEM_SIZE(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = cols * esz;
    if( _step == AUTO_STEP )
    {
        _step = minstep;
        flags |= CONTINUOUS_FLAG;
    }
    else
    {
        if( rows == 1 ) _step = minstep;
        CV_DbgAssert( _step >= minstep );

        if (_step % esz1 != 0)
        {
            CV_Error(Error::BadStep, "Step must be a multiple of esz1");
        }

        flags |= _step == minstep ? CONTINUOUS_FLAG : 0;
    }
    step[0] = _step;
    step[1] = esz;
    datalimit = datastart + _step * rows;
    dataend = datalimit - _step + minstep;
}

inline
Mat::Mat(Size _sz, int _type, void* _data, size_t _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_sz.height), cols(_sz.width),
      data((uchar*)_data), datastart((uchar*)_data), dataend(0), datalimit(0),
      allocator(0), u(0), size(&rows)
{
    size_t esz = CV_ELEM_SIZE(_type), esz1 = CV_ELEM_SIZE1(_type);
    size_t minstep = cols*esz;
    if( _step == AUTO_STEP )
    {
        _step = minstep;
        flags |= CONTINUOUS_FLAG;
    }
    else
    {
        if( rows == 1 ) _step = minstep;
        CV_DbgAssert( _step >= minstep );

        if (_step % esz1 != 0)
        {
            CV_Error(Error::BadStep, "Step must be a multiple of esz1");
        }

        flags |= _step == minstep ? CONTINUOUS_FLAG : 0;
    }
    step[0] = _step;
    step[1] = esz;
    datalimit = datastart + _step*rows;
    dataend = datalimit - _step + minstep;
}

template<typename _Tp, int n> inline
Mat::Mat(const Vec<_Tp, n>& vec, bool copyData)
    : flags(MAGIC_VAL | DataType<_Tp>::type | CV_MAT_CONT_FLAG), dims(2), rows(n), cols(1), data(0),
      datastart(0), dataend(0), allocator(0), u(0), size(&rows)
{
    if( !copyData )
    {
        step[0] = step[1] = sizeof(_Tp);
        datastart = data = (uchar*)vec.val;
        datalimit = dataend = datastart + rows * step[0];
    }
    else
        Mat(n, 1, DataType<_Tp>::type, (void*)vec.val).copyTo(*this);
}


template<typename _Tp> inline
Mat::Mat(const Point_<_Tp>& pt, bool copyData)
    : flags(MAGIC_VAL | DataType<_Tp>::type | CV_MAT_CONT_FLAG), dims(2), rows(2), cols(1), data(0),
      datastart(0), dataend(0), allocator(0), u(0), size(&rows)
{
    if( !copyData )
    {
        step[0] = step[1] = sizeof(_Tp);
        datastart = data = (uchar*)&pt.x;
        datalimit = dataend = datastart + rows * step[0];
    }
    else
    {
        create(2, 1, DataType<_Tp>::type);
        ((_Tp*)data)[0] = pt.x;
        ((_Tp*)data)[1] = pt.y;
    }
}


inline
Mat::~Mat()
{
    release();
    if( step.p != step.buf )
        fastFree(step.p);
}

inline
Mat& Mat::operator = (const Mat& m)
{
    if( this != &m )
    {
        if( m.u )
            CV_XADD(&m.u->refcount, 1);
        release();
        flags = m.flags;
        if( dims <= 2 && m.dims <= 2 )
        {
            dims = m.dims;
            rows = m.rows;
            cols = m.cols;
            step[0] = m.step[0];
            step[1] = m.step[1];
        }
        else
            copySize(m);
        data = m.data;
        datastart = m.datastart;
        dataend = m.dataend;
        datalimit = m.datalimit;
        allocator = m.allocator;
        u = m.u;
    }
    return *this;
}

inline
Mat Mat::row(int y) const
{
    return Mat(*this, Range(y, y + 1), Range::all());
}

inline
Mat Mat::col(int x) const
{
    return Mat(*this, Range::all(), Range(x, x + 1));
}

inline
Mat Mat::rowRange(int startrow, int endrow) const
{
    return Mat(*this, Range(startrow, endrow), Range::all());
}

inline
Mat Mat::rowRange(const Range& r) const
{
    return Mat(*this, r, Range::all());
}

inline
Mat Mat::colRange(int startcol, int endcol) const
{
    return Mat(*this, Range::all(), Range(startcol, endcol));
}

inline
Mat Mat::colRange(const Range& r) const
{
    return Mat(*this, Range::all(), r);
}

inline
Mat Mat::clone() const
{
    Mat m;
    copyTo(m);
    return m;
}

inline
void Mat::assignTo( Mat& m, int _type ) const
{
    if( _type < 0 )
        m = *this;
    else
            throw std::runtime_error("operation not available in minicv");
}

inline
void Mat::create(int _rows, int _cols, int _type)
{
    _type &= TYPE_MASK;
    if( dims <= 2 && rows == _rows && cols == _cols && type() == _type && data )
        return;
    int sz[] = {_rows, _cols};
    create(2, sz, _type);
}

inline
void Mat::create(Size _sz, int _type)
{
    create(_sz.height, _sz.width, _type);
}

inline
void Mat::addref()
{
    if( u )
        CV_XADD(&u->refcount, 1);
}

inline void Mat::release()
{
    if( u && CV_XADD(&u->refcount, -1) == 1 )
        deallocate();
    u = NULL;
    datastart = dataend = datalimit = data = 0;
    for(int i = 0; i < dims; i++)
        size.p[i] = 0;
}

inline
Mat Mat::operator()( Range _rowRange, Range _colRange ) const
{
    return Mat(*this, _rowRange, _colRange);
}

inline
Mat Mat::operator()( const Rect& roi ) const
{
    return Mat(*this, roi);
}

inline
Mat Mat::operator()(const Range* ranges) const
{
    return Mat(*this, ranges);
}

inline
bool Mat::isContinuous() const
{
    return (flags & CONTINUOUS_FLAG) != 0;
}

inline
bool Mat::isSubmatrix() const
{
    return (flags & SUBMATRIX_FLAG) != 0;
}

inline
size_t Mat::elemSize() const
{
    return dims > 0 ? step.p[dims - 1] : 0;
}

inline
size_t Mat::elemSize1() const
{
    return CV_ELEM_SIZE1(flags);
}

inline
int Mat::type() const
{
    return CV_MAT_TYPE(flags);
}

inline
int Mat::depth() const
{
    return CV_MAT_DEPTH(flags);
}

inline
int Mat::channels() const
{
    return CV_MAT_CN(flags);
}

inline
size_t Mat::step1(int i) const
{
    return step.p[i] / elemSize1();
}

inline
bool Mat::empty() const
{
    return data == 0 || total() == 0;
}

inline
size_t Mat::total() const
{
    if( dims <= 2 )
        return (size_t)rows * cols;
    size_t p = 1;
    for( int i = 0; i < dims; i++ )
        p *= size[i];
    return p;
}

inline
uchar* Mat::ptr(int y)
{
    CV_DbgAssert( y == 0 || (data && dims >= 1 && (unsigned)y < (unsigned)size.p[0]) );
    return data + step.p[0] * y;
}

inline
const uchar* Mat::ptr(int y) const
{
    CV_DbgAssert( y == 0 || (data && dims >= 1 && (unsigned)y < (unsigned)size.p[0]) );
    return data + step.p[0] * y;
}

template<typename _Tp> inline
_Tp* Mat::ptr(int y)
{
    CV_DbgAssert( y == 0 || (data && dims >= 1 && (unsigned)y < (unsigned)size.p[0]) );
    return (_Tp*)(data + step.p[0] * y);
}

template<typename _Tp> inline
const _Tp* Mat::ptr(int y) const
{
    CV_DbgAssert( y == 0 || (data && dims >= 1 && data && (unsigned)y < (unsigned)size.p[0]) );
    return (const _Tp*)(data + step.p[0] * y);
}

inline
uchar* Mat::ptr(int i0, int i1)
{
    CV_DbgAssert( dims >= 2 && data &&
                  (unsigned)i0 < (unsigned)size.p[0] &&
                  (unsigned)i1 < (unsigned)size.p[1] );
    return data + i0 * step.p[0] + i1 * step.p[1];
}

inline
const uchar* Mat::ptr(int i0, int i1) const
{
    CV_DbgAssert( dims >= 2 && data &&
                 (unsigned)i0 < (unsigned)size.p[0] &&
                 (unsigned)i1 < (unsigned)size.p[1] );
    return data + i0 * step.p[0] + i1 * step.p[1];
}

template<typename _Tp> inline
_Tp* Mat::ptr(int i0, int i1)
{
    CV_DbgAssert( dims >= 2 && data &&
                  (unsigned)i0 < (unsigned)size.p[0] &&
                  (unsigned)i1 < (unsigned)size.p[1] );
    return (_Tp*)(data + i0 * step.p[0] + i1 * step.p[1]);
}

template<typename _Tp> inline
const _Tp* Mat::ptr(int i0, int i1) const
{
    CV_DbgAssert( dims >= 2 && data &&
                  (unsigned)i0 < (unsigned)size.p[0] &&
                  (unsigned)i1 < (unsigned)size.p[1] );
    return (const _Tp*)(data + i0 * step.p[0] + i1 * step.p[1]);
}

inline
uchar* Mat::ptr(int i0, int i1, int i2)
{
    CV_DbgAssert( dims >= 3 && data &&
                  (unsigned)i0 < (unsigned)size.p[0] &&
                  (unsigned)i1 < (unsigned)size.p[1] &&
                  (unsigned)i2 < (unsigned)size.p[2] );
    return data + i0 * step.p[0] + i1 * step.p[1] + i2 * step.p[2];
}

inline
const uchar* Mat::ptr(int i0, int i1, int i2) const
{
    CV_DbgAssert( dims >= 3 && data &&
                  (unsigned)i0 < (unsigned)size.p[0] &&
                  (unsigned)i1 < (unsigned)size.p[1] &&
                  (unsigned)i2 < (unsigned)size.p[2] );
    return data + i0 * step.p[0] + i1 * step.p[1] + i2 * step.p[2];
}

template<typename _Tp> inline
_Tp* Mat::ptr(int i0, int i1, int i2)
{
    CV_DbgAssert( dims >= 3 && data &&
                  (unsigned)i0 < (unsigned)size.p[0] &&
                  (unsigned)i1 < (unsigned)size.p[1] &&
                  (unsigned)i2 < (unsigned)size.p[2] );
    return (_Tp*)(data + i0 * step.p[0] + i1 * step.p[1] + i2 * step.p[2]);
}

template<typename _Tp> inline
const _Tp* Mat::ptr(int i0, int i1, int i2) const
{
    CV_DbgAssert( dims >= 3 && data &&
                  (unsigned)i0 < (unsigned)size.p[0] &&
                  (unsigned)i1 < (unsigned)size.p[1] &&
                  (unsigned)i2 < (unsigned)size.p[2] );
    return (const _Tp*)(data + i0 * step.p[0] + i1 * step.p[1] + i2 * step.p[2]);
}

inline
uchar* Mat::ptr(const int* idx)
{
    int i, d = dims;
    uchar* p = data;
    CV_DbgAssert( d >= 1 && p );
    for( i = 0; i < d; i++ )
    {
        CV_DbgAssert( (unsigned)idx[i] < (unsigned)size.p[i] );
        p += idx[i] * step.p[i];
    }
    return p;
}

inline
const uchar* Mat::ptr(const int* idx) const
{
    int i, d = dims;
    uchar* p = data;
    CV_DbgAssert( d >= 1 && p );
    for( i = 0; i < d; i++ )
    {
        CV_DbgAssert( (unsigned)idx[i] < (unsigned)size.p[i] );
        p += idx[i] * step.p[i];
    }
    return p;
}

template<typename _Tp> inline
_Tp& Mat::at(int i0, int i1)
{
    CV_DbgAssert( dims <= 2 && data && (unsigned)i0 < (unsigned)size.p[0] &&
        (unsigned)(i1 * DataType<_Tp>::channels) < (unsigned)(size.p[1] * channels()) &&
        CV_ELEM_SIZE1(DataType<_Tp>::depth) == elemSize1());
    return ((_Tp*)(data + step.p[0] * i0))[i1];
}

template<typename _Tp> inline
const _Tp& Mat::at(int i0, int i1) const
{
    CV_DbgAssert( dims <= 2 && data && (unsigned)i0 < (unsigned)size.p[0] &&
        (unsigned)(i1 * DataType<_Tp>::channels) < (unsigned)(size.p[1] * channels()) &&
        CV_ELEM_SIZE1(DataType<_Tp>::depth) == elemSize1());
    return ((const _Tp*)(data + step.p[0] * i0))[i1];
}

template<typename _Tp> inline
_Tp& Mat::at(Point pt)
{
    CV_DbgAssert( dims <= 2 && data && (unsigned)pt.y < (unsigned)size.p[0] &&
        (unsigned)(pt.x * DataType<_Tp>::channels) < (unsigned)(size.p[1] * channels()) &&
        CV_ELEM_SIZE1(DataType<_Tp>::depth) == elemSize1());
    return ((_Tp*)(data + step.p[0] * pt.y))[pt.x];
}

template<typename _Tp> inline
const _Tp& Mat::at(Point pt) const
{
    CV_DbgAssert( dims <= 2 && data && (unsigned)pt.y < (unsigned)size.p[0] &&
        (unsigned)(pt.x * DataType<_Tp>::channels) < (unsigned)(size.p[1] * channels()) &&
        CV_ELEM_SIZE1(DataType<_Tp>::depth) == elemSize1());
    return ((const _Tp*)(data + step.p[0] * pt.y))[pt.x];
}

template<typename _Tp> inline
_Tp& Mat::at(int i0)
{
    CV_DbgAssert( dims <= 2 && data &&
                 (unsigned)i0 < (unsigned)(size.p[0] * size.p[1]) &&
                 elemSize() == CV_ELEM_SIZE(DataType<_Tp>::type) );
    if( isContinuous() || size.p[0] == 1 )
        return ((_Tp*)data)[i0];
    if( size.p[1] == 1 )
        return *(_Tp*)(data + step.p[0] * i0);
    int i = i0 / cols, j = i0 - i * cols;
    return ((_Tp*)(data + step.p[0] * i))[j];
}

template<typename _Tp> inline
const _Tp& Mat::at(int i0) const
{
    CV_DbgAssert( dims <= 2 && data &&
                 (unsigned)i0 < (unsigned)(size.p[0] * size.p[1]) &&
                 elemSize() == CV_ELEM_SIZE(DataType<_Tp>::type) );
    if( isContinuous() || size.p[0] == 1 )
        return ((const _Tp*)data)[i0];
    if( size.p[1] == 1 )
        return *(const _Tp*)(data + step.p[0] * i0);
    int i = i0 / cols, j = i0 - i * cols;
    return ((const _Tp*)(data + step.p[0] * i))[j];
}

template<typename _Tp> inline
_Tp& Mat::at(int i0, int i1, int i2)
{
    CV_DbgAssert( elemSize() == CV_ELEM_SIZE(DataType<_Tp>::type) );
    return *(_Tp*)ptr(i0, i1, i2);
}

template<typename _Tp> inline
const _Tp& Mat::at(int i0, int i1, int i2) const
{
    CV_DbgAssert( elemSize() == CV_ELEM_SIZE(DataType<_Tp>::type) );
    return *(const _Tp*)ptr(i0, i1, i2);
}

template<typename _Tp> inline
_Tp& Mat::at(const int* idx)
{
    CV_DbgAssert( elemSize() == CV_ELEM_SIZE(DataType<_Tp>::type) );
    return *(_Tp*)ptr(idx);
}

template<typename _Tp> inline
const _Tp& Mat::at(const int* idx) const
{
    CV_DbgAssert( elemSize() == CV_ELEM_SIZE(DataType<_Tp>::type) );
    return *(const _Tp*)ptr(idx);
}

template<typename _Tp, int n> inline
_Tp& Mat::at(const Vec<int, n>& idx)
{
    CV_DbgAssert( elemSize() == CV_ELEM_SIZE(DataType<_Tp>::type) );
    return *(_Tp*)ptr(idx.val);
}

template<typename _Tp, int n> inline
const _Tp& Mat::at(const Vec<int, n>& idx) const
{
    CV_DbgAssert( elemSize() == CV_ELEM_SIZE(DataType<_Tp>::type) );
    return *(const _Tp*)ptr(idx.val);
}


template<typename _Tp, typename Functor> inline
void Mat::forEach(const Functor& operation) {
    this->forEach_impl<_Tp>(operation);
}

template<typename _Tp, typename Functor> inline
void Mat::forEach(const Functor& operation) const {
    // call as not const
    (const_cast<Mat*>(this))->forEach<const _Tp>(operation);
}

template<typename _Tp> inline
Mat::operator std::vector<_Tp>() const
{
    std::vector<_Tp> v;
    copyTo(v);
    return v;
}


template<typename _Tp> inline
void Mat::push_back(const _Tp& elem)
{
    if( !data )
    {
        *this = Mat(1, 1, DataType<_Tp>::type, (void*)&elem).clone();
        return;
    }
    CV_Assert(DataType<_Tp>::type == type() && cols == 1
              /* && dims == 2 (cols == 1 implies dims == 2) */);
    const uchar* tmp = dataend + step[0];
    if( !isSubmatrix() && isContinuous() && tmp <= datalimit )
    {
        *(_Tp*)(data + (size.p[0]++) * step.p[0]) = elem;
        dataend = tmp;
    }
    else
        push_back_(&elem);
}

///////////////////////////// MatSize ////////////////////////////

inline
MatSize::MatSize(int* _p)
    : p(_p) {}

inline
Size MatSize::operator()() const
{
    CV_DbgAssert(p[-1] <= 2);
    return Size(p[1], p[0]);
}

inline
const int& MatSize::operator[](int i) const
{
    return p[i];
}

inline
int& MatSize::operator[](int i)
{
    return p[i];
}

inline
MatSize::operator const int*() const
{
    return p;
}

inline
bool MatSize::operator == (const MatSize& sz) const
{
    int d = p[-1];
    int dsz = sz.p[-1];
    if( d != dsz )
        return false;
    if( d == 2 )
        return p[0] == sz.p[0] && p[1] == sz.p[1];

    for( int i = 0; i < d; i++ )
        if( p[i] != sz.p[i] )
            return false;
    return true;
}

inline
bool MatSize::operator != (const MatSize& sz) const
{
    return !(*this == sz);
}



///////////////////////////// MatStep ////////////////////////////

inline
MatStep::MatStep()
{
    p = buf; p[0] = p[1] = 0;
}

inline
MatStep::MatStep(size_t s)
{
    p = buf; p[0] = s; p[1] = 0;
}

inline
const size_t& MatStep::operator[](int i) const
{
    return p[i];
}

inline
size_t& MatStep::operator[](int i)
{
    return p[i];
}

inline MatStep::operator size_t() const
{
    CV_DbgAssert( p == buf );
    return buf[0];
}

inline MatStep& MatStep::operator = (size_t s)
{
    CV_DbgAssert( p == buf );
    buf[0] = s;
    return *this;
}

//////////////////////////////// UMat ////////////////////////////////

inline
UMat::UMat(UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0), size(&rows)
{}

inline
UMat::UMat(int _rows, int _cols, int _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0), size(&rows)
{
    create(_rows, _cols, _type);
}

inline
UMat::UMat(Size _sz, int _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0), size(&rows)
{
    create( _sz.height, _sz.width, _type );
}

inline
UMat::UMat(int _dims, const int* _sz, int _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0), size(&rows)
{
    create(_dims, _sz, _type);
}

inline
UMat::UMat(const UMat& m)
: flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), allocator(m.allocator),
  usageFlags(m.usageFlags), u(m.u), offset(m.offset), size(&rows)
{
    addref();
    if( m.dims <= 2 )
    {
        step[0] = m.step[0]; step[1] = m.step[1];
    }
    else
    {
        dims = 0;
        copySize(m);
    }
}

inline
UMat& UMat::operator = (const UMat& m)
{
    if( this != &m )
    {
        const_cast<UMat&>(m).addref();
        release();
        flags = m.flags;
        if( dims <= 2 && m.dims <= 2 )
        {
            dims = m.dims;
            rows = m.rows;
            cols = m.cols;
            step[0] = m.step[0];
            step[1] = m.step[1];
        }
        else
            copySize(m);
        allocator = m.allocator;
        if (usageFlags == USAGE_DEFAULT)
            usageFlags = m.usageFlags;
        u = m.u;
        offset = m.offset;
    }
    return *this;
}

inline
UMat UMat::row(int y) const
{
    return UMat(*this, Range(y, y + 1), Range::all());
}

inline
UMat UMat::col(int x) const
{
    return UMat(*this, Range::all(), Range(x, x + 1));
}

inline
UMat UMat::rowRange(int startrow, int endrow) const
{
    return UMat(*this, Range(startrow, endrow), Range::all());
}

inline
UMat UMat::rowRange(const Range& r) const
{
    return UMat(*this, r, Range::all());
}

inline
UMat UMat::colRange(int startcol, int endcol) const
{
    return UMat(*this, Range::all(), Range(startcol, endcol));
}

inline
UMat UMat::colRange(const Range& r) const
{
    return UMat(*this, Range::all(), r);
}

inline
UMat UMat::clone() const
{
    UMat m;
    copyTo(m);
    return m;
}

inline
void UMat::assignTo( UMat& m, int _type ) const
{
    if( _type < 0 )
        m = *this;
    else
            throw std::runtime_error("operation not available in minicv");
}

inline
void UMat::create(int _rows, int _cols, int _type, UMatUsageFlags _usageFlags)
{
    _type &= TYPE_MASK;
    if( dims <= 2 && rows == _rows && cols == _cols && type() == _type && u )
        return;
    int sz[] = {_rows, _cols};
    create(2, sz, _type, _usageFlags);
}

inline
void UMat::create(Size _sz, int _type, UMatUsageFlags _usageFlags)
{
    create(_sz.height, _sz.width, _type, _usageFlags);
}

inline
void UMat::addref()
{
    if( u )
        CV_XADD(&(u->urefcount), 1);
}

inline void UMat::release()
{
    if( u && CV_XADD(&(u->urefcount), -1) == 1 )
        deallocate();
    for(int i = 0; i < dims; i++)
        size.p[i] = 0;
    u = 0;
}

inline
UMat UMat::operator()( Range _rowRange, Range _colRange ) const
{
    return UMat(*this, _rowRange, _colRange);
}

inline
UMat UMat::operator()( const Rect& roi ) const
{
    return UMat(*this, roi);
}

inline
UMat UMat::operator()(const Range* ranges) const
{
    return UMat(*this, ranges);
}

inline
bool UMat::isContinuous() const
{
    return (flags & CONTINUOUS_FLAG) != 0;
}

inline
bool UMat::isSubmatrix() const
{
    return (flags & SUBMATRIX_FLAG) != 0;
}

inline
size_t UMat::elemSize() const
{
    return dims > 0 ? step.p[dims - 1] : 0;
}

inline
size_t UMat::elemSize1() const
{
    return CV_ELEM_SIZE1(flags);
}

inline
int UMat::type() const
{
    return CV_MAT_TYPE(flags);
}

inline
int UMat::depth() const
{
    return CV_MAT_DEPTH(flags);
}

inline
int UMat::channels() const
{
    return CV_MAT_CN(flags);
}

inline
size_t UMat::step1(int i) const
{
    return step.p[i] / elemSize1();
}

inline
bool UMat::empty() const
{
    return u == 0 || total() == 0;
}

inline
size_t UMat::total() const
{
    if( dims <= 2 )
        return (size_t)rows * cols;
    size_t p = 1;
    for( int i = 0; i < dims; i++ )
        p *= size[i];
    return p;
}

inline bool UMatData::hostCopyObsolete() const { return (flags & HOST_COPY_OBSOLETE) != 0; }
inline bool UMatData::deviceCopyObsolete() const { return (flags & DEVICE_COPY_OBSOLETE) != 0; }
inline bool UMatData::deviceMemMapped() const { return (flags & DEVICE_MEM_MAPPED) != 0; }
inline bool UMatData::copyOnMap() const { return (flags & COPY_ON_MAP) != 0; }
inline bool UMatData::tempUMat() const { return (flags & TEMP_UMAT) != 0; }
inline bool UMatData::tempCopiedUMat() const { return (flags & TEMP_COPIED_UMAT) == TEMP_COPIED_UMAT; }

inline void UMatData::markDeviceMemMapped(bool flag)
{
  if(flag)
    flags |= DEVICE_MEM_MAPPED;
  else
    flags &= ~DEVICE_MEM_MAPPED;
}

inline void UMatData::markHostCopyObsolete(bool flag)
{
    if(flag)
        flags |= HOST_COPY_OBSOLETE;
    else
        flags &= ~HOST_COPY_OBSOLETE;
}
inline void UMatData::markDeviceCopyObsolete(bool flag)
{
    if(flag)
        flags |= DEVICE_COPY_OBSOLETE;
    else
        flags &= ~DEVICE_COPY_OBSOLETE;
}

//! @endcond

} //minicv

#endif
