#include <iostream>
#include <vector>
#include <type_traits>
#include <string>
// #include <cxxabi.h>
using namespace std;

template <int N>
struct FOR_
{
    static void extra(vector<int> param, string &s)
    {
        // cout << param[N] << endl;
        s = s + to_string(param[N]) + ",";
        FOR_<N - 1>::extra(param, s);
    }
};

template <>
struct FOR_<0>
{
    static void extra(vector<int> param, string &s)
    {
        s = s + to_string(param[0]);
        cout << s << endl;
    }
};

template<int M,int... N>
struct FORR {
    static void extra()
    {
        cout << M << endl;
        FORR<N...>::extra();
    }
};

template<int M> 
struct FORR<M>{
    static void extra()
    {
        cout << M << endl;
    }
};

template <typename T>
void print(const T t)
{
    cout << t << endl;
}

template <typename T, typename... Args>
void print(const T t, Args... args)
{
    cout << t << endl;
    print(args...);
}


// 这样写是可以有返回值的
#define USEPRIN(nums) \
    ({                                                    \
        string s;                                         \
        FOR_<sizeof(nums) / sizeof(int)>::extra(nums, s); \
        (s);                                              \
    })

// 这样写是不能返回值的
#define USE(N)                                      \
    {                                               \
        string s;                                   \
        FOR_<sizeof(N) / sizeof(int)>::extra(N, s); \
    }

// 模板判断参数类型
template<class T>
void testtype1(T t)
{
    cout << is_same<T, int>::value << endl;
}
template<class T>

void testtype2(T &t)
{   
    // cout << is_same<decay_t<T>, int>::value << endl;
    const bool flag =  is_same<decay_t<decltype(t)>, int>::value;
    // 使用c++17标准的constexpr可以解决不用分支不同调用的问题，因为只会编译符合的路径，不符合的相当于注释了，编译时就不会报 int没有substr的错误了
    if constexpr(flag) {
        cout << t + 1;
    } else {
        cout << t.substr(1);
    }
}

// 判断函数返回类型
template <class F>
auto invoke(F f)
{
    // const bool flag = is_same<decltype(f()), void>::value;
    // const bool flag = is_void<decltype(f())>::value;
    // xxxx_v这种很多，一般都是xxxx<>::value的简单方式  类似的还有xxxx_t ==> xxxx<>::type
    // const bool flag = is_void_v<decltype(f())>;
    const bool flag = is_void<invoke_result_t<F>>::value;
    if constexpr (flag)
    {
        cout << "void enter" << endl;
        f();
        cout << "void leave" << endl;
    }
    else
    {
        cout << "enter" << endl;
        auto ret = f();
        cout << "leave" << endl;
        return ret;
    }
}

// // 当invoke的变化非常大的时候，要写两种区分怎么写  c++20版本
// template <class F> requires(is_void<invoke_result_t<F>>::value)
// auto newinvoke(F f)
// {
//     cout << "void enter" << endl;
//     f();
//     cout << "void leave" << endl;
// }

// template <class F> requires(!is_void<invoke_result_t<F>>::value)
// auto newinvoke(F f)
// {
//         cout << "enter" << endl;
//         auto ret = f();
//         cout << "leave" << endl;
//         return ret;
// }

// 当invoke的变化非常大的时候，要写两种区分怎么写 
// template <class F, enable_if_t<is_void<invoke_result_t<F>>::value, int> = 0>
// decltype(f()) 会报错，因为下面才有f，所以这里用了F的空构造函数理论上如果有默认构造函数可以用F()()表示，没有的只能用declval<F>()()  它没有实际意义
template <class F, enable_if_t<is_void<decltype(declval<F>()())>::value, int> = 0>
auto newinvoke(F f)
{
    cout << "void enter" << endl;
    f();
    cout << "void leave" << endl;
}

template <class F, enable_if_t<!is_void<invoke_result_t<F>>::value, int> = 0>
auto newinvoke(F f)
{
        cout << "enter" << endl;
        auto ret = f();
        cout << "leave" << endl;
        return ret;
}

class Student {
    public:
    void study() {
        cout << "can study" << endl;
    }
};

class Teacher {
    public:
    void teach()
    {
        cout << "can teach" << endl;
    }
};

// //判断class是否有某函数 c++20版本 
// template<class T>
// void invokestudyfunction(T t)
// {
//     if constexpr (requires{t.study();}) {
//     t.study();
//     } else {
//         t.teach();
//     }
// }

// //判断class是否有某函数,比较蠢的办法，自己手动设置
// template<class T>
// struct has_study {
//     static const bool value = false;
// };

// template<>
// struct has_study<Student> {
//     static const bool value = true;
// };

// template<class T>
// void invokestudyfunction(T t)
// {
//     if constexpr (has_study<T>::value) {
//          t.study();
//     } else {
//         t.teach();
//     }
// }

//判断class是否有某函数,优化方法
template<class T, class Dummy = void>
struct has_study {
    static const bool value = false;
};

template<class T>
// void_t() 的含义是，它什么都不在乎，它只想知道里面的对象能不能编译成功，如果编译成功，就走这个 ，如果编译失败，这个特化模板的条件达不到，就走上面的
struct has_study<T, void_t<decltype(declval<T>().study())>> {
    static const bool value = true;
};

template<class T>
void invokestudyfunction(T t)
{
    if constexpr (has_study<T>::value) {
         t.study();
    } else {
        t.teach();
    }
}

int main()
{
    vector<int> nums = {87, 66, 31, 2, 8, 90, 1};
    // string s;
    // FOR_<sizeof(nums) / sizeof(int)>::extra (nums, s);
    // print<int>(87, 66, 31,2,8, 90,1);
    // USEPRIN(nums);
    string ss = USEPRIN(nums);
    USE(nums);
    FORR<3,5,6,8>::extra();

    int a = 88;
    string str = "777777";
    testtype2(str);

    newinvoke([]()->void{
        return;
    });
    newinvoke([]()->int{
        return 975;
    });

    Student student;
    invokestudyfunction(student);
    Teacher teacher;
    invokestudyfunction(teacher);
    return 0;
}