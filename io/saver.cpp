#include <saver.h>

Saver::Saver() {}

Saver::~Saver() {}

Saver::Saver(std::map<std::string, double> _map) : data_map(_map) {}

void Saver::save_data_to_file(std::string _filename) {
    std::string _temp;
    for (auto const& x : data_map) {
        _temp += x.first + "=" + std::to_string(x.second) + "\n";
    }
    std::ofstream save_data(_filename);

    if (save_data.is_open()) {
        save_data << _temp;
    }
    save_data.close();
}

void Saver::set_map(std::map<std::string, double> _map) { data_map = _map; }
