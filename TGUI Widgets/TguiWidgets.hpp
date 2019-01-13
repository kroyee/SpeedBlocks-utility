#ifndef TGUI_WIDGETS
#define TGUI_WIDGETS

#include <TGUI/TGUI.hpp>

namespace SB {

inline tgui::Theme defaultTheme;

template <typename WidgetType, typename ReturnType>
class Widget {
   public:
    Widget() : m_widget(WidgetType::create()) {}

    ReturnType& pos(float x, float y) {
        m_widget->setPosition(x, y);
        return static_cast<ReturnType&>(*this);
    }

    ReturnType& pos(const sf::Vector2f& position) {
        return pos(position.x, position.y);
    }

    ReturnType& size(float x, float y) {
        m_widget->setSize(x, y);
        return static_cast<ReturnType&>(*this);
    }

    ReturnType& size(const sf::Vector2f& vec) { return size(vec.x, vec.y); }

    ReturnType& add(tgui::Panel::Ptr panel) {
        panel->add(m_widget);
        return static_cast<ReturnType&>(*this);
    }

    typename WidgetType::Ptr& operator->() { return m_widget; }

    typename WidgetType::Ptr& get() { return m_widget; }

   protected:
    typename WidgetType::Ptr m_widget;
};

template <typename WidgetType, typename ReturnType>
class TextWidget : public Widget<WidgetType, ReturnType> {
    using WType = Widget<WidgetType, ReturnType>;

   public:
    ReturnType& text(std::string text) {
        this->m_widget->setText(std::move(text));
        return static_cast<ReturnType&>(*this);
    }

    ReturnType& text_size(unsigned size) {
        this->m_widget->setTextSize(size);
        return static_cast<ReturnType&>(*this);
    }
};

using tgui::bindBottom;
using tgui::bindHeight;
using tgui::bindLeft;
using tgui::bindPosition;
using tgui::bindRight;
using tgui::bindSize;
using tgui::bindTop;
using tgui::bindWidth;

class Button : public TextWidget<tgui::Button, Button> {
   public:
    Button& small();
    Button& medium();
    Button& large();
};

class Label : public TextWidget<tgui::Label, Label> {
   public:
    Label& small();
    Label& medium();
    Label& large();
};

class Edit : public TextWidget<tgui::EditBox, Edit> {
   public:
    Edit(const std::string& title = "");

    Edit& small();
    Edit& medium();
    Edit& large();

    Edit& title(const std::string& text);
    Edit& title_size(unsigned size);

    Edit& title_top() {
        m_title_top = true;
        update_title();
        return *this;
    }
    Edit& title_left() {
        m_title_top = false;
        update_title();
        return *this;
    }

    Edit& add(tgui::Panel::Ptr panel);
    Edit& num();
    Edit& pass();

   private:
    void update_title();
    Label m_title;
    bool m_title_top = false;
};

class CheckBox : public TextWidget<tgui::CheckBox, CheckBox> {
   public:
    CheckBox& small();
    CheckBox& medium();
    CheckBox& large();
};

class RadioButton : public TextWidget<tgui::RadioButton, RadioButton> {
   public:
    RadioButton& small();
    RadioButton& medium();
    RadioButton& large();
};

class ListBox : public Widget<tgui::ListBox, ListBox> {
   public:
};

}  // namespace SB

#endif
