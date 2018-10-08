#include <loader.h>

Loader::Loader() {}

Loader::~Loader() {}

Loader::Loader(std::string _filename) : filename(_filename) {}

void Loader::parseData() {
    std::ifstream savefile(filename);
    std::string content;

    if (savefile.is_open()) {
        while (getline(savefile, content)) {
            parseLine(content);
        }
        savefile.close();
    }
}

void Loader::parseLine(std::string _line) {
    std::string param_name = _line.substr(0, _line.find("="));
    double argument = std::atof(_line.substr(_line.find("=") + 1).c_str());
    if (std::find(validCommands.begin(), validCommands.end(), param_name) !=
        validCommands.end()) {
        // then the command is valid and we can add it
        parsed_data[param_name] = argument;
    }
}

void Loader::print_loaded_data() {
    for (auto const& entry : parsed_data) {
        printf("%s,%f\n", entry.first.c_str(), entry.second);
    }
}

std::map<std::string, double> Loader::get_data() { return parsed_data; }
