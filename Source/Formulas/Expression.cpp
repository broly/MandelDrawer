#include "Expression.h"


#include <iostream>
#include <string>


#include "Common.h"
#include "Functions.h"
#include "Operators.h"
#include "../CommonTools.h"
#include <sstream>
#include <string>

#include "OpCodes.h"

#define DEBUG_COMPILING 0

using namespace Mandel;

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

bool UnaryExpression::HasError()
{
    return Argument->HasError();
}

std::string UnaryExpression::GetErrorReason()
{
    if (Argument->HasError())
        return Argument->GetErrorReason();
    return "";
}

void UnaryExpression::Compile(CompilationInfo& Info, uint16& LastSlot)
{
    Argument->Compile(Info, LastSlot);
    
    const uint16 func_index = IndexOf(SingleFunctions, [=] (const SingleFunctionInfo& v) { return v.FunctionName == Token; });

    if (func_index != -1)
    {
        Info.Buffer << EOpCode::OP_CALL1 << func_index << LastSlot;
#if DEBUG_COMPILING
        std::cout << "COMPILING     " << "CALL1  " << func_index << ", " << LastSlot << std::endl;
#endif
    }
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

bool BinaryExpression::HasError()
{
    return Left->HasError() || Right->HasError();
}

std::string BinaryExpression::GetErrorReason()
{
    if (Left->HasError())
        return Left->GetErrorReason();
    if (Right->HasError())
        return Right->GetErrorReason();
    return "";
}

void BinaryExpression::Compile(CompilationInfo& Info, uint16& LastSlot)
{
    uint16 LeftSlot = 0;
    uint16 RightSlot = 0;
    
    Left->Compile(Info, LeftSlot);
    Right->Compile(Info, RightSlot);
    
    const uint16 func_index = IndexOf(BinaryOperators, [=] (const BinaryOperator& v) { return Token == v.Token; });

    if (func_index != -1)
    {
        Info.Buffer << EOpCode::OP_CALL2 << func_index << LeftSlot << RightSlot;
#if DEBUG_COMPILING
        std::cout << "COMPILING     " << "CALL2  " << func_index << ", " << LastSlot << ", " << RightSlot << std::endl;
#endif
    }

    LastSlot = LeftSlot;
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

bool NumberExpression::HasError()
{
    return false;
}

std::string NumberExpression::GetErrorReason()
{
    return "";
}

void NumberExpression::Compile(CompilationInfo& Info, uint16& LastSlot)
{
    const uint16 Slot = Info.GetNewSlot();
    const Complex Value = Evaluate();

    Info.Buffer << EOpCode::OP_MOVC << Slot << Value;
#if DEBUG_COMPILING
    std::cout << "COMPILING     " << "MOVC   " << Slot << ", " << Value << std::endl;
#endif

    LastSlot = Slot;
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

bool VariableExpression::HasError()
{
    return false;
}

std::string VariableExpression::GetErrorReason()
{
    return "";
}

void VariableExpression::Compile(CompilationInfo& Info, uint16& LastSlot)
{
    const uint16 MemSlot = Info.GetMemSlotFor(Token);
    const uint16 Slot = Info.GetNewSlot();
    
    Info.Buffer << EOpCode::OP_LOAD << Slot << MemSlot;
#if DEBUG_COMPILING
    std::cout << "COMPILING     " << "LOAD   " << MemSlot << ", " << Slot << std::endl;
#endif

    LastSlot = Slot;
}

Complex ErrorExpression::Evaluate()
{
    return {};
}

bool ErrorExpression::HasError()
{
    return true;
}

std::string ErrorExpression::GetErrorReason()
{
    return Reason;
}

void ErrorExpression::Compile(CompilationInfo& Info, uint16& LastSlot)
{
    
}

