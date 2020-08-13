#include "simdjson/error.h"

namespace {
namespace SIMDJSON_IMPLEMENTATION {
namespace ondemand {

/**
 * A forward-only JSON array.
 */
class array {
public:
  really_inline array() noexcept;

  //
  // Iterator interface
  //
  really_inline simdjson_result<ondemand::value> operator*() noexcept;
  really_inline bool operator!=(array &other) noexcept;
  really_inline array &operator++() noexcept;
  really_inline array begin() noexcept;
  really_inline array end() noexcept;

protected:
  really_inline array(ondemand::value &parent, error_code _error=SUCCESS) noexcept;

  really_inline void advance() noexcept;
  really_inline static simdjson_result<array> try_begin(ondemand::value &parent) noexcept;
  really_inline static array begin(ondemand::value &parent) noexcept;

  ondemand::value value;
  error_code error;

  friend class ondemand::value;
  friend struct simdjson_result<ondemand::value>;
  friend struct simdjson_result<ondemand::array>;
};

} // namespace ondemand
} // namespace SIMDJSON_IMPLEMENTATION
} // namespace {

namespace simdjson {

template<>
struct simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::array> : public internal::simdjson_result_base<SIMDJSON_IMPLEMENTATION::ondemand::array> {
public:
  really_inline simdjson_result(SIMDJSON_IMPLEMENTATION::ondemand::array &&value) noexcept; ///< @private
  really_inline simdjson_result(SIMDJSON_IMPLEMENTATION::ondemand::value &parent, error_code error) noexcept; ///< @private

  really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::value> operator[](std::string_view key) noexcept;

  really_inline SIMDJSON_IMPLEMENTATION::ondemand::array begin() noexcept;
  really_inline SIMDJSON_IMPLEMENTATION::ondemand::array end() noexcept;
};

} // namespace simdjson
