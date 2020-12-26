#pragma once
#include <complex>
#include <functional>
#include <iostream>
#include <map>
#include <vector>

#include "../Types.h"
#include "OpCodes.h"

class FormulaInterpreter
{
public:
    void SetVariables(std::vector<Complex*> VarsList);

    Complex Execute();


    void SetAtSlot(uint32 SlotIndex, Complex& Value);
    Complex& GetAtSlot(uint32 SlotIndex);
    void CopyToSlotFromSlot(uint32 DestSlotIndex, uint32 SourceSlotIndex);
    void PushSlotIndex(int32 SlotIndex);
    Complex& GetAtMemSlot(uint32 MemSlotIndex) const;

    template<typename T>
    void StepBytecode(uint8*& ByteCodePtrRef, const char* Reason = nullptr);

    uint16 Read_SlotIndex(uint8*& Bytecode);
    Complex* Read_ValuePtr(uint8*& Bytecode);
    

    std::vector<std::map<char*, Complex*>> Vars;

    std::vector<uint8> Bytecode;
    uint32 CommandsNum;

    std::vector<Complex> Slots;
    std::vector<Complex*> MemSlots;

    std::vector<int32> SlotsStack;
};

template <typename T>
void FormulaInterpreter::StepBytecode(uint8*& ByteCodePtrRef, const char* Reason)
{
    T Value = *(T*)(ByteCodePtrRef);
    ByteCodePtrRef += sizeof(T);
    if (Reason)
        std::cout << std::dec << "STEP BYTECODE +" << sizeof(T) << " (" << Reason << ")" << "   V: " << std::hex << "0x" << Value << std::endl;
    else
        std::cout << "STEP BYTECODE +" << sizeof(T) << std::endl;
}


// 5 + abs(x - sin(5 * x - 3 * x) / 32 * x) - 5 * y - pow(y - 2, x * sin(5 * x * (y - 6)))

//                    // LOAD  0, [x]
//                    // LOAD  1, [y]
//                    // MOV   2, 1
// a = (y - 6)        // SUB   2, (6, 0)
// a = x * a          // MUL   2, [0]
// a = 5 * a          // MUL   2, (5, 0)
// a = sin(a)         // PUSH  2
//                    // CALL  [sin]
// b = y - 2          // MOV   3, 1
//                    // SUB   3, (2, 0)
// a = pow(b, a)      // PUSH  3
//                    // PUSH  2
//                    // CALL  [pow]
// a = y - a          // SUBI  2, 1
// a = 5 * a          // MUL   2, (5, 0)

// c = 3 * x          // MOV   4, 0
//                    // MUL   4, (3, 0)
// d = 5 * x          // MOV   5, 0
//                    // MUL   5, (5, 0)
// c = d - c          // SUBI  4, 5 
// c = sin(c)         // PUSH  4
//                    // CALL [sin]
// c = c / 32         // DIV   4, (32, 0)
// c = c * x          // MUL   4, 1
// c = x - c          // SUBI  4, 0 
// c = abs(c)         // PUSH  4
                      // CALL [abs]

// a = c - a          // SUBI 2, 4
// a = 5 + a          // ADD  2, (5, 0)

