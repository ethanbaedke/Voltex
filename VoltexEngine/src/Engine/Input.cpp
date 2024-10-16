#include "vxpch.h"
#include "Input.h"

#include "Console.h"

namespace VoltexEngine {

	std::vector<Input::InputType> Input::m_Inputs[];

	bool Input::Init()
	{
		for (int i = 0; i < NUM_INPUT_CODES; i++)
		{
			m_Inputs[i].push_back(InputType::None);
		}

		VX_LOG("Input Initialized");
		return true;
	}

	void Input::Tick()
	{
		for (int i = 0; i < NUM_INPUT_CODES; i++)
		{
			// If the current code has inputs queued up, move them up one
			if (m_Inputs[i].size() > 1)
				m_Inputs[i].erase(m_Inputs[i].begin());

			// The current code has no inputs queued up, set its InputType to none
			else
				m_Inputs[i][0] = InputType::None;
		}
	}

	void Input::EnqueueInput(int codeNum, InputType type)
	{
		if (codeNum < 0 || codeNum > NUM_INPUT_CODES)
		{
			VX_WARNING("Input code number " + std::to_string(codeNum) + " not supported, must be in range 0 - " + std::to_string(NUM_INPUT_CODES));
			return;
		}

		m_Inputs[codeNum].push_back(type);
	}

}