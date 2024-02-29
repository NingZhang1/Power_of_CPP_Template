#include <boost/yap/expression.hpp>

#include <iostream>

int main()
{
    /// YAP is a library for building expression templates in C++14.
    /// terminal is a terminal expression, which is a leaf node in an expression tree.

    evaluate(boost::yap::make_terminal(std::cout) << "Hello" << ',' << " world!\n");

    return 0;
}
//]