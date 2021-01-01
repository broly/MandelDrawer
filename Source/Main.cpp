#ifndef QT_CORE_LIB

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <complex>
#include "MandelDrawer.h"
#include "Animation.h"
#include "Formulas/Formula.h"
#include "Formulas/FormulaCompiler.h"
#include "Formulas/FormulaInterpreter.h"
#include "Formulas/Parser.h"

using namespace Mandel;

/// This construction is needed to avoid libjpeg bug
FILE _iob[] = { *stdin, *stdout, *stderr };
extern "C" FILE * __cdecl __iob_func(void)
{
	return _iob;
}
/// Probably need to build libjpeg manually for x64


int main()
{
	// TODO: Interpreter test
	// Complex a = {1.0, 2.0};
	// Complex b = {3.0, 4.0};
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


	const MandelDrawerSettings Settings	
	{
		{2048, 2048},
		16,
		250,
		25.f,
		1.f,
		0.35f,
		true,
		{},
		{},
		EMandelDrawMethod::MultiThreaded_ByPixelOrder,
		true,
		"z^-2",
		nullptr
	};

	auto t = std::clock();

	Animation One(
		1,
		{
			{0.0, {0.0, 0.0}},
			{1.0, {0.3, 0.0}},
			{2.0, {0.37, 0.32}},
			{3.0, {0.36, 0.6}},
			{4.0, {0.11, 0.62}},
			{5.0, {-0.09, 0.65}},
			{6.0, {-0.5, 0.57}},
			{7.0, {-0.772, 0.137}},
			{8.0, {-0.762, 0.f}},
			{9.0, {-1.309, 0.f}},
			{10.0, {-1.767, 0.f}},
			{11.0, {-2.f, 0.f}},
		}, Settings, 5);

	One.StartAnimation();

	std::cout << "Total time: " <<(std::clock() - t)/(double) CLOCKS_PER_SEC << std::endl;

	
	//system(); //открывает бмп
	

	std::cout << "Good bye";
}

#endif
