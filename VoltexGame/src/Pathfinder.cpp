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
			point.worldPosition = position;
			point.xInd = x;
			point.yInd = y;

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

	std::vector<PathPoint*> stack;
	stack.push_back(startPoint);
	startPoint->processed = true;
	m_ProcessedCount++;

	while (stack.size() > 0)
	{
		// Pop a point off the stack
		PathPoint* current = stack.back();
		stack.pop_back();
		m_EvaluatedCount++;

		current->spriteComponent->Tint = Color(255, 255, 0, 255);

		// Check if we've reached our destination
		if (current == endPoint)
		{
			// Backtrack through all our preceding points and paint our path green
			current->spriteComponent->Tint = Color(0, 255, 0, 255);
			m_PathCount++;
			while (current->precedingPoint != nullptr)
			{
				current = current->precedingPoint;
				current->spriteComponent->Tint = Color(0, 255, 0, 255);
				m_PathCount++;
			}
			stack.clear();
		}
		else
		{
			// Handle its surrounding points
			if (PathPoint* above = GetAbove(current))
			{
				above->processed = true;
				stack.push_back(above);
				above->precedingPoint = current;
				above->spriteComponent->Tint = Color(0, 0, 255, 255);
				m_ProcessedCount++;
			}
			if (PathPoint* left = GetLeft(current))
			{
				left->processed = true;
				stack.push_back(left);
				left->precedingPoint = current;
				left->spriteComponent->Tint = Color(0, 0, 255, 255);
				m_ProcessedCount++;
			}
			if (PathPoint* right = GetRight(current))
			{
				right->processed = true;
				stack.push_back(right);
				right->precedingPoint = current;
				right->spriteComponent->Tint = Color(0, 0, 255, 255);
				m_ProcessedCount++;
			}
			if (PathPoint* below = GetBelow(current))
			{
				below->processed = true;
				stack.push_back(below);
				below->precedingPoint = current;
				below->spriteComponent->Tint = Color(0, 0, 255, 255);
				m_ProcessedCount++;
			}
		}
	}

	startPoint->spriteComponent->Tint = Color(0, 255, 255, 255);
	endPoint->spriteComponent->Tint = Color(0, 255, 255, 255);

	VX_LOG("Points processed: " + std::to_string(m_ProcessedCount));
	VX_LOG("Points evaluated: " + std::to_string(m_EvaluatedCount));
	VX_LOG("Points in path: " + std::to_string(m_PathCount));
	VX_LOG("Points processed but not evaluated: " + std::to_string(m_ProcessedCount - m_EvaluatedCount));
	VX_LOG("Points evaluated but not in path: " + std::to_string(m_EvaluatedCount - m_PathCount));
}

void Pathfinder::BredthFirst(const Vector& startPos, const Vector& endPos)
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

	std::vector<PathPoint*> queue;
	queue.push_back(startPoint);
	startPoint->processed = true;
	m_ProcessedCount++;

	while (queue.size() > 0)
	{
		// Pop a point off the stack
		PathPoint* current = queue.front();
		queue.erase(queue.begin());
		m_EvaluatedCount++;

		current->spriteComponent->Tint = Color(255, 255, 0, 255);

		// Check if we've reached our destination
		if (current == endPoint)
		{
			// Backtrack through all our preceding points and paint our path green
			current->spriteComponent->Tint = Color(0, 255, 0, 255);
			m_PathCount++;
			while (current->precedingPoint != nullptr)
			{
				current = current->precedingPoint;
				current->spriteComponent->Tint = Color(0, 255, 0, 255);
				m_PathCount++;
			}
			queue.clear();
		}
		else
		{
			// Handle its surrounding points
			if (PathPoint* below = GetBelow(current))
			{
				below->processed = true;
				queue.push_back(below);
				below->precedingPoint = current;
				below->spriteComponent->Tint = Color(0, 0, 255, 255);
				m_ProcessedCount++;
			}
			if (PathPoint* right = GetRight(current))
			{
				right->processed = true;
				queue.push_back(right);
				right->precedingPoint = current;
				right->spriteComponent->Tint = Color(0, 0, 255, 255);
				m_ProcessedCount++;
			}
			if (PathPoint* left = GetLeft(current))
			{
				left->processed = true;
				queue.push_back(left);
				left->precedingPoint = current;
				left->spriteComponent->Tint = Color(0, 0, 255, 255);
				m_ProcessedCount++;
			}
			if (PathPoint* above = GetAbove(current))
			{
				above->processed = true;
				queue.push_back(above);
				above->precedingPoint = current;
				above->spriteComponent->Tint = Color(0, 0, 255, 255);
				m_ProcessedCount++;
			}
		}
	}

	startPoint->spriteComponent->Tint = Color(0, 255, 255, 255);
	endPoint->spriteComponent->Tint = Color(0, 255, 255, 255);

	VX_LOG("Points processed: " + std::to_string(m_ProcessedCount));
	VX_LOG("Points evaluated: " + std::to_string(m_EvaluatedCount));
	VX_LOG("Points in path: " + std::to_string(m_PathCount));
	VX_LOG("Points processed but not evaluated: " + std::to_string(m_ProcessedCount - m_EvaluatedCount));
	VX_LOG("Points evaluated but not in path: " + std::to_string(m_EvaluatedCount - m_PathCount));
}

void Pathfinder::GreedySearch(const Vector& startPos, const Vector& endPos)
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

	std::vector<PathPoint*> priorityQueue;
	priorityQueue.push_back(startPoint);
	startPoint->processed = true;
	m_ProcessedCount++;

	while (priorityQueue.size() > 0)
	{
		// Pop a point off the stack
		PathPoint* current = priorityQueue.back();
		priorityQueue.pop_back();
		m_EvaluatedCount++;

		current->spriteComponent->Tint = Color(255, 255, 0, 255);

		// Check if we've reached our destination
		if (current == endPoint)
		{
			// Backtrack through all our preceding points and paint our path green
			current->spriteComponent->Tint = Color(0, 255, 0, 255);
			m_PathCount++;
			while (current->precedingPoint != nullptr)
			{
				current = current->precedingPoint;
				current->spriteComponent->Tint = Color(0, 255, 0, 255);
				m_PathCount++;
			}
			priorityQueue.clear();
		}
		else
		{
			// Handle its surrounding points
			if (PathPoint* above = GetAbove(current))
			{
				above->processed = true;
				above->heuristic = (endPoint->worldPosition - above->worldPosition).Length();
				above->precedingPoint = current;
				above->spriteComponent->Tint = Color(0, 0, 255, 255);
				m_ProcessedCount++;

				int i = 0;
				while (i < priorityQueue.size() && above->heuristic < priorityQueue[i]->heuristic)
					i++;
				priorityQueue.insert(priorityQueue.begin() + i, above);
			}
			if (PathPoint* left = GetLeft(current))
			{
				left->processed = true;
				left->heuristic = (endPoint->worldPosition - left->worldPosition).Length();
				left->precedingPoint = current;
				left->spriteComponent->Tint = Color(0, 0, 255, 255);
				m_ProcessedCount++;

				int i = 0;
				while (i < priorityQueue.size() && left->heuristic < priorityQueue[i]->heuristic)
					i++;
				priorityQueue.insert(priorityQueue.begin() + i, left);
			}
			if (PathPoint* right = GetRight(current))
			{
				right->processed = true;
				right->heuristic = (endPoint->worldPosition - right->worldPosition).Length();
				right->precedingPoint = current;
				right->spriteComponent->Tint = Color(0, 0, 255, 255);
				m_ProcessedCount++;

				int i = 0;
				while (i < priorityQueue.size() && right->heuristic < priorityQueue[i]->heuristic)
					i++;
				priorityQueue.insert(priorityQueue.begin() + i, right);
			}
			if (PathPoint* below = GetBelow(current))
			{
				below->processed = true;
				below->heuristic = (endPoint->worldPosition - below->worldPosition).Length();
				below->precedingPoint = current;
				below->spriteComponent->Tint = Color(0, 0, 255, 255);
				m_ProcessedCount++;

				int i = 0;
				while (i < priorityQueue.size() && below->heuristic < priorityQueue[i]->heuristic)
					i++;
				priorityQueue.insert(priorityQueue.begin() + i, below);
			}
		}
	}

	startPoint->spriteComponent->Tint = Color(0, 255, 255, 255);
	endPoint->spriteComponent->Tint = Color(0, 255, 255, 255);

	VX_LOG("Points processed: " + std::to_string(m_ProcessedCount));
	VX_LOG("Points evaluated: " + std::to_string(m_EvaluatedCount));
	VX_LOG("Points in path: " + std::to_string(m_PathCount));
	VX_LOG("Points processed but not evaluated: " + std::to_string(m_ProcessedCount - m_EvaluatedCount));
	VX_LOG("Points evaluated but not in path: " + std::to_string(m_EvaluatedCount - m_PathCount));
}

void Pathfinder::AStar(const Vector& startPos, const Vector& endPos)
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

	std::vector<PathPoint*> priorityQueue;
	priorityQueue.push_back(startPoint);
	startPoint->processed = true;
	m_ProcessedCount++;

	while (priorityQueue.size() > 0)
	{
		// Pop a point off the stack
		PathPoint* current = priorityQueue.back();
		priorityQueue.pop_back();
		m_EvaluatedCount++;

		current->spriteComponent->Tint = Color(255, 255, 0, 255);

		// Check if we've reached our destination
		if (current == endPoint)
		{
			// Backtrack through all our preceding points and paint our path green
			current->spriteComponent->Tint = Color(0, 255, 0, 255);
			m_PathCount++;
			while (current->precedingPoint != nullptr)
			{
				current = current->precedingPoint;
				current->spriteComponent->Tint = Color(0, 255, 0, 255);
				m_PathCount++;
			}
			priorityQueue.clear();
		}
		else
		{
			// Handle its surrounding points
			if (PathPoint* above = GetAbove(current))
			{
				above->processed = true;
				above->depth = current->depth + 1;
				above->heuristic = (endPoint->worldPosition - above->worldPosition).Length();
				above->precedingPoint = current;
				above->spriteComponent->Tint = Color(0, 0, 255, 255);
				m_ProcessedCount++;

				int i = 0;
				while (i < priorityQueue.size() && (above->heuristic + above->depth) < (priorityQueue[i]->heuristic + priorityQueue[i]->depth))
					i++;
				priorityQueue.insert(priorityQueue.begin() + i, above);
			}
			if (PathPoint* left = GetLeft(current))
			{
				left->processed = true;
				left->depth = current->depth + 1;
				left->heuristic = (endPoint->worldPosition - left->worldPosition).Length();
				left->precedingPoint = current;
				left->spriteComponent->Tint = Color(0, 0, 255, 255);
				m_ProcessedCount++;

				int i = 0;
				while (i < priorityQueue.size() && (left->heuristic + left->depth) < (priorityQueue[i]->heuristic + priorityQueue[i]->depth))
					i++;
				priorityQueue.insert(priorityQueue.begin() + i, left);
			}
			if (PathPoint* right = GetRight(current))
			{
				right->processed = true;
				right->depth = current->depth + 1;
				right->heuristic = (endPoint->worldPosition - right->worldPosition).Length();
				right->precedingPoint = current;
				right->spriteComponent->Tint = Color(0, 0, 255, 255);
				m_ProcessedCount++;

				int i = 0;
				while (i < priorityQueue.size() && (right->heuristic + right->depth) < (priorityQueue[i]->heuristic + priorityQueue[i]->depth))
					i++;
				priorityQueue.insert(priorityQueue.begin() + i, right);
			}
			if (PathPoint* below = GetBelow(current))
			{
				below->processed = true;
				below->depth = current->depth + 1;
				below->heuristic = (endPoint->worldPosition - below->worldPosition).Length();
				below->precedingPoint = current;
				below->spriteComponent->Tint = Color(0, 0, 255, 255);
				m_ProcessedCount++;

				int i = 0;
				while (i < priorityQueue.size() && (below->heuristic + below->depth) < (priorityQueue[i]->heuristic + priorityQueue[i]->depth))
					i++;
				priorityQueue.insert(priorityQueue.begin() + i, below);
			}
		}
	}

	startPoint->spriteComponent->Tint = Color(0, 255, 255, 255);
	endPoint->spriteComponent->Tint = Color(0, 255, 255, 255);

	VX_LOG("Points processed: " + std::to_string(m_ProcessedCount));
	VX_LOG("Points evaluated: " + std::to_string(m_EvaluatedCount));
	VX_LOG("Points in path: " + std::to_string(m_PathCount));
	VX_LOG("Points processed but not evaluated: " + std::to_string(m_ProcessedCount - m_EvaluatedCount));
	VX_LOG("Points evaluated but not in path: " + std::to_string(m_EvaluatedCount - m_PathCount));
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

		float currentDist = (m_Points[i].worldPosition - position).Length();
		if (currentDist < closestDist)
		{
			point = &(m_Points[i]);
			closestDist = currentDist;
		}
	}

	return point;
}

Pathfinder::PathPoint* Pathfinder::GetPathPoint(int x, int y)
{
	// Check out of bounds
	if (x < 0 || x >= m_XPointCount || y < 0 || y >= m_YPointCount)
		return nullptr;

	PathPoint* point = &(m_Points[x + (y * m_XPointCount)]);

	// Check collision and process availability
	if (point->colliding || point->processed)
		return nullptr;

	return point;
}

Pathfinder::PathPoint* Pathfinder::GetAbove(PathPoint* point)
{
	return GetPathPoint(point->xInd, point->yInd + 1);
}

Pathfinder::PathPoint* Pathfinder::GetBelow(PathPoint* point)
{
	return GetPathPoint(point->xInd, point->yInd - 1);
}

Pathfinder::PathPoint* Pathfinder::GetLeft(PathPoint* point)
{
	return GetPathPoint(point->xInd - 1, point->yInd);
}

Pathfinder::PathPoint* Pathfinder::GetRight(PathPoint* point)
{
	return GetPathPoint(point->xInd + 1, point->yInd);
}
