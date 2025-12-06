#include <earley.hpp>

void Earley::fit(Grammar grammar) { grammar_ = std::move(grammar); }

bool Earley::predict_prologue(const std::string &word) const {
  anchored_situations_.clear();
  difference_.clear();
  anchored_situations_.resize(word.length() + 1);

  auto [rule0, next] = grammar_.rules(Grammar::start);
  Situation start(rule0, 0, 0, 0);

  difference_.emplace(start);
  return true;
}

bool Earley::predict_epilogue(const std::string &word) const {
  auto [rule0, next] = grammar_.rules(Grammar::start);
  Situation finish(rule0, 1, 0, word.length());

  auto [begin, end] = anchored_situations_[word.length()].equal_range(0);
  for (auto it = begin; it != end; ++it) {
    if (it->second == finish) return true;
  }
  return false;
}

bool Earley::predict(const std::string &word) const {
  predict_prologue(word);

  closure(0);
  for (size_t i = 0; i < word.length(); ++i) {
    if (!grammar_.is_terminal(word[i])) {
      return false;
    }
    scan(i, word[i]);
    closure(i + 1);
  }

  return predict_epilogue(word);
}

void Earley::scan(size_t iter, char ch) const {
  for (const auto &[anchor, situation] : anchored_situations_[iter]) {
    if (anchor == ch) {
      difference_.emplace(situation.scan());
    }
  }
}

std::pair<Earley::mmap_t::const_iterator, Earley::mmap_t::const_iterator>
Earley::comp_situations(const Situation &situation) const noexcept {
  const auto &completed_situations = anchored_situations_[situation.begin()];
  if (situation.anchor() != 0) {
    auto end = completed_situations.end();
    return {end, end};
  }
  return completed_situations.equal_range(situation.rule());
}

Earley::set_t Earley::comp(size_t iter) const {
  set_t comp_diff;
  for (const auto &situation : difference_) {
    if (situation.rule() == Grammar::start) {
      continue;
    }
    auto [begin, end] = comp_situations(situation);
    for (auto it = begin; it != end; ++it) {
      comp_diff.emplace(situation.comp(it->second));
    }
  }
  return comp_diff;
}

std::pair<Grammar::mmap_t::const_iterator, Grammar::mmap_t::const_iterator>
Earley::pred_situations(const Situation &situation) const noexcept {
  return grammar_.rules(situation.anchor());
}

Earley::set_t Earley::pred(size_t iter) const {
  set_t pred_diff;
  for (const auto &situation : difference_) {
    if (situation.anchor() == Grammar::start) {
      continue;
    }
    auto [begin, end] = pred_situations(situation);
    for (auto rule = begin; rule != end; ++rule) {
      pred_diff.emplace(situation.pred(rule));
    }
  }
  return pred_diff;
}

void Earley::closure(size_t iter) const {
  set_t situations;

  while (!difference_.empty()) {
    set_t comp_diff = comp(iter);
    set_t pred_diff = pred(iter);

    for (const auto &situation : difference_) {
      anchored_situations_[iter].emplace(situation.anchor(), situation);
    }
    situations.merge(difference_);

    difference_.merge(comp_diff);
    difference_.merge(pred_diff);

    std::erase_if(difference_, [&situations](const Situation &situation) {
      return situations.contains(situation);
    });
  }
}