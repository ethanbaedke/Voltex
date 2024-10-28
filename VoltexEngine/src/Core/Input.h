#pragma once
#include "vxpch.h"

namespace VoltexEngine {

	enum class KeyCode
	{
		Num0 = 0,
		Num1 = 1,
		Num2 = 2,
		Num3 = 3,
		Num4 = 4,
		Num5 = 5,
		Num6 = 6,
		Num7 = 7,
		Num8 = 8,
		Num9 = 9,
		A = 10,
		B = 11,
		C = 12,
		D = 13,
		E = 14,
		F = 15,
		G = 16,
		H = 17,
		I = 18,
		J = 19,
		K = 20,
		L = 21,
		M = 22,
		N = 23,
		O = 24,
		P = 25,
		Q = 26,
		R = 27,
		S = 28,
		T = 29,
		U = 30,
		V = 31,
		W = 32,
		X = 33,
		Y = 34,
		Z = 35,
		RightArrow = 36,
		UpArrow = 37,
		LeftArrow = 38,
		DownArrow = 39,
		Tab = 40,
		Shift = 41,
		Ctrl = 42,
		Alt = 43,
		Space = 44,
		Enter = 45
	};

	enum class ClickCode
	{
		LeftMouse = 0,
		RightMouse = 1
	};

	class Input
	{

	public:

		enum class DiscreteType
		{
			None,
			Down,
			Up
		};

	private:

		/* This should be one greater than the index of the final element in the code menu enums */
		static const int NUM_KEY_CODES = 46;
		static const int NUM_CLICK_CODES = 2;

		static std::vector<DiscreteType> s_DiscreteInputs[NUM_KEY_CODES + NUM_CLICK_CODES];

		/* The position of the cursor in UI coordinates */
		static float s_CursorX;
		static float s_CursorY;

		/* These store cursor updates between frames so the true value of the cursor position is only changed once each frame */
		static float s_CursorXIntermediary;
		static float s_CursorYIntermediary;

	public:

		static bool Init();

		/* Pushes the next queued InputType for every code into the front of the list */
		static void Tick();

		/* The following functions return true if the code passed has the relevant input type, otherwise they return false */
		static inline bool KeyNone(KeyCode code) { return s_DiscreteInputs[static_cast<int>(code)][0] == DiscreteType::None; }
		static inline bool KeyDown(KeyCode code) { return s_DiscreteInputs[static_cast<int>(code)][0] == DiscreteType::Down; }
		static inline bool KeyUp(KeyCode code) { return s_DiscreteInputs[static_cast<int>(code)][0] == DiscreteType::Up; }
		static inline bool MouseNone(ClickCode code) { return s_DiscreteInputs[static_cast<int>(code) + NUM_KEY_CODES][0] == DiscreteType::None; }
		static inline bool MouseDown(ClickCode code) { return s_DiscreteInputs[static_cast<int>(code) + NUM_KEY_CODES][0] == DiscreteType::Down; }
		static inline bool MouseUp(ClickCode code) { return s_DiscreteInputs[static_cast<int>(code) + NUM_KEY_CODES][0] == DiscreteType::Up; }

		/* Add an input to the queue of a certain input code */
		static inline void EnqueueKey(KeyCode code, DiscreteType type) { s_DiscreteInputs[static_cast<int>(code)].push_back(type); }
		static inline void EnqueueClick(ClickCode code, DiscreteType type) { s_DiscreteInputs[static_cast<int>(code) + NUM_KEY_CODES].push_back(type); }
		
		/* Set/Get the cursor position */
		static inline void SetCursorPosition(float xPos, float yPos) { s_CursorXIntermediary = xPos; s_CursorYIntermediary = yPos; }
		static inline void GetCursorPosition(float* outXPos, float* outYPos) { *outXPos = s_CursorX; *outYPos = s_CursorY; }

	};

}