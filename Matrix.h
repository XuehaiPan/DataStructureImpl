//
// Created by 潘学海.
//

#ifndef DATASTRUCTURE_MATRIX_H
#define DATASTRUCTURE_MATRIX_H

template <class T>
class Matrix
{
protected:
	T **mat;
	unsigned int mrows, mcols;

public:
	Matrix(const unsigned int trows = 0, const unsigned int tcols = 0)
	{
		if (trows != 0 && tcols != 0)
		{
			mrows = trows, mcols = tcols;
			mat = new T *[mrows];
			for (unsigned int r = 0; r < mrows; ++r)
				mat[r] = new T[mcols];
		} else
		{
			mrows = 0, mcols = 0;
			mat = nullptr;
		}
	}

	Matrix(const Matrix &tmat)
			: mat(nullptr)
	{
		*this = tmat;
	}

	Matrix(const T array[], const unsigned int trows, const unsigned int tcols)
	{
		if (trows != 0 && tcols != 0)
		{
			mrows = trows, mcols = tcols;
			mat = new T *[mrows];
			for (unsigned int r = 0; r < mrows; ++r)
			{
				mat[r] = new T[mcols];
				for (unsigned int c = 0; c < mcols; ++c)
					mat[r][c] = array[c + r * mcols];
			}
		} else
		{
			mrows = 0, mcols = 0;
			mat = nullptr;
		}
	}

	Matrix &operator=(const Matrix &tmat)
	{
		if (!tmat.isEmpty())
		{
			resize(tmat.rows(), tmat.cols());
			for (unsigned int r = 0; r < mrows; ++r)
			{
				for (unsigned int c = 0; c < mcols; ++c)
					mat[r][c] = tmat[r][c];
			}
		} else
		{
			mrows = 0, mcols = 0;
			mat = nullptr;
		}
		return *this;
	}

	virtual ~Matrix()
	{
		clear();
	}

	void clear(const unsigned int trows = 0, const unsigned int tcols = 0)
	{
		if (!isEmpty())
		{
			for (unsigned int r = 0; r < mrows; ++r)
				delete[] mat[r];
			delete[] mat;
			mat = new T *[mrows];
			for (unsigned int r = 0; r < mrows; ++r)
				mat[r] = new T[mcols];
		}
	}

	void resize(const unsigned int trows = 0, const unsigned int tcols = 0)
	{
		clear();
		if (trows != 0 && tcols != 0)
		{
			mrows = trows, mcols = tcols;
			mat = new T *[mrows];
			for (unsigned int r = 0; r < mrows; ++r)
				mat[r] = new T[mcols];
		} else
		{
			mrows = 0, mcols = 0;
			mat = nullptr;
		}
	}

	inline unsigned int rows() const
	{
		return mrows;
	}

	inline unsigned int cols() const
	{
		return mcols;
	}

	inline bool isEmpty() const
	{
		return (mrows == 0 && mcols == 0);
	}

	const Matrix &transpose()
	{
		*this = transposed();
		return *this;
	}

	Matrix transposed() const
	{
		Matrix tmat(mcols, mrows);
		for (unsigned int r = 0; r < mrows; ++r)
		{
			for (unsigned int c = 0; c < mcols; ++c)
				tmat[c][r] = mat[r][c];
		}
		return tmat;
	}

	T trace() const
	{
		T tr = 0;
		if (mrows >= mcols)
		{
			for (unsigned int c = 0; c < mcols; ++c)
				tr += mat[c][c];
		} else
		{
			for (unsigned int r = 0; r < mrows; ++r)
				tr += mat[r][r];
		}
		return tr;
	}

	T det() const
	{
		if (!isEmpty() && mrows == mcols)
		{
			T d = 1;
			Matrix tmat(refed());
			for (unsigned int r = 0; r < mrows; ++r)
				d *= tmat[r][r];
			return d;
		} else
			return 0;
	}

	const Matrix &ref()
	{
		*this = refed();
		return *this;
	}

	Matrix refed() const
	{
		Matrix tmat(*this);
		for (unsigned int r = 0, c = 0; r < mrows - 1 && c < mcols; ++c)
		{
			for (unsigned int tr = r; tr < mrows; ++tr)
			{
				if (tmat[tr][c] != 0)
				{
					if (tr != r)
					{
						tmat.swapRow(r, tr);
						tmat.multiRow(r, -1);
					}
					break;
				}
			}
			if (tmat[r][c] != 0)
			{
				for (unsigned int tr = 0; tr < mrows; ++tr)
				{
					if (tr != r)
						tmat.addRow(tr, r, -tmat[tr][c] / tmat[r][c]);
				}
				++r;
			}
		}
		return tmat;
	}

	const Matrix &reff()
	{
		*this = reffed();
		return *this;
	}

	Matrix reffed() const
	{
		Matrix tmat(*this);
		tmat.ref();
		for (unsigned int r = 0, c = 0; r < mrows; ++r)
		{
			for (c = r; c < mcols; ++c)
			{
				if (tmat[r][c] != 0)
					break;
			}
			if (tmat[r][c] != 0)
				tmat.multiRow(r, 1 / tmat[r][c]);
			else
				continue;
		}
		return tmat;
	}

	const Matrix &multiRow(const unsigned int trow, const T k = 1) const
	{
		if (!isEmpty() && trow <= mrows)
		{
			for (unsigned int c = 0; c < mcols; ++c)
				mat[trow][c] *= k;
		}
		return *this;
	}

	Matrix multiRowed(const unsigned int trow, const T k = 1) const
	{
		Matrix tmat(*this);
		tmat.multiRow(trow, k);
		return tmat;
	}

	const Matrix &multiCol(const unsigned int tcol, const T k = 1) const
	{
		if (!isEmpty() && tcol <= mcols)
		{
			for (unsigned int r = 0; r < mrows; ++r)
				mat[r][tcol] *= k;
		}
		return *this;
	}

	Matrix multiColed(const unsigned int tcol, const T k = 1) const
	{
		Matrix tmat(*this);
		tmat.multiCol(tcol, k);
		return tmat;
	}

	const Matrix &addRow(const unsigned int row1, const unsigned int row2, const T k = 1) const
	{
		if (!isEmpty() && row1 <= mrows && row2 <= mrows)
		{
			for (unsigned int c = 0; c < mcols; ++c)
				mat[row1][c] += k * mat[row2][c];
		}
		return *this;
	}

	Matrix addRowed(const unsigned int row1, const unsigned int row2, const T k = 1) const
	{
		Matrix tmat(*this);
		tmat.addRow(row1, row2, k);
		return tmat;
	}

	const Matrix &addCol(const unsigned int col1, const unsigned int col2, const T k = 1) const
	{
		if (!isEmpty() && col1 <= mcols && col2 <= mcols)
		{
			for (unsigned int r = 0; r < mrows; ++r)
				mat[r][col1] += k * mat[r][col2];
		}
		return *this;
	}

	Matrix addColed(const unsigned int col1, const unsigned int col2, const T k = 1) const
	{
		Matrix tmat(*this);
		tmat.addCol(col1, col2, k);
		return tmat;
	}

	const Matrix &swapRow(const unsigned int row1, const unsigned int row2) const
	{
		if (!isEmpty() && row1 <= mrows && row2 <= mrows)
		{
			T *trow = mat[row1];
			mat[row1] = mat[row2];
			mat[row2] = trow;
		}
		return *this;
	}

	Matrix swapRowed(const unsigned int row1, const unsigned int row2) const
	{
		Matrix tmat(*this);
		tmat.swapRow(row1, row2);
		return tmat;
	}

	const Matrix &swapCol(const unsigned int col1, const unsigned int col2) const
	{
		if (!isEmpty() && col1 <= mcols && col2 <= mcols)
		{
			T temp;
			for (unsigned int r = 0; r < mrows; ++r)
			{
				temp = mat[r][col1];
				mat[r][col1] = mat[r][col2];
				mat[r][col2] = temp;
			}
		}
		return *this;
	}

	Matrix swapColed(const unsigned int col1, const unsigned col2) const
	{
		Matrix tmat(*this);
		tmat.swapCol(col1, col2);
		return tmat;
	}

	void fill(const T &key) const
	{
		if (!isEmpty())
		{
			for (unsigned int r = 0; r < mrows; ++r)
			{
				for (unsigned int c = 0; c < mcols; ++c)
					mat[r][c] = key;
			}
		}
	}

	T *const operator[](const unsigned int trow) const
	{
		return mat[trow];
	}

	Matrix operator+(const Matrix &tmat) const
	{
		Matrix rmat(*this);
		if (!isEmpty() && tmat.rows() == mrows && tmat.cols() == mcols)
		{
			for (unsigned int r = 0; r < mrows; ++r)
			{
				for (unsigned int c = 0; c < mcols; ++c)
					rmat[r][c] += tmat[r][c];
			}
		}
		return rmat;
	}

	const Matrix &operator+=(const Matrix &tmat)
	{
		if (!isEmpty() && tmat.rows() == mrows && tmat.cols() == mcols)
		{
			for (unsigned int r = 0; r < mrows; ++r)
			{
				for (unsigned int c = 0; c < mcols; ++c)
					mat[r][c] += tmat[r][c];
			}
		}
		return *this;
	}

	Matrix operator-(const Matrix &tmat) const
	{
		Matrix rmat(*this);
		if (!isEmpty() && tmat.rows() == mrows && tmat.cols() == mcols)
		{
			for (unsigned int r = 0; r < mrows; ++r)
			{
				for (unsigned int c = 0; c < mcols; ++c)
					rmat[r][c] -= tmat[r][c];
			}
		}
		return rmat;
	}

	const Matrix &operator-=(const Matrix &tmat)
	{
		if (!isEmpty() && tmat.rows() == mrows && tmat.cols() == mcols)
		{
			for (unsigned int r = 0; r < mrows; ++r)
			{
				for (unsigned int c = 0; c < mcols; ++c)
					mat[r][c] -= tmat[r][c];
			}
		}
		return *this;
	}

	Matrix operator*(const Matrix &tmat) const
	{
		if (!isEmpty() && !tmat.isEmpty() && tmat.rows() == mcols)
		{
			Matrix rmat(mrows, tmat.cols());
			for (unsigned int r = 0; r < mrows; ++r)
			{
				for (unsigned int c = 0; c < tmat.cols(); ++c)
				{
					rmat[r][c] = 0;
					for (unsigned int i = 0; i < mcols; ++i)
						rmat[r][c] += mat[r][i] * tmat[i][c];
				}
			}
			return rmat;
		} else
			return Matrix(0, 0);
	}

	const Matrix &operator*=(const Matrix &tmat)
	{
		*this = (*this) * tmat;
		return *this;
	}

	Matrix operator*(const T k) const
	{
		Matrix rmat(*this);
		if (!isEmpty())
		{
			for (unsigned int r = 0; r < mrows; ++r)
			{
				for (unsigned int c = 0; c < mcols; ++c)
					rmat[r][c] *= k;
			}
		}
		return rmat;
	}

	const Matrix &operator*=(const T k) const
	{
		if (!isEmpty())
		{
			for (unsigned int r = 0; r < mrows; ++r)
			{
				for (unsigned int c = 0; c < mcols; ++c)
					mat[r][c] *= k;
			}
		}
		return *this;
	}

	void forEach(void(*func)(T &)) const
	{
		for (unsigned int r = 0; r < mrows; ++r)
		{
			for (unsigned int c = 0; c < mcols; ++c)
				func(mat[r][c]);
		}
	}

	void forEach(void(*func)(T *const)) const
	{
		for (unsigned int r = 0; r < mrows; ++r)
		{
			for (unsigned int c = 0; c < mcols; ++c)
				func(&(mat[r][c]));
		}
	}

	void forEach(void(*func)(const unsigned int, const unsigned int, T &)) const
	{
		for (unsigned int r = 0; r < mrows; ++r)
		{
			for (unsigned int c = 0; c < mcols; ++c)
				func(r, c, mat[r][c]);
		}
	}

	void forEach(void(*func)(const unsigned int, const unsigned int, T *const)) const
	{
		for (unsigned int r = 0; r < mrows; ++r)
		{
			for (unsigned int c = 0; c < mcols; ++c)
				func(r, c, &(mat[r][c]));
		}
	}
};

template <class T>
Matrix<T> operator*(const T k, const Matrix<T> &tmat)
{
	return (tmat * k);
}

#endif //DATASTRUCTURE_MATRIX_H