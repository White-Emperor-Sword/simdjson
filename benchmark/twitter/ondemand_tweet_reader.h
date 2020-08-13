#ifndef TWITTER_ONDEMAND_TWEET_READER_H
#define TWITTER_ONDEMAND_TWEET_READER_H

#include "simdjson.h"
#include "tweet.h"
#include <vector>

SIMDJSON_TARGET_HASWELL

namespace twitter {
namespace {

using namespace simdjson;
using namespace haswell;
using namespace haswell::stage2;

struct ondemand_tweet_reader {
  std::vector<tweet> tweets;
  ondemand::parser parser;

  ondemand_tweet_reader();
  error_code read_tweets(padded_string &json);
  void clear();
}; // struct ondemand_tweet_reader

ondemand_tweet_reader::ondemand_tweet_reader() : tweets{}, parser{} {
}

really_inline uint64_t nullable_int(ondemand::value && value) {
  if (value.is_null()) { return 0; }
  return std::move(value);
}

error_code ondemand_tweet_reader::read_tweets(padded_string &buf) {
  tweets.clear();

  // Walk the document, parsing the tweets as we go
  UNUSED auto doc = parser.parse(buf);
  // for (ondemand::object tweet : doc["statuses"]) {
  //   twitter::tweet t;
  //   t.created_at = tweet["created_at"];
  //   t.id = tweet["id"];
  //   t.text = tweet["text"];
  //   t.in_reply_to_status_id = nullable_int(tweet["in_reply_to_status_id"]);
  //   {
  //     ondemand::object user = tweet["user"];
  //     t.user.id = user["id"];
  //     t.user.screen_name = user["screen_name"];
  //   }
  //   t.retweet_count = tweet["retweet_count"];
  //   t.favorite_count = tweet["favorite_count"];
  //   tweets.push_back(t);
  // }
  return SUCCESS;
}

} // unnamed namespace
} // namespace twitter

SIMDJSON_UNTARGET_REGION

#endif // TWITTER_SAX_TWEET_READER_H