#ifndef SCROLLPANEL_HPP
#define SCROLLPANEL_HPP

#include <algorithm>
#include <vector>
#include "TguiWidgets.hpp"

namespace os {

class ScrollList : public ScrollPanel {
   private:
    class Row {
       public:
        Row(uint16_t id, std::vector<tgui::Widget::Ptr>&& w) : id(id), widgets(std::move(w)) {}

        void del() {
            for (auto& w : widgets) w->getParent()->remove(w);
        }

        uint16_t id;
        bool selected = false;
        void pos(float y) {
            for (auto& w : widgets) w->setPosition(w->getPosition().x, y - w->getSize().y / 2.f);
        }

       private:
        std::vector<tgui::Widget::Ptr> widgets;
    };

    void update_positions();

    std::vector<Row> rows;
    std::vector<int> positions;
    Panel select_highlight;
    float height = 0;

   public:
    ScrollList();

    ScrollList& set_positions(std::vector<int> pos) { positions = std::move(pos); }

    template <class... Pos>
    ScrollList& set_positions(Pos... pos) {
        positions = {pos...};
        return *this;
    }

    ScrollList& row_height(float h) {
        height = h;
        return *this;
    }

    ScrollList& add(uint16_t id, std::vector<tgui::Widget::Ptr> w);

    template <class... Widgets>
    ScrollList& add(uint16_t id, Widgets&&... widgets) {
        std::vector<tgui::Widget::Ptr> w{widgets.get()...};
        add(id, std::move(w));
    }

    ScrollList& del(uint16_t id);

    std::size_t size() { return rows.size(); }

    ScrollList& pop_back() {
        if (!rows.empty()) {
            rows.back().del();
            rows.pop_back();
        }
    }
};

}  // namespace os

#endif  // SCROLLPANEL_HPP