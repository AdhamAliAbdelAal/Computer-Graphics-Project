#pragma once

#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/movement.hpp>
#include <systems/coin-generation.hpp>
#include <systems/coin-collection.hpp>
#include <systems/road-generator.hpp>
#include <systems/road-repeater.hpp>
#include <systems/battery-controller.hpp>
#include <asset-loader.hpp>
#include <iostream>
#include <irrKlang.h>
using namespace irrklang;

// This state shows how to use the ECS framework and deserialization.
class Playstate : public our::State
{
    float startTime = 0;      // of the countdown to gameover
    float pauseStartTime = 0; // Pausing the game should pause the countdown
    float chargeStartTime = 0;  // of the charge effect

    ISoundEngine *SoundEngine = nullptr;

    our::World world;              // The world that holds all entities and systems
    our::ForwardRenderer renderer; // The renderer that will render the world
    our::FreeCameraControllerSystem cameraController; // The camera controller system
    our::MovementSystem movementSystem; // The movement system (controls the movement of entities)
    // coin generation system is responsible for generating coins
    our::CoinGenerationSystem *coinGenerationSystem;

    // road repeater system is responsible for repeating the road
    our::RoadRepeaterSystem *roadRepeaterSystem;

    // coin collection system is responsible for collecting coins
    our::CoinCollectionSystem coinCollectionSystem;

    // road generation system is responsible for generating roads
    our::RoadGenerationSystem *roadGenerationSystem;

    // Battery system is responsible for collected coins
    our::BatterySystem *batteryController;

    // returns the name of the state
    std::string getName() override
    {
        return "play";
    }

    void onInitialize() override
    {
        // Initialize the sound engine
        SoundEngine= createIrrKlangDevice();
        SoundEngine->play2D("assets/sounds/background.mp3", true); // Play background music
        SoundEngine->setSoundVolume(0.3f);                          // Set volume (Lower than intitial volume)
        
        // First of all, we get the scene configuration from the app config
        auto &config = getApp()->getConfig()["scene"];
        // If we have assets in the scene config, we deserialize them
        if (config.contains("assets"))
        {
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if (config.contains("world"))
        {
            world.deserialize(config["world"]);
        }
        // if we have a coin in the scene config, we use it to hold the data of the coin
        if (config.contains("coin") && config.contains("fire") && config.contains("monster") && config.contains("turbo"))
        {
            if (!config["coin"].is_object())
                return;
            coinGenerationSystem = new our::CoinGenerationSystem(config["coin"], config["fire"], config["monster"], config["turbo"]);
        }
        // if we have a road in the scene config, we use it to hold the data of the road
        if (config.contains("road") && config.contains("fence"))
        {
            if (!config["road"].is_object() || !config["fence"].is_object())
                return;
            roadGenerationSystem = new our::RoadGenerationSystem(config["road"], config["fence"], &world);
        }

        // create road repeater system
        roadRepeaterSystem = new ::our::RoadRepeaterSystem(&world);

        // We initialize the camera controller system since it needs a pointer to the app
        cameraController.enter(getApp());
        // Then we initialize the renderer
        auto size = getApp()->getFrameBufferSize();
        renderer.initialize(size, config["renderer"]);

        // We initialize the battery controller system
        batteryController = new our::BatterySystem(config["world"], config["assets"]["textures"], &world);
    }

    void onDraw(double deltaTime) override
    {
        // Here, we just run a bunch of systems to control the world logic
        movementSystem.update(&world, (float)deltaTime, getApp());
        cameraController.update(&world, (float)deltaTime);

        // TODO: Add more systems here
        //  system 1 : call update function of coin generation system
        if (coinGenerationSystem)
            coinGenerationSystem->update(&world, (float)deltaTime);
        // system 2 : call update function of road repeater system
        if (roadRepeaterSystem)
            roadRepeaterSystem->update(&world, (float)deltaTime);
        // system 3 : call update function of coin collection system
        coinCollectionSystem.update(&world, (float)deltaTime);

        // system 4 : call update function of the battery system
        batteryController->update_battery(coinCollectionSystem.get_battery_charge());

        // And finally we use the renderer system to draw the scene (if 10 seconds passed, we change the background)
        renderer.render(&world);

        // Set Score (Pass the number of collected coins to the app)
        getApp()->setScore(coinCollectionSystem.get_num_of_collected_coins());

        // Get a reference to the keyboard object
        auto &keyboard = getApp()->getKeyboard();

        // Set the winning condition (25 coins)
        if (coinCollectionSystem.get_num_of_collected_coins()>=25)
        {
            // If the player won, go to the win state
            getApp()->changeState("win");
        }

        // Set the losing condition (0 charge in battery)
        if (coinCollectionSystem.get_battery_charge() == 0)
        {
            // If the player lost, go to the over state
            getApp()->changeState("over");
        }

        // If the player picks up a charge, apply the charge effect
        if (coinCollectionSystem.getCharge() && !renderer.getCharge())
        {
            // If the countdown is over, reset everything   (resets the doom mode)
            getApp()->setTimer(false);
            getApp()->setCountdown(5);
            coinCollectionSystem.setEgg(false);
            cameraController.setReversed(false);
            renderer.setDoomed(false);


            chargeStartTime = glfwGetTime();    // The start time of the charge effect
            renderer.setCharge(true);   // Set the renderer to charge mode

        }
        
        // If the player is hit with an egg, go Berserk
        if (coinCollectionSystem.getEgg() && !getApp()->getTimer())
        {
            //Reset the Charge mood
            renderer.setCharge(false);  // Set the renderer to normal mode
            coinCollectionSystem.setCharge(false); // Set the charge effect to false


            renderer.setDoomed(true);   // Set the renderer to Doom mode
            getApp()->setTimer(true);   // Set the timer (to start the countdown, and to make sure no doom mode is set again)
            cameraController.setReversed(true); // Reverse the controls
            startTime = glfwGetTime();          // The start time of the doom mode
            getApp()->setCountdownTime(startTime);  // Set the countdown time (to be used in application class)
        }

        // Check if the charge effect is over
        if (glfwGetTime() - chargeStartTime > 2.0f && renderer.getCharge())
        {
            renderer.setCharge(false);  // Set the renderer to normal mode
            coinCollectionSystem.setCharge(false); // Set the charge effect to false

        }

        // This condition is used to make sure that the doom mode is set for 5 seconds (Pauses excluded)
        if (glfwGetTime() - pauseReturnTime > (5.0f - (pauseStartTime - startTime)) && getApp()->getTimer())
        {
            // If the countdown is over, reset everything
            getApp()->setTimer(false);
            getApp()->setCountdown(5);
            coinCollectionSystem.setEgg(false);
            cameraController.setReversed(false);
            renderer.setDoomed(false);

            
        }

        // If the player pressed the P key, go to the pause state
        if (keyboard.justPressed(GLFW_KEY_P))
        {
            pauseStartTime = glfwGetTime();  // Set the pause start time
            getApp()->changeState("pause", true); // True Passed to control the Clearing of the color buffer in the application class
        }

        // If the escape key is pressed in this frame, go to the menu state
        if (keyboard.justPressed(GLFW_KEY_ESCAPE))
        {
            getApp()->changeState("menu");
        }
    }

    void onDestroy() override
    {

        // delete all systems pointers
        delete coinGenerationSystem;
        delete roadRepeaterSystem;
        delete roadGenerationSystem;
        coinCollectionSystem.reset();
        movementSystem.reset();
        // Don't forget to destroy the renderer
        renderer.destroy();
        // On exit, we call exit for the camera controller system to make sure that the mouse is unlocked
        cameraController.exit();
        // Clear the world
        world.clear();
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
        if (SoundEngine)
        {
            SoundEngine->stopAllSounds();
            SoundEngine->drop();
        }
    }
};
