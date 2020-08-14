namespace {
namespace SIMDJSON_IMPLEMENTATION {
namespace ondemand {

really_inline value json::as_value() noexcept { return { this, depth }; }

really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::array> json::get_array() noexcept { return as_value().get_array(); }
really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::object> json::get_object() noexcept { return as_value().get_object(); }
really_inline simdjson_result<uint64_t> json::get_uint64() noexcept { return as_value().get_uint64(); }
really_inline simdjson_result<int64_t> json::get_int64() noexcept { return as_value().get_int64(); }
really_inline simdjson_result<double> json::get_double() noexcept { return as_value().get_double(); }
really_inline simdjson_result<std::string_view> json::get_string() noexcept { return as_value().get_string(); }
really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::raw_json_string> json::get_raw_json_string() noexcept { return as_value().get_raw_json_string(); }
really_inline simdjson_result<bool> json::get_bool() noexcept { return as_value().get_bool(); }
really_inline bool json::is_null() noexcept { return as_value().is_null(); }

#if SIMDJSON_EXCEPTIONS
really_inline json::operator array() noexcept(false) { return as_value(); }
really_inline json::operator object() noexcept(false) { return as_value(); }
really_inline json::operator uint64_t() noexcept(false) { return as_value(); }
really_inline json::operator int64_t() noexcept(false) { return as_value(); }
really_inline json::operator double() noexcept(false) { return as_value(); }
really_inline json::operator std::string_view() noexcept(false) { return as_value(); }
really_inline json::operator raw_json_string() noexcept(false) { return as_value(); }
really_inline json::operator bool() noexcept(false) { return as_value(); }
#endif

really_inline array json::begin() noexcept { return as_value().begin(); }
really_inline array json::end() noexcept { return {}; }
really_inline simdjson_result<value> json::operator[](std::string_view key) noexcept { return as_value()[key]; }

really_inline json::json(json &&other) noexcept
  : index{other.index}, buf{other.buf}, string_buf{other.string_buf}, depth{other.depth} {}

really_inline json::json(const uint32_t *_index, const uint8_t *_buf, uint8_t *_string_buf, uint32_t _depth) noexcept
  : index{_index}, buf{_buf}, string_buf{_string_buf}, depth{_depth} {}

really_inline const uint8_t *json::advance() noexcept { return &buf[*(index++)]; }
really_inline const uint8_t *json::peek(int n) const noexcept { return &buf[*(index+n)]; }
really_inline uint32_t json::peek_index(int n) const noexcept { return *(index+n); }
really_inline bool json::advance_if(uint8_t structural) noexcept {
  bool found = buf[*index] == structural;
  index += found;
  return found;
}

really_inline simdjson_result<bool> json::begin_array() noexcept {
  if (*advance() != '[') { return INCORRECT_TYPE; }
  return first_array_element();
}

really_inline bool json::first_array_element() noexcept {
  if (advance_if(']')) {
    log_value("empty array");
    return false;
  }
  depth++;
  log_start_value("array");
  return true;
}

really_inline simdjson_result<bool> json::next_array_element() noexcept {
  uint8_t next = *advance();
  if (next == ']') {
    depth--;
    log_end_value("array");
    return false;
  }
  if (next != ',') {
    log_error("missing ,");
    return TAPE_ERROR;
  }
  return true;
}

really_inline simdjson_result<const uint8_t *> json::begin_object() noexcept {
  if (*advance() != '{') { return INCORRECT_TYPE; }
  return first_object_field();
}

really_inline simdjson_result<const uint8_t *> json::first_object_field() noexcept {
  // Check for {}
  const uint8_t *key_string = advance();
  if (*key_string == '}') {
    log_value("empty object");
    return nullptr;
  }

  depth++;
  log_start_value("object");

  // Check for " :
  if (*(key_string++) != '"') {
    log_error("missing key");
    return TAPE_ERROR;
  }
  if (*advance() != ':') {
    log_error("missing :");
    return TAPE_ERROR;
  }
  return key_string;
}

really_inline simdjson_result<const uint8_t *> json::next_object_field() noexcept {
  // Check whether we are finished
  uint8_t next = *advance();
  if (next == '}') {
    depth--;
    log_end_value("object");
    return nullptr;
  }
  if (next != ',') {
    log_error("missing comma");
  }

  // Read "key" :
  const uint8_t *key_string = advance();
  if (*(key_string++) != '"') {
    log_error("missing key");
    return TAPE_ERROR;
  }
  if (*advance() != ':') {
    log_error("missing :");
    return TAPE_ERROR;
  }

  return key_string;
}

template<bool DELTA=0>
really_inline void json::log_event(const char *type) const noexcept {
  logger::log_ondemand_line<DELTA>(*this, "", type, "");
}

template<bool DELTA=0>
really_inline void json::log_value(const char *type) const noexcept {
  logger::log_ondemand_line<DELTA>(*this, "", type, "");
}

template<bool DELTA=0>
really_inline void json::log_start_value(const char *type) const noexcept {
  logger::log_ondemand_line<DELTA>(*this, "+", type, "");
  if (logger::LOG_ENABLED) { logger::log_depth++; }
}

template<bool DELTA=0>
really_inline void json::log_end_value(const char *type) const noexcept {
  if (logger::LOG_ENABLED) { logger::log_depth--; }
  logger::log_ondemand_line<DELTA>(*this, "-", type, "");
}

template<bool DELTA=0>
really_inline void json::log_error(const char *error) const noexcept {
  logger::log_ondemand_line<DELTA>(*this, "", "ERROR", error);
}

} // namespace ondemand
} // namespace SIMDJSON_IMPLEMENTATION
} // namespace {

namespace simdjson {

really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::json>::simdjson_result(SIMDJSON_IMPLEMENTATION::ondemand::json &&value) noexcept
    : internal::simdjson_result_base<SIMDJSON_IMPLEMENTATION::ondemand::json>(std::forward<SIMDJSON_IMPLEMENTATION::ondemand::json>(value)) {}
really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::json>::simdjson_result(SIMDJSON_IMPLEMENTATION::ondemand::json &&value, error_code error) noexcept
    : internal::simdjson_result_base<SIMDJSON_IMPLEMENTATION::ondemand::json>(std::forward<SIMDJSON_IMPLEMENTATION::ondemand::json>(value), error) {}

// TODO make sure the passing of a pointer here isn't about to cause us trouble
really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::value> simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::json>::as_value() noexcept {
  // return simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::value>(&first, error());
  return { SIMDJSON_IMPLEMENTATION::ondemand::value(&first, first.depth), error() };
}
really_inline SIMDJSON_IMPLEMENTATION::ondemand::array simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::json>::begin() noexcept { return as_value().begin(); }
really_inline SIMDJSON_IMPLEMENTATION::ondemand::array simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::json>::end() noexcept { return as_value().end(); }
really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::value> simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::json>::operator[](std::string_view key) & noexcept { return as_value()[key]; }

really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::array> simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::json>::get_array() & noexcept { return as_value().get_array(); }
really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::object> simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::json>::get_object() & noexcept { return as_value().get_object(); }
really_inline simdjson_result<uint64_t> simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::json>::get_uint64() && noexcept { return as_value().get_uint64(); }
really_inline simdjson_result<int64_t> simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::json>::get_int64() && noexcept { return as_value().get_int64(); }
really_inline simdjson_result<double> simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::json>::get_double() && noexcept { return as_value().get_double(); }
really_inline simdjson_result<std::string_view> simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::json>::get_string() && noexcept { return as_value().get_string(); }
really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::raw_json_string> simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::json>::get_raw_json_string() && noexcept { return as_value().get_raw_json_string(); }
really_inline simdjson_result<bool> simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::json>::get_bool() && noexcept { return as_value().get_bool(); }
really_inline bool simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::json>::is_null() noexcept { return as_value().is_null(); }

#if SIMDJSON_EXCEPTIONS
really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::json>::operator SIMDJSON_IMPLEMENTATION::ondemand::array() & noexcept(false) { return as_value(); }
really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::json>::operator SIMDJSON_IMPLEMENTATION::ondemand::object() & noexcept(false) { return as_value(); }
really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::json>::operator uint64_t() && noexcept(false) { return as_value(); }
really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::json>::operator int64_t() && noexcept(false) { return as_value(); }
really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::json>::operator double() && noexcept(false) { return as_value(); }
really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::json>::operator std::string_view() && noexcept(false) { return as_value(); }
really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::json>::operator SIMDJSON_IMPLEMENTATION::ondemand::raw_json_string() && noexcept(false) { return as_value(); }
really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::json>::operator bool() && noexcept(false) { return as_value(); }
#endif

} // namespace simdjson
