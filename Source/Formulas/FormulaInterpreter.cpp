#include "FormulaInterpreter.h"


#include <iostream>
#include <set>

#include "Functions.h"
#include "OpCodeDefs.h"

using namespace Mandel;

void FormulaInterpreter::SetVariables(std::vector<Complex*> VarsList)
{
    MemSlots = VarsList;
}

Complex FormulaInterpreter::Execute()
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

void FormulaInterpreter::CopyToSlotFromSlot(uint32 DestSlotIndex, uint32 SourceSlotIndex)
{
    Complex& Value = Slots[SourceSlotIndex];
    SetAtSlot(DestSlotIndex, Value);
}

void FormulaInterpreter::PushSlotIndex(int32 SlotIndex)
{
    SlotsStack.push_back(SlotIndex);
}

Complex& FormulaInterpreter::GetAtMemSlot(uint32 MemSlotIndex) const
{
    Complex* ValuePtr = MemSlots[MemSlotIndex];
    return *ValuePtr;
}

uint16 FormulaInterpreter::Read_SlotIndex(uint8*& Bytecode)
{
    const uint16 Result = *(uint16*)Bytecode;
    StepBytecode<uint16>(Bytecode, "READ SLOT INDEX");
    return Result;
}

Complex* FormulaInterpreter::Read_ValuePtr(uint8*& Bytecode)
{
    Complex* Result = (Complex*)Bytecode;
    StepBytecode<Complex>(Bytecode, "READ COMPLEX NUMBER");
    return Result;
}
