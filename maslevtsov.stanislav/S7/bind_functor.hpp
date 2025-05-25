#ifndef BIND_FUNCTOR_HPP
#define BIND_FUNCTOR_HPP

#include <iostream>

namespace maslevtsov {
  template< class T >
  struct IOFuncBinder
  {
    using iofunc_t = void (*)(const T&, std::istream&, std::ostream&);
    using ifunc_t = void (*)(T&, std::istream&);
    using ofunc_t = void (*)(const T&, std::ostream&);

    IOFuncBinder():
      iofunc_(nullptr),
      ifunc_(nullptr),
      ofunc_(nullptr),
      arg_(T()),
      in_(nullptr),
      out_(nullptr)
    {}

    IOFuncBinder(iofunc_t iofunc, const T& arg, std::istream& in, std::ostream& out):
      iofunc_(iofunc),
      arg_(arg),
      in_(&in),
      out_(&out)
    {}

    IOFuncBinder(ifunc_t ifunc, T& arg, std::istream& in):
      ifunc_(ifunc),
      arg_(arg),
      in_(&in)
    {}

    IOFuncBinder(ofunc_t ofunc, const T& arg, std::ostream& out):
      ofunc_(ofunc),
      arg_(arg),
      out_(&out)
    {}

    IOFuncBinder& operator=(const IOFuncBinder& rhs)
    {
      iofunc_ = rhs.iofunc_;
      ifunc_ = rhs.ifunc_;
      ofunc_ = rhs.ofunc_;
      arg_ = rhs.arg_;
      in_ = rhs.in_;
      out_ = rhs.out_;
      return *this;
    }

    void operator()() const
    {
      if (in_ && out_) {
        iofunc_(arg_, *in_, *out_);
      } else if (in_ && !out_) {
        ifunc_(const_cast< T& >(arg_), *in_);
      } else if (!in_ && out_) {
        ofunc_(arg_, *out_);
      }
    }

  private:
    iofunc_t iofunc_ = nullptr;
    ifunc_t ifunc_ = nullptr;
    ofunc_t ofunc_ = nullptr;
    T arg_;
    std::istream* in_ = nullptr;
    std::ostream* out_ = nullptr;
  };
}

#endif
