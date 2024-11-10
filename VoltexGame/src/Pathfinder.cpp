#include "Pathfinder.h"

bool Pathfinder::s_LevelDataInitialized;
std::shared_ptr<Sprite> Pathfinder::s_PathSprite;

Pathfinder::Pathfinder(const Vector& BottomLeftBound, const Vector& TopRightBound, float stepSize)
	: m_XPointCount(0), m_YPointCount(0), m_StepSize(stepSize)
{
	if (!s_LevelDataInitialized)
	{
		s_PathSprite = Sprite::Create("textures/AIPath.png");
		s_LevelDataInitialized = true;
	}

	if (TopRightBound.X < BottomLeftBound.X || TopRightBound.Y < BottomLeftBound.Y)
	{
		VX_WARNING("Pathfinder fed invalid bounds");
		m_XPointCount = 0;
		m_YPointCount = 0;
	}
	else
	{
		m_XPointCount = ((TopRightBound.X - BottomLeftBound.X) / stepSize) + 1;
		m_YPointCount = ((TopRightBound.Y - BottomLeftBound.Y) / stepSize) + 1;
	}

	m_Points = std::vector<PathPoint>(m_XPointCount * m_YPointCount);

	// Initialize all points the pathfinder can use within its bounds
	for (int x = 0; x < m_XPointCount; x++)
	{
		for (int y = 0; y < m_YPointCount; y++)
		{
			Vector position = Vector(BottomLeftBound.X + (x * stepSize), BottomLeftBound.Y + (y * stepSize));
			PathPoint& point = m_Points[x + (y * m_XPointCount)];
			point.position = position;

			std::shared_ptr<GameObject> obj = GameObject::Create<GameObject>();
			obj->Position = position;
			obj->Depth = 1;

			point.spriteComponent = obj->AddComponent<SpriteComponent>();
			point.spriteComponent->Sprite = s_PathSprite;

			if (Application::OverlapPoint(position))
			{
				point.colliding = true;
				point.spriteComponent->Tint = Color(255, 0, 0, 255);
			}
		}
	}
}

void Pathfinder::DepthFirst(const Vector& startPos, const Vector& endPos)
{
	if (m_Points.size() == 0)
	{
		VX_WARNING("No points in pathfinder");
		return;
	}

	PathPoint* startPoint = GetClosestPathPoint(startPos);
	PathPoint* endPoint = GetClosestPathPoint(endPos);

	if (!startPoint || !endPoint)
	{
		VX_ERROR("Could not find a valid point in the pathfinder for the start or end position");
		return;
	}

	startPoint->spriteComponent->Tint = Color(0, 255, 0, 255);
	endPoint->spriteComponent->Tint = Color(0, 255, 0, 255);

	std::vector<PathPoint> stack;

	m_Points[0].spriteComponent->Tint = Color(0, 0, 255, 255);
}

Pathfinder::PathPoint* Pathfinder::GetClosestPathPoint(const Vector& position)
{
	// Get the path points closest to the given position
	PathPoint* point = nullptr;
	float closestDist = std::numeric_limits<float>().max();
	for (int i = 0; i < m_Points.size(); i++)
	{
		if (m_Points[i].colliding)
			continue;

		float currentDist = (m_Points[i].position - position).Length();
		if (currentDist < closestDist)
		{
			point = &(m_Points[i]);
			closestDist = currentDist;
		}
	}

	return point;
}