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
    m_widget->setAlignment(tgui::EditBox::Alignment::Center);
}

Edit& Edit::small() {
    title_size(18);
    return size(100, 30).text_size(18);
}

Edit& Edit::medium() {
    title_size(24);
    return size(150, 40).text_size(24);
}

Edit& Edit::large() {
    title_size(32);
    return size(200, 50).text_size(32);
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

Slider& Slider::min(float value) {
    m_widget->setMinimum(value);
    return *this;
}

Slider& Slider::max(float value) {
    m_widget->setMaximum(value);
    return *this;
}

Slider& Slider::step(float value) {
    m_widget->setStep(value);
    return *this;
}

}  // namespace SB
