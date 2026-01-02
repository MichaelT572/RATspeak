#include "front/parser.h"
#include "runtime/runtime.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "__________    ________________                           __    \n";
        std::cout << "\\______   \\  /  _  \\__    ___/___________   ____ _____  |  | __\n";
        std::cout << " |       _/ /  /_\\  \\|    | /  ___/\\____ \\_/ __ \\\\__  \\ |  |/ /\n";
        std::cout << " |    |   \\/    |    \\    | \\___ \\ |  |_> >  ___/ / __ \\|    < \n";
        std::cout << " |____|_  /\\____|__  /____|/____  >|   __/ \\___  >____  /__|_ \\\n";
        std::cout << "        \\/         \\/           \\/ |__|        \\/     \\/     \\/\n";
        std::cout << "Version 2.0 - Created by Michael Torres\n";

        return 0;
    } else if (argc > 2) {
        std::cerr << "Invalid number of arguments; try \"RATspeak filename\" or \"RATspeak -e\"\n";
        return 1;
    }

    if (((std::string) argv[1]) == "-e") {
        std::cout << "🐀🧀💊🕳️🏭🧪🧱🗑️🔼🧵📡☣\n";
        return 0;
    }

    std::string file_name = argv[1];
    try {
        Parser parser(file_name);
        std::unique_ptr<Node> ast = parser.build_ast();
        // print_ast(ast.get());
        call_func(((Program*) ast.get()), "squeak", 0, 0);
        run();

    } catch (std::runtime_error e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}

// cp ./temp/RATspeak ~/Apps/RATspeak/