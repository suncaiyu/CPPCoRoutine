// Co_Routines.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

#include <iostream>
#include <coroutine>
using namespace std;
/* 一个简单的协程的例子，猜数字 */
struct CoRet {
    struct promise_type {
        int in = 0;
        int out = 0;
        int result = 0;
        // suspend_always 与 suspend_never always需要停止，never不需要

        // 初始化
        suspend_never initial_suspend() { return {}; }
        // 结束
        suspend_always final_suspend() noexcept { return {}; }
        // 报错
        void unhandled_exception() {}
        // 每次返回时的返回值
        CoRet get_return_object() {
            return { 
                coroutine_handle<promise_type>::from_promise(*this)
            };
        }
        // 如果没有co_return关键字，你必须有return_void
        //void return_void() noexcept {}
        void return_value(int r) {
            result = r;
        }
        suspend_always yield_value(int r) {
            out = r;
            return {};
        }
    };
    // handle对象，继续调用resume等的
    coroutine_handle<promise_type> handle;
};

struct Input {
    int *pIn = nullptr;
    // 判断遇到co_await/co_yield时候要不要停止并跳回到调用
    bool await_ready() { return false; }
    // 当需要停止的时候，在跳回之前需要做什么操作
    void await_suspend(coroutine_handle<CoRet::promise_type> h) {
        if (pIn == nullptr) {
            pIn = &h.promise().in;
        }
    }
    int await_resume() { return *pIn; }
};
// TODO: 在此处引用程序需要的其他标头。
