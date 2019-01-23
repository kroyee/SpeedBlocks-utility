#ifndef Trigger_HPP
#define Trigger_HPP

#include <functional>
#include <utility>
#include <vector>

namespace os {

template <class Data>
class OnOffSwitch {
   public:
    using Action = std::function<void(const Data&)>;
    using Element = std::pair<bool, Action>;
    using Container = std::vector<Element>;

    OnOffSwitch(Container& c) : container(c), index(c.size() - 1) {}

    void on() { container[index].first = true; }
    void off() { container[index].first = false; }

    static Element make_element(Action a) { return std::make_pair(true, std::move(a)); }
    static OnOffSwitch make_switch(Container& c) { return {c}; }
    static void make_action(Element& element, const Data& data) {
        if (element.first) element.second(data);
    }

   private:
    Container& container;
    std::size_t index;
};

template <class Data>
struct NoSwitch {
    using Action = std::function<void(const Data&)>;
    using Container = std::vector<Action>;

    static Action make_element(Action a) { return {std::move(a)}; }
    static void make_switch(Container&) {}
    static void make_action(Action& action, const Data& data) { action(data); }
};

template <class Data, template <class> class SwitchType = OnOffSwitch>
class CallbackCollection {
   public:
    using Switch = SwitchType<Data>;

    template <typename F>
    auto add_callback(F f) {
        actions.push_back(Switch::make_element(std::move(f)));

        return Switch::make_switch(actions);
    }

    void trigger_callbacks(const Data& data) {
        for (auto& a : actions) Switch::make_action(a, data);
    }

   private:
    typename Switch::Container actions;
};

template <typename Data>
class Trigger : public CallbackCollection<Data> {
   public:
    void set(Data data) { value = std::move(data); }

    template <class... T>
    Trigger(T&&... t) : value(std::forward<T>(t)...) {}

    Trigger& operator=(Data data) {
        value = std::move(data);
        trigger_callbacks(value);

        return *this;
    }

    operator Data&() { return value; }

   private:
    Data value;
};

}  // namespace os

#endif  // Trigger_HPP