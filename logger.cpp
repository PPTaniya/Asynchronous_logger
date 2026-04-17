#include <iostream>
#include <sstream>
#include <mutex>
#include <iomanip>
#include <thread>
#include <chrono>
#include <string>
#include <atomic>
#include <queue>
#include <condition_variable>


using namespace std;

class AsyncLogger{

    public:

    enum class Level {
        INFO,
        WARNING,
        DEBUG,
        VERBOSE,
        ERROR,
        FATAL
    };

    AsyncLogger():stop_flag(false){
        worker = thread(&AsyncLogger::processLog,this);
    }

    ~AsyncLogger(){
        stop();
    }


    void stop()
    {
        {
            lock_guard<mutex> lock(mtx);
            stop_flag = true;
        }
        cv.notify_one();
        if(worker.joinable())
        worker.join();
    }

    void log(Level level,const string& msg){
    {
        lock_guard<mutex> lock(mtx);
        log_queue.push(formatMessage(level, msg));
    }
    cv.notify_one();
    }

    private:

    queue<string> log_queue;
    mutex mtx;
    condition_variable cv;
    atomic<bool> stop_flag;
    thread worker;

    void processLog()
    {
        while(true)
        {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [this] {return !log_queue.empty() || stop_flag;});
            
            while(!log_queue.empty()){
                cout<<log_queue.front()<<endl;
                log_queue.pop();
            }
            if(log_queue.empty() && stop_flag)
            break;
        }
    }


    string formatMessage(Level level, const string& msg)
    {
    ostringstream oss;
    oss << timestamp()
        << " [ " << levelToString(level) << " ] " 
        << " [ TID " << this_thread::get_id() << " ] "
        << msg;

    return oss.str();
    }

    string levelToString(Level level)
    {
    switch(level){
        case Level::INFO: return "INFO";
        case Level::WARNING: return "WARNING";
        case Level::DEBUG: return "DEBUG";
        case Level::VERBOSE: return "VERBOSE";
        case Level::ERROR: return "ERROR";
        case Level::FATAL: return "FATAL";
    }

    return "UNKNOWN";
    }

    string timestamp()
    {
    auto now = chrono::system_clock::now();
    auto time = chrono::system_clock::to_time_t(now);

    ostringstream oss;
    oss << put_time(localtime(&time), "%H:%M:%S" );
    return oss.str();
    }
};


AsyncLogger logger;


void user_worker(int id)
{
    for (int i = 0; i < 10; i++)
    {
        logger.log(AsyncLogger::Level::INFO, 
        "worker " + to_string(id) + " iteration " + to_string(i+1)
        );
        this_thread::sleep_for(chrono::milliseconds(50));
    }

}


int main()
{
    thread t1(user_worker, 1);
    thread t2(user_worker, 2);
    t1.join();
    t2.join();
    logger.stop();
}
