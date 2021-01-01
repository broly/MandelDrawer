#include "FormulaInterpreter.h"


#include <iostream>
#include <set>

#include "Functions.h"
#include "OpCodeDefs.h"

using namespace Mandel;

void FormulaInterpreter::SetCompiledData(CompilationInfo& CompileInfo, uint8  InLastSlot)
{
    CompiledData = CompileInfo;
    LastSlot = InLastSlot;

    Bytecode = CompiledData.Buffer.Data;
    Bytecode_Size = Bytecode.size();
    MemSlots.clear();
    Slots.clear();
    for (auto& NameSlot_Pair : CompiledData.VariableSlots)
    {
        for (auto& NamePtr_Pair : Vars->Vars)
        {
            if (NameSlot_Pair.first == NamePtr_Pair.first)
            {
                SetAtMemSlot(NameSlot_Pair.second, NamePtr_Pair.second);
            }
        }
    }
}

void FormulaInterpreter::SetVars(std::shared_ptr<VariablesList> InVars)
{
    Vars = InVars;
}

void FormulaInterpreter::SetVariables(std::vector<Complex*> VarsList)
{
    MemSlots = VarsList;
}

void FormulaInterpreter::SetVariable(std::string Name, Complex* VarPtr)
{
    uint16 Slot = CompiledData.VariableSlots[Name];
    SetAtMemSlot(Slot, VarPtr);
}


FORCEINLINE static void HandleOpCode(EOpCode OpCode, FormulaInterpreter* Interpreter, uint8*& Bytecode)
{
    switch (OpCode)
    {
        case EOpCode::OP_LOAD:  return OpCodes::LOAD(*Interpreter, Bytecode);
        case EOpCode::OP_MOVC:  return OpCodes::MOVC(*Interpreter, Bytecode);
        case EOpCode::OP_CALL1: return OpCodes::CALL1(*Interpreter, Bytecode);
        case EOpCode::OP_CALL2: return OpCodes::CALL2(*Interpreter, Bytecode);
    }
}

Complex FormulaInterpreter::Execute()
{
    uint8* MostRecentBytecodePtr = Bytecode.data();

    const uint8* EndPtr = MostRecentBytecodePtr + Bytecode_Size;
    
    while (true)
    {
        // Read opcode
        const uint8 OpCode = MostRecentBytecodePtr[0];

#ifndef DEBUG_INTERPRETER
        auto iter = AllowedOpCodes.find(OpCode);
        if (iter == AllowedOpCodes.end())
        {
            printf("Omg!");
            return {0.f, 0.f};
        }
#endif
        
        StepBytecode(uint8, MostRecentBytecodePtr, OpCode2Str((EOpCode)OpCode));
        HandleOpCode((EOpCode)OpCode, this, MostRecentBytecodePtr);

        if (MostRecentBytecodePtr >= EndPtr)
            break;
    }

    return GetAtSlot(LastSlot);
}


void FormulaInterpreter::SetAtSlot(uint32 SlotIndex, Complex& Value)
{
    if (Slots.size() <= SlotIndex)
        Slots.resize(SlotIndex + 10, {0.f, 0.f});
    Slots[SlotIndex] = Value;
}

Complex& FormulaInterpreter::GetAtSlot(uint32 SlotIndex)
{
    return Slots[SlotIndex];
}

Complex& FormulaInterpreter::GetAtMemSlot(uint32 MemSlotIndex) const
{
    Complex* ValuePtr = MemSlots[MemSlotIndex];
    return *ValuePtr;
}

void FormulaInterpreter::SetAtMemSlot(uint32 MemSlotIndex, Complex* ValuePtr)
{
    if (MemSlotIndex >= MemSlots.size())
    {
        MemSlots.resize(MemSlotIndex + 1);
    }
    MemSlots[MemSlotIndex] = ValuePtr;
}
