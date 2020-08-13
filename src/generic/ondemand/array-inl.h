namespace {
namespace SIMDJSON_IMPLEMENTATION {
namespace ondemand {

really_inline array::array() noexcept : value{}, error{} {}
really_inline array::array(ondemand::value &parent, error_code _error) noexcept : value(parent.json, parent.depth+1), error{_error} {}
really_inline simdjson_result<value> array::operator*() noexcept { return { value, error }; }
really_inline bool array::operator!=(array &) noexcept { return value.json->depth < value.depth; }
really_inline array &array::operator++() noexcept { advance(); return *this; }

really_inline void array::advance() noexcept {
  // The user MUST NOT continue iterating if there is an error.
  SIMDJSON_ASSUME(!error);

  // Finish iterating any partly-iterated child arrays/objects
  value.finish();

  value.json->next_array_element();
}

really_inline simdjson_result<ondemand::array> array::try_begin(ondemand::value &parent) noexcept {
  SIMDJSON_ASSUME(parent.depth == parent.json->depth);
  bool has_element;
  if (auto error = parent.json->begin_array().get(has_element)) { return { parent, error }; }
  return array(parent);
}

really_inline array array::begin(ondemand::value &parent) noexcept {
  SIMDJSON_ASSUME(parent.depth == parent.json->depth);
  bool has_element;
  if (auto error = parent.json->begin_array().get(has_element)) { return { parent, error }; }
  return { parent };
}

really_inline array array::end() noexcept {
  return {};
}

} // namespace ondemand
} // namespace SIMDJSON_IMPLEMENTATION
} // namespace {

namespace simdjson {

really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::array>::simdjson_result(SIMDJSON_IMPLEMENTATION::ondemand::array &&value) noexcept
    : internal::simdjson_result_base<SIMDJSON_IMPLEMENTATION::ondemand::array>(std::forward<SIMDJSON_IMPLEMENTATION::ondemand::array>(value)) {}
really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::array>::simdjson_result(SIMDJSON_IMPLEMENTATION::ondemand::value &parent, error_code error) noexcept
    : internal::simdjson_result_base<SIMDJSON_IMPLEMENTATION::ondemand::array>({ parent, error }, error) {}

really_inline SIMDJSON_IMPLEMENTATION::ondemand::array simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::array>::begin() noexcept {
  return first;
}
really_inline SIMDJSON_IMPLEMENTATION::ondemand::array simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::array>::end() noexcept {
  return {};
}

} // namespace simdjson
