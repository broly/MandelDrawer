#pragma once

#include <set>

#include "../Types.h"

/**
 * OpCode enumeration
 *
 * OpCode used in @see FormulaInterpreter as a payload of the interpretation of bytecode
 * 
 * Each opcode must be registered in next entities:
 *  mainly @see InstructionsInfo [OpCodeDefs.h]
 *  @see AllowedOpCodes [OpCodes.h]
 *  @see OpCode2StrMap [OpCodes.h]
 */
enum class EOpCode : uint8
{
    OP_LOAD = 0x0,      // LOAD   SLOT, MEM_SLOT                    ; Load from memory to slot
    OP_MOV = 0x1,       // MOV    SLOT, SLOT                        ; Copy from slot to slot
    OP_MOVC = 0x2,      // MOVC   SLOT, COMPLEX_NUMBER              ; Copy complex number to slot
    OP_STORE = 0x3,     // STORE  MEM, SLOT                         ; Store from slot to memory
    
    OP_ADD = 0x10,      // ADD    SLOT, SLOT                        ; Slot1 = Slot1 + Slot2
    OP_ADDC = 0x11,     // ADDC   SLOT, COMPLEX_NUMBER              ; Slot1 = Slot1 + C
    
    OP_SUB = 0x20,      // SUB    SLOT, SLOT                        ; Slot1 = Slot1 - Slot2
    OP_SUBC = 0x21,     // SUBC   SLOT, COMPLEX_NUMBER              ; Slot1 = Slot1 - C
    OP_SUBCI = 0x22,    // SUBCI  SLOT, COMPLEX_NUMBER  # inversed  ; Slot1 = C - Slot1
    
    OP_MUL = 0x30,      // MUL    SLOT, SLOT                        ; Slot1 = Slot1 * Slot2
    OP_MULC = 0x31,     // MULC   SLOT, COMPLEX_NUMBER              ; Slot1 = Slot1 * C
    
    OP_DIV = 0x40,      // DIV    SLOT, SLOT                        ; Slot1 = Slot1 / Slot2
    OP_DIVC = 0x41,     // DIVC   SLOT, COMPLEX_NUMBER              ; Slot1 = Slot1 / C
    OP_DIVCI = 0x42,    // DIVCI  SLOT, COMPLEX_NUMBER  # inversed  ; Slot1 = C / Slot1
    
    OP_POW = 0x50,      // POW    SLOT, SLOT                        ; Slot1 = Slot1 ^ Slot2
    OP_POWC = 0x51,     // POWC   SLOT, COMPLEX_NUMBER              ; Slot1 = Slot1 ^ C
    OP_POWCI = 0x52,    // POWCI  SLOT, COMPLEX_NUMBER  # inversed  ; Slot1 = C ^ Slot1

    OP_NEG = 0x60,      // NEG    SLOT                              ; Slot1 = -Slot1
    
    OP_CALL1 = 0x70,    // CALL   FUNC_ID                           ; Calls function with one parameter
    OP_CALL2 = 0x71,    // CALL2  FUNC_ID                           ; Calls function with two parameters

    OP_PUSH = 0x80,     // PUSH   SLOT                              ; Pushes SlotId to stack (for function call)
    OP_PUSHC = 0x81,    // PUSH   COMPLEX_NUMBER                    ; Pushes Complex number to stack (for function call)

    OP_CALLF = 0x90,   // CALLF   SLOT, FUNC_SLOT                   ; Calls function FUNC_SLOT on SLOT:   Slot = sin(Slot)
};

/**
 * These codes are allowed to use and not restricted by @see FormulaInterpreter
 */
static std::set<EOpCode> AllowedOpCodes = {
    EOpCode::OP_LOAD,
    EOpCode::OP_MOV,
    EOpCode::OP_MOVC,
    EOpCode::OP_STORE,
    EOpCode::OP_ADD,
    EOpCode::OP_ADDC,
    EOpCode::OP_SUB,
    EOpCode::OP_SUBC,
    EOpCode::OP_SUBCI,
    EOpCode::OP_MUL,
    EOpCode::OP_MULC,
    EOpCode::OP_DIV,
    EOpCode::OP_DIVC,
    EOpCode::OP_DIVCI,
    EOpCode::OP_POW,
    EOpCode::OP_POWC,
    EOpCode::OP_POWCI,
    EOpCode::OP_NEG,
    EOpCode::OP_CALL1,
    EOpCode::OP_CALL2,
    EOpCode::OP_PUSH,
    EOpCode::OP_PUSHC,
};

/**
 * Each opcode should has string representation in debug purposes
 */
static std::map<EOpCode, const char*> OpCode2StrMap = {
    {EOpCode::OP_LOAD, "LOAD"},
    {EOpCode::OP_MOV, "MOV"},
    {EOpCode::OP_MOVC, "MOVC"},
    {EOpCode::OP_STORE, "STORE"},
    {EOpCode::OP_ADD, "ADD"},
    {EOpCode::OP_ADDC, "ADDC"},
    {EOpCode::OP_SUB, "SUB"},
    {EOpCode::OP_SUBC, "SUBC"},
    {EOpCode::OP_SUBCI, "SUBCI"},
    {EOpCode::OP_MUL, "MUL"},
    {EOpCode::OP_MULC, "MULC"},
    {EOpCode::OP_DIV, "DIV"},
    {EOpCode::OP_DIVC, "DIVC"},
    {EOpCode::OP_DIVCI, "DIVCI"},
    {EOpCode::OP_POW, "POW"},
    {EOpCode::OP_POWC, "POWC"},
    {EOpCode::OP_POWCI, "POWCI"},
    {EOpCode::OP_NEG, "NEG"},
    {EOpCode::OP_CALL1, "CALL1"},
    {EOpCode::OP_CALL2, "CALL2"},
    {EOpCode::OP_PUSH, "PUSH"},
    {EOpCode::OP_PUSHC, "PUSHC"},
};

/**
 * Converts Opcode to c string
 */
static const char* OpCode2Str(EOpCode OpCode)
{    
    auto it = OpCode2StrMap.find(OpCode);
    if (it != OpCode2StrMap.end())
        return it->second;
    return "";
}