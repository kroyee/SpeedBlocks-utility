#ifndef BUTTONGRID_HPP
#define BUTTONGRID_HPP

#include <vector>

#include "TguiWidgets.hpp"

namespace os {

class ButtonGrid : public PanelType<tgui::ScrollablePanel, ButtonGrid> {
   public:
    ButtonGrid();

    template <class... Buttons>
    ButtonGrid& add(Buttons... buttons) {
        (wrap->add(buttons.get()), ...);
        if (padding_size) {
            //(buttons.padding(padding_size), ...);
        }
        return *this;
    }

    LabeledButton& get_button(uint16_t id);
    ButtonGrid& del(uint16_t id);

    ButtonGrid& padding(float p);

   private:
    struct ButtonID {
        uint16_t id;
        LabeledButton button;
    };
    std::vector<ButtonID> button_ids;
    tgui::HorizontalWrap::Ptr wrap;
    float padding_size = 0;
};

}  // namespace os

#endif  // BUTTONGRID_HPP