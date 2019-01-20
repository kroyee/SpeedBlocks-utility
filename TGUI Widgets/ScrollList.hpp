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

    template <class... Pos>
    ScrollList& set_positions(Pos... pos) {
        positions = {pos...};
        return *this;
    }

    ScrollList& row_height(float h) {
        height = h;
        return *this;
    }

    template <class... Widgets>
    ScrollList& add(uint16_t id, Widgets&&... widgets) {
        std::vector<tgui::Widget::Ptr> w{widgets.get()...};
        for (std::size_t i = 0, end = std::min(w.size(), positions.size()); i < end; ++i) {
            w[i]->setPosition(positions[i], 0);
            m_widget->add(w[i]);
        }
        for (auto& wid : w)
            if (wid->getSize().y + 10 > height) height = wid->getSize().y + 10;

        if (!std::any_of(rows.begin(), rows.end(), [&](Row& row) {
                if (row.id == id) {
                    row.del();
                    row = Row(id, std::move(w));
                    return true;
                }
                return false;
            })) {
            rows.emplace_back(id, std::move(w));
        }

        update_positions();
        return *this;
    }

    ScrollList& del(uint16_t id);
};

}  // namespace os

#endif  // SCROLLPANEL_HPP