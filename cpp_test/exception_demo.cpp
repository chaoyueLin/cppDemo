
#include <iostream>
#include <stdexcept>

using namespace std;

class matrix
{
public:
  friend matrix
  operator*(const matrix&,
            const matrix&);
    matrix(size_t nrows, size_t ncols) : nrows_(nrows), ncols_(ncols)
    {
        data_ = new float[nrows * ncols];
    }
    ~matrix()
    {
        delete[] data_;
    }

private:
    float *data_;
    size_t nrows_;
    size_t ncols_;
};

matrix operator*(const matrix& lhs,
                 const matrix& rhs)
{
  if (lhs.ncols_ != rhs.nrows_) {
    throw std::runtime_error(
      "matrix sizes mismatch");
  }
  matrix result(lhs.nrows_, rhs.ncols_);
  // 进行矩阵乘法运算
  return result;
}

int main()
{
    matrix a(5, 5);
    matrix b(5, 5);
    matrix c = a * b;
}