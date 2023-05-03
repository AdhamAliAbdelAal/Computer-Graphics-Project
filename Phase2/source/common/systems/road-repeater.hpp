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
    // the road repeater system is responsible for repeating the road
    // This system is added as a simple example for how use the ECS framework to implement logic.
    class RoadRepeaterSystem
    {
        // deque to hold the roads
        deque<Entity *> dq;

    public:

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
            // sorting the roads according to their z position
            sort(dq.begin(), dq.end(), cmp);
        }
        // This should be called every frame to update all entities containing a MovementComponent.
        void update(World *world, float deltaTime)
        {
            if (dq.empty())
                return;
            Entity *road1 = dq.front();
            // cout << "deque size : " << dq.front()->localTransform.position.z << '\n';
            if (road1->localTransform.position.z > 15)
            {
                // cout << "3adda\n";
                dq.pop_front();
                if (dq.empty())
                    return;
                Entity *road3 = dq.back();
                // repeat the road 1 as it is out of the screen
                road1->localTransform.position.z = road3->localTransform.position.z - 14;
                
                dq.push_back(road1);
            }
        }
    };

}
