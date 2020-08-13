namespace {
namespace SIMDJSON_IMPLEMENTATION {
namespace ondemand {

WARN_UNUSED really_inline error_code parser::allocate(size_t new_capacity, size_t new_max_depth) noexcept {
  // string_capacity copied from document::allocate
  _capacity = 0;
  _max_depth = 0;
  size_t string_capacity = ROUNDUP_N(5 * new_capacity / 3 + 32, 64);
  string_buf.reset(new (std::nothrow) uint8_t[string_capacity]);
  SIMDJSON_TRY( dom_parser.set_capacity(new_capacity) );
  SIMDJSON_TRY( dom_parser.set_max_depth(DEFAULT_MAX_DEPTH) );
  _capacity = new_capacity;
  _max_depth = new_max_depth;
  return SUCCESS;
}

really_inline simdjson_result<json> parser::parse(const padded_string &buf) noexcept {
  // Allocate if needed
  error_code error;
  if (_capacity < buf.size()) {
    error = allocate(buf.size(), _max_depth);
    if (error) {
      return { { dom_parser.structural_indexes.get(), (const uint8_t *)buf.data(), string_buf.get() }, error };
    }
  }

  // Run stage 1.
  error = dom_parser.stage1((const uint8_t *)buf.data(), buf.size(), false);
  
  return { { dom_parser.structural_indexes.get(), (const uint8_t *)buf.data(), string_buf.get() }, error };
}


} // namespace ondemand
} // namespace SIMDJSON_IMPLEMENTATION
} // namespace {
