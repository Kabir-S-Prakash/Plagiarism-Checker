#include "structures.hpp"
// -----------------------------------------------------------------------------
#include <memory>
#include <chrono>
#include <mutex>
#include <map>
#include <thread>
#include <unordered_set>
#include <unordered_map>
#include <atomic>
#include <condition_variable>
#include <queue>
// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and classes here

class plagiarism_checker_t {
    // You should NOT modify the public interface of this class.
public:
    plagiarism_checker_t(void);
    plagiarism_checker_t(std::vector<std::shared_ptr<submission_t>> 
                            __submissions);
    ~plagiarism_checker_t(void);
    void add_submission(std::shared_ptr<submission_t> __submission);

protected:
    // TODO: Add members and function signatures here
    std::unordered_set<std::shared_ptr<submission_t>> initial_submissions;
    std::mutex mtx;  // Mutex for thread synchronization
    // Timestamp record of submissions
    std::unordered_map<std::shared_ptr<submission_t>, std::chrono::time_point<std::chrono::system_clock>> submission_timestamps;
    // Map to store tokens for each submission
    std::unordered_map<std::shared_ptr<submission_t>, std::vector<int>> submission_tokens_map;
    // Flagged submissions to ensure flagged only once
    std::unordered_set<std::shared_ptr<submission_t>> flagged_submissions;
    // Counter for active threads
    std::atomic<int> active_threads{0};
    // Condition variable to wait for threads to finish
    std::condition_variable cv;
    bool terminate = false;
    // Submission files queued
    std::queue<std::shared_ptr<submission_t>> submission_queue;
    std::thread worker_thread;

    void worker_thread_function();
    void checkPlag(std::shared_ptr<submission_t> submission);
    // End TODO
};
