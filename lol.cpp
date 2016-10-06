#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;

typedef unsigned int uint;

#define ii for(uint i=0; i<left.getRows(); ++i) { 
#define jj for(uint j=0; j<right.getCols(); ++j) { 
#define kk for(uint k=0; k<left.getCols(); ++k) {
#define matrix }}}
#define calc result.addToElem(left, right, i, j, k);

#define FORALLELEMS for(uint i=0; i<rows; ++i) { for(uint j=0; j<cols; ++j) {
#define EXEC }}

class Matrix 
{
    vector< vector <double> > values;
    uint cols;
    uint rows;
public:
    Matrix(uint ncols, uint nrows, double fMin, double fMax)
    {
        cols = ncols;
        rows = nrows;
        FORALLELEMS
            if (j == 0) values.push_back(vector<double>());
            values[i].push_back(0);
        EXEC
        
        fillRandElems(fMin, fMax);
    }
    
    Matrix(string path)
    {
        ifstream matrixFileStream(path.c_str());
        matrixFileStream >> rows;
        matrixFileStream >> cols;
        
        FORALLELEMS
                double readValue;
                matrixFileStream >> readValue;
                if (j == 0) values.push_back(vector<double>());
                values[i].push_back(readValue);
        EXEC
    }
    
    double operator() (uint i, uint j) const
    {
        if (i < 0 || j < 0 || i > rows || j > cols)
            throw "Некорректные значения индексов!";
        else return values[i][j];
    }
    
    void addToElem(Matrix left, Matrix right, uint i, uint j, uint k)
    {
        values[i][j] += left(i, k)*right(k, j);
    }
    
    inline uint getCols() const
    {
        return cols;
    }
    
    inline uint getRows() const
    {
        return rows;
    }
    
    void writeToBinaryFile(string path) const
    {
        ofstream outputFile;
        outputFile.open(path.c_str(), ios::out | ios::binary);
        outputFile << rows << " ";
        outputFile << cols << " ";
        FORALLELEMS
            outputFile << values[i][j] << " ";
        EXEC
    }
    
    void print() const
    {
        FORALLELEMS
            cout << values[i][j] << "  ";
            if (j == cols-1) cout << endl;
        EXEC
    }
    
    void fillRandElems(double fMin, double fMax)
    {
        srand(time(0));
        FORALLELEMS
            double f = (double)rand() / RAND_MAX;
            values[i][j] = fMin + f * (fMax - fMin);
        EXEC
    }
    
    
};

void multiplicate(Matrix left, Matrix right, Matrix &result, uint type)
{
    if (left.getCols() != right.getRows() || result.getRows() != left.getRows() 
        || result.getCols() != right.getCols())
        throw "Некорректные размеры матриц!";
        
    switch (type)
    {
        case 0: ii jj kk calc matrix break;
        case 1: ii kk jj calc matrix break;
        case 2: jj ii kk calc matrix break;
        case 3: jj kk ii calc matrix break;
        case 4: kk ii jj calc matrix break;
        case 5: kk jj ii calc matrix break;
        default: throw "Нет такого типа умножения!";
    }
}

double eps = 0.0001;
double calcDif(Matrix f, Matrix s)
{
    double result = 0;
    
    for (uint i=0; i<f.getRows(); ++i)
    {
        for (uint j=0; j<f.getCols(); ++j)
        {
            result = max (fabs(f(i, j) - s(i, j)), result);
        }
    }
    
    return result;
}


int main(int argc, char **argv)
{
    if (argc == 1)
    {
        Matrix m(50, 50, -100, 100);
        m.writeToBinaryFile("300x300-2.bin");
    }
    else if (argc == 2)
    {
        if (string(argv[1])==string("--test"))
        {
            string firstFile="./1.bin";
            Matrix first(firstFile);
            
            string secondFile="./2.bin";
            Matrix second(secondFile);
            
            string rFile="./3.bin";
            Matrix r(rFile);
            
            Matrix result(first.getRows(), second.getCols(), 0, 0);
            multiplicate(first, second, result, 0);
            
            double d = calcDif(result, r);
            
            if (d < eps) cout << "Everything is ok!\n";
            else cout << d << " Bad result!\n";
        }
    }
    else if (argc == 3)
	{
	    if (string(argv[1])==string("--report"))
	    {
		    string fileLfet="./300x300-1.bin";
		    string fileRight="./300x300-2.bin";
		    Matrix left(fileLfet);
    		Matrix right(fileRight);
    		Matrix result(left.getRows(), right.getCols(), 0, 0);
    		
    		long start = clock();
    		for (int i=0; i<10; ++i)
    		{
    			multiplicate(left, right, result,argv[2][0]-'0');
    		}
    		long stop = clock();
    		
    		cout << (int)(argv[2][0]-'0') << " " << (double)(stop-start)/(double)CLOCKS_PER_SEC/10 << endl;
    		return 0;
	    }
	}
	else if (argc!=5)
	{
		cout << "Аргументов не пять";
		return -1;
	}
    return 0;
}