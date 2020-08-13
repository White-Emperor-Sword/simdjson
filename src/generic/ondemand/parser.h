#include "simdjson/error.h"

namespace {
namespace SIMDJSON_IMPLEMENTATION {
namespace ondemand {

class array;
class object;
class value;
class raw_json_string;

/**
 * A JSON fragment iterator.
 *
 * This holds the actual iterator as well as the buffer for writing strings.
 */
class parser {
public:
  really_inline parser() noexcept = default;
  really_inline parser(parser &&other) noexcept = default;
  really_inline parser(const parser &other) = delete;
  really_inline parser &operator=(const parser &other) = delete;

  WARN_UNUSED error_code allocate(size_t capacity, size_t max_depth=DEFAULT_MAX_DEPTH) noexcept;
  WARN_UNUSED simdjson_result<json> parse(const padded_string &json) noexcept;
private:
  dom_parser_implementation dom_parser{};
  std::unique_ptr<uint8_t[]> string_buf{};
  size_t _capacity{0};
  size_t _max_depth{0};
};

} // namespace ondemand
} // namespace SIMDJSON_IMPLEMENTATION
} // namespace {
