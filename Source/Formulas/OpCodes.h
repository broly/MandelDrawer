#pragma once

#include <set>
#include <vector>

#include "../Types.h"

namespace Mandel
{
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
        OP_LOAD = 0,
        OP_MOVC = 1,
        OP_CALL1 = 2,
        OP_CALL2 = 3
    };

    /**
    * These codes are allowed to use and not restricted by @see FormulaInterpreter
    */
    static std::set<EOpCode> AllowedOpCodes = {
        EOpCode::OP_LOAD,
        EOpCode::OP_MOVC,
        EOpCode::OP_CALL1,
        EOpCode::OP_CALL2,
    };

    /**
    * Each opcode should has string representation in debug purposes
    */
    static std::map<EOpCode, const char*> OpCode2StrMap = {
        {EOpCode::OP_LOAD, "LOAD"},
        {EOpCode::OP_MOVC, "MOVC"},
        {EOpCode::OP_CALL1, "CALL1"},
        {EOpCode::OP_CALL2, "CALL2"},
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

    struct OpCodeSequence
    {
        std::vector<uint8> Data;

        friend OpCodeSequence& operator<<(OpCodeSequence& Left, uint8 Value)
        {
            Left.AddBytes(Value);
            return Left;
        }
        
        friend OpCodeSequence& operator<<(OpCodeSequence& Left, EOpCode Value)
        {
            Left.AddBytes(Value);
            return Left;
        }

        friend OpCodeSequence& operator<<(OpCodeSequence& Left, uint16 Value)
        {
            Left.AddBytes(Value);
            return Left;
        }

        friend OpCodeSequence& operator<<(OpCodeSequence& Left, uint32 Value)
        {
            Left.AddBytes(Value);
            return Left;
        }

        friend OpCodeSequence& operator<<(OpCodeSequence& Left, uint64 Value)
        {
            Left.AddBytes(Value);
            return Left;
        }

        friend OpCodeSequence& operator<<(OpCodeSequence& Left, Complex Value)
        {
            Left.AddBytes(Value);
            return Left;
        }

        template<typename T>
        void AddBytes(T Value)
        {
            size_t OldSize = Data.size();
            Data.resize(OldSize + sizeof(Value));
            memcpy(Data.data() + Data.size() - sizeof(Value), &Value, sizeof(Value));
        }
    };
}
