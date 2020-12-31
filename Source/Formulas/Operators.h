#pragma once
#include <functional>
#include <string>
#include <vector>
#include <complex>


#include "../Types.h"


namespace Mandel
{
    namespace OperatorsPrivate
    {
        static inline Complex cceil(Complex C)
        {
            return Complex(std::ceil(C.real()), std::ceil(C.imag()));
        }


        struct _BinaryOperatorsImpls
        {
            static Complex Add(Complex A, Complex B) { return A + B; }
            static Complex Sub(Complex A, Complex B) { return A - B; }
            static Complex Mul(Complex A, Complex B) { return A * B; }
            static Complex Div(Complex A, Complex B) { return A / B; }
            static Complex Mod(Complex A, Complex B) { return A + B * cceil(-(A / B)); }
            static Complex Pow(Complex A, Complex B) { return std::pow(A, B); }
        };

        using BinaryOperatorFuncType = Complex (*)(Complex A, Complex B);
    
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
}