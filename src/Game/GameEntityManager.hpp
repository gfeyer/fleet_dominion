#ifndef GAME_ENTITY_MANAGER_HPP
#define GAME_ENTITY_MANAGER_HPP

#include <vector>
#include <algorithm>

#include "Core/EntityManager.hpp"
#include "Components/FactoryComponent.hpp"
#include "Components/DroneComponent.hpp"
#include "Components/ShieldComponent.hpp"

namespace Game {

    class GameEntityManager {
    private:
        EntityManager coreManager; // Composition: EntityManager instance

        // Game-specific filtered lists
        std::vector<EntityID> factoryEntities;
        std::vector<EntityID> shieldEntities;
        std::vector<EntityID> droneEntities;

    public:
        // Create a new entity
        EntityID createEntity() {
            return coreManager.createEntity();
        }

        // Default constructor
        GameEntityManager() = default;

        // Prevent copy
        GameEntityManager(const GameEntityManager&) = delete;
        GameEntityManager& operator=(const GameEntityManager&) = delete;

        // Remove an entity
        void removeEntity(EntityID id) {
            if (coreManager.hasEntity(id)) {
                auto& entity = coreManager.getEntity(id);

                if (entity.hasComponent<Components::FactoryComponent>()) {
                    factoryEntities.erase(std::remove(factoryEntities.begin(), factoryEntities.end(), id), factoryEntities.end());
                }
                if (entity.hasComponent<Components::ShieldComponent>()) {
                    shieldEntities.erase(std::remove(shieldEntities.begin(), shieldEntities.end(), id), shieldEntities.end());
                }
                if (entity.hasComponent<Components::DroneComponent>()) {
                    droneEntities.erase(std::remove(droneEntities.begin(), droneEntities.end(), id), droneEntities.end());
                }

                coreManager.removeEntity(id);
            }
        }

        // Add a component and update lists
        template<typename T>
        void addComponent(EntityID id, T component) {
            coreManager.addComponent<T>(id, component);

            if constexpr (std::is_same<T, Components::FactoryComponent>::value) {
                factoryEntities.push_back(id);
            }
            if constexpr (std::is_same<T, Components::ShieldComponent>::value) {
                shieldEntities.push_back(id);
            }
            if constexpr (std::is_same<T, Components::DroneComponent>::value) {
                droneEntities.push_back(id);
            }
        }

        // Get game-specific entity lists
        const std::vector<EntityID>& getFactories() const {
            return factoryEntities;
        }
        const std::vector<EntityID>& getShields() const {
            return shieldEntities;
        }
        const std::vector<EntityID>& getDrones() const {
            return droneEntities;
        }


        template<typename T>
        T* getComponent(EntityID id) {
            if (coreManager.hasEntity(id)) {
                return coreManager.getEntity(id).getComponent<T>();
            }
            return nullptr;
        }

        // Remove a component and update lists
        template<typename T>
        void removeComponent(EntityID id) {
            coreManager.removeComponent<T>(id);

            if constexpr (std::is_same<T, Components::FactoryComponent>::value) {
                factoryEntities.erase(std::remove(factoryEntities.begin(), factoryEntities.end(), id), factoryEntities.end());
            }
            if constexpr (std::is_same<T, Components::DroneComponent>::value) {
                droneEntities.erase(std::remove(droneEntities.begin(), droneEntities.end(), id), droneEntities.end());
            }
        }

        // Access a specific entity
        Entity& getEntity(EntityID id) {
            return coreManager.getEntity(id);
        }

        // Access all entities read-only
        const std::unordered_map<EntityID, Entity>& getAllEntities() const {
            return coreManager.getAllEntities();
        }

        // Read-Write Access
        std::unordered_map<EntityID, Entity>& getAllEntities() {
            return coreManager.getAllEntities();
        }

        // Get all Entity IDs
        std::vector<EntityID> getAllEntityIDs() const {
            std::vector<EntityID> entityIDs;
            const auto& allEntities = coreManager.getAllEntities();

            entityIDs.reserve(allEntities.size()); // Reserve space for efficiency

            for (const auto& [id, entity] : allEntities) {
                entityIDs.push_back(id);
            }

            return entityIDs;
        }
    };
}

#endif // GAME_ENTITY_MANAGER_HPP
