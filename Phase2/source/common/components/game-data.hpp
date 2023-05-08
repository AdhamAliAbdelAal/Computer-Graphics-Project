#pragma once

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/movement.hpp>
#include <systems/coin-generation.hpp>
#include <systems/coin-collection.hpp>
#include <systems/road-generator.hpp>
#include <systems/road-repeater.hpp>

using namespace our;

namespace our {
    
    class GameData {
    public:
        World *world;
        our::ForwardRenderer renderer;
        
        our::FreeCameraControllerSystem *cameraController;
        our::MovementSystem movementSystem;
        
        // coin generation system is responsible for generating coins
        our::CoinGenerationSystem *coinGenerationSystem;
        
        // road repeater system is responsible for repeating the road
        our::RoadRepeaterSystem *roadRepeaterSystem;
        

        // coin collection system is responsible for collecting coins
        our::CoinCollectionSystem coinCollectionSystem;
        

        // road generation system is responsible for generating roads
        our::RoadGenerationSystem *roadGenerationSystem;    
    };

}