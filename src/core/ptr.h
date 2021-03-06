#pragma once

#include <memory>

namespace atom {

template<typename T, typename D = std::default_delete<T>>
using uptr = std::unique_ptr<T, D>;

template<typename T>
using sptr = std::shared_ptr<T>;

}
