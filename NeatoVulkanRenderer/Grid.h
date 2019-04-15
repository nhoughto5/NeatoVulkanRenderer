#pragma once
class Grid
{
public:
	Grid();
	~Grid();
	std::vector<glm::vec3> getVertexData();
	glm::vec3 getGridColour();
private:
	int const m_Size { 10 };
	std::vector<glm::vec3> gridData;
};

