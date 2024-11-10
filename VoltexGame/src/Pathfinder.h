#pragma once

#include <VXCore.h>

using namespace VoltexEngine;

class Pathfinder
{

private:

	static bool s_LevelDataInitialized;

	static std::shared_ptr<Sprite> s_PathSprite;

	struct PathPoint
	{
		std::shared_ptr<SpriteComponent> spriteComponent;
		Vector position;
		int depth;
		bool colliding = false;

		PathPoint()
			: depth(0), position(Vector())
		{
		}
	};

	std::vector<PathPoint> m_Points;

	int m_XPointCount;
	int m_YPointCount;

	float m_StepSize;

public:

	Pathfinder(const Vector& BottomLeftBound, const Vector& TopRightBound, float stepSize);

	void DepthFirst(const Vector& startPos, const Vector& endPos);

private:

	PathPoint* GetClosestPathPoint(const Vector& position);

};