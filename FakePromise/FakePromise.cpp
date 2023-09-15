// FakePromise.cpp: 定义应用程序的入口点。
//

#include "FakePromise.h"
#include <future>
#include <thread>
// 一个简单的线程中需要依赖上一步的状态执行下一步的例子
using namespace std;

class FakePromise {
public:
    FakePromise() {
        p = new promise<bool>();
        p->set_value(true);
    }
    template <typename T1, typename T2>
    FakePromise *then(T1 func, T2 v) {
        bool flag = p->get_future().get();
        delete p;
        p = new promise<bool>();
        this_thread::sleep_for(std::chrono::seconds(3));
        if (v % 2 == 0) {
            func(v);
            p->set_value(true);
        }
        else {
            p->set_value(false);
        }
        return this;
    }
    ~FakePromise() {
        delete p;
    }
    promise<bool> *p = nullptr;
};

void funcc(int c) {
    cout << c << endl;
}
void fake(promise<void> &po)
{
    FakePromise fp;
    fp.then(funcc, 4);
    po.set_value();
}

int main()
{
    promise<void >p;
    thread t(&fake, std::ref(p));
    t.detach();
    cout << ">>>>" << endl;
    p.get_future().wait();
    return 0;
}