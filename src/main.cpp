#include <morisson.h>
#include <stdlib.h>
#include <cstring>

#define PARAM(X)                         \
    if (#X == param) {                   \
        X = argument;                    \
        printf("Set %s to %f\n", #X, X); \
    };

double waveheight = 4, waterdepth = 10, period = 18, precision = 0.00001,
       pilediameter = 3, pileroughness = 0.00001, nu = 0.000001, timestep = 1,
       tstart = 0, tend = 18, dz = 1, xlocation = 0, timeinstant = 6;

void setParameter(std::string _input) {
    std::string param = _input.substr(0, _input.find("="));
    double argument = std::atof(_input.substr(_input.find("=") + 1).c_str());
    std::vector<std::string> validCommands = {
        "waveheight",   "waterdepth",    "period", "precision",
        "pilediameter", "pileroughness", "nu",     "timestep",
        "tstart",       "tend",          "dz",     "xlocation",
        "timeinstant"};
    if (std::find(validCommands.begin(), validCommands.end(), param) !=
        validCommands.end()) {
        PARAM(waveheight);
        PARAM(waterdepth);
        PARAM(period);
        PARAM(precision);
        PARAM(pilediameter);
        PARAM(pileroughness);
        PARAM(nu);
        PARAM(timestep);
        PARAM(tstart);
        PARAM(tend);
        PARAM(dz);
        PARAM(xlocation);
        PARAM(timeinstant);
    }

    if (param.compare("help") == 0) {
        std::string howto =
            "Possible commands are:\n"
            "\nwaveheight\nwaterdepth\nperiod\nprecision\npilediameter\npilerou"
            "ghness\nnu\ntimestep\ntstart\ntend\ndz (depth "
            "interval)\nxlocation\ntime\n\nuse "
            "for example "
            "waveheight=12\n\n********************\n";
        printf("%s\n", howto.c_str());
    }
}

int main(int argc, char *argv[]) {
    for (size_t i = 0; i < argc; i++) {
        setParameter(argv[i]);
    }

    Wave wave;

    wave.setHeight(waveheight);
    wave.setWaterDepth(waterdepth);
    wave.setPeriod(period);
    wave.setPrecision(precision);
    Pile pile(pilediameter);
    pile.setSurfaceRoughness(pileroughness);

    wave.calculateWaveLength();
    Morisson morisson1(&pile, &wave);
    morisson1.setNu(nu);

    morisson1.calculateForces(dz, timeinstant);
    morisson1.setTimestep(timestep);
    morisson1.calculateForcesOverTime(tstart, tend, dz, xlocation);
    morisson1.writeToFile();
    return 0;
}
