#include "Sphere.h"

Sphere::Sphere(const glm::mat4& vp,float radius, unsigned sectors, unsigned stacks, int upAxis)
	: radius(radius), sectors(sectors), stacks(stacks), vp(vp), upAxis(upAxis)
{
	std::vector<float>().swap(vertices);
    std::vector<float>().swap(normals);
    std::vector<float>().swap(texCoords);

    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
    float s, t;                                     // vertex texCoord

    float sectorStep = 2 * PI / sectors;
    float stackStep = PI / stacks;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stacks; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectors+1) vertices per stack
        // first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= sectors; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // normalized vertex normal (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);

            // vertex tex coord (s, t) range between [0, 1]
            s = (float)j / sectors;
            t = 1.0f-(float)i / stacks;
            texCoords.push_back(s);
            texCoords.push_back(t);
        }
    }

    int k1, k2;
    for (int i = 0; i < stacks; ++i)
    {
        k1 = i * (sectors + 1);     // beginning of current stack
        k2 = k1 + sectors + 1;      // beginning of next stack

        for (int j = 0; j < sectors; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if (i != (stacks - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }

            // store indices for lines
            // vertical lines for all stacks, k1 => k2
            lineIndices.push_back(k1);
            lineIndices.push_back(k2);
            if (i != 0)  // horizontal lines except 1st stack, k1 => k+1
            {
                lineIndices.push_back(k1);
                lineIndices.push_back(k1 + 1);
            }
        }
    }
	if (upAxis != 3)
		changeUpAxis(3, upAxis);
    std::vector<float> intercalated = getIntercalatedVerticesAndTexCoords();
	model = glm::mat4(1.0f);
	center = glm::vec3(0.0f, 0.0f, 0.0f);
	va = new VertexArray();
	vb = new VertexBuffer(true, intercalated.data(), intercalated.size() * sizeof(float));
	ib = new IndexBuffer(indices.data(), indices.size());
	VertexBufferLayout layout;
	layout.Push<float>(3);
    layout.Push<float>(2);
    va->addBuffer(*vb, layout);
    shader = new Shader(SHADERS_PATH"/sphere.shader");
    texture = new Texture(TEXTURES_PATH"/moon.png");
    
    shader->Bind();
	texture->Bind();
	shader->setUniform1i("u_Texture", 0);
    shader->setUniform3f("ballCenter", 0.0f, 0.0f, 0.0f);
    shader->setUniformMat4f("model", model);
    shader->setUniformMat4f("viewProjection", vp);
    linesShader = new Shader(SHADERS_PATH"/linesShader.shader");
    linesShader->Bind();
    linesShader->setUniform3f("ballCenter", 0.0f, 0.0f, 0.0f);
    linesShader->setUniformMat4f("model", model);
    linesShader->setUniformMat4f("viewProjection", vp);

}

Sphere::~Sphere()
{
	delete va;
	delete vb;
	delete ib;
	delete shader;
	delete texture;
}

void Sphere::draw()
{
    shader->Bind();
    if (texture)
    {
        texture->Bind();
    }
	renderer.DrawTriangles(*va, *ib, *shader);
}

void Sphere::drawLines()
{
	shader->Bind();
	renderer.DrawLines(*va, *ib, *linesShader);
}

void Sphere::update()
{
	shader->Bind();
    texture->Bind();
    shader->setUniform3f("ballCenter", 0.0f, 0.0f, 0.0f);
    shader->setUniformMat4f("model", model);
    shader->setUniformMat4f("viewProjection", vp);
    shader->setUniform1i("u_Texture", 0);
    linesShader->Bind();
    linesShader->setUniform3f("ballCenter", 0.0f, 0.0f, 0.0f);
    linesShader->setUniformMat4f("model", model);
    linesShader->setUniformMat4f("viewProjection", vp);
}

std::vector<float> Sphere::getIntercalatedVerticesAndTexCoords() const {
    std::vector<float> intercalated;
    intercalated.reserve(vertices.size() + texCoords.size());

    size_t vertexCount = vertices.size() / 3;
    size_t texCoordCount = texCoords.size() / 2;
    size_t count = std::min(vertexCount, texCoordCount);

    for (size_t i = 0; i < count; ++i) {
        intercalated.push_back(vertices[i * 3]);
        intercalated.push_back(vertices[i * 3 + 1]);
        intercalated.push_back(vertices[i * 3 + 2]);
        intercalated.push_back(texCoords[i * 2]);
        intercalated.push_back(texCoords[i * 2 + 1]);
    }

    // If there are remaining vertices or texCoords, append them
    for (size_t i = count; i < vertexCount; ++i) {
        intercalated.push_back(vertices[i * 3]);
        intercalated.push_back(vertices[i * 3 + 1]);
        intercalated.push_back(vertices[i * 3 + 2]);
    }

    for (size_t i = count; i < texCoordCount; ++i) {
        intercalated.push_back(texCoords[i * 2]);
        intercalated.push_back(texCoords[i * 2 + 1]);
    }

    return intercalated;
}

void Sphere::rotateLeft()
{
	model = glm::rotate(model, glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Sphere::rotateRight()
{
	model = glm::rotate(model, glm::radians(-0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Sphere::changeUpAxis(int from, int to)
{
    // initial transform matrix cols
    float tx[] = { 1.0f, 0.0f, 0.0f };    // x-axis (left)
    float ty[] = { 0.0f, 1.0f, 0.0f };    // y-axis (up)
    float tz[] = { 0.0f, 0.0f, 1.0f };    // z-axis (forward)

    // X -> Y
    if (from == 1 && to == 2)
    {
        tx[0] = 0.0f; tx[1] = 1.0f;
        ty[0] = -1.0f; ty[1] = 0.0f;
    }
    // X -> Z
    else if (from == 1 && to == 3)
    {
        tx[0] = 0.0f; tx[2] = 1.0f;
        tz[0] = -1.0f; tz[2] = 0.0f;
    }
    // Y -> X
    else if (from == 2 && to == 1)
    {
        tx[0] = 0.0f; tx[1] = -1.0f;
        ty[0] = 1.0f; ty[1] = 0.0f;
    }
    // Y -> Z
    else if (from == 2 && to == 3)
    {
        ty[1] = 0.0f; ty[2] = 1.0f;
        tz[1] = -1.0f; tz[2] = 0.0f;
    }
    //  Z -> X
    else if (from == 3 && to == 1)
    {
        tx[0] = 0.0f; tx[2] = -1.0f;
        tz[0] = 1.0f; tz[2] = 0.0f;
    }
    // Z -> Y
    else
    {
        ty[1] = 0.0f; ty[2] = -1.0f;
        tz[1] = 1.0f; tz[2] = 0.0f;
    }

    std::size_t i, j;
    std::size_t count = vertices.size();
    float vx, vy, vz;
    float nx, ny, nz;
    for (i = 0, j = 0; i < count; i += 3, j += 8)
    {
        // transform vertices
        vx = vertices[i];
        vy = vertices[i + 1];
        vz = vertices[i + 2];
        vertices[i] = tx[0] * vx + ty[0] * vy + tz[0] * vz;   // x
        vertices[i + 1] = tx[1] * vx + ty[1] * vy + tz[1] * vz;   // y
        vertices[i + 2] = tx[2] * vx + ty[2] * vy + tz[2] * vz;   // z

        // transform normals
        nx = normals[i];
        ny = normals[i + 1];
        nz = normals[i + 2];
        normals[i] = tx[0] * nx + ty[0] * ny + tz[0] * nz;   // nx
        normals[i + 1] = tx[1] * nx + ty[1] * ny + tz[1] * nz;   // ny
        normals[i + 2] = tx[2] * nx + ty[2] * ny + tz[2] * nz;   // nz
    }
}

