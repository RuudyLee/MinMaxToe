#include "Entity.h"
#include <glm\gtc\matrix_transform.hpp>

Entity::Entity() {
	_Mesh = nullptr;
	_Texture = nullptr;
}

Entity::Entity(std::shared_ptr<Mesh> _mesh, std::shared_ptr<Texture> _texture)
: _Mesh(_mesh)
, _Texture(_texture)
{}

Entity::~Entity() {
}

void Entity::Init(std::shared_ptr<Mesh> _mesh, std::shared_ptr<Texture> _texture) {
	_Mesh = _mesh;
	_Texture = _texture;

	_Width = _mesh->_Width;
}

void Entity::Draw() {
	_Texture->Bind();
	glBindVertexArray(_Mesh->GetVAO());
	glDrawArrays(GL_TRIANGLES, 0, _Mesh->GetNumVertices());
	_Mesh->Unbind();
	_Texture->Unbind();
}

void Entity::Translate(glm::vec3 _vec) {
	_Transform = glm::translate(_Transform, _vec);
}

void Entity::Rotate(float _rot, glm::vec3 _axis) {
	_Transform = glm::rotate(_Transform, _rot, _axis);
}

void Entity::Scale(float _fac) {
	_Transform = glm::scale(_Transform, glm::vec3(_fac, _fac, _fac));
}

void Entity::Scale(glm::vec3 _fac) {
	_Transform = glm::scale(_Transform, _fac);
}

glm::mat4 Entity::GetTransform() {
	return _Transform;
}

bool Entity::operator==(const Entity& _other) {
	return EntityID == _other.EntityID;
}