#pragma once
using namespace std;

class Matrix
{
	private:
		int rows_, cols_;
		double** matrix_;

	public:
		Matrix();// конструктор по умолчанию
		Matrix(int, int); // конструктор с параметрами
		Matrix(int, int, double); // конструктор с параметрами
		~Matrix();
		void print();
		void SetValue(int, int, double);
		Matrix(const Matrix& m);
		Matrix(Matrix&& m);

		bool EqMatrix(const Matrix& other);
		void SumMatrix(const Matrix& other);
		void SubMatrix(const Matrix& other);
		void MulNumber(const double num);
		void MulMatrix(const Matrix& other);
		Matrix Transpose();
		Matrix CalcComplements();
		double Determinant();
		Matrix InverseMatrix();
		Matrix operator+(const Matrix& other) const;
		Matrix operator-(const Matrix& other) const;
		Matrix operator*(const Matrix& other) const;
		Matrix operator*(double scalar) const;
		bool operator==(const Matrix& other) const;
		Matrix& operator=(const Matrix& other);
		Matrix& operator+=(const Matrix& other);
		Matrix& operator-=(const Matrix& other);
		Matrix& operator*=(const Matrix& other);
		Matrix& operator*=(double scalar);
		double& operator()(int i, int j);

};


