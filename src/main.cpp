#include <morisson.h>
#include <stdlib.h>
#include <windowgui.h>
#include <cstring>

#define PARAM(X)       \
    if (#X == param) { \
        X = argument;  \
    };

double waveheight = 4, waterdepth = 10, period = 18, precision = 0.00001,
       pilediameter = 3, pileroughness = 0.00001, nu = 0.000001, timestep = 1,
       tstart = 0, tend = 0, dz = 1, xlocation = 0, timeinstant = 6,
       overTCalcStore = 1, currentvelocity = 0, impact_angle = -1.0;

int width = 355, height = 600;
bool writepermission = 1, GUI = 1, with_impact = true;
void updateParams(Wave *w, Pile *p, Morisson *m) {
    w->setHeight(waveheight);
    w->setWaterDepth(waterdepth);
    w->setPeriod(period);
    w->setPrecision(precision);
    p->setSurfaceRoughness(pileroughness);
    p->setDiameter(pilediameter);
    w->calculateWaveLength();
    w->setCurrent(currentvelocity);
    m->setNu(nu);
    m->setImpactAngle(impact_angle);
    m->setTimestep(timestep);
    m->setTimstepCalcStorage(overTCalcStore);
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
        PARAM(height);
        PARAM(width);
        PARAM(writepermission);
        PARAM(GUI);
        PARAM(currentvelocity);
        PARAM(impact_angle);
        printf("set %s to %f\n", param.c_str(), argument);
    }

    if (param.compare("help") == 0) {
        std::string howto = "Possible commands are:\n";
        for (auto x : validCommands) {
            howto += x + "\n";
        }
        howto +=
            "for example waveheight=12\nwritepermission=1 or "
            "0\n\n********************\n";

        printf("%s\n", howto.c_str());
    }
}

int main(int argc, char *argv[]) {
    for (size_t i = 0; i < argc; i++) {
        setParameter(argv[i]);
    }
    Wave *wave = new Wave();
    Pile *pile = new Pile(pilediameter);
    Morisson *morisson1 = new Morisson(pile, wave);
    updateParams(wave, pile, morisson1);

    GUIWindow ge1(width, height, "waves App");
    /**********/
    // Wave info
    Panel waves("wave information");

    Variable waveH("waveheight", &waveheight);
    Variable waterD("waterdepth", &waterdepth);
    Variable waveT("period", &period);
    Variable waveP("precision", &precision);
    Variable visco("nu", &nu);
    Variable currentVel("Current [m/s]", &currentvelocity);

    std::vector<Variable *> waveVars = {&waveH, &waterD, &waveT,
                                        &waveP, &visco,  &currentVel};
    waves.addVariables(&waveVars);

    ge1.addPanel(&waves);
    //*********/
    // Pile info
    Panel piles("pile information");

    piles.setPanelPosition(0, 450);
    Variable PileD("pilediameter", &pilediameter);
    Variable PileR("pileroughness", &pileroughness);

    std::vector<Variable *> pileVars = {&PileD, &PileR};
    piles.addVariables(&pileVars);  // why copy??

    ge1.addPanel(&piles);

    /*******/
    // calculation settings
    Panel calcSettings("Calculation Settings");

    calcSettings.setPanelPosition(0, 200);
    Variable timeStep("timestep", &timestep);
    Variable tStrt("start time", &tstart);
    Variable tEndt("end time", &tend);
    Variable stepSize("stepsize", &dz);
    Variable xlo("x location", &xlocation);
    Variable Tinstant("Time point", &timeinstant);
    Variable IntervalCalcStore("Store dt calcs", &overTCalcStore);
    Variable impactAngle("impact angle (deg)", &impact_angle);

    std::vector<Variable *> calcVars = {
        &timeStep,          &tStrt,      &tEndt, &stepSize, &xlo, &Tinstant,
        &IntervalCalcStore, &impactAngle};
    calcSettings.addVariables(&calcVars);
    ge1.addPanel(&calcSettings);
    //  now triggered by button
    ButtonPanel simpleCalc("calculate at t");
    simpleCalc.setPosition(170, 0);

    ge1.addButtonPanel(&simpleCalc);

    ge1.makeGUIWindow();
    printf("value of calcstore: %f\n", overTCalcStore);
    if (overTCalcStore != 1.0) {
        morisson1->mOverTCalcStorage = false;
    } else {
        morisson1->mOverTCalcStorage = true;
    }
    // TIME TO PLACE BUTTONS
    simpleCalc.buttonHelp->addButton(
        simpleCalc.mName, [&morisson1, &ge1, &pile, &wave]() {
            updateParams(wave, pile, morisson1);
            morisson1->calculateForces(dz, timeinstant);
            MessageDialog *m = new MessageDialog(
                ge1.screen, MessageDialog::Type::Warning, "Calculated!",
                "Calculation completed, now press 'store calculation'", "ok");
        });
    simpleCalc.buttonHelp->addButton(
        "store calculation", [&morisson1]() { morisson1->storeCalculation(); });
    simpleCalc.buttonHelp->addButton("Calculate over time", [&morisson1, &wave,
                                                             &pile, &ge1]() {
        if (tend != tstart) {
            updateParams(wave, pile, morisson1);
            std::string dialogText = (overTCalcStore == 1)
                                         ? "The calculation has been stored"
                                         : "The calculation wasn't stored";
            morisson1->calculateForcesOverTime(tstart, tend, dz, xlocation);
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
    if (writepermission) {
        morisson1->mOverTCalcStorage = true;
    }
    if (tstart != tend) {
        morisson1->calculateForcesOverTime(tstart, tend, dz, xlocation);
    }

    morisson1->calculateForces(dz, timeinstant);
    if (writepermission) {
        morisson1->storeCalculation();
        morisson1->writeToFile();
    }

    // make window
    if (GUI) {
        ge1.startLoop();
    }
    delete morisson1, wave, pile;
    return 0;
}
