#include "vxpch.h"
#include "Input.h"

#include "Console.h"

namespace VoltexEngine {

	std::vector<Input::DiscreteType> Input::m_DiscreteInputs[];

	bool Input::Init()
	{
		for (int i = 0; i < NUM_KEY_CODES + NUM_CLICK_CODES; i++)
		{
			m_DiscreteInputs[i].push_back(DiscreteType::None);
		}

		VX_LOG("Input Initialized");
		return true;
	}

	void Input::Tick()
	{
		for (int i = 0; i < NUM_KEY_CODES + NUM_CLICK_CODES; i++)
		{
			// If the current code has inputs queued up, move them up one
			if (m_DiscreteInputs[i].size() > 1)
				m_DiscreteInputs[i].erase(m_DiscreteInputs[i].begin());

			// The current code has no inputs queued up, set its InputType to none
			else
				m_DiscreteInputs[i][0] = DiscreteType::None;
		}
	}

}