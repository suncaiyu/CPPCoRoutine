// Co_Routines.cpp: 定义应用程序的入口点。
//

#include "Co_Routines.h"
#include <chrono>
#include <ctime>

using namespace std;

CoRet Guess()
{
    auto chrononow = std::chrono::system_clock::now();
    //cout << chrononow << endl;

    time_t t = time(nullptr);
    struct tm *now = localtime(&t);
    stringstream timeStr;

    timeStr << now->tm_year + 1900 << "年";
    timeStr << now->tm_mon + 1 << "月";
    timeStr << now->tm_mday << "日 ";
    timeStr << now->tm_hour << ":";
    timeStr << now->tm_min << ":";
    timeStr << now->tm_sec;
    cout << timeStr.str() << endl;

    srand(now->tm_sec);

    int random = (rand() % 99) + 1;
    Input input;
    int g = -99;
    while (g != random) {
        g = co_await input;
        cout << "corountine : You guess " << g << endl;
        co_yield (random < g ? 1 : (random == g ? 0 : -1));
    }
    auto chronoend = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = chronoend - chrononow;
    cout << "耗时 : " << diff << endl;
    co_return random;
}

int main()
{
    cout << "我将会生成一个随机数，在 1 - 99之间，请猜一个数字，看对不对" << endl;
    CoRet ret = Guess();
    // 模拟用户输入
    int guess = -1;
    while (!ret.handle.done()) {
        cout << "请输入一个数字" << endl;
        cin >> guess;
        ret.handle.promise().in = guess;
        ret.handle.resume();
        string res = (ret.handle.promise().out == 1 ? "larger" : (ret.handle.promise().out == 0 ? "same" : "smaller"));
        cout << "main : The number you guessed is  " << res << endl;
        ret.handle.resume();
    }
    cout << "main : random result is " << ret.handle.promise().result << endl;
    return 0;
}
