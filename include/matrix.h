class MatAllocator
{
public:
	MatAllocator() {}
	virtual ~MatAllocator() {}

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
	virtual BufferPoolController* getBufferPoolController(const char* id = NULL) const;
};

struct MatSize
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

struct MatStep
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

class Mat
{
public:
	Mat();

	Mat(int rows, int cols, int type);

	Mat(int rows, int cols, int type, const Scalar& s);

	Mat(const Mat& m);

	Mat(int rows, int cols, int type, void* data, size_t step=AUTO_STEP);

//	Mat(const Mat& m, const Rect& roi);
//	template<typename _Tp> explicit Mat(const std::vector<_Tp>& vec, bool copyData=false);

	~Mat();

	Mat& operator = (const Mat& m);

	Mat row(int y) const;

	Mat col(int x) const;

	Mat rowRange(int startrow, int endrow) const;

	Mat colRange(int startcol, int endcol) const;

	Mat clone() const;

	void copyTo( OutputArray m ) const;

	void convertTo( OutputArray m, int rtype, double alpha=1, double beta=0 ) const;

	Mat& operator = (const Scalar& s);

	Mat reshape(int cn, int rows=0) const;

	static MatExpr zeros(int rows, int cols, int type);

	static MatExpr ones(int rows, int cols, int type);

	static MatExpr eye(int rows, int cols, int type);

	void create(int rows, int cols, int type);

	void addref();

	void release();

	void deallocate();

	void copySize(const Mat& m);

	void reserve(size_t sz);

	void resize(size_t sz);

	void resize(size_t sz, const Scalar& s);

	bool isContinuous() const;

	/// @brief Returns the matrix element size in bytes.
	size_t elemSize() const;

	/// @brief Returns the size of each matrix element channel in bytes.
//	size_t elemSize1() const;

	/// @brief Returns the type of a matrix element.
	int type() const;

	/// @brief Returns the depth of a matrix element.
	int depth() const;

	/// @brief Returns the number of matrix channels.
	int channels() const;

	/// @brief Returns a normalized step.
	size_t step1(int i=0) const;

	/// @brief Returns true if the array has no elements.
	bool empty() const;

	/// @brief Returns the total number of array elements.
	size_t total() const;

	/// @brief Returns a pointer to the specified matrix row.
	uchar* ptr(int i0=0);
	const uchar* ptr(int i0=0) const;

	uchar* ptr(int i0, int i1);
	const uchar* ptr(int i0, int i1) const;

	template<typename _Tp> _Tp* ptr(int i0=0);
	template<typename _Tp> const _Tp* ptr(int i0=0) const;

	template<typename _Tp> _Tp* ptr(int i0, int i1);
	template<typename _Tp> const _Tp* ptr(int i0, int i1) const;

	/// @brief Returns a reference to the specified array element.
	template<typename _Tp> _Tp& at(int i0=0);
	template<typename _Tp> const _Tp& at(int i0=0) const;

	template<typename _Tp> _Tp& at(int i0, int i1);
	template<typename _Tp> const _Tp& at(int i0, int i1) const;

//	template<typename _Tp> _Tp& at(Point pt);
//	template<typename _Tp> const _Tp& at(Point pt) const;

	/// @brief Returns the matrix iterator and sets it to the first matrix element.
	template<typename _Tp> MatIterator_<_Tp> begin();
	template<typename _Tp> MatConstIterator_<_Tp> begin() const;

	/// @brief Returns the matrix iterator and sets it to the after-last matrix element.
	template<typename _Tp> MatIterator_<_Tp> end();
	template<typename _Tp> MatConstIterator_<_Tp> end() const;

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
	//! pointer to the data
	uchar* data;

	//! helper fields used in locateROI and adjustROI
	const uchar* datastart;
	const uchar* dataend;
	const uchar* datalimit;

	//! custom allocator
	MatAllocator* allocator;
	//! and the standard allocator
	static MatAllocator* getStdAllocator();

	//! interaction with UMat
	UMatData* u;

	MatSize size;
	MatStep step;

protected:
	template<typename _Tp, typename Functor> void forEach_impl(const Functor& operation);
};
