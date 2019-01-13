#include "TguiWidgets.hpp"

namespace SB {
Button& Button::small() { return size(100, 30).text_size(18); }

Button& Button::medium() { return size(150, 40).text_size(24); }

Button& Button::large() { return size(200, 50).text_size(32); }

Label& Label::small() { return text_size(18); }

Label& Label::medium() { return text_size(24); }

Label& Label::large() { return text_size(32); }

Edit::Edit(const std::string& title_) {
    title(title_);
    update_title();
}

Edit& Edit::small() {
    title_size(16);
    return size(100, 30).text_size(18);
}

Edit& Edit::medium() {
    title_size(22);
    return size(150, 40).text_size(24);
}

Edit& Edit::large() {
    title_size(30);
    return size(200, 50).text_size(32);
}

Edit& Edit::title(const std::string& text) {
    m_title.text(text);
    return *this;
}

Edit& Edit::title_size(unsigned size) {
    m_title.text_size(size);
    return *this;
}

void Edit::update_title() {
    if (m_title_top)
        m_title->setPosition(
            bindLeft(m_widget) +
                (bindWidth(m_widget) - bindWidth(m_title.get())) / 2,
            bindTop(m_widget) - bindHeight(m_title.get()) - 5);
    else
        m_title->setPosition(bindLeft(m_widget) - bindWidth(m_title.get()) - 10,
                             bindTop(m_widget) + 5);
}

Edit& Edit::add(tgui::Panel::Ptr panel) {
    panel->add(m_title.get());
    panel->add(m_widget);
    return *this;
}

Edit& Edit::num() {
    m_widget->setInputValidator(tgui::EditBox::Validator::UInt);
    return *this;
}

Edit& Edit::pass() {
    m_widget->setPasswordCharacter('*');
    return *this;
}

CheckBox& CheckBox::small() { return size(10, 10).text_size(18); }

CheckBox& CheckBox::medium() { return size(20, 20).text_size(24); }

CheckBox& CheckBox::large() { return size(30, 30).text_size(32); }

RadioButton& RadioButton::small() { return size(10, 10).text_size(18); }

RadioButton& RadioButton::medium() { return size(20, 20).text_size(24); }

RadioButton& RadioButton::large() { return size(30, 30).text_size(32); }
}  // namespace SB
