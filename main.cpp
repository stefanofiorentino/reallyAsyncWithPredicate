#include <iostream>
#include <future>

template<typename F, typename... Ts>
inline auto reallyAsync(F &&f, Ts &&... params) -> decltype(std::async(std::launch::async, std::forward<F>(f),
                                                                       std::forward<Ts>(params)...))
{
    return std::async(std::launch::async, std::forward<F>(f), std::forward<Ts>(params)...);
}

int main()
{
    std::condition_variable cv;
    std::mutex mtx;

    auto counter = 0u;
    reallyAsync([&](std::function<void(int)> step_counter, std::function<void(void)> finish)
                {
                    std::cout << "counting..." << std::endl;
                    while (counter<5)
                    {
                        if (step_counter)
                            step_counter(++counter);
                        std::this_thread::sleep_for(std::chrono::seconds(1));
                    }
                    if (finish)
                        finish();
                }, [&](int counter)
                {
                    std::cout << counter << " seconds passed" << std::endl;
                    cv.notify_one();
                }, [&]()
                {
                    std::cout << "...finish" << std::endl;
                    cv.notify_one();
                });

    std::unique_lock<std::mutex> lk(mtx);
    cv.wait(lk, [&]{ return counter >= 5; });
    return 0;
}