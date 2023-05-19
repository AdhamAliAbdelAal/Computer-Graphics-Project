#pragma once

#include "../ecs/world.hpp"
#include "../components/movement.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <vector>
#include <iostream>
using namespace std;
#define MAX_LEVELS_TO_WIN 5
#define LEVEL_WEIGHT 4.0    // weight of the level
#define LOW_ENERGY_BAR_COLOR "battery_bar_red"
#define MEDIUM_ENERGY_BAR_COLOR "battery_bar_yellow"
#define HIGH_ENERGY_BAR_COLOR "battery_bar_green"

namespace our
{

    /**
     * This class represents a system for managing the battery.
     * It allows for updating the battery level by adding or removing battery level bars.
     * 
     * The class uses a World object to create and manipulate entities for the battery bars.
     * It also utilizes a JSON configuration for the world and textures.
     */
    class BatterySystem
    {
        nlohmann::json worldJson;
        World *worldPtr;
        int current_battery_level;
        nlohmann::json texturesJson;

        /**
            * To remove a single battery level bar entity from the World.
            * 
            * @param bar_number The number of the bar to remove
        */
        void remove_bar(int bar_number)
        {
            string bar_name = "bar-" + std::to_string(bar_number);
            for (auto entity : this->worldPtr->getEntities())
            {
                if (entity->name == bar_name)
                {
                    this->worldPtr->markForRemoval(entity);
                    this->worldPtr->deleteMarkedEntities();
                    return;
                }

            }
 
        }

        /**
            * To Remove all the battery level bar entities from the World.
        */
        void remove_all_bars()
        {
            for(int i = 1; i <= 5; i++)
                remove_bar(i);
        }

        /**
            * To add a single battery level bar entity to the World.
            * 
            * @param bar_number The number of the bar to add
        */
        void add_bar(int bar_number)
        {
            string bar_name = "bar-" + std::to_string(bar_number);
            for (auto& obj : this->worldJson) {
                if (obj["name"] == bar_name) {
                    
                    obj.at("components")[0].at("material") = this->change_bar_color();
                    this->worldPtr->objectDeserialize(obj);
                    break;
                }
            }
 
        }

        /**
            * To determine the color for the battery level bar based on the current battery level.
            * 
            * @return The color for the battery level bar
        */
         string change_bar_color(){
            switch (this->current_battery_level)
            {
            case 1:
                return LOW_ENERGY_BAR_COLOR;
                break;

            case 2:
            case 3:
                return MEDIUM_ENERGY_BAR_COLOR;
                break;
            
            case 4:
            case 5:
                return HIGH_ENERGY_BAR_COLOR;
                break;

            default:
                return LOW_ENERGY_BAR_COLOR;
                break;
            }
            
        }

    public:

        /**
           * @param worldJson The JSON configuration for the world
           * @param texturesJson The JSON configuration for the textures used for the battery bars
           * @param worldPtr A pointer to the World object for managing entities
        */
        BatterySystem(const nlohmann::json &worldJson, const nlohmann::json &texturesJson, World *worldPtr) : worldJson(worldJson), texturesJson(texturesJson), worldPtr(worldPtr)
        {
            this->remove_all_bars();
            this->current_battery_level = 0;
        }

        /**
           * Updates the battery level by adding or removing battery level bar entities.
           * 
           * @param level The new battery level
        */
        void update_battery(int level){

            this->current_battery_level = level;

            // normal logic
            this->remove_all_bars();
            for(int i = 1; i <= this->current_battery_level; i++)
                this->add_bar(i);
            
            return;
        }
    };

}
