#include "simdjson/error.h"

namespace {
namespace SIMDJSON_IMPLEMENTATION {
namespace ondemand {

class array;
class json;
class object;
class raw_json_string;

/**
 * An ephemeral JSON value returned during iteration.
 *
 * Invalidated when any other iteration occurs, including child array/object iteration.
 */
class value {
public:
  really_inline value() noexcept;
  really_inline value(const value &other) noexcept;
  really_inline simdjson_result<array> get_array() && noexcept;
  really_inline simdjson_result<object> get_object() && noexcept;
  really_inline simdjson_result<uint64_t> get_uint64() && noexcept;
  really_inline simdjson_result<int64_t> get_int64() && noexcept;
  really_inline simdjson_result<double> get_double() && noexcept;
  really_inline simdjson_result<std::string_view> get_string() && noexcept;
  really_inline simdjson_result<raw_json_string> get_raw_json_string() && noexcept;
  really_inline simdjson_result<bool> get_bool() && noexcept;
  really_inline bool is_null() noexcept;

#if SIMDJSON_EXCEPTIONS
  really_inline operator array() && noexcept(false);
  really_inline operator object() && noexcept(false);
  really_inline operator uint64_t() && noexcept(false);
  really_inline operator int64_t() && noexcept(false);
  really_inline operator double() && noexcept(false);
  really_inline operator std::string_view() && noexcept(false);
  really_inline operator raw_json_string() && noexcept(false);
  really_inline operator bool() && noexcept(false);
#endif

  really_inline array begin() noexcept;
  really_inline array end() noexcept;
  really_inline simdjson_result<value> operator[](std::string_view key) && noexcept;

protected:
  really_inline value(ondemand::json *json, uint32_t depth) noexcept;
  /** Get to the end of any child arrays and objects we started but didn't finish */
  really_inline void finish() noexcept;
  really_inline const uint8_t *consume() noexcept;

  ondemand::json *json;
  uint32_t depth;
  bool consumed{false};
  friend struct simdjson_result<value>;
  friend struct simdjson_result<object>;
  friend struct simdjson_result<ondemand::json>;
  friend class ondemand::json;
  friend class field;
  friend class object;
  friend class array;
};

} // namespace ondemand
} // namespace SIMDJSON_IMPLEMENTATION
} // namespace {

namespace simdjson {

template<>
struct simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::value> : public internal::simdjson_result_base<SIMDJSON_IMPLEMENTATION::ondemand::value> {
public:
  really_inline simdjson_result(SIMDJSON_IMPLEMENTATION::ondemand::value &&value) noexcept; ///< @private
  really_inline simdjson_result(SIMDJSON_IMPLEMENTATION::ondemand::value &&value, error_code error) noexcept; ///< @private
  really_inline simdjson_result(const SIMDJSON_IMPLEMENTATION::ondemand::value &value, error_code error=SUCCESS) noexcept; ///< @private
  // TODO not sure why I had to make this.
  really_inline simdjson_result(const simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::value> &other) noexcept;

  really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::array> get_array() && noexcept;
  really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::object> get_object() && noexcept;
  really_inline simdjson_result<uint64_t> get_uint64() && noexcept;
  really_inline simdjson_result<int64_t> get_int64() && noexcept;
  really_inline simdjson_result<double> get_double() && noexcept;
  really_inline simdjson_result<std::string_view> get_string() && noexcept;
  really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::raw_json_string> get_raw_json_string() && noexcept;
  really_inline simdjson_result<bool> get_bool() && noexcept;
  really_inline bool is_null() noexcept;

#if SIMDJSON_EXCEPTIONS
  really_inline operator SIMDJSON_IMPLEMENTATION::ondemand::array() && noexcept(false);
  really_inline operator SIMDJSON_IMPLEMENTATION::ondemand::object() && noexcept(false);
  really_inline operator uint64_t() && noexcept(false);
  really_inline operator int64_t() && noexcept(false);
  really_inline operator double() && noexcept(false);
  really_inline operator std::string_view() && noexcept(false);
  really_inline operator SIMDJSON_IMPLEMENTATION::ondemand::raw_json_string() && noexcept(false);
  really_inline operator bool() && noexcept(false);
#endif

  really_inline SIMDJSON_IMPLEMENTATION::ondemand::array begin() noexcept;
  really_inline SIMDJSON_IMPLEMENTATION::ondemand::array end() noexcept;
  really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::value> operator[](std::string_view key) && noexcept;
};

} // namespace simdjson
