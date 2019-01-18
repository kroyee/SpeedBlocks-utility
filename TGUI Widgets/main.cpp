#include <SFML/Window.hpp>
#include <iostream>
#include "TguiWidgets.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    tgui::Gui gui(window);

    SB::defaultTheme.load("neutralUI.txt");
    tgui::Theme::setDefault(&SB::defaultTheme);

    tgui::Panel::Ptr panel = tgui::Panel::create();
    panel->setSize(800, 600);

    SB::Edit().text("Hey").add(panel).small().pos(200, 0).title("Hey yah");
    SB::Edit().text("Hey").add(panel).medium().pos(200, 200).title("super long and wierd title");
    SB::Edit().text("Hey").add(panel).large().pos(200, 400).title("Yo!").title_top();

    gui.add(panel);

    // run the program as long as the window is open
    while (window.isOpen()) {
        // check all the window's events that were triggered since the last
        // iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed) window.close();

            gui.handleEvent(event);
        }

        gui.draw();
        window.display();
    }

    return 0;
}
