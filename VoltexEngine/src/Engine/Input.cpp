#include "vxpch.h"
#include "Input.h"

#include "Console.h"

namespace VoltexEngine {

	std::vector<Input::InputType> Input::m_KeyInputs[];

	bool Input::Init()
	{
		for (int i = 0; i < NUM_KEY_CODES; i++)
		{
			m_KeyInputs[i].push_back(InputType::None);
		}

		VX_LOG("Input Initialized");
		return true;
	}

	void Input::Tick()
	{
		for (int i = 0; i < NUM_KEY_CODES; i++)
		{
			// If the current code has inputs queued up, move them up one
			if (m_KeyInputs[i].size() > 1)
				m_KeyInputs[i].erase(m_KeyInputs[i].begin());

			// The current code has no inputs queued up, set its InputType to none
			else
				m_KeyInputs[i][0] = InputType::None;
		}
	}

}