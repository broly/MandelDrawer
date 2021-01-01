#pragma once
#include <complex>
#include <functional>
#include <iostream>
#include <map>
#include <vector>


#include "Expression.h"
#include "../Types.h"
#include "../CommonTools.h"
#include "OpCodes.h"

#define DEBUG_INTERPRETER 0

#if DEBUG_INTERPRETER
    #define StepBytecode(type, ByteCodePtrRef, Msg) _StepBytecode<type>(ByteCodePtrRef, Msg)
#else
    #define StepBytecode(type, ByteCodePtrRef, Msg) _StepBytecode<type>(ByteCodePtrRef, nullptr)
#endif

namespace Mandel
{
    class FormulaInterpreter
    {
    public:
        FormulaInterpreter()
            : LastSlot(0)
            , Bytecode_Size(0)
        {
            Slots.resize(20);
        }
        
        void SetVariables(std::vector<Complex*> VarsList);

        void SetVariable(std::string Name, Complex* VarPtr);

        Complex Execute();


        void SetAtSlot(uint32 SlotIndex, Complex& Value);
        Complex& GetAtSlot(uint32 SlotIndex);
        Complex& GetAtMemSlot(uint32 MemSlotIndex) const;
        void SetAtMemSlot(uint32 MemSlotIndex, Complex* ValuePtr);

        template<typename T>
        FORCEINLINE void _StepBytecode(uint8*& ByteCodePtrRef, const char* Reason = nullptr)
        {
            T Value = *(T*)(ByteCodePtrRef);
            ByteCodePtrRef += sizeof(T);
#if DEBUG_INTERPRETER
            if (Reason)
                std::cout << std::dec << "STEP BYTECODE +" << sizeof(T) << " (" << Reason << ")" << "   V: " << std::hex << "0x" << Value << std::endl;
            else
                std::cout << "STEP BYTECODE +" << sizeof(T) << std::endl;
#endif
        }

        FORCEINLINE uint16 Read_SlotIndex(uint8*& Bytecode)
        {
            const uint16 Result = *(uint16*)Bytecode;
            StepBytecode(uint16, Bytecode, "READ SLOT INDEX");
            return Result;
        }
        
        FORCEINLINE Complex* Read_ValuePtr(uint8*& Bytecode)
        {
            Complex* Result = (Complex*)Bytecode;
            StepBytecode(Complex, Bytecode, "READ COMPLEX NUMBER");
            return Result;
        }

        
        void SetCompiledData(CompilationInfo& CompileInfo, uint8 LastSlot);

        void SetVars(std::shared_ptr<VariablesList> InVars);

        FORCEINLINE bool HasCompiledData() const
        {
            return Bytecode_Size > 0;
        }
    

        std::shared_ptr<VariablesList> Vars;

        std::vector<uint8> Bytecode;

        uint32 Bytecode_Size;

        std::vector<Complex> Slots;
        std::vector<Complex*> MemSlots;

        std::vector<int32> SlotsStack;

        CompilationInfo CompiledData;
        uint16 LastSlot;
    };


}