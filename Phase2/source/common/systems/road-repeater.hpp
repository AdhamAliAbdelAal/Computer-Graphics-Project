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
    struct Road
    {
        Entity *road;
        vector<Entity *> fence_left = vector<Entity *>(2);
        vector<Entity *> fence_right = vector<Entity *>(2);
    };
    bool cmp(Entity *a, Entity *b)
    {
        return a->localTransform.position.z > b->localTransform.position.z;
    }
    // the road repeater system is responsible for repeating the road
    // This system is added as a simple example for how use the ECS framework to implement logic.
    class RoadRepeaterSystem
    {
        // deque to hold the roads
        deque<Road> dq;

    public:
        RoadRepeaterSystem(World *world)
        {
            const unordered_set<Entity *> entities = world->getEntities();
            vector<Entity *> roads;
            vector<Entity *> left_fences, right_fences;
            for (auto it : entities)
            {
                if (it->name == "road")
                {
                    roads.push_back(it);
                }
                if (it->name == "left fence")
                {
                    left_fences.push_back(it);
                }
                if (it->name == "right fence")
                {
                    right_fences.push_back(it);
                }
            }
            // sorting the roads according to their z position
            sort(roads.begin(), roads.end(), cmp);
            // sorting the fences according to their z position
            sort(left_fences.begin(), left_fences.end(), cmp);
            sort(right_fences.begin(), right_fences.end(), cmp);

            for (int i = 0; i < roads.size(); i++)
            {
                Road road;
                road.road = roads[i];
                int fenceIndex = 2 * i;
                // every road has 2 fences
                for (int j = 0; j < 2; ++j)
                {
                    road.fence_left[j] = left_fences[fenceIndex + j];
                    road.fence_right[j] = right_fences[fenceIndex + j];
                }
                dq.push_back(road);
            }
        }
        // This should be called every frame to update all entities containing a MovementComponent.
        void update(World *world, float deltaTime)
        {
            if (dq.empty())
                return;
            Road road1 = dq.front();
            // cout<<"road : "<<road1.road->localTransform.position.z<<"\n fence1 : "<<road1.fence_left[0]->localTransform.position.z<<"\n fence2 : "<<road1.fence_left[1]->localTransform.position.z<<"\n fence3 : "<<road1.fence_right[0]->localTransform.position.z<<"\n fence4 : "<<road1.fence_right[1]->localTransform.position.z<<"\n";

            // cout << "deque size : " << dq.front()->localTransform.position.z << '\n';
            if (road1.road->localTransform.position.z > 15)
            {
                // cout << "3adda\n";
                dq.pop_front();
                if (dq.empty())
                    return;
                Road road3 = dq.back();
                // repeat the road 1 as it is out of the screen
                road1.road->localTransform.position.z = road3.road->localTransform.position.z - 14;
                for (int j = 0; j < 2; ++j)
                {
                    road1.fence_left[j]->localTransform.position.z = road3.fence_left[j]->localTransform.position.z - 14;
                    road1.fence_right[j]->localTransform.position.z = road3.fence_right[j]->localTransform.position.z - 14;
                }

                dq.push_back(road1);
            }
        }
    };

}
