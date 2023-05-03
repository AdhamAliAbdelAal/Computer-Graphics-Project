#pragma once

#include "../ecs/world.hpp"
#include "../components/movement.hpp"
#include "../ecs/entity.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>
#include <deque>
#include <vector>
#include <algorithm>

using namespace std;

namespace our
{
    bool cmp(Entity *a, Entity *b)
    {
        return a->localTransform.position.z > b->localTransform.position.z;
    }
    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/components/movement.hpp"
    class RoadRepeaterSystem
    {
        deque<Entity *> dq;

    public:
        // this is the data for the coin
        // to hold the data of the coin read from app config to use it later in the update function

        RoadRepeaterSystem(World *world)
        {
            const unordered_set<Entity *> entities = world->getEntities();
            // vector<Entity *> v;
            for (auto it : entities)
            {
                // if the entity has a movement component
                if (it->name == "road")
                {
                    dq.push_back(it);
                }
            }
            sort(dq.begin(), dq.end(), cmp);
        }
        // This should be called every frame to update all entities containing a MovementComponent.
        void update(World *world, float deltaTime)
        {
            if (dq.empty())
                return;
            Entity *road1 = dq.front();
            cout << "deque size : " << dq.front()->localTransform.position.z << '\n';
            if (road1->localTransform.position.z > 15)
            {
                cout << "3adda\n";
                dq.pop_front();
                if (dq.empty())
                    return;
                Entity *road3 = dq.back();
                road1->localTransform.position.z = road3->localTransform.position.z - 14;
                dq.push_back(road1);
            }
        }
    };

}
