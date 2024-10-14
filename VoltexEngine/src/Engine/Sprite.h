#pragma once
#include "vxpch.h"

#include "Event.h"

namespace VoltexEngine {

	class Sprite
	{

	public:

		/* Broadcasts a sprite whenever one is created with the path to its texture */
		static Event_TwoParam<std::weak_ptr<Sprite>, const std::string&> s_OnSpriteCreated;

	private:

		/* This is used exclusively by the renderer */
		unsigned int m_TextureID;

	public:

		/* Creates and returns a new sprite */
		static std::shared_ptr<Sprite> Create(const std::string& texturePath);

		/* These should only be called by the renderer */
		inline void SetTextureID(unsigned int textureID) { m_TextureID = textureID; }
		inline unsigned int GetTextureID() const { return m_TextureID; }

	private:

		Sprite(const std::string& texturePath);

	};

}

