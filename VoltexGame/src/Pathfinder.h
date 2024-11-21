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
		Vector worldPosition;
		int xInd = 0;
		int yInd = 0;
		int depth = 0;
		int heuristic = 0;
		bool colliding = false;
		bool processed = false;
		PathPoint* precedingPoint;
	};

	std::vector<PathPoint> m_Points;

	int m_XPointCount;
	int m_YPointCount;

	float m_StepSize;

	/* Debugging */
	int m_ProcessedCount = 0;
	int m_EvaluatedCount = 0;
	int m_PathCount = 0;

public:

	Pathfinder(const Vector& BottomLeftBound, const Vector& TopRightBound, float stepSize);

	void DepthFirst(const Vector& startPos, const Vector& endPos);

	void BredthFirst(const Vector& startPos, const Vector& endPos);

	void GreedySearch(const Vector& startPos, const Vector& endPos);

	void AStar(const Vector& startPos, const Vector& endPos);

private:

	/* Returns the path point closest to a world coordinate */
	PathPoint* GetClosestPathPoint(const Vector& position);

	/* Returns a path point at the given coordinates in path coordinates if one exists, is not colliding with anything, and has not yet been processed. Returns nullptr otherwise */
	PathPoint* Pathfinder::GetPathPoint(int x, int y);

	/* Returns a PathPoint in the relevant direction if one exists, is not colliding, and has not been processed yet */
	PathPoint* GetAbove(PathPoint* point);
	PathPoint* GetBelow(PathPoint* point);
	PathPoint* GetLeft(PathPoint* point);
	PathPoint* GetRight(PathPoint* point);

};