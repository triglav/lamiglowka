#pragma once

#include <FastDelegate.h>
#include <glog/logging.h>

namespace tajga {
namespace core {

template <typename Signature>
class Slot {
 public:
  Slot() = default;
  explicit Slot(Signature fn)
    : delegate_(fn) {
    }

  template <typename T, typename MemberFunctionPointer>
    Slot(T * obj, MemberFunctionPointer fn)
    : delegate_(obj, fn) {
    }

  ~Slot() = default;

  Slot(Slot<Signature> const & other)
    : delegate_(other.delegate_) {
    }

  Slot<Signature> & operator= (Slot<Signature> const & other) {
    if (this != &other) {
      delegate_ = other.delegate_;
    }
    return *this;
  }

//  Slot(Slot &&) = default;
//  Slot & operator= (Slot &&) = default;

  template <typename ... Args>
    void Invoke(Args && ... args) const {
      CHECK(!delegate_.empty()) << "Can not invoke an empty delegate!";
      delegate_(std::forward<Args>(args)...);
    }

  template <typename ... Args>
    void operator() (Args && ... args) const {
      Invoke(std::forward<Args>(args)...);
    }

  bool IsEmpty() const { return delegate_.empty(); }

  static Slot<Signature> Bind(Signature fn) {
    return Slot<Signature>(fn);
  }

  template <typename T, typename MemberFunctionPointer>
    static Slot<Signature> Bind(T * obj, MemberFunctionPointer fn) {
      return Slot<Signature>(obj, fn);
    }

  bool operator==(Slot<Signature> const & other) const {
    return delegate_ == other.delegate_;
  }

  bool operator!=(Slot<Signature> const & other) const {
    return !operator==(other);
  }

  bool operator<(Slot<Signature> const & other) const {
    return delegate_ < other.delegate_;
  }

  bool operator>(Slot<Signature> const & other) const {
    return delegate_ > other.delegate_;
  }

  bool operator<=(Slot<Signature> const & other) const {
    return !operator>(other);
  }

  bool operator>=(Slot<Signature> const & other) const {
    return !operator<(other);
  }

 private:
  fastdelegate::FastDelegate<Signature> delegate_;
};  // class Slot

}  // namespace core
}  // namespace tajga

