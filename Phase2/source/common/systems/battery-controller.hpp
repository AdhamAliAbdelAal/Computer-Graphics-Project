#pragma once

#include "../ecs/world.hpp"
#include "../components/movement.hpp"
#include "../components/battery_bar.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <vector>
#include <iostream>
using namespace std;
#define MAX_COINS_TO_WIN 1

namespace our
{

    class BatterySystem
    {
        
    public:

        void update(World *world, string action)
        {
            const unordered_set<Entity *> entities = world->getEntities();
            for (auto entity : entities)
            {
                if (entity->name == action)
                {
                    world->markForRemoval(entity);
                    world->deleteMarkedEntities();

                }
            }
 
        }

        bool update_battery(World *world, int coins_counter){
            string action;
            const int LEVEL_AMOUNT = 3;
            switch (coins_counter)
            {
            case LEVEL_AMOUNT:
                action = "bar-1";
                break;
            case 2 * LEVEL_AMOUNT:
                action = "bar-2";
                break;
            case 3 * LEVEL_AMOUNT:
                action = "bar-3";
                break;
            case 4 * LEVEL_AMOUNT:
                action = "bar-4";
                break;
            case 5 * LEVEL_AMOUNT:
                action = "bar-5";
                break;
            default:
                break;
            }
            this->update(world, action);
            if(coins_counter >= MAX_COINS_TO_WIN){
                return true;
            }
            return false;
        }
    };

}
