#include <earley.hpp>

template <class T>
inline void hash_combine(size_t &seed, const T &v) {
  std::hash<T> hasher;
  seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

namespace std {
template <>
struct hash<Grammar::mmap_t::const_iterator> {
  size_t operator()(const Grammar::mmap_t::const_iterator &it) const noexcept {
    return std::hash<decltype(&it->first)>{}(&it->first);
  }
};
}  // namespace std

Earley::Situation::Situation(rule_t rule, size_t offset, size_t begin,
                             size_t end)
    : rule_(rule), offset_(offset), begin_(begin), end_(end) {}

char Earley::Situation::rule() const noexcept { return rule_->first; }

char Earley::Situation::anchor() const noexcept {
  return rule_->second[offset_];
}

size_t Earley::Situation::begin() const noexcept { return begin_; }

Earley::Situation Earley::Situation::scan() const noexcept {
  return Situation(rule_, offset_ + 1, begin_, end_ + 1);
}

Earley::Situation Earley::Situation::pred(rule_t rule) const noexcept {
  return Situation(rule, 0, end_, end_);
}

Earley::Situation Earley::Situation::comp(
    const Situation &situation) const noexcept {
  return Situation(situation.rule_, situation.offset_ + 1, situation.begin_,
                   end_);
}

size_t Earley::Situation::hash() const noexcept {
  size_t seed = 0;
  hash_combine(seed, rule_);
  hash_combine(seed, offset_);
  hash_combine(seed, begin_);
  hash_combine(seed, end_);
  return seed;
}

size_t Earley::SituationHash::operator()(
    const Situation &situation) const noexcept {
  return situation.hash();
}