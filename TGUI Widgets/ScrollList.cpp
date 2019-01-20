#include "ScrollList.hpp"

#include <iostream>

namespace os {

ScrollList::ScrollList() {
    m_widget->setHorizontalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
    m_widget->add(select_highlight.hide().get());
    select_highlight->setRenderer(defaultTheme.getRenderer("HighlightPanel"));
    m_widget->connect("MousePressed", [&](tgui::Vector2f pos) {
        int row_num = 0;
        while (pos.y > height) {
            ++row_num;
            pos.y -= height;
        }

        rows[row_num].selected = true;

        select_highlight.pos(0, row_num * height).size(m_widget->getSize().x - m_widget->getScrollbarWidth(), height).show();
        select_highlight.show();
    });
}

void ScrollList::update_positions() {
    auto pos = height / 2;
    for (auto& row : rows) {
        row.pos(pos);
        pos += height;
    }
}

ScrollList& ScrollList::del(uint16_t id) {
    for (auto it = rows.begin(), end = rows.end(); it != end; ++it) {
        if (it->id == id) {
            it->del();
            rows.erase(it);
            break;
        }
    }

    update_positions();
    return *this;
}

}  // namespace os