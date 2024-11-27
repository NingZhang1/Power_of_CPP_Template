#include <iostream>
#include <vector>
#include <chrono>

// Regular C version
void multiply_add_c(const std::vector<double>& x, const std::vector<double>& y, std::vector<double>& z, double a, double b)
{
    size_t n = x.size();
    for (size_t i = 0; i < n; ++i)
    {
        z[i] = a * x[i] + b * y[i];
    }
}

std::vector<double> operator*(const std::vector<double>& vec, double scalar)
{
    std::vector<double> result(vec.size());
    const auto          n = vec.size();
    for (size_t i = 0; i < n; ++i)
    {
        result[i] = vec[i] * scalar;
    }
    return result;
}

std::vector<double> operator*(double scalar, const std::vector<double>& vec)
{
    std::vector<double> result(vec.size());
    const auto          n = vec.size();
    for (size_t i = 0; i < n; ++i)
    {
        result[i] = vec[i] * scalar;
    }
    return result;
}

std::vector<double> operator+(const std::vector<double>& lhs, const std::vector<double>& rhs)
{
    std::vector<double> result(lhs.size());
    const auto          n = lhs.size();
    for (size_t i = 0; i < n; ++i)
    {
        result[i] = lhs[i] + rhs[i];
    }
    return result;
}

// Operator overloading version
class Vector
{
public:
    std::vector<double> data;

    Vector(size_t size) : data(size) {}
    Vector(const std::vector<double>& vec) : data(vec) {}

    Vector operator*(double scalar) const
    {
        Vector result(data.size());
        for (size_t i = 0; i < data.size(); ++i)
        {
            result.data[i] = data[i] * scalar;
        }
        return result;
    }

    Vector operator+(const Vector& other) const
    {
        Vector result(data.size());
        for (size_t i = 0; i < data.size(); ++i)
        {
            result.data[i] = data[i] + other.data[i];
        }
        return result;
    }
};

// Expression template classes
template <typename LHS, typename RHS>
class BinaryExpression
{
public:
    const LHS& lhs;
    const RHS& rhs;

    BinaryExpression(const LHS& lhs, const RHS& rhs) : lhs(lhs), rhs(rhs) {}

    double operator[](size_t i) const { return lhs[i] + rhs[i]; }

    size_t size() const { return lhs.size(); }
};

template <typename VecType>
class ScalarMult
{
public:
    const VecType& vec;
    double         scalar;

    ScalarMult(const VecType& vec, double scalar) : vec(vec), scalar(scalar) {}
    ScalarMult(double scalar, const VecType& vec) : vec(vec), scalar(scalar) {}

    double operator[](size_t i) const { return vec[i] * scalar; }

    size_t size() const { return vec.size(); }
};

template <typename LHS, typename RHS>
BinaryExpression<LHS, RHS> operator+(const LHS& lhs, const RHS& rhs)
{
    return BinaryExpression<LHS, RHS>(lhs, rhs);
}

template <typename VecType>
ScalarMult<VecType> operator*(double scalar, const VecType& vec)
{
    return ScalarMult<VecType>(vec, scalar);
}

template <typename VecType>
ScalarMult<VecType> operator*(const VecType& vec, double scalar)
{
    return ScalarMult<VecType>(vec, scalar);
}

// A Vector wrapper for expression templates
class ETVector
{
public:
    std::vector<double> data;

    ETVector(size_t size) : data(size) {}
    ETVector(const std::vector<double>& vec) : data(vec) {}

    template <typename Expression>
    ETVector& operator=(const Expression& expr)
    {
        for (size_t i = 0; i < data.size(); ++i)
        {
            data[i] = expr[i];
        }
        return *this;
    }

    double operator[](size_t i) const { return data[i]; }
    size_t size() const { return data.size(); }
};

// Testing
int main()
{
    const size_t        N = 1'000'000'0; // Vector size
    std::vector<double> x(N, 1.0), y(N, 2.0), z(N, 0.0);
    Vector              vx(x), vy(y), vz(z);
    ETVector            etx(x), ety(y), etz(N);

    double a = 2.0, b = 3.0;

    // C-style version
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100; ++i)
    {
        multiply_add_c(x, y, z, a, b);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "C version: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 100 << " mu s\n";

    z = a * x + b * y;

    // Operator overloading version
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10; ++i)
    {
        vz = (vx * a + vy * b);
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Operator overloading version: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 10 << " mu s\n";

    // Expression template version
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100; ++i)
    {
        etz = (etx * a + ety * b);
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Expression template version: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 100 << " mu s\n";

    return 0;
}
