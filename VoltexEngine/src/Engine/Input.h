#pragma once
#include "vxpch.h"

namespace VoltexEngine {

	enum class KeyCode
	{
		None = 0,
		Num0 = 1,
		Num1 = 2,
		Num2 = 3,
		Num3 = 4,
		Num4 = 5,
		Num5 = 6,
		Num6 = 7,
		Num7 = 8,
		Num8 = 9,
		Num9 = 10,
		A = 11,
		B = 12,
		C = 13,
		D = 14,
		E = 15,
		F = 16,
		G = 17,
		H = 18,
		I = 19,
		J = 20,
		K = 21,
		L = 22,
		M = 23,
		N = 24,
		O = 25,
		P = 26,
		Q = 27,
		R = 28,
		S = 29,
		T = 30,
		U = 31,
		V = 32,
		W = 33,
		X = 34,
		Y = 35,
		Z = 36,
		RightArrow = 37,
		UpArrow = 38,
		LeftArrow = 39,
		DownArrow = 40,
		Tab = 41,
		Shift = 42,
		Ctrl = 43,
		Alt = 44,
		Space = 45,
		Enter = 46
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
		static inline bool KeyNone(KeyCode code) { return m_KeyInputs[static_cast<int>(code)][0] == InputType::None; }
		static inline bool KeyDown(KeyCode code) { return m_KeyInputs[static_cast<int>(code)][0] == InputType::KeyDown; }
		static inline bool KeyUp(KeyCode code) { return m_KeyInputs[static_cast<int>(code)][0] == InputType::KeyUp; }

		/* Add an input to the queue of a certain input code */
		static inline void EnqueueKey(KeyCode code, InputType type) { m_KeyInputs[static_cast<int>(code)].push_back(type); }

	private:

		/* This should be one greater than the index of the final element in the KeyCodes enum */
		static const int NUM_KEY_CODES = 47;

		static std::vector<InputType> m_KeyInputs[NUM_KEY_CODES];

	};

}