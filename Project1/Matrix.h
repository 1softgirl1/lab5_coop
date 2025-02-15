#pragma once
#include <iostream>
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
		const double& operator()(int i, int j) const;

};

int main() {
	setlocale(LC_ALL, "Russian");
	// Тестируем конструктор по умолчанию
	Matrix matDefault;
	cout << "\nМатрица по умолчанию (3x3):" << endl;
	matDefault.print();

	// Тестируем конструктор с параметрами
	Matrix matParam(2, 2);
	cout << "\nМатрица с параметрами (2x2):" << endl;
	matParam.print();

	// Тестируем конструктор с параметрами и заполнением
	Matrix matFilled(3, 3, 5.0);
	cout << "\nМатрица, заполненная 5.0 (3x3):" << endl;
	matFilled.print();

	// Тестируем сложение
	Matrix sumResult = matFilled; 
	sumResult.SumMatrix(matFilled); 
	cout << "\nРезультат сложения матрицы с самой собой:" << endl;
	sumResult.print();


	// Тестируем вычитание
	Matrix subResult = sumResult; 
	subResult.SubMatrix(matFilled); 
	cout << "\nРезультат вычитания:" << endl;
	subResult.print();

	// Тестируем умножение на число
	Matrix mulResult = matFilled; 
	mulResult.MulNumber(2.0); 
	cout << "\nРезультат умножения на 2.0:" << endl;
	mulResult.print();

	// Тестируем равенство матриц
	if (matFilled.EqMatrix(mulResult)) {
		cout << "\nМатрицы равны." << endl;
	}
	else {
		cout << "\nМатрицы не равны." << endl;
	}

	// Тестируем конструктор копирования
	Matrix copyMat = matFilled; 
	cout << "\nКопия матрицы:" << endl;
	copyMat.print();
	
	Matrix matA(2, 2);
	matA.SetValue(0, 0, 1);
	matA.SetValue(0, 1, 2);
	matA.SetValue(1, 0, 3);
	matA.SetValue(1, 1, 4);

	Matrix matB(2, 2);
	matB.SetValue(0, 0, 5);
	matB.SetValue(0, 1, 6);
	matB.SetValue(1, 0, 7);
	matB.SetValue(1, 1, 8);

	cout << "\nМатрица A:" << endl;
	matA.print();

	cout << "\nМатрица B:" << endl;
	matB.print();

	// Тестируем умножение матриц
	Matrix mulResult1 = matA; 
	mulResult1.MulMatrix(matB); 
	cout << "\nРезультат умножения A * B:" << endl;
	mulResult1.print();

	// Тестируем транспонирование
	Matrix transposed = matA.Transpose();
	cout << "\nТранспонированная матрица A:" << endl;
	transposed.print();

	// Тестируем вычисление алгебраических дополнений
	Matrix complements = matA.CalcComplements();
	cout << "\nАлгебраические дополнения матрицы A:" << endl;
	complements.print();

	// Тестируем определитель
	double det = matA.Determinant();
	cout << "\nОпределитель матрицы A: " << det << endl;

	// Тестируем обратную матрицу
	Matrix inverse = matA.InverseMatrix();
	cout << "\nОбратная матрица A:" << endl;
	inverse.print();

	// Тестируем сложение матриц
	Matrix sumResult2 = matA + matB;
	cout << "\nРезультат A + B:" << endl;
	sumResult2.print();

	// Тестируем вычитание матриц
	Matrix subResult2  = matA - matB;
	cout << "\nРезультат A - B:" << endl;
	subResult2.print();

	// Тестируем умножение матриц
	Matrix mulResult2 = matA * matB;
	cout << "\nРезультат A * B:" << endl;
	mulResult2.print();

	// Тестируем умножение на скаляр
	double scalar = 2.0;
	Matrix scalarMulResult = matA * scalar;
	cout << "\nРезультат A * " << scalar << ":" << endl;
	scalarMulResult.print();

	// Тестируем оператор равенства
	bool isEqual = (matA == matB);
	cout << "\nМатрицы A и B равны? " << (isEqual ? "Да" : "Нет") << endl;

	// Тестируем оператор присваивания
	Matrix matC = matA; 
	cout << "\nМатрица C (копия A):" << endl;
	matC.print();

	// Изменяем матрицу C и проверяем равенство
	matC.SetValue(0, 0, 10);
	cout << "\nПосле изменения C:" << endl;
	matC.print();

	isEqual = (matA == matC);
	cout << "\nМатрицы A и C равны? " << (isEqual ? "Да" : "Нет") << endl;

	// Тестируем оператор умножения на скаляр (operator+=)
	matA += matB;
	cout << "\nРезультат A += B " <<  endl;
	matA.print();

	// Тестируем оператор умножения на скаляр (operator+=)
	matA -= matB;
	cout << "\nРезультат A -= B " << endl;
	matA.print();

	// Тестируем оператор умножения на матрицу (operator*=)
	matA *= matB;
	cout << "\nРезультат A *= B:" << endl;
	matA.print(); 

	// Тестируем оператор умножения на скаляр (operator*=)
	double scalar2 = 2.0;
	matA *= scalar2;
	cout << "\nРезультат A *= " << scalar2 << ":" << endl;
	matA.print(); 

	// Тестируем оператор доступа ()
	try {
		cout << "\nЗначение A(1,1): " << matA(1, 1) << endl; 
		cout << "\nЗначение A(2,2): " << matA(2, 2) << endl; 
	}
	catch (const std::out_of_range& e) {
		cout << e.what() << endl; 
	}


	return 0;
};
