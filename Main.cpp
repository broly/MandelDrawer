﻿#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <complex>
#include "MandelDrawer.h"
#include "Animation.h"
#include "Formulas/Formula.h"
#include "Formulas/FormulaCompiler.h"
#include "Formulas/FormulaInterpreter.h"
#include "Formulas/Parser.h"


/// This construction is needed to avoid libjpeg bug
FILE _iob[] = { *stdin, *stdout, *stderr };
extern "C" FILE * __cdecl __iob_func(void)
{
	return _iob;
}
/// Probably need to build libjpeg manually for x64


int main()
{
	std::complex<float> x = 1.f;
	std::complex<float> y = 2.f;

	Formula f("(-5 + 5i) + x", {{{"x", &x}, {"y", &y}}});

	std::cout << f.EvaluateOnFly();
	
	// TODO: Interpreter test
	// std::complex<float> a = {1.0, 2.0};
	// std::complex<float> b = {3.0, 4.0};
	// FormulaInterpreter fi;
	// fi.SetVariables({&a, &b});
	// std::vector<uint8> bytecode = {
	// 	(uint8)EOpCode::OP_LOAD,      0x1, 0x0,         0x1, 0x0,
	// 	(uint8)EOpCode::OP_LOAD,      0x0, 0x0,         0x0, 0x0,
	// 	(uint8)EOpCode::OP_ADD,		  0x1, 0x0,		    0x0, 0x0,
	// 	(uint8)EOpCode::OP_PUSH,	  0x1, 0x0,
	// 	
    // };
	// fi.Bytecode = bytecode;
	// fi.CommandsNum = 4;
	// fi.Execute();
	// std::cout << fi.Slots[fi.SlotsStack[0]];

	// Animation One(
	// 	25,
	// 	{
	// 		{0.0, {0.0, 0.0}},
	// 		{1.0, {0.3, 0.0}},
	// 		{2.0, {0.37, 0.32}},
	// 		{3.0, {0.36, 0.6}},
	// 		{4.0, {0.11, 0.62}},
	// 		{5.0, {-0.09, 0.65}},
	// 		{6.0, {-0.5, 0.57}},
	// 		{7.0, {-0.772, 0.137}},
	// 		{8.0, {-0.762, 0.f}},
	// 		{9.0, {-1.309, 0.f}},
	// 		{10.0, {-1.767, 0.f}},
	// 		{11.0, {-2, 0.f}},
	// 	}, 0.33f, 250, 25, {1000, 1000}, {0.f}, 5.f);
// 
	// One.StartAnimation();

	
	//system(); //открывает бмп
	

	std::cout << "Good bye";
}