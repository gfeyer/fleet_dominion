#include "scene.hpp"

#include "util/logger.hpp"
#include "resource.hpp"
#include "config.hpp"
#include "config.hpp"
#include "components.hpp"
#include "entity.hpp"
#include "systems.hpp"

Scene::Scene()
{
    log_info << "Creating Scene";

    // Create Background
    Entity background;

    auto& texture = Resource::Manager::getInstance().getTexture(Resource::Paths::BACKGROUND_4);
    background.addComponent(SpriteComponent{texture});
    sf::Vector2u windowSize = sf::Vector2u(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT);
    sf::Vector2u textureSize = texture.getSize();
    background.addComponent(
        TransformComponent{
            sf::Vector2f(0, 0),
            0.f,
            sf::Vector2f(
                static_cast<float>(windowSize.x) / textureSize.x,
                static_cast<float>(windowSize.y) / textureSize.y
            )
        }
    );
    // entityManager.emplace(background.id, std::move(background));

    for(int i=0; i < 6; ++i){
        // Create Factories
        Entity factory; 
        factory.addComponent(FactoryComponent{"Factory_" + std::to_string(i)});
        factory.addComponent(TransformComponent{sf::Vector2f(rand() % Config::SCREEN_WIDTH, rand() % Config::SCREEN_HEIGHT), 0, sf::Vector2f(1, 1)});
        auto shape = sf::RectangleShape({Config::FACTORY_SIZE, Config::FACTORY_SIZE});
        shape.setFillColor(sf::Color::Cyan);
        factory.addComponent(ShapeComponent{std::make_shared<sf::RectangleShape>(shape)});
        factory.addComponent(TextComponent{"Factory #" + std::to_string(i), 
            Resource::Manager::getInstance().getFont(Resource::Paths::FONT_TOXIGENESIS), 
            18, 
            sf::Color::White, 
            sf::Vector2f(Config::FACTORY_SIZE+5, 5)
        });

        entityManager.emplace(factory.id, std::move(factory));
    }

    // Create Outposts
    for(int i=0; i<6; ++i){
        Entity outpost;
        outpost.addComponent(OutpostComponent{"Outpost_" + std::to_string(i)});
        outpost.addComponent(TransformComponent{sf::Vector2f(rand() % Config::SCREEN_WIDTH - Config::OUTPOST_RADIUS, rand() % Config::SCREEN_HEIGHT-Config::OUTPOST_RADIUS), 0, sf::Vector2f(1, 1)});
        auto shape = sf::CircleShape(Config::OUTPOST_RADIUS);
        shape.setFillColor(sf::Color::Green);
        outpost.addComponent(ShapeComponent{std::make_shared<sf::CircleShape>(shape)});
        outpost.addComponent(TextComponent{"Outpost #" + std::to_string(i), 
            Resource::Manager::getInstance().getFont(Resource::Paths::FONT_TOXIGENESIS), 
            18, 
            sf::Color::White, 
            sf::Vector2f(Config::OUTPOST_RADIUS*2, 5)
        });

        entityManager.emplace(outpost.id, std::move(outpost));
    }

    // Create Drones
    for(int i=0; i<12; ++i){
        Entity drone;
        drone.addComponent(DroneComponent{"Drone_" + std::to_string(i)});
        drone.addComponent(TransformComponent{sf::Vector2f(rand() % Config::SCREEN_WIDTH, rand() % Config::SCREEN_HEIGHT), 0, sf::Vector2f(1, 1)});
        
        auto shape = std::make_shared<sf::ConvexShape>();
        shape->setPointCount(3);
        shape->setOrigin(sf::Vector2f(0.f, 0.f));
        shape->setPoint(0, sf::Vector2f(0.f, -Config::DRONE_LENGTH));  // Top point
        shape->setPoint(1, sf::Vector2f(-Config::DRONE_LENGTH, Config::DRONE_LENGTH)); // Bottom-left point
        shape->setPoint(2, sf::Vector2f(Config::DRONE_LENGTH, Config::DRONE_LENGTH));  // Bottom-right point

        shape->setFillColor(sf::Color::Red);
        drone.addComponent(ShapeComponent{shape});
        drone.addComponent(TextComponent{"Drone #" + std::to_string(i), 
            Resource::Manager::getInstance().getFont(Resource::Paths::FONT_TOXIGENESIS), 
            18, 
            sf::Color::White, 
            sf::Vector2f(Config::DRONE_LENGTH*2, 5)
        });

        entityManager.emplace(drone.id, std::move(drone));
    }

}

Scene::~Scene()
{
    log_info << "Destroying Scene";
}

void Scene::update(float dt)
{
    TextUpdateSystem(entityManager, dt);
}

void Scene::render(sf::RenderWindow &window)
{
    // Render background
    // window.draw(this->backgroundSprite);

    // for(auto& entity : entities_old)
    // {
    //     entity->render(window);
    // }

    RenderSystem(entityManager, window);

}

void Scene::handleInput(sf::Event &event)
{
    log_info << "World Input Handled";
    // for(auto& entity : entities_old)
    // {
    //     entity->handleInput(event);
    // }
}
