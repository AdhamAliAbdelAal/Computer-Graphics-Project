#pragma once

#include "../ecs/world.hpp"
#include "../components/movement.hpp"
#include "../components/gain.hpp"
#include "../ecs/entity.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>
#include <irrKlang.h>

using namespace std;
using namespace irrklang;

namespace our
{

    // the coin collection system is responsible for collecting the coins
    // This system is added as a simple example for how use the ECS framework to implement logic.
    class CoinCollectionSystem
    {
    private:
        // the minimum distance between the coin and the player
        float coin_min_dist = 1.0f;
        float fire_min_dist = 1.5f;
        float monster_min_dist = 1.0f;
        ll effect_delay = 0;
        ISoundEngine *SoundEngine;

    public:
        int score = 0;
        int battery_charge = 5;
        CoinCollectionSystem()
        {
            SoundEngine = createIrrKlangDevice();
        }

        void reset()
        {
            score = 0;
            battery_charge = 5;
        }
        // This should be called every frame to update all entities.
        bool update(World *world, float deltaTime)
        {
            // if (time(0) - effect_delay == 1)
            // {
            //     SoundEngine->stopAllSounds();
            // }
            const unordered_set<Entity *> entities = world->getEntities();
            Entity *ball = nullptr;
            for (auto it : entities)
            {
                if (it->name == "ball")
                {
                    ball = it;
                    break;
                }
            }
            if (!ball)
                return false;
            glm::vec3 ball_position = ball->localTransform.position;
            for (auto it : entities)
            {
                GainComponent *gainComponent = it->getComponent<GainComponent>();
                if (gainComponent)
                {
                    // get the position of the entity
                    glm::vec3 position = it->localTransform.position;
                    float distance = glm::distance(position, ball_position);
                    float min_dist = gainComponent->gain == 1 ? coin_min_dist : (gainComponent->gain == -1) ? fire_min_dist
                                                                                                            : monster_min_dist;
                    // if the distance between the coin and the player is less than the minimum distance
                    if (distance <= min_dist)
                    {

                        if (gainComponent->effect == "score")
                        {
                            // effect_delay = time(0);
                            if (gainComponent->gain == 1)
                            {
                                SoundEngine->play2D("assets/sounds/coin.wav", false, false, true);
                            }
                            else{
                                SoundEngine->play2D("assets/sounds/fire.mp3", false, false, true);
                            }
                            score += gainComponent->gain;
                        }
                        else if (gainComponent->effect == "battery")
                        {
                            if(gainComponent->gain == 1){
                                SoundEngine->play2D("assets/sounds/electric.wav", false, false, true);
                            }
                            else{
                                SoundEngine->play2D("assets/sounds/egg.mp3", false, false, true);
                            }
                            battery_charge += gainComponent->gain;
                            if (gainComponent->gain == -1)
                            {
                                world->markForRemoval(it);
                                world->deleteMarkedEntities();
                                return true;
                            }
                        }
                        world->markForRemoval(it);
                        // SoundEngine->drop();
                    }
                }
            }
            world->deleteMarkedEntities();

            return false;
        }

        int get_num_of_collected_coins()
        {
            return score;
        }
    };

}
