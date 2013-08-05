#ifndef GENERATOR_HPP
#define GENERATOR_HPP
namespace tf{
struct skipgen
{
    int start;
    int skip;
    skipgen(int start_ = 0, int skip_ = 1):start(start_), skip(skip_) {}
    int operator()()
    {
        int rt = start;
        start += skip;
        return rt;
    }
};
}
#endif // GENERATOR_HPP
