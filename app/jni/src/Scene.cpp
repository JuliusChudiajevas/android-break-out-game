#include "Scene.hpp"

#include "Components.hpp"
#include "Entity.hpp"
#include "entt.hpp"

Scene::Scene() {
}

Scene::~Scene() {
}

Entity Scene::createEntity() {
  Entity entity(registry.create(), this);
  return entity;
}
