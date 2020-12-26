#pragma once
#include <functional>
#include <string>
#include <vector>
#include <complex>


#include "../Types.h"

namespace OperatorsPrivate
{
    static inline std::complex<float> cceil(std::complex<float> C)
    {
        return std::complex<float>(std::ceil(C.real()), std::ceil(C.imag()));
    }


    struct _BinaryOperatorsImpls
    {
        static std::complex<float> Add(std::complex<float> A, std::complex<float> B) { return A + B; }
        static std::complex<float> Sub(std::complex<float> A, std::complex<float> B) { return A - B; }
        static std::complex<float> Mul(std::complex<float> A, std::complex<float> B) { return A * B; }
        static std::complex<float> Div(std::complex<float> A, std::complex<float> B) { return A / B; }
        static std::complex<float> Mod(std::complex<float> A, std::complex<float> B) { return A + B * cceil(-(A / B)); }
        static std::complex<float> Pow(std::complex<float> A, std::complex<float> B) { return std::pow(A, B); }
    };

    using BinaryOperatorFuncType = std::complex<float> (*)(std::complex<float> A, std::complex<float> B);
    
}

struct BinaryOperator
{
    std::string Token;
    uint8 Priority;
    OperatorsPrivate::BinaryOperatorFuncType Func;
};


static std::vector<BinaryOperator> BinaryOperators
{
        { "+", 1, OperatorsPrivate::_BinaryOperatorsImpls::Add },
        { "-", 1, OperatorsPrivate::_BinaryOperatorsImpls::Sub },
        { "*", 2, OperatorsPrivate::_BinaryOperatorsImpls::Mul },
        { "/", 2, OperatorsPrivate::_BinaryOperatorsImpls::Div },
        { "%", 2, OperatorsPrivate::_BinaryOperatorsImpls::Mod },
        { "**", 3, OperatorsPrivate::_BinaryOperatorsImpls::Pow },
        { "^", 3, OperatorsPrivate::_BinaryOperatorsImpls::Pow },
};

static int get_priority(const std::string& token)
{
    auto pred = [token] (const BinaryOperator& op) { return op.Token == token;};
    auto entry = std::find_if(BinaryOperators.begin(), BinaryOperators.end(), pred);
    if (entry != BinaryOperators.end())
        return entry->Priority;
    return 0;
}
