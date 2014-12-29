#include "Matrix.h"
#include <stdio.h>
#include <string.h>
using namespace std;

Matrix::Matrix(int row , int col)
{

  	data = new float*[row];
 	 for(int i = 0; i < row; i++) {
		data[i] = new float[col];
  	}
	rows = row;
	cols = col;
}

Matrix::Matrix(const Matrix& m)
{
	rows = m.rows;
	cols = m.cols;

	data = new float*[rows];


	for(int i = 0; i < rows; i++){
		data[i] = new float[cols];
		for(int j=0; j<cols; j++)
		{
			data[i][j] = m.data[i][j];
		}
	}
}

int Matrix::numRows()
{
	return rows;
}

int Matrix::numCols()
{
	return cols;
}


float* Matrix::access(const int i , const int j) const
{
	return &data[i][j];
}


ostream& operator<<(ostream &os, Matrix &m) 
{
	int rows = m.rows;
	int cols = m.cols;
	os << rows << " " << cols << endl;
	for(int i = 0; i < rows; i++)
	{
		for(int j = 0; j < cols; j++)
		{
			os << m.data[i][j] << "  ";
		}
		os << endl;
	}
}


Matrix Matrix::readMatrix(string filename)
{
	ifstream matrixData(filename.c_str());
	string params;
	string line;

	getline(matrixData,params);
	// convert from str -> char*
	char param_c_str[params.length() + 1];
	strcpy(param_c_str,params.c_str());
	int rows = atoi(strtok(param_c_str, " "));
	int cols = atoi(strtok(NULL,""));
	//cout << "Rows : " << rows << " Cols: " << cols << endl;
	Matrix temp(rows,cols);
	for(int i = 0; i < rows; i++){
		getline(matrixData , line);

		// convert from str -> char*
		char line_c_str[line.length() + 1];
		strcpy(line_c_str,line.c_str());

		float value = atoi(strtok(line_c_str , " "));
		temp.data[i][0] = value;
		//cout << "R,C: " << i << " " << 0 << endl;
		for(int j=1; j<cols; j++)
		{
			float number = atoi(strtok(NULL," "));
			temp.data[i][j] = number;
			//cout << "R,C: " << i << " "<< j << ":" << number << endl;
		}
	}

	return temp;

}
