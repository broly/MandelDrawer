#pragma once
#include <complex>
#include <memory>
#include <string>

#include "Common.h"

namespace Mandel
{
    struct VariablesList;

    struct ExpressionBase
    {
        ExpressionBase(std::string InToken)
            : Token(InToken)
        {}
        std::string Token; // Stored token (operation, value, variable)
    
        virtual ~ExpressionBase()
        {}

        virtual Complex Evaluate() = 0;

        virtual bool HasError() = 0;

        virtual std::string GetErrorReason() = 0;
    };



    struct UnaryExpression : ExpressionBase
    {
        UnaryExpression(std::string InToken, std::shared_ptr<ExpressionBase> a)
            : ExpressionBase(InToken), Argument{a}
        {}
    
        std::shared_ptr<ExpressionBase> Argument;

        virtual Complex Evaluate() override;
        virtual bool HasError() override;
        virtual std::string GetErrorReason() override;
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

    
        virtual Complex Evaluate() override;
        virtual bool HasError() override;
        virtual std::string GetErrorReason() override;
    };

    struct NumberExpression : ExpressionBase
    {
        NumberExpression(std::string InToken)
            : ExpressionBase(InToken)
            , bWasCached(false)
        {}

        bool bWasCached;
        Complex CachedNumber;
    
        virtual Complex Evaluate() override;
        virtual bool HasError() override;
        virtual std::string GetErrorReason() override;
    };



    struct VariableExpression : ExpressionBase
    {
        VariableExpression(std::string InToken, std::shared_ptr<VariablesList> InVars)
            : ExpressionBase(InToken)
            , Vars(InVars)
        {}

    
        virtual Complex Evaluate() override;
        virtual bool HasError() override;
        virtual std::string GetErrorReason() override;
    
        std::shared_ptr<VariablesList> Vars;
    };

    struct ErrorExpression : ExpressionBase
    {
        ErrorExpression(std::string InToken, std::string ErrorReason)
            : ExpressionBase(InToken)
            , Reason(ErrorReason)
        {}

        virtual Complex Evaluate() override;
        virtual bool HasError() override;
        virtual std::string GetErrorReason() override;
        
        std::string Reason;
    };

}