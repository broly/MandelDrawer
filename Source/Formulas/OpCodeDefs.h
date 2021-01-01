#pragma once

#include "OpCodes.h"
#include "FormulaInterpreter.h"
#include "Operators.h"

namespace Mandel
{
    /**
    * type of opcode action (FormulaInterpreter& Interpreter, uint8*& Bytecode)
    * @see InstructionsInfo
    */
    using OpCode_FuncType = void(class FormulaInterpreter&, uint8*&);

    namespace OpCodes
    {
        static void LOAD(FormulaInterpreter& Interpreter, uint8*& Bytecode)
        {
            // -- Read bytes from bytecode --
            const uint16 SlotIndex = Interpreter.Read_SlotIndex(Bytecode);
            const uint16 MemSlotIndex = Interpreter.Read_SlotIndex(Bytecode);
            

            // -- Payload --
            auto ValueRef = Interpreter.GetAtMemSlot(MemSlotIndex);
            Interpreter.SetAtSlot(SlotIndex, ValueRef);
        }

        static void MOVC(FormulaInterpreter& Interpreter, uint8*& Bytecode)
        {
            const uint16 DestSlotIndex = Interpreter.Read_SlotIndex(Bytecode);
            Complex* ValuePtr = Interpreter.Read_ValuePtr(Bytecode);
            Interpreter.SetAtSlot(DestSlotIndex, *ValuePtr);
        }

        static void CALL1(FormulaInterpreter& Interpreter, uint8*& Bytecode)
        {
            const uint16 FuncSlotIndex = Interpreter.Read_SlotIndex(Bytecode);
            const uint16 SlotIndex = Interpreter.Read_SlotIndex(Bytecode);

            const SingleFunctionType& func = SingleFunctions[FuncSlotIndex].Func;
            auto& Value = Interpreter.GetAtSlot(SlotIndex);
            Value = func(Value);
        }

        static void CALL2(FormulaInterpreter& Interpreter, uint8*& Bytecode)
        {
            const uint16 FuncSlotIndex = Interpreter.Read_SlotIndex(Bytecode);
            const uint16 Slot1Index = Interpreter.Read_SlotIndex(Bytecode);
            const uint16 Slot2Index = Interpreter.Read_SlotIndex(Bytecode);

            auto& Value1 = Interpreter.GetAtSlot(Slot1Index);
            auto& Value2 = Interpreter.GetAtSlot(Slot2Index);

            const auto& func = BinaryOperators[FuncSlotIndex].Func;
            Value1 = func(Value1, Value2);
        }
    }

    /**
    * Here is table of all opcodes about what exactly doing each
    *
    * Each operation has implementation in the form of lambda
    * Each operation has access to interpreter @see FormulaInterpreter
    * Each operation should offset current bytecode pointer
    */
    static std::function<OpCode_FuncType> InstructionsInfo[] = {
        OpCodes::LOAD,
        OpCodes::MOVC,
        OpCodes::CALL1,
        OpCodes::CALL2
    };
}
