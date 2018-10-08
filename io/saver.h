#include <fstream>
#include <iostream>
#include <map>
#include <string>

class Saver {
   public:
    std::map<std::string, double> data_map;

    Saver();
    ~Saver();
    Saver(std::map<std::string, double> _map);

    void set_map(std::map<std::string, double> _map);
    void save_data_to_file(std::string _filename);
};
