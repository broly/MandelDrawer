#pragma once
#include "Vector2D.h"
#include "Formulas/Formula.h"

namespace Mandel
{
	/**
	* Fractal drawing method.
	*	todo Probably must be extended
	*/
	enum class EMandelDrawMethod
	{
		// SingleThread,  // todo: planned
		// MultiThreaded_ByChunk,  // todo DEPRECATED
		MultiThreaded_ByPixelOrder,
        // CUDA,  // todo: planned
    };


	struct MandelDrawerSettings
	{
		/** General resolution of picture */
		IntVector2D Resolution;
	
		/** Count of threads that launches workers */
		int NumThreads = 16;

		/** Fractal calculation iteration limit */
		int IterLimit = 150;

		/** Fractal calculation escape value */
		float EscapeValue = 8.f;

		/** The brightness of picture */
		float Brightness = 1.f;

		/** Scale factor of drawing fractal */
		float DrawScale = 1.f;

		/** Is this fractal calculation in julia mode? @see JuliaValue */
		bool bJuliaMode = false;

		/** Julia value coefficient (works only with @see bJuliaMode) */
		FloatVector2D JuliaValue = {0.f, 0.f };

		/** Drawing offset of fractal */
		FloatVector2D DrawOffset = {0.f, 0.f };

		/** Render method */
		EMandelDrawMethod MandelDrawMethod = EMandelDrawMethod::MultiThreaded_ByPixelOrder;

		/** */
		bool bUsesCustomFormula = false;

		/** */
		std::string CustomFormula = "";

		/** */
		std::shared_ptr<VariablesList> FormulaVariables = nullptr;
	};
}