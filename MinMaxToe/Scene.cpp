#include "Scene.h"
#include <iostream>
#include <algorithm>

Scene::Scene() {
}


Scene::~Scene() {
}

void Scene::Init(std::shared_ptr<ShaderProgram> _sp) {
	_ShaderProgram = _sp;
}

void Scene::Draw() {
	for (Entity *e : _GameObjects) {
		_ShaderProgram->SendUniformMat4("uModel", &e->GetTransform()[0][0], false);
		e->Draw();
	}
}

void Scene::Update(float deltaTime) {
}

void Scene::AddGameObject(Entity *_gameObject) {
	_GameObjects.push_back(_gameObject);
}

void Scene::RemoveGameObject(Entity *_gameObject) {
	std::vector<Entity*>::iterator it = std::find(_GameObjects.begin(), _GameObjects.end(), _gameObject);

	if (it != _GameObjects.end()) {
		std::swap(*it, _GameObjects.back());
		_GameObjects.pop_back();
	}
	else {
		std::cout << "WARNING: No Game Object found in the Scene.\n";
	}
}