#pragma once

#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/movement.hpp>
#include <systems/coin-generation.hpp>
#include <systems/coin-collection.hpp>
#include <systems/road-repeater.hpp>
#include <asset-loader.hpp>
#include<iostream>

// This state shows how to use the ECS framework and deserialization.
class Playstate: public our::State {

    our::World world;
    our::ForwardRenderer renderer;
    our::FreeCameraControllerSystem cameraController;
    our::MovementSystem movementSystem;
    // coin generation system is responsible for generating coins
    our::CoinGenerationSystem *coinGenerationSystem;

    // road repeater system is responsible for repeating the road
    our::RoadRepeaterSystem *roadRepeaterSystem;

    // coin collection system is responsible for collecting coins
    our::CoinCollectionSystem coinCollectionSystem;

    void onInitialize() override {
        // First of all, we get the scene configuration from the app config
        auto& config = getApp()->getConfig()["scene"];
        // If we have assets in the scene config, we deserialize them
        if(config.contains("assets")){
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if(config.contains("world")){
            world.deserialize(config["world"]);
            cout<<"world deserialized : "<<typeid(config["world"]).name()<<'\n';
        }
        // if we have a coin in the scene config, we use to hold the data of the coin
        if(config.contains("coin")&&config.contains("fire")){
            if(!config["coin"].is_object()) return;
            coinGenerationSystem=new our::CoinGenerationSystem(config["coin"],config["fire"]);
        }

        // create road repeater system
        roadRepeaterSystem= new::our::RoadRepeaterSystem(&world);

        // We initialize the camera controller system since it needs a pointer to the app
        cameraController.enter(getApp());
        // Then we initialize the renderer
        auto size = getApp()->getFrameBufferSize();
        renderer.initialize(size, config["renderer"]);
    }

    void onDraw(double deltaTime) override {
        // Here, we just run a bunch of systems to control the world logic
        movementSystem.update(&world, (float)deltaTime);
        cameraController.update(&world, (float)deltaTime);

        //TODO: Add more systems here
        // system 1 : call update function of coin generation system
        if(coinGenerationSystem) coinGenerationSystem->update(&world, (float)deltaTime);
        // system 2 : call update function of road repeater system
        if(roadRepeaterSystem) roadRepeaterSystem->update(&world, (float)deltaTime);
        // system 3 : call update function of coin collection system
        coinCollectionSystem.update(&world, (float)deltaTime);


        // And finally we use the renderer system to draw the scene
        renderer.render(&world);

        // Get a reference to the keyboard object
        auto& keyboard = getApp()->getKeyboard();

        if(keyboard.justPressed(GLFW_KEY_ESCAPE)){
            // If the escape  key is pressed in this frame, go to the play state
            getApp()->changeState("menu");
        }
    }

    void onDestroy() override {
        // Don't forget to destroy the renderer
        renderer.destroy();
        // On exit, we call exit for the camera controller system to make sure that the mouse is unlocked
        cameraController.exit();
        // Clear the world
        world.clear();
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
    }
};