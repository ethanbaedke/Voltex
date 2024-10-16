#pragma once
#include "vxpch.h"

namespace VoltexEngine {

	enum class InputCode
	{
		SPACE = 32,
		QUOTE = 39,
		LESSTHAN = 44,
		MINUS = 45,
		GREATERTHAN = 46,
		FORWARDSLASH = 47,
		NUM0 = 48,
		NUM1 = 49,
		NUM2 = 50,
		NUM3 = 51,
		NUM4 = 52,
		NUM5 = 53,
		NUM6 = 54,
		NUM7 = 55,
		NUM8 = 56,
		NUM9 = 57,
		COLON = 59,
		PLUS = 61,
		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,
		LBRACKET = 91,
		BACKSLASH = 92,
		RBRACKET = 93,
		TILDE = 96,
		ESC = 256,
		ENTER = 257,
		TAB = 258,
		DELETE = 261,
		RightArrow = 262,
		LeftArrow = 263,
		DownArrow = 264,
		UpArrow = 265,
		CAPSLK = 280,
		LSHIFT = 340,
		LCTRL = 341,
		LALT = 342,
		RALT = 346,
		RSHIFT = 344,
		RCTRL = 345
	};

	class Input
	{

	public:

		enum class InputType
		{
			None,
			KeyDown,
			KeyUp
		};

	public:

		static bool Init();

		/* Pushes the next queued InputType for every code into the front of the list */
		static void Tick();

		/* The following functions return true if the InputCode has the passed has that InputType this frame, otherwise they return false */
		static inline bool None(InputCode code) { return m_Inputs[static_cast<int>(code)][0] == InputType::None; }
		static inline bool KeyDown(InputCode code) { return m_Inputs[static_cast<int>(code)][0] == InputType::KeyDown; }
		static inline bool KeyUp(InputCode code) { return m_Inputs[static_cast<int>(code)][0] == InputType::KeyUp; }

		/* Add an input to the queue of a certain input code */
		static void EnqueueInput(int codeNum, InputType type);

	private:

		static const int NUM_INPUT_CODES = 266;

		static std::vector<InputType> m_Inputs[NUM_INPUT_CODES];

	};

}