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

// Testing
int main()
{
    const size_t        N = 1'000'000'0; // Vector size
    std::vector<double> x(N, 1.0), y(N, 2.0), z(N, 0.0);
    Vector              vx(x), vy(y), vz(z);

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

    // Operator overloading version, you should find it very slow.
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10; ++i)
    {
        vz = (vx * a + vy * b);
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Operator overloading version: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 10 << " mu s\n";

    return 0;
}
