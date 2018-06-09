cmake_minimum_required(VERSION 3.10)
project(SIC_XE_Assembler)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_executable(SIC_XE_Assembler
        include/AssemblyListing.h
        include/ObjectProgram.h
        include/OpCodeTable.h
        include/SourceLine.h
        include/SourceProgram.h
        include/SymbolTable.h
        include/SyntaxValidator.h
        include/ValidatorUtilities.h
        include/OpInfo.h
        include/UpdateLocationCounter.h
        include/SymbolInfo.h
        include/LiteralTable.h
        include/DetectLiterals.h
        include/ExpressionEvaluator.h
        include/Pass2.h
        src/AssemblyListing.cpp
        src/ObjectProgram.cpp
        src/OpCodeTable.cpp
        src/SourceLine.cpp
        src/SourceProgram.cpp
        src/SymbolTable.cpp
        src/SyntaxValidator.cpp
        src/ValidatorUtilities.cpp
        src/OpInfo.cpp
        src/UpdateLocationCounter.cpp
        src/SymbolInfo.cpp
        src/DetectLiterals.cpp
        src/LiteralTable.cpp
        src/ExpressionEvaluator.cpp
        src/Pass2.cpp
        main.cpp include/ObjectCodeGenerator.h src/ObjectCodeGenerator.cpp include/RegistersTable.h src/RegistersTable.cpp)
