#include "vxpch.h"
#include "Input.h"

#include "Console.h"

namespace VoltexEngine {

	std::vector<Input::DiscreteType> Input::s_DiscreteInputs[];
	float Input::s_CursorX;
	float Input::s_CursorY;
	float Input::s_CursorXIntermediary;
	float Input::s_CursorYIntermediary;

	bool Input::Init()
	{
		for (int i = 0; i < NUM_KEY_CODES + NUM_CLICK_CODES; i++)
		{
			s_DiscreteInputs[i].push_back(DiscreteType::None);
		}

		VX_LOG("Input Initialized");
		return true;
	}

	void Input::Tick()
	{
		// Update keys
		for (int i = 0; i < NUM_KEY_CODES + NUM_CLICK_CODES; i++)
		{
			// If the current code has inputs queued up, move them up one
			if (s_DiscreteInputs[i].size() > 1)
				s_DiscreteInputs[i].erase(s_DiscreteInputs[i].begin());

			// The current code has no inputs queued up, set its InputType to none
			else
				s_DiscreteInputs[i][0] = DiscreteType::None;
		}

		// Update cursor
		s_CursorX = s_CursorXIntermediary;
		s_CursorY = s_CursorYIntermediary;
	}

}