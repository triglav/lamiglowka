#pragma once

#include <vector>

#include "core/slot.h"

namespace tajga {
namespace core {

template <typename Signature>
class Signal {
 public:
  Signal() = default;
  ~Signal() {
    // deletes all connections
    slots_.clear();
  }

  Signal(Signal<Signature> const &) = delete;
  Signal<Signature> & operator= (Signal<Signature> const &) = delete;

//  Signal(Signal &&) = default;
//  Signal & operator= (Signal &&) = default;

  template <typename ... Args>
    void Connect(Args && ... args) {
      slots_.emplace_back(std::forward<Args>(args)...);
    }

  template <typename ... Args>
    void Disconnect(Args && ... args) {
      auto it = std::find(begin(slots_), end(slots_),
          Slot<Signature>(std::forward<Args>(args)...));
      if (it != end(slots_)) {
        slots_.erase(it);
      }
    }


  template <typename ... Args>
    void Emit(Args && ... args) const {
      for (auto const & s : slots_) {
        c->Invoke(std::forward<Args>(args)...);
      }
    }

//  template <typename ... Args>
//  void operator() (Args && ... args) const {
//    Emit(std::forward<args>(args)...);
//  }

 private:
  std::vector<Slot<Signature> > slots_;
};  // class Signal

}  // namespace core
}  // namespace tajga

