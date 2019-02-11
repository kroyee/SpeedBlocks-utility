#ifndef GUISERIALIZE_HPP
#define GUISERIALIZE_HPP

#include <vector>
#include "../Network packets/NetworkPacketHandler.hpp"
#include "../helpers/MyTypeTraits.hpp"

struct GUI_Property {
    GUI_Property(uint8_t id, std::vector<uint16_t> d) : id(id), data(std::move(d)) {}
    GUI_Property(uint8_t id) : id(id) {}

    uint8_t id;
    std::vector<uint16_t> data;
};

struct GUI_Widget {
    GUI_Widget(uint8_t id, std::vector<GUI_Property> p, std::vector<GUI_Widget> w) : id(id), properties(std::move(p)), children(std::move(w)) {}
    GUI_Widget(uint8_t id, std::vector<GUI_Property> p) : id(id), properties(std::move(p)) {}

    uint8_t id;
    std::vector<GUI_Property> properties;
    std::vector<GUI_Widget> children;
};

PACKET(NP_GUIContainer) { std::vector<GUI_Widget> widgets; };

namespace os {

namespace GUI_Serialize {

template <class T>
using has_text_size = decltype(std::declval<T>().text_size(0));

template <class T>
constexpr bool has_text_size_v = os::detect_v<has_text_size, T>;

template <class T>
using has_center = decltype(std::declval<T>().center());

template <class T>
constexpr bool has_center_v = os::detect_v<has_center, T>;

enum GUI_Properties { POS, SIZE, TEXT_SIZE, HIDE, CENTER, IDENTIFIER, TARGET, PROPERTY_COUNT };

enum GUI_Targets { BACK, TARGETID, TARGET_COUNT };

enum GUI_WidgetID { LABEL, BUTTON, EDIT, BUTTONGRID, SCROLLLIST, SLIDER, TITLEDBUTTON, WIDGET_COUNT };

template <class Widget>
auto make_widget(const std::vector<GUI_Property>& properties) {
    Widget w;

    std::array<void (*)(Widget&, std::vector<uint16_t>&), PROPERTY_COUNT> property_functions;

    property_functions[POS] = [](Widget& w, std::vector<uint16_t>& data) { w.pos(data[0], data[1]); };
    property_functions[SIZE] = [](Widget& w, std::vector<uint16_t>& data) { w.size(data[0], data[1]); };
    property_functions[TEXT_SIZE] = [](Widget& w, std::vector<uint16_t>& data) {
        if constexpr (has_text_size_v<Widget>) w.text_size(data[0]);
    };
    property_functions[HIDE] = [](Widget& w, std::vector<uint16_t>& data) { w->setVisible(data[0]) };
    property_functions[CENTER] = [](Widget& w, std::vector<uint16_t>& data) {
        if constexpr (has_center_v<Widget>) w.center();
    };
    property_functions[IDENTIFIER] = [](Widget& w, std::vector<uint16_t>& data) { w.indentifier(data[0]); };

    for (auto& p : properties) property_functions[p.id](w, p.data);

    return w;
}

}  // namespace GUI_Serialize

}  // namespace os

#endif