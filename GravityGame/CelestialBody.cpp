#include<CelestialBody.h>


namespace Gravity
{
	Marker::Marker(const Camera &c, float x, float y, float z)
		: position(x,y,z), model(1.0f), viewProjection(c.getViewProj())
	{
		model = glm::translate(model, position);
		float size = 4.0f;
		vertices = {
			-size, 0.0f, 0.0f,
			size, 0.0f, 0.0f,
			0.0f, -size, 0.0f,
			0.0f, size, 0.0f,
			0.0f, 0.0f, -size,
			0.0f, 0.0f, size
		};
		va = new VertexArray();
		vb = new VertexBuffer(true, vertices.data(), vertices.size() * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		va->addBuffer(*vb, layout);
		shader = new Shader(SHADERS_PATH"/marker.shader");
		shader->Bind();
		shader->setUniformMat4f("model", model);
		shader->setUniformMat4f("viewProjection", viewProjection);
	}
	void Marker::update()
	{
		model = glm::translate(glm::mat4(1.0f), position);
		shader->Bind();
		shader->setUniformMat4f("model", model);
		shader->setUniformMat4f("viewProjection", viewProjection);
	}
	void Marker::draw() const
	{
		static Renderer renderer;
		renderer.DrawLines(*va,0, 6, *shader);
	}

	Grid::Grid(const Camera& c)
		: viewProjection(c.getViewProj())
	{
		const float size = 120.0f;
		const float step = 60.0f;
		for (float i = 0.0f; i <= 2.0f * size; i += step)
		{
			for (float j = 0.0f; j <= 2.0f * size; j += step)
			{
				vertices.push_back(size - j);
				vertices.push_back(size);
				vertices.push_back(size - i);
				vertices.push_back(size - j);
				vertices.push_back(-size);
				vertices.push_back(size - i);
			}
		}
		for (float i = 0.0f; i <= 2.0f * size; i += step)
		{
			for (float j = 0.0f; j <= 2.0f * size; j += step)
			{
				vertices.push_back(size);
				vertices.push_back(size - j);
				vertices.push_back(size - i);
				vertices.push_back(-size);
				vertices.push_back(size - j);
				vertices.push_back(size - i);
			}
		}
		for (float i = 0.0f; i <= 2.0f * size; i += step)
		{
			for (float j = 0.0f; j <= 2.0f * size; j += step)
			{
				vertices.push_back(size - i);
				vertices.push_back(size - j);
				vertices.push_back(size);
				vertices.push_back(size - i);
				vertices.push_back(size - j);
				vertices.push_back(-size);
			}
		}
		va = new VertexArray();
		vb = new VertexBuffer(true, vertices.data(), vertices.size() * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		va->addBuffer(*vb, layout);
		shader = new Shader(SHADERS_PATH"/grid.shader");
		shader->Bind();
		shader->setUniformMat4f("viewProjection", viewProjection);
	}

	void Grid::draw() const
	{
		static Renderer renderer;
		renderer.DrawLines(*va, 0, vertices.size() / 3, *shader);
	}

	void Grid::update()
	{
		shader->Bind();
		shader->setUniformMat4f("viewProjection", viewProjection);
	}





	CelestialBody::CelestialBody(float x, float y, float z, float mass, bool movable)
		: position(x, y, z), velocity(0.0f), acceleration(0.0f), mass(mass), movable(movable)
	{
	}
	CelestialBody::CelestialBody(glm::vec3 position, float mass, bool movable)
		: CelestialBody(position.x, position.y, position.z, mass, movable)
	{
	}

	void CelestialBody::update(float deltaTime)
	{
		velocity += acceleration * deltaTime;
		position += velocity * deltaTime;
	}

	void CelestialBody::getAttracted(CelestialBody& other)
	{
		float softneningFactor = 0.1f;
		float distance = glm::distance(position, other.position);
		float force = GRAVITY_CONSTANT * mass * other.mass / (distance * distance + softneningFactor*softneningFactor);
		glm::vec3 direction = other.position - position;
		acceleration = direction * force / mass;
	}

	bool CelestialBody::getMovable() const
	{
		return movable;
	}

	Star::Star(float x, float y, float z, float radius, float mass, bool movable, const Camera& c, std::string shader)
		: CelestialBody(x, y, z, mass, movable), radius(radius), sphere(c.getViewProj(), x, y, z, radius, shader)
	{
	}

	Star::Star(glm::vec3 position, float radius, float mass, bool movable, const Camera& c, std::string shader)
		: Star(position.x, position.y, position.z, radius, mass,movable, c,shader)
	{	

	}

	void Star::draw()
	{
		sphere.draw();
	}

	void Star::update(float deltaTime)
	{
		static float currentFrame = 0.0f;
		sphere.setUniform1f("iTime", currentFrame);
		velocity += acceleration * deltaTime;
		position += velocity * deltaTime;
		if (position.x >= 120.0f)
			position.x = -position.x;
		if (position.x <= -120.0f)
			position.x = -position.x;
		if (position.y >= 120.0f)
			position.y = -position.y;
		if (position.y <= -120.0f)
			position.y = -position.y;
		if (position.z >= 120.0f)
			position.z = -position.z;
		if (position.z <= -120.0f)
			position.z = -position.z;
		sphere.setCenter(position);
		currentFrame += deltaTime;
	}

	void Star::updateVP()
	{
		sphere.updatePosition();
		sphere.update();
	}

	Planet::Planet(float x, float y, float z, float radius, float mass, bool movable,const Camera& c)
		: CelestialBody(x, y, z, mass, movable), radius(radius), 
		sphere(c.getViewProj(), x, y, z, radius, std::string("/sphere.shader"), std::string("/earth2.png"),36,18,2)
	{
		sphere.setUniform3f("lightColor", 1.0f, 1.0f, 1.0f);
		sphere.setUniform3f("lightPos", 10.0f, 10.0f, 10.0f);
	}

	Planet::Planet(glm::vec3 position, float radius, float mass, bool movable, const Camera& c)
		: Planet(position.x, position.y, position.z, radius, mass,movable, c)
	{
		sphere.setUniform3f("lightColor", 1.0f, 1.0f, 1.0f);
		sphere.setUniform3f("lightPos", 10.0f, 10.0f, 10.0f);
	}

	void Planet::draw()
	{
		sphere.draw();
	}


	void Planet::update(float deltaTime)
	{
		velocity += acceleration * deltaTime;
		position += velocity * deltaTime;
		if (position.x > 120.0f || position.x < -120.0f)
			position.x = -position.x;
		if (position.y > 120.0f || position.y < -120.0f)
			position.y = -position.y;
		if (position.z > 120.0f || position.z < -120.0f)
			position.z = -position.z;
		sphere.setCenter(position);
	}

	void Planet::updateVP()
	{
		sphere.updatePosition();
		sphere.update();
	}


	void BlackHole::draw()
	{
		sphere.draw();
	}



	Portal::Portal(const Camera& camera, float radius, float xc, float yc, float zc)
		: view(camera.getView()),proj(camera.getProj()), radius(radius), center(xc,yc,zc)
	{
		vertices = {
			-radius,	-radius,
			 radius,	-radius,
			 radius,	radius,
			-radius,	radius
		};
		indices = {
			3,0,1,
			1,2,3
		};
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, center);
		va = new VertexArray();
		vb = new VertexBuffer(true, vertices.data(), vertices.size() * sizeof(float));
		VertexBufferLayout layout;
		ib = new IndexBuffer(indices.data(), indices.size());
		layout.Push<float>(2);
		va->addBuffer(*vb, layout);
		shader = new Shader(SHADERS_PATH"/portal2.shader");
		shader->Bind();
		shader->setUniformMat4f("view", view);
		shader->setUniformMat4f("projection", proj);
		shader->setUniformMat4f("model", model);
	}

	void Portal::draw()
	{
		static Renderer renderer;
		renderer.DrawTriangles(*va, *ib, *shader);
	}

	void Portal::update(float deltaTime)
	{
		static float currentFrame = 0.0f;
		shader->Bind();
		shader->setUniform1f("iTime", currentFrame);
		shader->setUniformMat4f("view", view);
		shader->setUniformMat4f("projection", proj);
		currentFrame += deltaTime;
	}

	void Portal::setPosition(float x, float y, float z)
	{
		center = glm::vec3(x, y, z);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, center);
		shader->Bind();
		shader->setUniformMat4f("model", model);
	}

}
