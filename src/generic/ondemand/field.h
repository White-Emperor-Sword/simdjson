#include "simdjson/error.h"

namespace {
namespace SIMDJSON_IMPLEMENTATION {
namespace ondemand {

/**
 * A JSON field in an object.
 */
class field : public value {
public:
  really_inline field() noexcept;

  really_inline raw_json_string key() noexcept;
protected:
  really_inline field(const uint8_t *key_string, ondemand::json *_json, uint32_t _depth) noexcept;
  raw_json_string _key{};
  friend struct simdjson_result<ondemand::field>;
  friend class object;
};

} // namespace ondemand
} // namespace SIMDJSON_IMPLEMENTATION
} // namespace {
