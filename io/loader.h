#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class Loader {
   public:
    std::string filename;
    std::vector<std::string> validCommands = {"waveheight",   "waterdepth",
                                              "period",       "precision",
                                              "pilediameter", "pileroughness",
                                              "nu",           "timestep",
                                              "tstart",       "tend",
                                              "dz",           "xlocation",
                                              "timeinstant",  "height",
                                              "width",        "writepermission",
                                              "GUI",          "currentvelocity",
                                              "impact_angle"};

    std::map<std::string, double> parsed_data;

    Loader();
    Loader(std::string _filename);
    ~Loader();

    void parseData();
    void parseLine(std::string _line);
    void print_loaded_data();
    std::map<std::string, double> get_data();
};

