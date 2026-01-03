#include "front/parser.h"
#include "runtime/runtime.h"

/*
TO DO:

Optimize finding objects
Add wall updates to remove overlapping wall
*/

int main(int argc, char* argv[]) {
    std::string file_name = "";
    bool visualize = false;

    if (argc < 2) {
        std::cout << "__________    ________________                           __    \n";
        std::cout << "\\______   \\  /  _  \\__    ___/___________   ____ _____  |  | __\n";
        std::cout << " |       _/ /  /_\\  \\|    | /  ___/\\____ \\_/ __ \\\\__  \\ |  |/ /\n";
        std::cout << " |    |   \\/    |    \\    | \\___ \\ |  |_> >  ___/ / __ \\|    < \n";
        std::cout << " |____|_  /\\____|__  /____|/____  >|   __/ \\___  >____  /__|_ \\\n";
        std::cout << "        \\/         \\/           \\/ |__|        \\/     \\/     \\/\n";
        std::cout << "Version 2.0 - Created by Michael Torres\n";

        return 0;
    } else {
        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];

            if (arg[0] == '-') {
                if (arg == "-e") {
                    std::cout << "🐀🧀💊🕳 🏭🧪🧱🗑 🔼🧵📡☣ \n";
                } else if (arg == "-v") {
                    visualize = true;
                } else {
                    std::cerr << "Invalid arguments: " + arg + "\n";
                    return 1;
                }
            } else {
                if (file_name == "") {
                    file_name = arg;
                } else {
                    std::cerr << "Invalid number of arguments; try \"RATspeak filename\" or \"RATspeak -e\"\n";
                    return 1;
                }
            }
        }
    }

    if (file_name == "") {return 0;}
    
    try {
        Parser parser(file_name);
        ast = parser.build_ast();
        // print_ast(ast.get());
        build_func("squeak", 0, 0);
        run(visualize);

    } catch (std::runtime_error e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}

// cp ./temp/RATspeak ~/Apps/RATspeak/