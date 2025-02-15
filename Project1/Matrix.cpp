#include "Matrix.h"
using namespace std;

// Конструктор по умолчанию
Matrix::Matrix() : rows_(3), cols_(3) {
    matrix_ = new double* [rows_]; 
    for (int i = 0; i < rows_; ++i) {
        matrix_[i] = new double[cols_]; 
        for (int j = 0; j < cols_; ++j) {
            matrix_[i][j] = 0.0; 
        }
    }
}

// Конструктор с параметрами
Matrix::Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
    matrix_ = new double* [rows_];
    for (int i = 0; i < rows_; ++i) {
        matrix_[i] = new double[cols_];
        for (int j = 0; j < cols_; ++j) {
            matrix_[i][j] = 0.0;
        }
    }
}
// Конструктор с параметрами
Matrix::Matrix(int rows, int cols, double n) : rows_(rows), cols_(cols) {
    matrix_ = new double* [rows_];
    for (int i = 0; i < rows_; ++i) {
        matrix_[i] = new double[cols_];
        for (int j = 0; j < cols_; ++j) {
            matrix_[i][j] = n;
        }
    }
}

// Деструктор
Matrix::~Matrix() {
    for (int i = 0; i < rows_; ++i) {
        delete[] matrix_[i]; 
    }
    delete[] matrix_; 
}

// Конструктор копирования
Matrix::Matrix(const Matrix& m)  {
    rows_ = m.rows_;
    cols_ = m.cols_;
    matrix_ = new double* [rows_];
    for (int i = 0; i < rows_; ++i) {
        matrix_[i] = new double[cols_]; 
        for (int j = 0; j < cols_; ++j) {
            matrix_[i][j] = m.matrix_[i][j]; 
        }
    }
}

Matrix::Matrix(Matrix&& m): rows_(m.rows_), cols_(m.cols_), matrix_(m.matrix_)
{
    m.rows_ = 0;
    m.cols_ = 0;
    m.matrix_ = nullptr;
}

bool Matrix::EqMatrix(const Matrix& other)
{
    if (rows_ != other.rows_ || cols_ != other.cols_) return false;
    for (int i = 0; i < rows_; ++i)
        for (int j = 0; j < cols_; ++j)
            if (matrix_[i][j] != other.matrix_[i][j]) return false;
    return true;
}

void Matrix::SumMatrix(const Matrix& other)
{
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        cout<<"Различная размерность матриц";
    }
    for (int i = 0; i < rows_; ++i)
        for (int j = 0; j < cols_; ++j)
            matrix_[i][j] += other.matrix_[i][j];
}

void Matrix::SubMatrix(const Matrix& other)
{
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        cout << "Различная размерность матриц";
    }
    for (int i = 0; i < rows_; ++i)
        for (int j = 0; j < cols_; ++j)
            matrix_[i][j] -= other.matrix_[i][j];
}

void Matrix::MulNumber(const double num)
{
    for (int i = 0; i < rows_; ++i)
        for (int j = 0; j < cols_; ++j)
            matrix_[i][j] *= num;
}


void Matrix::MulMatrix(const Matrix& other) {

    if (cols_ != other.rows_) {
        cout << "Число столбцов первой матрицы не равно числу строк второй матрицы";
    }

    Matrix result(rows_, other.cols_);
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < other.cols_; ++j) {
            result.matrix_[i][j] = 0;
            for (int k = 0; k < cols_; ++k) {
                result.matrix_[i][j] += matrix_[i][k] * other.matrix_[k][j];
            }
        }
    }
    *this = result;
}

Matrix Matrix::Transpose()
{
    Matrix transposed(cols_, rows_);
    for (int i = 0; i < rows_; ++i)
        for (int j = 0; j < cols_; ++j)
            transposed.matrix_[j][i] = matrix_[i][j];
    return transposed;
}



Matrix Matrix::CalcComplements() {
    if (rows_ != cols_) {
        cout << "Матрица не является квадратной";
    }

    Matrix complements(rows_, cols_);
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            Matrix subMatrix(rows_ - 1, cols_ - 1);
            for (int subi = 0, si = 0; si < rows_; ++si) {
                for (int subj = 0, sj = 0; sj < cols_; ++sj) {
                    if (si != i && sj != j) {
                        subMatrix.matrix_[subi][subj] = matrix_[si][sj];
                        subj++;
                    }
                }
                if (si != i) subi++;
            }
            // Определитель подматрицы
            complements.matrix_[i][j] = (i + j) % 2 == 0 ? subMatrix.Determinant() : -subMatrix.Determinant();
        }
    }
    return complements;
}


double Matrix::Determinant() {
    if (rows_ != cols_) {
        cout << "Матрица не является квадратной";
    }

    if (rows_ == 1) {
        return matrix_[0][0];
    }
    else if (rows_ == 2) {
        return matrix_[0][0] * matrix_[1][1] - matrix_[0][1] * matrix_[1][0];
    }

    double det = 0;
    for (int i = 0; i < rows_; ++i) {
        Matrix subMatrix(rows_ - 1, cols_ - 1);
        for (int subi = 0; subi < rows_ - 1; ++subi) {
            for (int subj = 0; subj < cols_ - 1; ++subj) {
                subMatrix.matrix_[subi][subj] = matrix_[subi + 1][(subj >= i) ? subj + 1 : subj];
            }
        }
        det += (i % 2 == 0 ? 1 : -1) * matrix_[0][i] * subMatrix.Determinant();
    }
    return det;
}

Matrix Matrix::InverseMatrix() {
    double det = Determinant();
    if (det == 0) {
        cout << "Определитель матрицы равен 0";
    }

    Matrix complements = CalcComplements();
    Matrix adjugate(rows_, cols_);

    // Транспонирование матрицы алгебраических дополнений
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            adjugate.matrix_[j][i] = complements.matrix_[i][j]; 
        }
    }

    Matrix inverse(rows_, cols_);
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            inverse.matrix_[i][j] = adjugate.matrix_[i][j] / det;
        }
    }

    return inverse;
}

    

Matrix Matrix::operator+(const Matrix& other) const {
    Matrix result(*this);  
    result.SumMatrix(other);  
    return result;
}

Matrix Matrix::operator-(const Matrix& other) const {
    Matrix result(*this);
    result.SubMatrix(other);
    return result;
}


Matrix Matrix::operator*(const Matrix& other) const {
    Matrix result(*this);  
    result.MulMatrix(other);  
    return result;
}

Matrix Matrix::operator*(double scalar) const {
    Matrix result(*this);
    result.MulNumber(scalar);
    return result;
}

bool Matrix::operator==(const Matrix& other) const {
    Matrix result(*this);
    bool flag = result.EqMatrix(other);
    return flag;
}
Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) { 
        for (int i = 0; i < rows_; ++i) {
            delete[] matrix_[i];
        }
        delete[] matrix_;

        rows_ = other.rows_;
        cols_ = other.cols_;
        matrix_ = new double* [rows_];
        for (int i = 0; i < rows_; ++i) {
            matrix_[i] = new double[cols_];
            for (int j = 0; j < cols_; ++j) {
                matrix_[i][j] = other.matrix_[i][j];
            }
        }
    }
    return *this;
}

Matrix& Matrix::operator+=(const Matrix& other) {
    SumMatrix(other); 
    return *this;
}

Matrix& Matrix::operator-=(const Matrix& other) {
    SubMatrix(other);
    return *this;
}

Matrix& Matrix::operator*=(const Matrix& other) {
    MulMatrix(other);
    return *this;
}

Matrix& Matrix::operator*=(double scalar) {
    MulNumber(scalar);
    return *this;
}

double& Matrix::operator()(int i, int j) {
    if (i < 0 || i >= rows_ || j < 0 || j >= cols_) {
        throw std::out_of_range("Индекс за пределами матрицы");
    }
    return matrix_[i][j];
}

const double& Matrix::operator()(int i, int j) const {
    if (i < 0 || i >= rows_ || j < 0 || j >= cols_) {
        throw std::out_of_range("Индекс за пределами матрицы");
    }
    return matrix_[i][j];
}

void Matrix::print() {
    for (int i = 0; i < rows_; ++i) {
        cout << '\n';
        for (int j = 0; j < cols_; ++j) {
            cout << matrix_[i][j] << ' ';
        }
    }
}

void Matrix::SetValue(int row, int col, double value) {
    if (row < 0 || row >= rows_ || col < 0 || col >= cols_) {
        throw std::out_of_range("Индекс выходит за пределы матрицы");
    }
    matrix_[row][col] = value;
}


