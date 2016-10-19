#pragma once
#include <glm\glm.hpp>
#include <memory>
#include "Texture.h"
#include "Mesh.h"

// Temporary? I can probably think of a better solution
static unsigned int EntityIDCounter = 0;

class Entity {
public:
	Entity();
	Entity(std::shared_ptr<Mesh> _mesh, std::shared_ptr<Texture> _texture);
	~Entity();

	void Init(std::shared_ptr<Mesh> _mesh, std::shared_ptr<Texture> _texture);
	void Draw();

	void Translate(glm::vec3 _vec);
	void Rotate(float _rot, glm::vec3 _axis);
	void Scale(float _fac);
	void Scale(glm::vec3 _fac);

	glm::mat4 GetTransform();

	bool operator==(const Entity& _other);

	float _Width = 0.0f;
private:
	unsigned int EntityID = EntityIDCounter++;
	std::shared_ptr<Mesh> _Mesh = nullptr;
	std::shared_ptr<Texture> _Texture = nullptr;
	glm::mat4 _Transform = glm::mat4(1.0f);

};

