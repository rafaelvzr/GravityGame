#pragma once
#include"renderer.h"
#include"VertexBufferLayout.h"
#include"Texture.h"
#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>
#include"imgui.h"

#define PI 3.14159265359 
class Sphere
{
private:
	std::vector<float> vertices;
	std::vector<unsigned> indices;
	std::vector<unsigned> lineIndices;
	std::vector<float> normals;
	std::vector<float> texCoords;
	float radius;
	unsigned sectors;
	unsigned stacks;
	VertexArray* va;
	VertexBuffer* vb;
	IndexBuffer* ib;
	Shader* shader;
	Shader* linesShader;
	Texture* texture;
	Renderer renderer;
	glm::vec3 center;
	glm::mat4 model;
	const glm::mat4 &vp;
	int upAxis; //X=1, Y=2, or Z=3
public:
	Sphere(const glm::mat4& vp, float radius = 1, unsigned sectors = 36, unsigned stacks = 18, int upAxis = 3);
	~Sphere();
	void draw();
	void drawLines();
	void update();
	inline std::vector<float> getVertices()		const { return vertices; }
	inline std::vector<unsigned> getIndices()	const { return indices; }
	inline glm::vec3 getCenter()				const { return center; }
	std::vector<float> getIntercalatedVerticesAndTexCoords() const;
	void rotateLeft();
	void rotateRight();
private:
	void changeUpAxis(int from, int to);
};