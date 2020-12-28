#include "Expression.h"

#include <string>


#include "Common.h"
#include "Functions.h"
#include "Operators.h"
#include "CommonTools.h"
#include <sstream>
#include <string>

bool ReplaceStr(std::string& str, const std::string& from, const std::string& to)
{
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

Complex UnaryExpression::Evaluate()
{
    if (const auto func_info = FindBy(SingleFunctions, [=] (const SingleFunctionInfo& v) { return v.FunctionName == Token; }))
    {
        auto v = Argument->Evaluate();
        return func_info->Func(v);
    }
    return {};
}

Complex BinaryExpression::Evaluate()
{
    if (const auto func_info = FindBy(BinaryOperators, [=] (const BinaryOperator& v) { return Token == v.Token; }))
    {
        auto A = Left->Evaluate();
        auto B = Right->Evaluate();
        return func_info->Func(A, B);
    }
    
    return {};
}

template<typename T>
T StringToNumber(const std::string& numberAsString)
{
    T valor;

    std::stringstream stream(numberAsString);
    stream >> valor;
    if (stream.fail()) {
        std::runtime_error e(numberAsString);
        throw e;
    }
    return valor;
}

Complex NumberExpression::Evaluate()
{
    if (!bWasCached)
    {
        std::string tk = Token;
        const bool bIsImaginary = ReplaceStr(tk, "i", "");
        const Float Value = StringToNumber<Float>(tk); // std::stof(tk);
        CachedNumber = bIsImaginary ? Complex{0.f, Value} : Complex{Value, 0.f};
        bWasCached = true;
    }
    return CachedNumber;
}

Complex VariableExpression::Evaluate()
{
    if (Vars)
        for (auto& pair : Vars->Vars)
        {
            if (pair.first == Token)
                return *pair.second;
        }

    throw std::runtime_error(std::string("Can't find variable: ") + Token);
}
