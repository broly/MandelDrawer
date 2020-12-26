#pragma once
#include <complex>
#include <memory>
#include <string>

#include "Common.h"


struct VariablesList;

struct ExpressionBase
{
    ExpressionBase(std::string InToken)
        : Token(InToken)
    {}
    std::string Token; // Stored token (operation, value, variable)
    
    virtual ~ExpressionBase()
    {}

    virtual std::complex<float> Evaluate() = 0;
};



struct UnaryExpression : ExpressionBase
{
    UnaryExpression(std::string InToken, std::shared_ptr<ExpressionBase> a)
        : ExpressionBase(InToken), Argument{a}
    {}
    
    std::shared_ptr<ExpressionBase> Argument;

    virtual std::complex<float> Evaluate() override;
};



struct BinaryExpression : ExpressionBase
{
    BinaryExpression(std::string InToken, std::shared_ptr<ExpressionBase> InLeft, std::shared_ptr<ExpressionBase> InRight)
        : ExpressionBase(InToken)
        , Left{InLeft}
        , Right{InRight}
    {}
    std::shared_ptr<ExpressionBase> Left;
    std::shared_ptr<ExpressionBase> Right;

    
    virtual std::complex<float> Evaluate() override;
};

struct NumberExpression : ExpressionBase
{
    NumberExpression(std::string InToken)
        : ExpressionBase(InToken)
    {}

    
    virtual std::complex<float> Evaluate() override;
};



struct VariableExpression : ExpressionBase
{
    VariableExpression(std::string InToken, VariablesList InVars)
        : ExpressionBase(InToken)
        , Vars(InVars)
    {}

    
    virtual std::complex<float> Evaluate() override;
    
    VariablesList Vars;
};


