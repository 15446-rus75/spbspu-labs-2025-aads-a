#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S1_LIST_UTILS_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S1_LIST_UTILS_HPP

#include "forward-list.hpp"
#include "type-utils.hpp"

namespace kizhin {
  NumbersT::value_type safeAccumulate(const NumbersT&, NumbersT::value_type = {});
  ForwardList< NumbersT > convert(const ForwardList< NumbersT >&);
}

#endif
