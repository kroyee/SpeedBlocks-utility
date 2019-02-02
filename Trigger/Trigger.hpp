#ifndef Trigger_HPP
#define Trigger_HPP

#include <functional>
#include <utility>
#include <vector>

namespace os {

template <class CallbackCollection, class... Data>
class OnOffSwitch {
   public:
    using Action = std::function<void(const Data&...)>;
    using Element = std::pair<bool, Action>;
    using Container = std::vector<Element>;

    OnOffSwitch(Container& c) : container(c), index(c.size() - 1) {}

    void on() { container[index].first = true; }
    void off() { container[index].first = false; }

    friend CallbackCollection;

   private:
    static Element make_element(Action a) { return std::make_pair(true, std::move(a)); }
    static OnOffSwitch make_switch(Container& c) { return {c}; }

    static void make_action(Element& element, const Data&... data) {
        if (element.first) element.second(data...);
    }

    Container& container;
    std::size_t index;
};

template <class CallbackCollection, class... Data>
struct NoSwitch {
    using Action = std::function<void(const Data&...)>;
    using Container = std::vector<Action>;

    friend CallbackCollection;

   private:
    static Action make_element(Action a) { return {std::move(a)}; }
    static void make_switch(Container&) {}
    static void make_action(Action& action, const Data&... data) { action(data...); }
};

template <class Data, template <class...> class SwitchType, class... Arg>
class CallbackCollection_impl {
   public:
    using Switch = SwitchType<CallbackCollection_impl, Arg...>;

    template <typename F>
    auto add(F f) {
        actions.push_back(Switch::make_element(std::move(f)));

        return Switch::make_switch(actions);
    }

    void trigger(const Arg&... data) {
        for (auto& a : actions) Switch::make_action(a, data...);
    }

   private:
    typename Switch::Container actions;
};

template <class Data, template <class...> class SwitchType, class = void>
struct CallbackCollectionChooser {
    using Type = CallbackCollection_impl<Data, SwitchType, Data>;
};

template <class Data, template <class...> class SwitchType>
struct CallbackCollectionChooser<Data, SwitchType, std::enable_if_t<std::is_empty_v<Data>>> {
    using Type = CallbackCollection_impl<Data, SwitchType>;
};

template <typename Data, template <class...> class SwitchType = OnOffSwitch>
using CallbackCollection = typename CallbackCollectionChooser<Data, SwitchType>::Type;

template <class Data, class = void>
struct Event {
   public:
    template <class F>
    static auto add(F f) {
        return callbacks.add(std::move(f));
    }

    static void trigger(const Data& data) { callbacks.trigger(data); }

   private:
    inline static CallbackCollection<Data> callbacks;
};

template <class Data>
struct Event<Data, std::enable_if_t<std::is_empty_v<Data>>> {
   public:
    template <class F>
    static auto add(F f) {
        return callbacks.add(std::move(f));
    }

    static void trigger() { callbacks.trigger(); }

   private:
    inline static CallbackCollection<Data> callbacks;
};

template <class T>
using Switch = typename CallbackCollection<T>::Switch;

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