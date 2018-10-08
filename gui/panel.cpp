#include <panel.h>

Variable::Variable() {}

Variable::Variable(std::string _name, double *_val)
    : mVarName(_name), value(_val) {}

Variable::~Variable() {}

std::string Variable::getName() { return mVarName; }

double *Variable::getValue() { return value; }

Panel::Panel() {}

Panel ::~Panel() {}

Panel::Panel(std::string _name) : mTitle(_name) {}

void Panel::addVariables(std::vector<Variable *> *_variables) {
    mVariables = _variables;
}

void Panel::drawPanel(Screen *_screen) {
    panelForm = new FormHelper(_screen);
    ref<Window> window = panelForm->addWindow(Eigen::Vector2i(x, y), mTitle);
    panelForm->addGroup("");
    for (auto x : *mVariables) {
        double t;
        panelForm->addVariable(x->getName(), *x->value);
    }
}

void Panel::setPanelPosition(int _x, int _y) {
    x = _x;
    y = _y;
}


ButtonPanel::ButtonPanel() {}
ButtonPanel::~ButtonPanel() { delete buttonHelp; }
void ButtonPanel::drawButton(Screen *_sc) {
    buttonHelp = new FormHelper(_sc);
    ref<Window> window = buttonHelp->addWindow(Eigen::Vector2i(x, y), mName);
    buttonHelp->addGroup("");
}

ButtonPanel::ButtonPanel(std::string _name) : mName(_name) {}

void ButtonPanel::setPosition(int _x, int _y) {
    x = _x;
    y = _y;
}
