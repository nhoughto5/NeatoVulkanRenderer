#include "nvrpch.h"
#include "Grid.h"

Grid::Grid()
{
}

Grid::~Grid()
{
}

std::vector<glm::vec3> Grid::getVertexData()
{
	for (int x = 0; x < m_Size; ++x) {
		for (int y = 0; y < m_Size; ++y) {
			gridData.push_back(glm::vec3(m_Size, 0, m_Size));
		}
	}
	return gridData;
}

glm::vec3 Grid::getGridColour()
{
	return glm::vec3(1.0f, 0.0f, 0.0f);
}