#pragma once
#include <vector>
#include <memory>
#include "Entity.h"
#include "ShaderProgram.h"

class Scene
{
public:
	Scene();
	~Scene();

	void Init(std::shared_ptr<ShaderProgram> _sp);
	void Draw();
	void Update(float deltaTime);

	void AddGameObject(Entity _gameObject);
	void RemoveGameObject(Entity _gameObject);

private:
	std::shared_ptr<ShaderProgram> _ShaderProgram;
	std::vector<Entity> _GameObjects;
};

