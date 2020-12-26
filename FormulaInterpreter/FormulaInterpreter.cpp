#include "FormulaInterpreter.h"


#include <iostream>
#include <set>

#include "Functions.h"
#include "OpCodeDefs.h"


void FormulaInterpreter::SetVariables(std::vector<std::complex<float>*> VarsList)
{
    MemSlots = VarsList;
}

std::complex<float> FormulaInterpreter::Execute()
{
    uint8* MostRecentBytecodePtr = Bytecode.data();
    
    uint32 CommandIndex = 0;
    while (CommandIndex < CommandsNum)
    {
        // Read opcode
        EOpCode OpCode = (EOpCode)MostRecentBytecodePtr[0];
        auto iter = AllowedOpCodes.find(OpCode);
        if (iter == AllowedOpCodes.end())
        {
            printf("Omg!");
            return {0.f, 0.f};
        }
        
        // Get opcode callback, todo: optimize from map to array
        auto OpCallback = InstructionsInfo[OpCode];
        // Shift from opcode to operands
        StepBytecode<uint8>(MostRecentBytecodePtr, OpCode2Str(OpCode));
        // Call opcode action with operands
        OpCallback(*this, MostRecentBytecodePtr);
        // Next command
        CommandIndex++;
    }

    return {0.f, 0.f};
}

void FormulaInterpreter::SetAtSlot(uint32 SlotIndex, std::complex<float>& Value)
{
    if (Slots.size() <= SlotIndex)
        Slots.resize(SlotIndex + 10, {0.f, 0.f});
    Slots[SlotIndex] = Value;
}

std::complex<float>& FormulaInterpreter::GetAtSlot(uint32 SlotIndex)
{
    return Slots[SlotIndex];
}

void FormulaInterpreter::CopyToSlotFromSlot(uint32 DestSlotIndex, uint32 SourceSlotIndex)
{
    std::complex<float>& Value = Slots[SourceSlotIndex];
    SetAtSlot(DestSlotIndex, Value);
}

void FormulaInterpreter::PushSlotIndex(int32 SlotIndex)
{
    SlotsStack.push_back(SlotIndex);
}

std::complex<float>& FormulaInterpreter::GetAtMemSlot(uint32 MemSlotIndex) const
{
    std::complex<float>* ValuePtr = MemSlots[MemSlotIndex];
    return *ValuePtr;
}

uint16 FormulaInterpreter::Read_SlotIndex(uint8*& Bytecode)
{
    const uint16 Result = *(uint16*)Bytecode;
    StepBytecode<uint16>(Bytecode, "READ SLOT INDEX");
    return Result;
}

std::complex<float>* FormulaInterpreter::Read_ValuePtr(uint8*& Bytecode)
{
    std::complex<float>* Result = (std::complex<float>*)Bytecode;
    StepBytecode<std::complex<float>>(Bytecode, "READ COMPLEX NUMBER");
    return Result;
}
