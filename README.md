# Asynchronous_logger (c++)

A simple **asynchronous, thread-safe logging system** implemented in C++. This logger uses a background worker thread to process log messages, minimizing blocking in application threads.

---

## 🚀 Features

- Thread-safe logging using `std::mutex`
- Asynchronous log processing with a dedicated worker thread
- Multiple log levels:
  - INFO
  - WARNING
  - DEBUG
  - VERBOSE
  - ERROR
  - FATAL
- Timestamped log messages
- Thread ID included in logs
- Graceful shutdown with queue draining

---

## 🧠 How It Works

- Multiple threads call `logger.log(...)`
- Messages are pushed into a `std::queue`
- A background thread (`worker`) waits on a `std::condition_variable`
- When notified, it processes and prints log messages to `std::cout`
- On shutdown, remaining messages are flushed before exit

---

## 📂 File Structure
.
├── main.cpp # Contains AsyncLogger implementation and usage example
└── README.md # Project documentation


---

## ⚙️ Build Instructions

Compile using any C++11 (or later) compatible compiler:

```bash
g++ -std=c++11 -pthread main.cpp -o async_logger


▶️ Usage Example
logger.log(AsyncLogger::Level::INFO, "Hello from worker thread!");

Sample Output
12:30:45 [ INFO ]  [ TID 140735 ] worker 1 iteration 1
12:30:45 [ INFO ]  [ TID 140736 ] worker 2 iteration 1

🧵 Multithreading Demo

The example program creates two worker threads:
thread t1(user_worker, 1);
thread t2(user_worker, 2);
Each thread logs 10 messages asynchronously.

🛑 Shutdown Behavior
logger.stop() signals the worker thread to stop
Ensures all queued messages are processed before exiting
Automatically called in the destructor as a safeguard

⚠️ Notes & Limitations
Uses std::localtime, which is not thread-safe
Logs are printed only to stdout (no file logging yet)
No log filtering by level (all messages are processed)
Queue is unbounded (may grow under heavy load)

🔧 Possible Improvements
Add file output support
Implement log level filtering
Replace std::localtime with a thread-safe alternative
Add log rotation
Use a bounded queue to prevent memory growth
Support formatted logging (like printf or fmt)

📜 License
This project is open-source and free to use for learning and development purposes.

