#include <windowgui.h>

GUIWindow::GUIWindow() {}

GUIWindow::GUIWindow(int _width, int _height, std::string _name)
    : mHeight(_height), mWidth(_width), mName(_name) {}

void GUIWindow::makeGUIWindow() {
    nanogui::init();
    screen = new Screen(nanogui::Vector2i(mWidth, mHeight), mName, true, false,
                        8, 8, 24, 2);

    // Window placement

    drawPanels();
    drawButtons();
}

void GUIWindow::startLoop() {
    screen->setVisible(true);
    screen->performLayout();

    nanogui::mainloop();
    nanogui::shutdown();
}

void GUIWindow::addPanel(Panel *_panel) { panels.push_back(_panel); }
void GUIWindow::addButtonPanel(ButtonPanel *_bt) { buttons.push_back(_bt); }

GUIWindow::~GUIWindow() {}

void GUIWindow::drawPanels() {
    for (auto x : panels) {
        x->drawPanel(screen);
    }
}

void GUIWindow::drawButtons() {
    for (auto x : buttons) {
        x->drawButton(screen);
    }
}

void GUIWindow::refresh() {}
