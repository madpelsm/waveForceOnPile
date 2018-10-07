#include <nanogui/nanogui.h>
#include <string>
using namespace nanogui;
class Variable {
   public:
    std::string mVarName;
    double *value;

    Variable();
    Variable(std::string _name, double *val);
    ~Variable();

    std::string getName();
    double *getValue();
};
class Panel {
   public:
    int x = 0, y = 0;
    FormHelper *panelForm;
    std::string mTitle;
    std::vector<Variable *> *mVariables;
    Panel();
    ~Panel();
    Panel(std::string _title);

    void drawPanel(Screen *_screen);
    void addVariables(std::vector<Variable *> *_mVariables);
    void setPanelPosition(int x, int y);
};

class ButtonPanel {
   public:
    int x = 0, y = 0;
    FormHelper *buttonHelp;
    std::string mName;
    ButtonPanel();
    ~ButtonPanel();
    ButtonPanel(std::string _name);
    void drawButton(Screen *_sc);
    void setPosition(int _x, int _y);
};
