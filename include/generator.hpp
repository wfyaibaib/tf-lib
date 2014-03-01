#ifndef GENERATOR_HPP
#define GENERATOR_HPP
#include <ctime>
#include <string>
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
// 可重复 随机数生成器
class RandGen
{
 int limit;
public:
 RandGen(int li = 10) : limit(li) 
 {
 srand(time(0));
 }
 int operator()()
 {
 return rand() % limit;
 }
};

// 唯一随机数生成器
// Generate unique random numbers from 0 to mod
class URandGen
{

 int limit;
 set<int> used;
public:
 URandGen(int li = 10) : limit(li) 
 {
 srand(size_t(time(NULL)));

 }
 int operator()()
 {
 while(1)
 {
 int i = rand() % limit;
 if(used.find(i) == used.end())
 {
 used.insert(i);
 return i;
 }
 }
 }
};
// 等差生成器
class SkipGen
{
 int i;
 int skp;
public:
 SkipGen(int start = 0, int skip = 1) : i(start), skp(skip) {}
 int operator()()
 {
 int r = i;
 i += skp;
 return r;
 }
};
// ModGen
class ModGen
{
 int i;
public:
 ModGen(int ii = 2) : i(ii) 
 {
 }
 int operator()(int val)
 {
 return val % i;
 }
};
// Bool 生成器
class BRand
{
public:
 BRand()
 {
 srand(time(0));
 }
 bool operator()()
 {
 return rand() % 2 == 0;
 }
};
// Char 生成器 大小写字母
class CharGen
{
 static const std::string source;
 static const int len;
public:
 CharGen()
 {
 //srand(time(0));
 }
 char operator()()
 {
 return source[rand() % len];
 }
};
const std::string CharGen::source = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const int CharGen::len = (CharGen::source).size();

//累加器
template <class Ty>
class SumGen
{
 Ty init;
public:
 SumGen(Ty t = Ty()) : init(t) {}
 void operator()(Ty& obj)
 {
 init += obj; 
 }
 Ty getSum()
 {
 return init;
 }
};
// 斐波那契数列发生器
class FibonacciGen
{
 int n;
 int val[2];
public:
 FibonacciGen() : n(0)
 {
 val[0] = val[1] = 0;
 }
 int operator()()
 {
 int res = n > 2 ? val[0] + val[1] : n > 0 ? 1 : 0;
 n++;
 val[0] = val[1];
 val[1] = res;
 return res;

 }
 int count()
 {
 return n;
 }
};
// string对象生成器
class StringGen
{
 size_t len;
public:
 StringGen(size_t le) : len(le)
 {
 srand(time(0));
 }

 std::string operator()()
 {
 std::string res;
 CharGen Char;
 for (size_t l = len; l != 0; l--)
 {
 res += Char();
 }
 return res;
 }
};

}
#endif // GENERATOR_HPP
