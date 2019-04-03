#include <iostream>

class Matrix
{
private:
    const size_t rows;
    const size_t cols;
    int *data;

    class Row
    {
    private:
        const size_t cols;
        int *data;
    public:
        Row(int *row, size_t cols) : data(row), cols(cols) {}
        int operator[](size_t element) const
        {
            if (cols <= element)
                throw std::out_of_range("");
            return (data[element]);
        }
        int &operator[](size_t element)
        {
            if (cols <= element)
                throw std::out_of_range("");
            return (data[element]);
        }
    };

public:
    Matrix(size_t rows, size_t cols) : rows(rows), cols(cols), data(new int[rows * cols]) {}
    ~Matrix()
    {
        delete[] data;
    }

    Row operator[](int row)
    {
        if (row < rows)
            return (Row(&data[row * cols], cols));
        else
           throw std::out_of_range(""); 
    }

    const Row operator[](int row) const
    {
        if (row < rows)
            return (Row(&data[row * cols], cols));
        else
           throw std::out_of_range("");
    }

    int getRows()
    {
        return (rows);
    }
    int getColumns()
    {
        return (cols);
    }

    Matrix &operator *= (int num)
    {
        for(int i = 0; i < rows * cols; ++i)
            data[i] *= num;
        return (*this);
    }

    bool operator == (const Matrix &m) const
    {
        bool equals = (m.rows == rows && m.cols ==cols);
        for(int i = 0; i < rows * cols && equals; ++i)
            equals = equals && (data[i] == m.data[i]);
        return (equals);
    }

    bool operator != (const Matrix &m) const
    {
        return (!operator == (m));
    }
};