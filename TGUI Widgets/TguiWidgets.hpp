#ifndef TGUI_WIDGETS
#define TGUI_WIDGETS

#include <TGUI/TGUI.hpp>
#include "WidgetAlign.h"

namespace os {

inline tgui::Theme defaultTheme;

template <typename WidgetType, typename ReturnType>
class Widget {
   public:
    Widget() : m_widget(WidgetType::create()) {}

    ReturnType& pos(float x, float y) {
        m_widget->setPosition(x, y);
        return static_cast<ReturnType&>(*this);
    }

    ReturnType& pos(const sf::Vector2f& position) { return pos(position.x, position.y); }

    ReturnType& size(float x, float y) {
        m_widget->setSize(x, y);
        return static_cast<ReturnType&>(*this);
    }

    ReturnType& size(const sf::Vector2f& vec) { return size(vec.x, vec.y); }

    ReturnType& width(float w) { return size(w, m_widget->getSize().y); }

    ReturnType& height(float h) { return size(m_widget->getSize().x, h); }

    ReturnType& show() {
        m_widget->setVisible(false);
        return static_cast<ReturnType&>(*this);
    }

    ReturnType& hide() {
        m_widget->setVisible(true);
        return static_cast<ReturnType&>(*this);
    }

    ReturnType& enable() {
        m_widget->setEnabled(true);
        return static_cast<ReturnType&>(*this);
    }

    ReturnType& disable() {
        m_widget->setEnabled(false);
        return static_cast<ReturnType&>(*this);
    }

    ReturnType& add_to(Panel panel) {
        panel->add(m_widget);
        return static_cast<ReturnType&>(*this);
    }

    ReturnType& del() { m_widget->getParent()->remove(m_widget); }

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

    ReturnType& center() {
        if constexpr (std::is_same_v < WidgetType, tgui::Label)
            this->m_widget->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
        else if constexpr (std::is_same_v < WidgetType, tgui::EditBox)
            this->m_widget->setAlignment(tgui::EditBox::Alignment::Center);

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

class Label : public TextWidget<tgui::Label, Label> {
   public:
    Label& small();
    Label& medium();
    Label& large();
};

template <typename WidgetType>
class TitledWidget;

template <template <typename, typename> typename WidgetBase, typename WidgetType, typename ReturnType>
class TitledWidget<WidgetBase<WidgetType, ReturnType>> : public WidgetBase<WidgetType, ReturnType> {
   public:
    TitledWidget() { update_title(); }

    ReturnType& title(const std::string& text) {
        m_title.text(text);
        return static_cast<ReturnType&>(*this);
    }

    ReturnType& title_size(unsigned size) {
        m_title.text_size(size);
        return static_cast<ReturnType&>(*this);
    }

    ReturnType& title_top() {
        m_title_top = true;
        update_title();
        return static_cast<ReturnType&>(*this);
    }
    ReturnType& title_left() {
        m_title_top = false;
        update_title();
        return static_cast<ReturnType&>(*this);
    }

    ReturnType& add(tgui::Panel::Ptr panel) {
        panel->add(m_title.get());
        panel->add(this->m_widget);
        return static_cast<ReturnType&>(*this);
    }

    ReturnType& bind() {}

   protected:
    Label m_title;

   private:
    bool m_title_top = false;
    void update_title() {
        if (m_title_top)
            m_title->setPosition(bindLeft(this->m_widget) + (bindWidth(this->m_widget) - bindWidth(m_title.get())) / 2,
                                 bindTop(this->m_widget) - bindHeight(m_title.get()) - 5);
        else
            m_title->setPosition(bindLeft(this->m_widget) - bindWidth(m_title.get()) - 10, bindTop(this->m_widget) + 5);
    }
};

class Button : public TextWidget<tgui::Button, Button> {
   public:
    Button& small();
    Button& medium();
    Button& large();

    template <typename... T>
    Button& connect(T&&... t) {
        m_widget->connect(std::forward<T>(t)...);
    }
};

class Edit : public TitledWidget<TextWidget<tgui::EditBox, Edit>> {
   public:
    Edit(const std::string& title = "");

    Edit& small();
    Edit& medium();
    Edit& large();

    Edit& num();
    Edit& pass();
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

class ListBox : public Widget<tgui::ListBox, ListBox> {};

class TextBox : public Widget<tgui::TextBox, TextBox> {};

class ChatBox : public Widget<tgui::ChatBox, ChatBox> {};

class Slider : public TitledWidget<TextWidget<tgui::Slider, Slider>> {
    Slider& min(float value);
    Slider& max(float value);
    Slider& step(float value);
};

class Panel : public Widget<tgui::Panel, Panel> {
   public:
    template <typename... WType>
    Panel& add(WType&&... widget) {
        (std::forward<WType>(widget).add(m_widget), ...);
        return *this;
    }

    Panel& background();

    Panel& background(const sf::Color& color);

    Align<> align(int x, int y, int gap = 0, bool down = true) { return Align<>(*this, x, y, gap, down); }

    template <typename... T>
    Align<T...> align(int x, int y, int gap = 0, bool down = true) {
        return Align<T...>(*this, x, y, gap, down);
    }

    template <typename W>
    Align<> under(W& w) {
        return {w};
    }

    template <typename W>
    Align<> right_of(W& w) {
        return (Align<>{w, 0, false} << Vertical{0});
    }
};

}  // namespace os

#endif
