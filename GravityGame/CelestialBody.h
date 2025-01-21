#pragma once
#include <Sphere.h>
#include <Camera.h> 

#define GRAVITY_CONSTANT 1.0f

namespace Gravity 
{
	class Marker
	{
	private:
		glm::vec3 position;
		glm::mat4 model;
		const glm::mat4& viewProjection;
		std::vector<float> vertices;
		VertexArray* va;
		VertexBuffer* vb;
		Shader* shader;
	public:
		Marker(const Camera&c, float x = 0.0f, float y = 0.0f, float z = 0.0f);
		glm::vec3& getPosition() { return position; }
		void moveXAxis(float d, float deltaTime) { position.x += d * deltaTime; }
		void moveYAxis(float d, float deltaTime) { position.y += d * deltaTime; }
		void moveZAxis(float d, float deltaTime) { position.z += d * deltaTime; }
		void update();
		void draw() const;
	};

	class Grid
	{
	private:
		std::vector<float> vertices;
		VertexArray* va;
		VertexBuffer* vb;
		Shader* shader;
		const glm::mat4& viewProjection;
	public:
		Grid(const Camera& c);
		void draw() const;
		void update();
	};

	class CelestialBody
	{
	protected:
		glm::vec3 position;
		glm::vec3 velocity;
		glm::vec3 acceleration;
		bool movable;
		float mass;
	public:
		CelestialBody(float x, float y, float z, float mass, bool movable);
		CelestialBody(glm::vec3 position, float mass, bool movable);
		virtual void update(float deltaTime);
		virtual void updateVP() = 0;
		virtual void draw() = 0;
		void getAttracted(CelestialBody& other);
		bool getMovable() const;
		inline const glm::vec3& getAcceleration() const { return acceleration; } //debug
		inline const glm::vec3& getPosition() const { return position; } //debug
		inline const glm::vec3& getVelocity() const { return velocity; } //debug
		virtual void setVelocity(float xv, float yv, float zv) { velocity = glm::vec3(xv, yv, zv); } //debug purposes
	};

	class Star : public CelestialBody
	{
	private:
		float radius;
		Sphere sphere;
	public:
		Star(float x, float y, float z, float radius, float mass,bool movable, const Camera& c, std::string shader);
		Star(glm::vec3 position, float radius, float mass, bool movable, const Camera& c, std::string shader);
		void draw() override;
		void update(float deltaTime) override;
		void updateVP() override;
		void setVelocity(float xv, float yv, float zv) override { velocity = glm::vec3(xv, yv, zv); } //debug purposes
	};

	class Planet : public CelestialBody
	{
	private:
		float radius;
		Sphere sphere;
	public:
		Planet(float x, float y, float z, float radius, float mass,bool movable, const Camera& c);
		Planet(glm::vec3 position, float radius, float mass, bool movable, const Camera& c);
		void draw() override;
		void update(float deltaTime) override;
		void updateVP() override;
		void setVelocity(float xv, float yv, float zv) override { velocity = glm::vec3(xv, yv, zv); } //debug purposes

	};

	class BlackHole : public CelestialBody
	{
	private:
		float radius;
		Sphere sphere;
	public:
		void draw() override;
	};

	class Portal  //portal to end the level
	{
	private:
		float radius;
		std::vector<float> vertices;
		std::vector<unsigned> indices;
		VertexArray* va;
		VertexBuffer* vb;
		IndexBuffer* ib;
		Shader* shader;
		const glm::mat4& view;
		const glm::mat4& proj;
		glm::vec3 center;
	public:
		Portal(const Camera& camera, float radius, float xc = 120.0f, float yc = 120.0f, float zc = 120.0f);
		void draw();
		void update(float deltaTime);
		void setPosition(float x, float y, float z);
		const glm::vec3& getCenter() const { return center; }
		float getRadius() const { return radius; }
	};

}
