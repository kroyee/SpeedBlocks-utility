#include "ButtonGrid.hpp"

namespace os {

ButtonGrid::ButtonGrid() {
    wrap = tgui::HorizontalWrap::create();
    wrap->setSize(bindSize(m_widget));
    m_widget->add(wrap);
}

LabeledButton& ButtonGrid::get_button(uint16_t id) {
    static LabeledButton def;

    for (auto& b : button_ids)
        if (b.id == id) return b.button;

    return def;
}

ButtonGrid& ButtonGrid::del(uint16_t id) {
    for (auto& b : button_ids)
        if (b.id == id) wrap->remove(b.button.get());

    return *this;
}

ButtonGrid& ButtonGrid::padding(float p) {
    padding_size = p;
    for (auto& b : button_ids) b.button.padding(p);

    return *this;
}

}  // namespace os