#include <iostream>
#include <fstream>
#include <flags/flags.h>
#include <json/json.hpp>

#include <application.hpp>

#include "states/menu-state.hpp"
#include "states/play-state.hpp"
#include "states/game-over-state.hpp"
#include "states/pause-menu-state.hpp"
#include "states/win-state.hpp"
#include "states/shader-test-state.hpp"
#include "states/mesh-test-state.hpp"
#include "states/transform-test-state.hpp"
#include "states/pipeline-test-state.hpp"
#include "states/texture-test-state.hpp"
#include "states/sampler-test-state.hpp"
#include "states/material-test-state.hpp"
#include "states/entity-test-state.hpp"
using namespace std;

int main(int argc, char** argv) {
    // ISoundEngine *SoundEngine = createIrrKlangDevice();
    // SoundEngine->play2D("stranger things.mp3", true);
    
    flags::args args(argc, argv); // Parse the command line arguments
    // config_path is the path to the json file containing the application configuration
    // Default: "config/app.json"
    std::string config_path = args.get<std::string>("c", "config/app.jsonc");
    // run_for_frames is how many frames to run the application before automatically closing
    // This is useful for testing multiple configurations in a batch
    // Default: 0 where the application runs indefinitely until manually closed
    int run_for_frames = args.get<int>("f", 0);

    // Open the config file and exit if failed
    std::ifstream file_in(config_path);
    if(!file_in){
        std::cerr << "Couldn't open file: " << config_path << std::endl;
        return -1;
    }
    // Read the file into a json object then close the file
    nlohmann::json app_config = nlohmann::json::parse(file_in, nullptr, true, true);
    file_in.close();

    // Create the application
    our::Application app(app_config);
    
    
    // Register all the states of the project in the application
    app.registerState<Menustate>("menu");
    app.registerState<Playstate>("play");
    app.registerState<Overstate>("over");
    app.registerState<Pausestate>("pause");
    app.registerState<Winstate>("win");
    app.registerState<ShaderTestState>("shader-test");
    app.registerState<MeshTestState>("mesh-test");
    app.registerState<TransformTestState>("transform-test");
    app.registerState<PipelineTestState>("pipeline-test");
    app.registerState<TextureTestState>("texture-test");
    app.registerState<SamplerTestState>("sampler-test");
    app.registerState<MaterialTestState>("material-test");
    app.registerState<EntityTestState>("entity-test");
    // Then choose the state to run based on the option "start-scene" in the config
    if(app_config.contains(std::string{"start-scene"})){
        cout<<app_config["start-scene"].get<std::string>()<<'\n';
        app.changeState(app_config["start-scene"].get<std::string>());
    }

    // Finally run the application
    // Here, the application loop will run till the terminatio condition is statisfied
    return app.run(run_for_frames);


    /********************************************SUMMARY OF THE GAME STATES********************************************/
    /*
    Doom State: A state reached when hit by an egg, a countdown starts with an effect that mimics a drunken effect, you can pause the game
    during that countdown, pause time won't be considered in the countdown. The countdown will resume after the pause is over.
    The countodown is 5 seconds.   (There is no class for this state, it is handled in the play state)

    Pause State: A state reached when you press the P key, you can resume the game by pressing the P key again.

    Over State: A state reached when you lose, you can go back to the menu by pressing the escape key.

    Menu State: A state reached when you hit the esc button, you can restart the game by pressing the space key.

    Win State: A state reached when you collect 25 coins, you can restart the game by pressing the space key.

    Play State: The main state of the game, you can pause the game by pressing the P key, you can go back to the menu by pressing the esc key.
    */
}