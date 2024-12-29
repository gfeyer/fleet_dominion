#ifndef ENEMY_AI_COMPONENT_HPP
#define ENEMY_AI_COMPONENT_HPP

namespace Components {
    struct EnemyAIComponent {
        float decisionTimer = 0.f;
        float decisionTimerMax = 1.f; // decide every second
        EntityID highlightedEntityID = 0;
    };
}

#endif // ENEMY_AI_COMPONENT_HPP