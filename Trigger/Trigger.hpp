#ifndef Trigger_HPP
#define Trigger_HPP

#include <functional>
#include <utility>
#include <vector>

namespace os {

template <typename T>
class Trigger {
   public:
    using Action = std::function<void(T)>;
    using Pair = std::pair<bool, Action>;
    using Container = std::vector<Pair>;

    class TriggerSwitch {
       public:
        TriggerSwitch(Container& c, std::size_t p) : cont(c), pos(p) {}
        void on() { cont[pos].first = true; }

        void off() { cont[pos].first = false; }

       private:
        Container& cont;
        const std::size_t pos;
    };

    void set(T t) { value = std::move(t); }

    Trigger& operator=(T t) {
        value = std::move(t);

        for (auto& f : actions)
            if (f.first) f.second(value);

        return *this;
    }

    operator T&() { return value; }

    template <typename F>
    TriggerSwitch on_update(F f) {
        actions.emplace_back(true, std::move(f));

        return {actions, actions.size() - 1};
    }

   private:
    T value{};

    Container actions;
};

}  // namespace os

#endif  // Trigger_HPP