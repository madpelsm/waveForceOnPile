#include <loader.h>
#include <morisson.h>
#include <saver.h>
#include <stdlib.h>
#include <windowgui.h>
#include <cstring>

std::map<std::string, double> parameters;
std::string defaultsave = "save.txt";
int width = 355, height = 600;
bool writepermission = 0, GUI = 1, with_impact = true;
void updateParams(Wave *w, Pile *p, Morisson *m) {
    w->setHeight(parameters["waveheight"]);
    w->setWaterDepth(parameters["waterdepth"]);
    w->setPeriod(parameters["period"]);
    w->setPrecision(parameters["precision"]);
    p->setSurfaceRoughness(parameters["pileroughness"]);
    p->setDiameter(parameters["pilediameter"]);
    w->calculateWaveLength();
    w->setCurrent(parameters["currentvelocity"]);
    m->setNu(parameters["nu"]);
    m->setImpactAngle(parameters["impact_angle"]);
    m->setTimestep(parameters["timestep"]);
    m->setTimstepCalcStorage(parameters["overTCalcStore"]);
}
void setParameter(std::string _input) {
    std::string param = _input.substr(0, _input.find("="));
    double argument = std::atof(_input.substr(_input.find("=") + 1).c_str());
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
    if (std::find(validCommands.begin(), validCommands.end(), param) !=
        validCommands.end()) {
        parameters[param.c_str()] = argument;
        printf("set %s to %f\n", param.c_str(), argument);
    }

    else if (param == "defaultsave") {
        defaultsave = _input.substr(_input.find("=") + 1);
        printf("Loading default savefile:%s \n", defaultsave.c_str());
    }

    if (param.compare("help") == 0) {
        std::string howto = "Possible commands are:\n";
        for (auto x : validCommands) {
            howto += x + "\n";
        }
        howto += "defaultsave\n";
        howto +=
            "for example waveheight=12\nwritepermission=1 or "
            "0\n\n********************\n";

        printf("%s\n", howto.c_str());
    }
}

int main(int argc, char *argv[]) {
    parameters["GUI"] = GUI;
    for (size_t i = 0; i < argc; i++) {
        setParameter(argv[i]);
    }
    // default load file
    //
    if (argc > 1) {
        Loader *dr = new Loader(defaultsave);
        dr->parseData();
        parameters = dr->get_data();
        delete dr;
        width = parameters["width"] > 0 ? parameters["width"] : width;
        height = parameters["height"] > 0 ? parameters["height"] : height;
        printf("Arguments detected\n");
    }
    for (size_t i = 0; i < argc; i++) {
        setParameter(argv[i]);
    }
    Wave *wave = new Wave();
    Pile *pile = new Pile(parameters["pilediameter"]);
    Morisson *morisson1 = new Morisson(pile, wave);

    GUIWindow ge1(width, height, "waves App");
    /**********/
    // Wave info
    Panel waves("wave information");
    Variable waveH("waveheight", &parameters["waveheight"]);
    Variable waterD("waterdepth", &parameters["waterdepth"]);
    Variable waveT("period", &parameters["period"]);
    Variable waveP("precision", &parameters["precision"]);
    Variable visco("nu", &parameters["nu"]);
    Variable currentVel("Current [m/s]", &parameters["currentvelocity"]);

    std::vector<Variable *> waveVars = {&waveH, &waterD, &waveT,
                                        &waveP, &visco,  &currentVel};
    waves.addVariables(&waveVars);

    ge1.addPanel(&waves);
    //*********/
    // Pile info
    Panel piles("pile information");

    piles.setPanelPosition(0, 450);
    Variable PileD("pilediameter", &parameters["pilediameter"]);
    Variable PileR("pileroughness", &parameters["pileroughness"]);

    std::vector<Variable *> pileVars = {&PileD, &PileR};
    piles.addVariables(&pileVars);  // why copy??

    ge1.addPanel(&piles);

    /*******/
    // calculation settings
    Panel calcSettings("Calculation Settings");

    calcSettings.setPanelPosition(0, 200);
    Variable timeStep("timestep", &parameters["timestep"]);
    Variable tStrt("start time", &parameters["tstart"]);
    Variable tEndt("end time", &parameters["tend"]);
    Variable stepSize("stepsize", &parameters["dz"]);
    Variable xlo("x location", &parameters["xlocation"]);
    Variable Tinstant("Time point", &parameters["timeinstant"]);
    Variable IntervalCalcStore("Store dt calcs", &parameters["overTCalcStore"]);
    Variable impactAngle("impact angle (deg)", &parameters["impact_angle"]);

    std::vector<Variable *> calcVars = {
        &timeStep,          &tStrt,      &tEndt, &stepSize, &xlo, &Tinstant,
        &IntervalCalcStore, &impactAngle};
    calcSettings.addVariables(&calcVars);
    ge1.addPanel(&calcSettings);
    ButtonPanel simpleCalc("calculate at t");
    simpleCalc.setPosition(170, 0);

    ge1.addButtonPanel(&simpleCalc);

    ge1.makeGUIWindow();
    if (parameters["overTCalcStore"] != 1.0) {
        morisson1->mOverTCalcStorage = false;
    } else {
        morisson1->mOverTCalcStorage = true;
    }
    // TIME TO PLACE BUTTONS
    simpleCalc.buttonHelp->addButton(simpleCalc.mName, [&morisson1, &ge1, &pile,
                                                        &wave]() {
        updateParams(wave, pile, morisson1);
        morisson1->calculateForces(parameters["dz"], parameters["timeinstant"]);
        MessageDialog *m = new MessageDialog(
            ge1.screen, MessageDialog::Type::Warning, "Calculated!",
            "Calculation completed, now press 'store calculation'", "ok");
    });
    simpleCalc.buttonHelp->addButton(
        "store calculation", [&morisson1]() { morisson1->storeCalculation(); });
    simpleCalc.buttonHelp->addButton("Calculate over time", [&morisson1, &wave,
                                                             &pile, &ge1]() {
        if (parameters["tend"] != parameters["tstart"]) {
            updateParams(wave, pile, morisson1);
            std::string dialogText =
                (parameters["overTCalcStore"] == 1)
                    ? "The calculation has been stored"
                    : "The intermediate calculations weren't stored";
            morisson1->calculateForcesOverTime(
                parameters["tstart"], parameters["tend"], parameters["dz"],
                parameters["xlocation"]);
            MessageDialog *m =
                new MessageDialog(ge1.screen, MessageDialog::Type::Warning,
                                  "Calculation Completed!", dialogText, "ok");

        } else {
            MessageDialog *m = new MessageDialog(
                ge1.screen, MessageDialog::Type::Warning, "start=end",
                "Please chose the end time different from the starttime", "ok");
        }
    });
    simpleCalc.buttonHelp->addButton("export calculation", [&morisson1]() {
        std::string fileLoc = file_dialog({{"csv", ""}}, true);
        printf("%s\n", fileLoc.c_str());
        morisson1->writeToFile(fileLoc);
    });
    simpleCalc.buttonHelp->addButton(
        "Load Save", [&morisson1, &pile, &wave, &ge1]() {
            std::string fileLoc = file_dialog({{"txt", ""}}, false);
            printf("%s\n", fileLoc.c_str());
            Loader ldrT(fileLoc);
            ldrT.parseData();
            parameters = ldrT.get_data();
            updateParams(wave, pile, morisson1);
            ge1.refresh();
        });
    simpleCalc.buttonHelp->addButton("Save settings", []() {
        std::string fileLoc = file_dialog({{"txt", ""}}, true);
        Saver svr(parameters);
        svr.save_data_to_file(fileLoc);
    });

    // make window
    if (parameters["GUI"] == 1) {
        ge1.startLoop();
    }
    updateParams(wave, pile, morisson1);
    if (parameters["writepermission"]) {
        morisson1->mOverTCalcStorage = true;

        morisson1->calculateForces(parameters["dz"], parameters["timeinstant"]);
        morisson1->storeCalculation();
        morisson1->writeToFile();
        morisson1->calculateForcesOverTime(parameters["tstart"],
                                           parameters["tend"], parameters["dz"],
                                           parameters["xlocation"]);
    }
    delete morisson1, wave, pile;
    return 0;
}
