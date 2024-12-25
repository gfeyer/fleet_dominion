#ifndef SCENE_HPP
#define SCENE_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

#include "entity.hpp"

class EntityOld;
class Scene{
private:
    std::unordered_map<int, Entity> entityManager;

public:
    Scene();
    ~Scene();   
    void update(float dt);
    void render(sf::RenderWindow& window);
    void handleInput(sf::Event& event);
};

#endif // SCENE_HPP