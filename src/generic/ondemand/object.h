#include "simdjson/error.h"

namespace {
namespace SIMDJSON_IMPLEMENTATION {
namespace ondemand {

/**
 * A forward-only JSON object.
 */
class object {
public:
  really_inline object() noexcept;

  really_inline simdjson_result<value> operator[](std::string_view key) noexcept;

  //
  // iterator interface
  //
  really_inline simdjson_result<ondemand::field&> operator*() noexcept;
  really_inline bool operator==(const object &other) noexcept;
  really_inline bool operator!=(const object &other) noexcept;
  really_inline object &operator++() noexcept;

  really_inline object begin() noexcept;
  really_inline object end() noexcept;

protected:
  /** Create a valid object */
  really_inline object(ondemand::value &parent, const uint8_t *key_string) noexcept;
  /** Create an error object */
  really_inline object(ondemand::value &parent, error_code _error) noexcept;

  really_inline void advance() noexcept;
  static really_inline simdjson_result<ondemand::object> try_begin(ondemand::value &parent) noexcept;
  static really_inline object begin(ondemand::value &parent) noexcept;

  ondemand::field field;
  // TODO check whether this gets cleverly elided by the compiler or not. If not, store it in the json
  // so we don't keep multiple of them
  error_code error;

  friend class value;
  friend class json;
  friend class simdjson_result<ondemand::object>;
};

} // namespace ondemand
} // namespace SIMDJSON_IMPLEMENTATION
} // namespace {

namespace simdjson {

template<>
struct simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::object> : public internal::simdjson_result_base<SIMDJSON_IMPLEMENTATION::ondemand::object> {
public:
  really_inline simdjson_result(SIMDJSON_IMPLEMENTATION::ondemand::object &&value) noexcept; ///< @private
  really_inline simdjson_result(SIMDJSON_IMPLEMENTATION::ondemand::value &parent, error_code error) noexcept; ///< @private

  really_inline SIMDJSON_IMPLEMENTATION::ondemand::object begin() noexcept;
  really_inline SIMDJSON_IMPLEMENTATION::ondemand::object end() noexcept;
  really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::value> operator[](std::string_view key) noexcept;
};

} // namespace simdjson
