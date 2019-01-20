#ifndef Events_HPP
#define Events_HPP

#include <functional>
#include <utility>
#include <vector>

namespace os {

template <class Data>
class Events {
   private:
    using Function = std::function<void(Data&)>;
    using Callback = std::pair<bool, Function>;
    static inline std::vector<Callback> callbacks;

    class EventSwitch {
       public:
        EventSwitch(std::vector<Callback>& vec, std::size_t index) : vec(vec), index(index) {}
        void on() { vec[index].first = true; }
        void off() { vec[index].first = false; }

       private:
        std::vector<Callback>& vec;
        std::size_t index;
    };

   public:
    template <class Functor>
    static EventSwitch add(Functor&& f) {
        callbacks.emplace_back(true, Function(std::forward<Functor>(f)));
        return {callbacks, callbacks.size() - 1};
    }

    static void push(Data data) {
        for (auto& cb : callbacks)
            if (cb.first) cb.second(data);
    }
};

}  // namespace os

#endif  // Events_HPP