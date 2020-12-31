#pragma once

#include "OpCodes.h"
#include "FormulaInterpreter.h"

namespace Mandel
{
    /**
    * type of opcode action (FormulaInterpreter& Interpreter, uint8*& Bytecode)
    * @see InstructionsInfo
    */
    using OpCode_FuncType = void(class FormulaInterpreter&, uint8*&);

    /**
    * Here is table of all opcodes about what exactly doing each
    *
    * Each operation has implementation in the form of lambda
    * Each operation has access to interpreter @see FormulaInterpreter
    * Each operation should offset current bytecode pointer
    */
    static std::map<EOpCode, std::function<OpCode_FuncType>> InstructionsInfo = {
        {
            EOpCode::OP_LOAD,
            [] (FormulaInterpreter& Interpreter, uint8*& Bytecode)
            {
                // -- Read bytes from bytecode --
                const uint16 SlotIndex = Interpreter.Read_SlotIndex(Bytecode);
                const uint16 MemSlotIndex = Interpreter.Read_SlotIndex(Bytecode);

                // -- Payload --
                auto ValueRef = Interpreter.GetAtMemSlot(MemSlotIndex);
                Interpreter.SetAtSlot(SlotIndex, ValueRef);
            }
        },
        {
            EOpCode::OP_MOV,
            [] (FormulaInterpreter& Interpreter, uint8*& Bytecode)
            {
                const uint16 DestSlotIndex = Interpreter.Read_SlotIndex(Bytecode);
                const uint16 SourceSlotIndex = Interpreter.Read_SlotIndex(Bytecode);

                Interpreter.CopyToSlotFromSlot(DestSlotIndex, SourceSlotIndex);
            }
        },
        {
            EOpCode::OP_MOVC,
            [] (FormulaInterpreter& Interpreter, uint8*& Bytecode)
            {
                const uint16 DestSlotIndex = Interpreter.Read_SlotIndex(Bytecode);
                Complex* ValuePtr = Interpreter.Read_ValuePtr(Bytecode);
                Interpreter.SetAtSlot(DestSlotIndex, *ValuePtr);
            }
        },
        {
            EOpCode::OP_STORE,
            [] (FormulaInterpreter& Interpreter, uint8*& Bytecode)
            {
                    
            }
        },
        {
            EOpCode::OP_ADD,
            [] (FormulaInterpreter& Interpreter, uint8*& Bytecode)
            {
                const uint16 SlotIndex = Interpreter.Read_SlotIndex(Bytecode);
                const uint16 OtherIndex = Interpreter.Read_SlotIndex(Bytecode);

                auto& Value = Interpreter.GetAtSlot(SlotIndex);
                auto& OtherValue = Interpreter.GetAtSlot(OtherIndex);
                Value += OtherValue;
            }
        },
        {
            EOpCode::OP_ADDC,
            [] (FormulaInterpreter& Interpreter, uint8*& Bytecode)
            {
                const uint16 SlotIndex = Interpreter.Read_SlotIndex(Bytecode);
                Complex* ValuePtr = Interpreter.Read_ValuePtr(Bytecode);

                auto& Value = Interpreter.GetAtSlot(SlotIndex);
                Value += *ValuePtr;
            }
        },
        {
            EOpCode::OP_SUB,
            [] (FormulaInterpreter& Interpreter, uint8*& Bytecode)
            {
                const uint16 SlotIndex = Interpreter.Read_SlotIndex(Bytecode);
                const uint16 OtherIndex = Interpreter.Read_SlotIndex(Bytecode);

                auto& Value = Interpreter.GetAtSlot(SlotIndex);
                auto& OtherValue = Interpreter.GetAtSlot(OtherIndex);
                Value -= OtherValue;
            }
        },
        {
            EOpCode::OP_SUBC,
            [] (FormulaInterpreter& Interpreter, uint8*& Bytecode)
            {
                const uint16 SlotIndex = Interpreter.Read_SlotIndex(Bytecode);
                Complex* ValuePtr = Interpreter.Read_ValuePtr(Bytecode);

                auto& Value = Interpreter.GetAtSlot(SlotIndex);
                Value -= *ValuePtr;
            }
        },
        {
            EOpCode::OP_MUL,
            [] (FormulaInterpreter& Interpreter, uint8*& Bytecode)
            {
                const uint16 SlotIndex = Interpreter.Read_SlotIndex(Bytecode);
                const uint16 OtherIndex = Interpreter.Read_SlotIndex(Bytecode);

                auto& Value = Interpreter.GetAtSlot(SlotIndex);
                auto& OtherValue = Interpreter.GetAtSlot(OtherIndex);
                Value *= OtherValue;
            }
        },
        {
            EOpCode::OP_MULC,
            [] (FormulaInterpreter& Interpreter, uint8*& Bytecode)
            {
                const uint16 SlotIndex = Interpreter.Read_SlotIndex(Bytecode);
                Complex* ValuePtr = Interpreter.Read_ValuePtr(Bytecode);

                auto& Value = Interpreter.GetAtSlot(SlotIndex);
                Value *= *ValuePtr;
            }
        },
        {
            EOpCode::OP_DIV,
            [] (FormulaInterpreter& Interpreter, uint8*& Bytecode)
            {
                const uint16 SlotIndex = Interpreter.Read_SlotIndex(Bytecode);
                const uint16 OtherIndex = Interpreter.Read_SlotIndex(Bytecode);

                auto& Value = Interpreter.GetAtSlot(SlotIndex);
                auto& OtherValue = Interpreter.GetAtSlot(OtherIndex);
                Value /= OtherValue;
            }
        },
        {
            EOpCode::OP_DIVC,
            [] (FormulaInterpreter& Interpreter, uint8*& Bytecode)
            {
                const uint16 SlotIndex = Interpreter.Read_SlotIndex(Bytecode);
                Complex* ValuePtr = Interpreter.Read_ValuePtr(Bytecode);

                auto& Value = Interpreter.GetAtSlot(SlotIndex);
                Value /= *ValuePtr;
            }
        },
        {
            EOpCode::OP_PUSH,
            [] (FormulaInterpreter& Interpreter, uint8*& Bytecode)
            {
                const uint16 SlotIndex = Interpreter.Read_SlotIndex(Bytecode);
                    
                Interpreter.PushSlotIndex(SlotIndex);
            }
        },

        {
            EOpCode::OP_CALLF,
            [] (FormulaInterpreter& Interpreter, uint8*& Bytecode)
            {
                const uint16 SlotIndex = Interpreter.Read_SlotIndex(Bytecode);
                const uint16 FuncSlotIndex = Interpreter.Read_SlotIndex(Bytecode);

                const SingleFunctionType func = SingleFunctions[FuncSlotIndex].Func;
                auto& Value = Interpreter.GetAtSlot(SlotIndex);
                Value = func(Value);
            }
        },
    };
}