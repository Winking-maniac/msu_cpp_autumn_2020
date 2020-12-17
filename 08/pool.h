#include <assert.h>
#include <functional>
#include <vector>
#include <future>
#include <queue>
#include <thread>

class ThreadPool
{
public:
    explicit ThreadPool(size_t poolSize);
    ~ThreadPool() noexcept;
    
    ThreadPool(ThreadPool&) = delete;
    ThreadPool& operator=(ThreadPool&) = delete;
    
    // pass arguments by value
    template <class Func, class... Args>
    auto exec(Func func, Args... args) -> std::future<decltype(func(args...))>;
private:
    size_t size;
    std::queue<std::function<void()>> tasks;
    std::vector<std::thread> threads;
    std::mutex mutex;
    friend void working_thread(ThreadPool& pool);
    std::condition_variable new_task;
    bool terminate;
};

void
working_thread(ThreadPool & pool)
{
    while (true) {
        {        
            std::unique_lock<std::mutex> wait_task(pool.mutex);
            while(pool.tasks.empty() && !pool.terminate) {
               pool.new_task.wait(wait_task);
            }
        }
        if (pool.terminate) {
            return;
        }
        
        pool.mutex.lock();
        std::function<void()> cur_task = std::move(pool.tasks.front());
        pool.tasks.pop();
        pool.mutex.unlock();
        
        cur_task();
    }
}

ThreadPool::ThreadPool(size_t poolSize) : size(poolSize)
{
    assert(poolSize != 0);
    for (unsigned i = 0; i < poolSize; i++) {
        threads.emplace_back([this]() { working_thread(*this); });
    }
}

ThreadPool::~ThreadPool() noexcept
{
    terminate = true;
    new_task.notify_all();
    for (size_t i =  0; i < size; i++) {
        threads[i].join();
    }
}

template <class Func, class... Args>
auto ThreadPool::exec(Func func, Args... args) -> std::future<decltype(func(args...))>
{
    auto task = std::make_shared<std::packaged_task<decltype(func(args...))()>>
        ( [func, args...](){ return func(args...); } );
    
    mutex.lock();
    tasks.emplace([task]() {(*task)();} );
    mutex.unlock();
    
    new_task.notify_one();
    return task->get_future();
}