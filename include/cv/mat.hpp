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

#ifndef __OPENCV_CORE_MAT_HPP__
#define __OPENCV_CORE_MAT_HPP__

#ifndef __cplusplus
#  error mat.hpp header must be compiled as C++
#endif

#include "matx.hpp"
#include "types.hpp"

namespace minicv
{

//! @addtogroup core_basic
//! @{

enum { ACCESS_READ=1<<24, ACCESS_WRITE=1<<25,
    ACCESS_RW=3<<24, ACCESS_MASK=ACCESS_RW, ACCESS_FAST=1<<26 };

class CV_EXPORTS _OutputArray;

//////////////////////// Input/Output Array Arguments /////////////////////////////////

class CV_EXPORTS _InputArray
{
public:
    enum {
        KIND_SHIFT = 16,
        FIXED_TYPE = 0x8000 << KIND_SHIFT,
        FIXED_SIZE = 0x4000 << KIND_SHIFT,
        KIND_MASK = 31 << KIND_SHIFT,

        NONE              = 0 << KIND_SHIFT,
        MAT               = 1 << KIND_SHIFT,
        MATX              = 2 << KIND_SHIFT,
        STD_VECTOR        = 3 << KIND_SHIFT,
        STD_VECTOR_VECTOR = 4 << KIND_SHIFT,
        STD_VECTOR_MAT    = 5 << KIND_SHIFT,
        EXPR              = 6 << KIND_SHIFT,
        OPENGL_BUFFER     = 7 << KIND_SHIFT,
        CUDA_HOST_MEM     = 8 << KIND_SHIFT,
        CUDA_GPU_MAT      = 9 << KIND_SHIFT,
        UMAT              =10 << KIND_SHIFT,
        STD_VECTOR_UMAT   =11 << KIND_SHIFT,
        STD_BOOL_VECTOR   =12 << KIND_SHIFT
    };

    _InputArray();
    _InputArray(int _flags, void* _obj);
    _InputArray(const Mat& m);
    template<typename _Tp> _InputArray(const Mat_<_Tp>& m);
    template<typename _Tp> _InputArray(const _Tp* vec, int n);
    template<typename _Tp, int m, int n> _InputArray(const Matx<_Tp, m, n>& matx);
    _InputArray(const double& val);
    _InputArray(const UMat& um);

    Mat getMat(int idx=-1) const;
    Mat getMat_(int idx=-1) const;
    UMat getUMat(int idx=-1) const;

    int getFlags() const;
    void* getObj() const;
    Size getSz() const;

    int kind() const;
    int dims(int i=-1) const;
    int cols(int i=-1) const;
    int rows(int i=-1) const;
    Size size(int i=-1) const;
    int sizend(int* sz, int i=-1) const;
    bool sameSize(const _InputArray& arr) const;
    size_t total(int i=-1) const;
    int type(int i=-1) const;
    int depth(int i=-1) const;
    int channels(int i=-1) const;
    bool isContinuous(int i=-1) const;
    bool isSubmatrix(int i=-1) const;
    bool empty() const;
    void copyTo(const _OutputArray& arr) const;
    void copyTo(const _OutputArray& arr, const _InputArray & mask) const;
    size_t offset(int i=-1) const;
    size_t step(int i=-1) const;
    bool isMat() const;
    bool isUMat() const;
    bool isMatVector() const;
    bool isUMatVector() const;
    bool isMatx() const;

    ~_InputArray();

protected:
    int flags;
    void* obj;
    Size sz;

    void init(int _flags, const void* _obj);
    void init(int _flags, const void* _obj, Size _sz);
};


class CV_EXPORTS _OutputArray : public _InputArray
{
public:
    enum
    {
        DEPTH_MASK_8U = 1 << CV_8U,
        DEPTH_MASK_8S = 1 << CV_8S,
        DEPTH_MASK_16U = 1 << CV_16U,
        DEPTH_MASK_16S = 1 << CV_16S,
        DEPTH_MASK_32S = 1 << CV_32S,
        DEPTH_MASK_32F = 1 << CV_32F,
        DEPTH_MASK_64F = 1 << CV_64F,
        DEPTH_MASK_ALL = (DEPTH_MASK_64F<<1)-1,
        DEPTH_MASK_ALL_BUT_8S = DEPTH_MASK_ALL & ~DEPTH_MASK_8S,
        DEPTH_MASK_FLT = DEPTH_MASK_32F + DEPTH_MASK_64F
    };

    _OutputArray();
    _OutputArray(int _flags, void* _obj);
    _OutputArray(Mat& m);
    template<typename _Tp> _OutputArray(Mat_<_Tp>& m);
    template<typename _Tp> _OutputArray(_Tp* vec, int n);
    template<typename _Tp, int m, int n> _OutputArray(Matx<_Tp, m, n>& matx);
    _OutputArray(UMat& m);

    _OutputArray(const Mat& m);
    template<typename _Tp> _OutputArray(const Mat_<_Tp>& m);
    template<typename _Tp> _OutputArray(const _Tp* vec, int n);
    template<typename _Tp, int m, int n> _OutputArray(const Matx<_Tp, m, n>& matx);
    _OutputArray(const UMat& m);

    bool fixedSize() const;
    bool fixedType() const;
    bool needed() const;
    Mat& getMatRef(int i=-1) const;
    UMat& getUMatRef(int i=-1) const;
    void create(Size sz, int type, int i=-1, bool allowTransposed=false, int fixedDepthMask=0) const;
    void create(int rows, int cols, int type, int i=-1, bool allowTransposed=false, int fixedDepthMask=0) const;
    void create(int dims, const int* size, int type, int i=-1, bool allowTransposed=false, int fixedDepthMask=0) const;
    void createSameSize(const _InputArray& arr, int mtype) const;
    void release() const;
    void clear() const;
    void setTo(const _InputArray& value, const _InputArray & mask = _InputArray()) const;

    void assign(const UMat& u) const;
    void assign(const Mat& m) const;
};


class CV_EXPORTS _InputOutputArray : public _OutputArray
{
public:
    _InputOutputArray();
    _InputOutputArray(int _flags, void* _obj);
    _InputOutputArray(Mat& m);
    template<typename _Tp> _InputOutputArray(Mat_<_Tp>& m);
    template<typename _Tp> _InputOutputArray(_Tp* vec, int n);
    template<typename _Tp, int m, int n> _InputOutputArray(Matx<_Tp, m, n>& matx);
    _InputOutputArray(UMat& m);

    _InputOutputArray(const Mat& m);
    template<typename _Tp> _InputOutputArray(const Mat_<_Tp>& m);
    template<typename _Tp> _InputOutputArray(const _Tp* vec, int n);
    template<typename _Tp, int m, int n> _InputOutputArray(const Matx<_Tp, m, n>& matx);
    _InputOutputArray(const UMat& m);
};

typedef const _InputArray& InputArray;
typedef InputArray InputArrayOfArrays;
typedef const _OutputArray& OutputArray;
typedef OutputArray OutputArrayOfArrays;
typedef const _InputOutputArray& InputOutputArray;
typedef InputOutputArray InputOutputArrayOfArrays;

CV_EXPORTS InputOutputArray noArray();

/////////////////////////////////// MatAllocator //////////////////////////////////////

//! Usage flags for allocator
enum UMatUsageFlags
{
    USAGE_DEFAULT = 0,

    // buffer allocation policy is platform and usage specific
    USAGE_ALLOCATE_HOST_MEMORY = 1 << 0,
    USAGE_ALLOCATE_DEVICE_MEMORY = 1 << 1,
    USAGE_ALLOCATE_SHARED_MEMORY = 1 << 2, // It is not equal to: USAGE_ALLOCATE_HOST_MEMORY | USAGE_ALLOCATE_DEVICE_MEMORY

    __UMAT_USAGE_FLAGS_32BIT = 0x7fffffff // Binary compatibility hint
};

struct CV_EXPORTS UMatData;

/** @brief  Custom array allocator
*/
class CV_EXPORTS MatAllocator
{
public:
    MatAllocator() {}
    virtual ~MatAllocator() {}

    // let's comment it off for now to detect and fix all the uses of allocator
    //virtual void allocate(int dims, const int* sizes, int type, int*& refcount,
    //                      uchar*& datastart, uchar*& data, size_t* step) = 0;
    //virtual void deallocate(int* refcount, uchar* datastart, uchar* data) = 0;
    virtual UMatData* allocate(int dims, const int* sizes, int type,
                               void* data, size_t* step, int flags, UMatUsageFlags usageFlags) const = 0;
    virtual bool allocate(UMatData* data, int accessflags, UMatUsageFlags usageFlags) const = 0;
    virtual void deallocate(UMatData* data) const = 0;
    virtual void map(UMatData* data, int accessflags) const;
    virtual void unmap(UMatData* data) const;
    virtual void download(UMatData* data, void* dst, int dims, const size_t sz[],
                          const size_t srcofs[], const size_t srcstep[],
                          const size_t dststep[]) const;
    virtual void upload(UMatData* data, const void* src, int dims, const size_t sz[],
                        const size_t dstofs[], const size_t dststep[],
                        const size_t srcstep[]) const;
    virtual void copy(UMatData* srcdata, UMatData* dstdata, int dims, const size_t sz[],
                      const size_t srcofs[], const size_t srcstep[],
                      const size_t dstofs[], const size_t dststep[], bool sync) const;

    // default implementation returns DummyBufferPoolController
//    virtual BufferPoolController* getBufferPoolController(const char* id = NULL) const;
};


/////////////////////////////////////// Mat ///////////////////////////////////////////

// note that umatdata might be allocated together
// with the matrix data, not as a separate object.
// therefore, it does not have constructor or destructor;
// it should be explicitly initialized using init().
struct CV_EXPORTS UMatData
{
    enum { COPY_ON_MAP=1, HOST_COPY_OBSOLETE=2,
        DEVICE_COPY_OBSOLETE=4, TEMP_UMAT=8, TEMP_COPIED_UMAT=24,
        USER_ALLOCATED=32, DEVICE_MEM_MAPPED=64};
    UMatData(const MatAllocator* allocator);
    ~UMatData();

    // provide atomic access to the structure
    void lock();
    void unlock();

    bool hostCopyObsolete() const;
    bool deviceCopyObsolete() const;
    bool deviceMemMapped() const;
    bool copyOnMap() const;
    bool tempUMat() const;
    bool tempCopiedUMat() const;
    void markHostCopyObsolete(bool flag);
    void markDeviceCopyObsolete(bool flag);
    void markDeviceMemMapped(bool flag);

    const MatAllocator* prevAllocator;
    const MatAllocator* currAllocator;
    int urefcount;
    int refcount;
    uchar* data;
    uchar* origdata;
    size_t size;

    int flags;
    void* handle;
    void* userdata;
    int allocatorFlags_;
};

struct CV_EXPORTS MatSize
{
    explicit MatSize(int* _p);
    Size operator()() const;
    const int& operator[](int i) const;
    int& operator[](int i);
    operator const int*() const;
    bool operator == (const MatSize& sz) const;
    bool operator != (const MatSize& sz) const;

    int* p;
};

struct CV_EXPORTS MatStep
{
    MatStep();
    explicit MatStep(size_t s);
    const size_t& operator[](int i) const;
    size_t& operator[](int i);
    operator size_t() const;
    MatStep& operator = (size_t s);

    size_t* p;
    size_t buf[2];
protected:
    MatStep& operator = (const MatStep&);
};

class CV_EXPORTS Mat
{
public:
    Mat();

    Mat(int rows, int cols, int type);

    Mat(Size size, int type);

    Mat(int rows, int cols, int type, const Scalar& s);

    Mat(Size size, int type, const Scalar& s);

    Mat(int ndims, const int* sizes, int type);

    Mat(int ndims, const int* sizes, int type, const Scalar& s);

    Mat(const Mat& m);

    Mat(int rows, int cols, int type, void* data, size_t step=AUTO_STEP);

    Mat(Size size, int type, void* data, size_t step=AUTO_STEP);

    Mat(int ndims, const int* sizes, int type, void* data, const size_t* steps=0);

    Mat(const Mat& m, const Range& rowRange, const Range& colRange=Range::all());

    Mat(const Mat& m, const Rect& roi);

    Mat(const Mat& m, const Range* ranges);


    template<typename _Tp, int n> explicit Mat(const Vec<_Tp, n>& vec, bool copyData=true);

    template<typename _Tp> explicit Mat(const Point_<_Tp>& pt, bool copyData=true);

    ~Mat();

    Mat& operator = (const Mat& m);


    UMat getUMat(int accessFlags, UMatUsageFlags usageFlags = USAGE_DEFAULT) const;

    Mat row(int y) const;

    Mat col(int x) const;

    Mat rowRange(int startrow, int endrow) const;

    Mat rowRange(const Range& r) const;

    Mat colRange(int startcol, int endcol) const;

    Mat colRange(const Range& r) const;

    Mat diag(int d=0) const;

    static Mat diag(const Mat& d);

    Mat clone() const;

    void copyTo( OutputArray m ) const;

    void assignTo( Mat& m, int type=-1 ) const;

    Mat& operator = (const Scalar& s);

    Mat& setTo(InputArray value, InputArray mask=noArray());

    Mat reshape(int cn, int rows=0) const;

    Mat reshape(int cn, int newndims, const int* newsz) const;




    Mat cross(InputArray m) const;

    double dot(InputArray m) const;

    void create(int rows, int cols, int type);
    void create(Size size, int type);
    void create(int ndims, const int* sizes, int type);

    void addref();
    void release();

    void deallocate();
    void copySize(const Mat& m);

    void reserve(size_t sz);
    void resize(size_t sz);
    void resize(size_t sz, const Scalar& s);

    void push_back_(const void* elem);
    template<typename _Tp> void push_back(const _Tp& elem);
    template<typename _Tp> void push_back(const Mat_<_Tp>& elem);
    void push_back(const Mat& m);

    void pop_back(size_t nelems=1);

    void locateROI( Size& wholeSize, Point& ofs ) const;
    Mat& adjustROI( int dtop, int dbottom, int dleft, int dright );

    Mat operator()( Range rowRange, Range colRange ) const;
    Mat operator()( const Rect& roi ) const;
    Mat operator()( const Range* ranges ) const;

    template<typename _Tp> operator std::vector<_Tp>() const;

    bool isContinuous() const;
    bool isSubmatrix() const;

    size_t elemSize() const;
    size_t elemSize1() const;
    int type() const;

    int depth() const;
    int channels() const;
    size_t step1(int i=0) const;

    bool empty() const;
    size_t total() const;

    int checkVector(int elemChannels, int depth=-1, bool requireContinuous=true) const;

    uchar* ptr(int i0=0);
    const uchar* ptr(int i0=0) const;

    uchar* ptr(int i0, int i1);
    const uchar* ptr(int i0, int i1) const;

    uchar* ptr(int i0, int i1, int i2);
    const uchar* ptr(int i0, int i1, int i2) const;

    uchar* ptr(const int* idx);
    const uchar* ptr(const int* idx) const;
    template<int n> uchar* ptr(const Vec<int, n>& idx);
    template<int n> const uchar* ptr(const Vec<int, n>& idx) const;

    template<typename _Tp> _Tp* ptr(int i0=0);
    template<typename _Tp> const _Tp* ptr(int i0=0) const;
    template<typename _Tp> _Tp* ptr(int i0, int i1);
    template<typename _Tp> const _Tp* ptr(int i0, int i1) const;
    template<typename _Tp> _Tp* ptr(int i0, int i1, int i2);
    template<typename _Tp> const _Tp* ptr(int i0, int i1, int i2) const;
    template<typename _Tp> _Tp* ptr(const int* idx);
    template<typename _Tp> const _Tp* ptr(const int* idx) const;
    template<typename _Tp, int n> _Tp* ptr(const Vec<int, n>& idx);
    template<typename _Tp, int n> const _Tp* ptr(const Vec<int, n>& idx) const;

    template<typename _Tp> _Tp& at(int i0=0);
    template<typename _Tp> const _Tp& at(int i0=0) const;
    template<typename _Tp> _Tp& at(int i0, int i1);
    template<typename _Tp> const _Tp& at(int i0, int i1) const;

    template<typename _Tp> _Tp& at(int i0, int i1, int i2);
    template<typename _Tp> const _Tp& at(int i0, int i1, int i2) const;

    template<typename _Tp> _Tp& at(const int* idx);
    template<typename _Tp> const _Tp& at(const int* idx) const;

    template<typename _Tp, int n> _Tp& at(const Vec<int, n>& idx);
    template<typename _Tp, int n> const _Tp& at(const Vec<int, n>& idx) const;

    template<typename _Tp> _Tp& at(Point pt);
    template<typename _Tp> const _Tp& at(Point pt) const;

    template<typename _Tp, typename Functor> void forEach(const Functor& operation);
    template<typename _Tp, typename Functor> void forEach(const Functor& operation) const;

    enum { MAGIC_VAL  = 0x42FF0000, AUTO_STEP = 0, CONTINUOUS_FLAG = CV_MAT_CONT_FLAG, SUBMATRIX_FLAG = CV_SUBMAT_FLAG };
    enum { MAGIC_MASK = 0xFFFF0000, TYPE_MASK = 0x00000FFF, DEPTH_MASK = 7 };

    int flags;
    int dims;
    int rows, cols;
    uchar* data;

    const uchar* datastart;
    const uchar* dataend;
    const uchar* datalimit;

    MatAllocator* allocator;
    static MatAllocator* getStdAllocator();

    //! interaction with UMat
    UMatData* u;

    MatSize size;
    MatStep step;

protected:
    template<typename _Tp, typename Functor> void forEach_impl(const Functor& operation);
};

/** @todo document */
class CV_EXPORTS UMat
{
public:
    UMat(UMatUsageFlags usageFlags = USAGE_DEFAULT);

    UMat(int rows, int cols, int type, UMatUsageFlags usageFlags = USAGE_DEFAULT);
    UMat(Size size, int type, UMatUsageFlags usageFlags = USAGE_DEFAULT);

    UMat(int rows, int cols, int type, const Scalar& s, UMatUsageFlags usageFlags = USAGE_DEFAULT);
    UMat(Size size, int type, const Scalar& s, UMatUsageFlags usageFlags = USAGE_DEFAULT);

    UMat(int ndims, const int* sizes, int type, UMatUsageFlags usageFlags = USAGE_DEFAULT);
    UMat(int ndims, const int* sizes, int type, const Scalar& s, UMatUsageFlags usageFlags = USAGE_DEFAULT);

    UMat(const UMat& m);

    //! creates a matrix header for a part of the bigger matrix
    UMat(const UMat& m, const Range& rowRange, const Range& colRange=Range::all());
    UMat(const UMat& m, const Rect& roi);
    UMat(const UMat& m, const Range* ranges);
    //! builds matrix from minicv::Vec; the data is copied by default
    template<typename _Tp, int n> explicit UMat(const Vec<_Tp, n>& vec, bool copyData=true);
    //! builds matrix from minicv::Matx; the data is copied by default
    template<typename _Tp, int m, int n> explicit UMat(const Matx<_Tp, m, n>& mtx, bool copyData=true);
    //! builds matrix from a 2D point
    template<typename _Tp> explicit UMat(const Point_<_Tp>& pt, bool copyData=true);
    //! builds matrix from a 3D point

    //! destructor - calls release()
    ~UMat();
    //! assignment operators
    UMat& operator = (const UMat& m);

    Mat getMat(int flags) const;

    //! returns a new matrix header for the specified row
    UMat row(int y) const;
    //! returns a new matrix header for the specified column
    UMat col(int x) const;
    //! ... for the specified row span
    UMat rowRange(int startrow, int endrow) const;
    UMat rowRange(const Range& r) const;
    //! ... for the specified column span
    UMat colRange(int startcol, int endcol) const;
    UMat colRange(const Range& r) const;
    //! ... for the specified diagonal
    // (d=0 - the main diagonal,
    //  >0 - a diagonal from the lower half,
    //  <0 - a diagonal from the upper half)
    UMat diag(int d=0) const;
    //! constructs a square diagonal matrix which main diagonal is vector "d"
    static UMat diag(const UMat& d);

    //! returns deep copy of the matrix, i.e. the data is copied
    UMat clone() const;
    //! copies the matrix content to "m".
    // It calls m.create(this->size(), this->type()).
    void copyTo( OutputArray m ) const;
    //! copies those matrix elements to "m" that are marked with non-zero mask elements.
    void copyTo( OutputArray m, InputArray mask ) const;

    void assignTo( UMat& m, int type=-1 ) const;

    //! sets every matrix element to s
    UMat& operator = (const Scalar& s);
    //! sets some of the matrix elements to s, according to the mask
    UMat& setTo(InputArray value, InputArray mask=noArray());
    //! creates alternative matrix header for the same data, with different
    // number of channels and/or different number of rows. see cvReshape.
    UMat reshape(int cn, int rows=0) const;
    UMat reshape(int cn, int newndims, const int* newsz) const;

    //! matrix transposition by means of matrix expressions
    UMat t() const;
    //! matrix inversion by means of matrix expressions
    UMat inv(int method=DECOMP_LU) const;
    //! per-element matrix multiplication by means of matrix expressions
    UMat mul(InputArray m, double scale=1) const;

    //! computes dot-product
    double dot(InputArray m) const;

    //! Matlab-style matrix initialization
    static UMat zeros(int rows, int cols, int type);
    static UMat zeros(Size size, int type);
    static UMat zeros(int ndims, const int* sz, int type);
    static UMat ones(int rows, int cols, int type);
    static UMat ones(Size size, int type);
    static UMat ones(int ndims, const int* sz, int type);
    static UMat eye(int rows, int cols, int type);
    static UMat eye(Size size, int type);

    //! allocates new matrix data unless the matrix already has specified size and type.
    // previous data is unreferenced if needed.
    void create(int rows, int cols, int type, UMatUsageFlags usageFlags = USAGE_DEFAULT);
    void create(Size size, int type, UMatUsageFlags usageFlags = USAGE_DEFAULT);
    void create(int ndims, const int* sizes, int type, UMatUsageFlags usageFlags = USAGE_DEFAULT);

    //! increases the reference counter; use with care to avoid memleaks
    void addref();
    //! decreases reference counter;
    // deallocates the data when reference counter reaches 0.
    void release();

    //! deallocates the matrix data
    void deallocate();
    //! internal use function; properly re-allocates _size, _step arrays
    void copySize(const UMat& m);

    //! locates matrix header within a parent matrix. See below
    void locateROI( Size& wholeSize, Point& ofs ) const;
    //! moves/resizes the current matrix ROI inside the parent matrix.
    UMat& adjustROI( int dtop, int dbottom, int dleft, int dright );
    //! extracts a rectangular sub-matrix
    // (this is a generalized form of row, rowRange etc.)
    UMat operator()( Range rowRange, Range colRange ) const;
    UMat operator()( const Rect& roi ) const;
    UMat operator()( const Range* ranges ) const;

    //! returns true iff the matrix data is continuous
    // (i.e. when there are no gaps between successive rows).
    // similar to CV_IS_MAT_CONT(cvmat->type)
    bool isContinuous() const;

    //! returns true if the matrix is a submatrix of another matrix
    bool isSubmatrix() const;

    //! returns element size in bytes,
    // similar to CV_ELEM_SIZE(cvmat->type)
    size_t elemSize() const;
    //! returns the size of element channel in bytes.
    size_t elemSize1() const;
    //! returns element type, similar to CV_MAT_TYPE(cvmat->type)
    int type() const;
    //! returns element type, similar to CV_MAT_DEPTH(cvmat->type)
    int depth() const;
    //! returns element type, similar to CV_MAT_CN(cvmat->type)
    int channels() const;
    //! returns step/elemSize1()
    size_t step1(int i=0) const;
    //! returns true if matrix data is NULL
    bool empty() const;
    //! returns the total number of matrix elements
    size_t total() const;

    //! returns N if the matrix is 1-channel (N x ptdim) or ptdim-channel (1 x N) or (N x 1); negative number otherwise
    int checkVector(int elemChannels, int depth=-1, bool requireContinuous=true) const;

    void* handle(int accessFlags) const;
    void ndoffset(size_t* ofs) const;

    enum { MAGIC_VAL  = 0x42FF0000, AUTO_STEP = 0, CONTINUOUS_FLAG = CV_MAT_CONT_FLAG, SUBMATRIX_FLAG = CV_SUBMAT_FLAG };
    enum { MAGIC_MASK = 0xFFFF0000, TYPE_MASK = 0x00000FFF, DEPTH_MASK = 7 };

    /*! includes several bit-fields:
         - the magic signature
         - continuity flag
         - depth
         - number of channels
     */
    int flags;
    //! the matrix dimensionality, >= 2
    int dims;
    //! the number of rows and columns or (-1, -1) when the matrix has more than 2 dimensions
    int rows, cols;

    //! custom allocator
    MatAllocator* allocator;
    UMatUsageFlags usageFlags; // usage flags for allocator
    //! and the standard allocator
    static MatAllocator* getStdAllocator();

    // black-box container of UMat data
    UMatData* u;

    // offset of the submatrix (or 0)
    size_t offset;

    MatSize size;
    MatStep step;

protected:
};

/////////////////////////////////// NAryMatIterator //////////////////////////////////

class CV_EXPORTS NAryMatIterator
{
public:
    //! the default constructor
    NAryMatIterator();
    //! the full constructor taking arbitrary number of n-dim matrices
    NAryMatIterator(const Mat** arrays, uchar** ptrs, int narrays=-1);
    //! the full constructor taking arbitrary number of n-dim matrices
    NAryMatIterator(const Mat** arrays, Mat* planes, int narrays=-1);
    //! the separate iterator initialization method
    void init(const Mat** arrays, Mat* planes, uchar** ptrs, int narrays=-1);

    //! proceeds to the next plane of every iterated matrix
    NAryMatIterator& operator ++();
    //! proceeds to the next plane of every iterated matrix (postfix increment operator)
    NAryMatIterator operator ++(int);

    //! the iterated arrays
    const Mat** arrays;
    //! the current planes
    Mat* planes;
    //! data pointers
    uchar** ptrs;
    //! the number of arrays
    int narrays;
    //! the number of hyper-planes that the iterator steps through
    size_t nplanes;
    //! the size of each segment (in elements)
    size_t size;
protected:
    int iterdepth;
    size_t idx;
};


} // minicv

#include "mat.inl.hpp"

#endif // __OPENCV_CORE_MAT_HPP__
