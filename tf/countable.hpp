#ifndef COUNTABLE_HPP
#define COUNTABLE_HPP
namespace tf{
struct countable
{
    size_t cnt;

    countable(size_t cnt_ = 0) : cnt(cnt_) {}

    size_t increaseCnt() {return ++cnt;}
    size_t decreaseCnt() {return --cnt;}
    size_t size() const { return cnt;}
};
}
#endif // COUNTABLE_HPP
