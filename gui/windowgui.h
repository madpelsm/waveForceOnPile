#include <nanogui/nanogui.h>
#include <panel.h>
#include <stdio.h>
#include <string>
using namespace nanogui;
class GUIWindow {
   public:
    Screen *screen;
    Window *w;
    std::vector<Panel *> panels;
    std::vector<ButtonPanel *> buttons;
    std::string mName;
    int mWidth, mHeight;
    GUIWindow();
    GUIWindow(int _width, int _height, std::string _name);
    ~GUIWindow();

    void makeGUIWindow();
    void addPanel(Panel *_panel);
    void addButtonPanel(ButtonPanel *_button);
    void drawPanels();
    void drawButtons();
    void startLoop();
};
