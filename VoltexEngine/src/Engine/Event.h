#pragma once
#include "vxpch.h"

namespace VoltexEngine {

	/* An event is an object that can hold links to various functions, and calls these functions when it's invoked */

	/* An event whos callback functions take no parameters */
	class Event_NoParam
	{

		/* The signature of a function this event can hold */
		using Signature = std::function<void()>;

	public:

		/* Add a function to be called when this event is invoked */
		void AddCallback(Signature function)
		{
			m_Functions.push_back(function);
		}

		/* Call all stored functions on this event */
		void Invoke()
		{
			for (Signature func : m_Functions)
			{
				func();
			}
		}

	private:

		/* A list of all functions that should be called when this event is invoked */
		std::vector<Signature> m_Functions;

	};

	/* An event whos callback functions take one parameter */
	template <typename T>
	class Event_OneParam
	{

		/* The signature of a function this event can hold */
		using Signature = std::function<void(T)>;

	public:

		/* Add a function to be called when this event is invoked */
		void AddCallback(Signature function)
		{
			m_Functions.push_back(function);
		}

		/* Call all stored functions on this event */
		void Invoke(T t)
		{
			for (Signature func : m_Functions)
			{
				func(t);
			}
		}

	private:

		/* A list of all functions that should be called when this event is invoked */
		std::vector<Signature> m_Functions;

	};

	/* An event whos callback functions take two parameters */
	template <typename T1, typename T2>
	class Event_TwoParam
	{

		/* The signature of a function this event can hold */
		using Signature = std::function<void(T1, T2)>;

	public:

		/* Add a function to be called when this event is invoked */
		void AddCallback(Signature function)
		{
			m_Functions.push_back(function);
		}

		/* Call all stored functions on this event */
		void Invoke(T1 t1, T2 t2)
		{
			for (Signature func : m_Functions)
			{
				func(t1, t2);
			}
		}

	private:

		/* A list of all functions that should be called when this event is invoked */
		std::vector<Signature> m_Functions;

	};

}
