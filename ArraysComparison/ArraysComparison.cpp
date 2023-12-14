#include <iostream>
#include <time.h>
#include <chrono>

using namespace std;
const int row = 500;
const int col = 500;

template <typename T>
class SparseArray {
private:
    class ArrayEntry {
    public:
        int ColumnNumber;
        T Value;
        ArrayEntry* NextEntry;
        
        ArrayEntry(int columnNumber, T value) {
            ColumnNumber = columnNumber;
            Value = value;
            NextEntry = nullptr;
        }
    };

    class ArrayRow {
    public:
        int RowNumber;
        ArrayRow* NextRow;
        ArrayEntry* RowSentinel;
        
        ArrayRow(int rowNumber) {
            RowNumber = rowNumber;
            NextRow = nullptr;
            RowSentinel = nullptr;
        }
    };

    ArrayRow* arrayRowSentinel;

public:
    
    SparseArray()  {
        arrayRowSentinel = new ArrayRow(-1);
    }

    ~SparseArray() {        
        delete arrayRowSentinel;
    }
   
    ArrayRow* FindRowBefore(int row, ArrayRow* arrayRowSentinel) {
        ArrayRow* arrayRow = arrayRowSentinel;

        while (arrayRow->NextRow != nullptr && arrayRow->NextRow->RowNumber < row) {
            arrayRow = arrayRow->NextRow;
        }

        return arrayRow;
    }
    
    ArrayEntry* FindColumnBefore(int column, ArrayEntry* rowSentinel) {
        ArrayEntry* arrayEntry = rowSentinel;

        while (arrayEntry->NextEntry != nullptr && arrayEntry->NextEntry->ColumnNumber < column) {
            arrayEntry = arrayEntry->NextEntry;
        }

        return arrayEntry;
    }
    
    T GetValue(int row, int column) {
        ArrayRow* arrayRow = FindRowBefore(row, arrayRowSentinel);
        arrayRow = arrayRow->NextRow;

        if (arrayRow == nullptr || arrayRow->RowNumber > row) {
            return T();  
        }

        ArrayEntry* arrayEntry = FindColumnBefore(column, arrayRow->RowSentinel);
        arrayEntry = arrayEntry->NextEntry;

        if (arrayEntry == nullptr || arrayEntry->ColumnNumber > column) {
            return T();  
        }

        return arrayEntry->Value;
    }

    void SetValue(int row, int column, T value) {
        
        if (value == 0) {
            DeleteEntry(row, column);
            return;
        }
        
        ArrayRow* arrayRow = FindRowBefore(row, arrayRowSentinel);
        
        if (arrayRow->NextRow == nullptr || arrayRow->NextRow->RowNumber > row) {
            ArrayRow* newRow = new ArrayRow(row);
            newRow->NextRow = arrayRow->NextRow;
            arrayRow->NextRow = newRow;

            ArrayEntry* sentinelEntry = new ArrayEntry(-1, T());
            newRow->RowSentinel = sentinelEntry;
            sentinelEntry->NextEntry = nullptr;
        }
       
        arrayRow = arrayRow->NextRow;
      
        ArrayEntry* arrayEntry = FindColumnBefore(column, arrayRow->RowSentinel);
       
        if (arrayEntry->NextEntry == nullptr || arrayEntry->NextEntry->ColumnNumber > column) {
            ArrayEntry* newEntry = new ArrayEntry(column, value);
            newEntry->NextEntry = arrayEntry->NextEntry;
            arrayEntry->NextEntry = newEntry;
        }
        else if (arrayEntry->NextEntry->ColumnNumber == column) {            
            arrayEntry->NextEntry->Value = value;
        }
    }
    
    void DeleteEntry(int row, int column) {
        ArrayRow* arrayRow = FindRowBefore(row, arrayRowSentinel);

        if (arrayRow->NextRow == nullptr || arrayRow->NextRow->RowNumber > row) {
            return;
        }

        ArrayRow* targetRow = arrayRow->NextRow;
        ArrayEntry* arrayEntry = FindColumnBefore(column, targetRow->RowSentinel);

        if (arrayEntry->NextEntry == nullptr || arrayEntry->NextEntry->ColumnNumber > column) {
            return;
        }

        arrayEntry->NextEntry = arrayEntry->NextEntry->NextEntry;

        if (targetRow->RowSentinel->NextEntry != nullptr) {
            return;
        }

        arrayRow->NextRow = arrayRow->NextRow->NextRow;
    }
};

class Timer {
private: 
    using clock_t = chrono::high_resolution_clock;
    using second_t = chrono::duration<double, ratio<1,1000> >;
    chrono::time_point<clock_t> m_beg;
public:
    Timer() : m_beg(clock_t::now()) {}
    void reset() { m_beg = clock_t::now(); }
    double elapsed() const {
        return chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
    }
};

void StaticRectangularArrayTest() {

    double timer1;    
    int array[row][col];
    Timer t1;    
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            array[i][j] = rand()%2;            
        }  

    }
    timer1 = t1.elapsed();
    cout << "Time of filling a rectangular static array: " << timer1 << " ms" << endl;
    t1.reset();
    array[0][0];
    timer1 = t1.elapsed();
    cout << "Access time to the first element of the rectangular static array: " << timer1 << " ms" << endl;
    t1.reset();
    array[row - 1][row - 1];
    timer1 = t1.elapsed();
    cout << "Access time to the last element of the rectangular static array: " << timer1 << " ms" << endl;
    cout << endl;
    
}

void DynamicRectangularArrayTest() {
    
    double timer1;    
    int **array = new int *[row];
    for (int i = 0; i < row; i++) {
        array[i] = new int[col];
    }
    Timer t1;
    for (int i = 0; i < row; i++) {       
        for (int j = 0; j < col; j++) {
            array[i][j] = rand() % 2;
        }

    }
    timer1 = t1.elapsed();
    cout << "Time to filling a rectangular dynamic array: " << timer1 << " ms" << endl;
    t1.reset();
    array[0][0];
    timer1 = t1.elapsed();
    cout << "Access time to the first element of the rectangular dynamic array: " << timer1 << " ms" << endl;
    t1.reset();
    array[row - 1][row - 1];
    timer1 = t1.elapsed();
    cout << "Access time to the last element of the rectangular dynamic array: " << timer1 << " ms" << endl;

    cout << endl;
    for (int r = 0; r < row; r++)
        delete[] array[r];
    delete[] array;
   

}

void StaticTriangularArrayTest() {

    double timer1;    
    int array[(row*row+row)/2];
    Timer t1;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j <= i; j++)
            array[(i * i + i) / 2 + j] = rand() % 2;

    }    
    timer1 = t1.elapsed();
    cout << "Time of filling a triangular static array: " << timer1 << " ms" << endl;
    t1.reset();
    array[0];
    timer1 = t1.elapsed();
    cout << "Access time to the first element of the triangular static array: " << timer1 << " ms" << endl;
    t1.reset();
    array[(row*row+row)/2 - 1];
    timer1 = t1.elapsed();
    cout << "Access time to the last element of the triangular static array: " << timer1 << " ms" << endl;
    cout << endl;
    
}

void DynamicTriangularArrayTest() {
        
    double timer1;    
    int** array = new int * [row];
    for (int i = 0; i < row; i++) {
        array[i] = new int[i + 1];
    }
    Timer t1;    
    for (int i = 0; i < row; i++) {        
        for (int j = 0; j <= i; j++)
            array[i][j] = rand() % 2;
        
    }
    timer1 = t1.elapsed();
    cout << "Time to filling a triangular dynamic array: " << timer1 << " ms" << endl;
    t1.reset();
    array[0][0];
    timer1 = t1.elapsed();
    cout << "Access time to the first element of the triangular dynamic array: " << timer1 << " ms" << endl;
    t1.reset();
    array[row - 1][row - 1];   
    timer1 = t1.elapsed();
    cout << "Access time to the last element of the triangular dynamic array: " << timer1 << " ms" << endl;
    cout << endl;
    for (int r = 0; r < row; r++)
        delete[] array[r];
    delete[] array;
}

void SparceArrayTest() {

    double timer1;
    SparseArray<int> array;
    Timer t1;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            array.SetValue(i, j, rand() % 2);
        }
    }
    timer1 = t1.elapsed();
    cout << "Time to filling a sparse array: " << timer1 << " ms" << endl;
    t1.reset();
    array.GetValue(0,0);
    timer1 = t1.elapsed();
    cout << "Access time to the first element of the sparse array: " << timer1 << " ms" << endl;
    t1.reset();
    array.GetValue(row-1, col-1);
    timer1 = t1.elapsed();
    cout << "Access time to the last element of the sparse array: " << timer1 << " ms" << endl;
    cout << endl;
}

int main()
{
    
    cout << "======== Comparison arrays with " << row * col << " elements ========" << endl << endl;
    StaticRectangularArrayTest();
    DynamicRectangularArrayTest();
    StaticTriangularArrayTest();
    DynamicTriangularArrayTest();
    SparceArrayTest();
    
    
    
}

