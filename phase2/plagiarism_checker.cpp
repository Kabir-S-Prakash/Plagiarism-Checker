#include "plagiarism_checker.hpp"
// You should NOT add ANY other includes to this file.
// Do NOT add "using namespace std;".

// TODO: Implement the methods of the plagiarism_checker_t class
plagiarism_checker_t::plagiarism_checker_t(){
    worker_thread = std::thread(&plagiarism_checker_t::worker_thread_function, this);
}
plagiarism_checker_t::~plagiarism_checker_t(){
    {
        std::lock_guard<std::mutex> lock(mtx);
        terminate = true;
        cv.notify_all();
    }

    if (worker_thread.joinable()) {
        worker_thread.join();
    }

    std::cerr << "Destructor called for plagiarism_checker_t\n";
}

plagiarism_checker_t::plagiarism_checker_t(std::vector<std::shared_ptr<submission_t>> __submissions){
    auto timestamp = std::chrono::system_clock::now();

    for (const auto& submission : __submissions) {
    // Adding submission
    initial_submissions.insert(submission);
    submission_timestamps[submission] = timestamp; 

    // Tokenize the submission
    tokenizer_t tokenizer(submission->codefile);
    submission_tokens_map[submission] = tokenizer.get_tokens();
    }

    // Start the worker thread after initialization
    worker_thread = std::thread(&plagiarism_checker_t::worker_thread_function, this);
}

void plagiarism_checker_t::add_submission(std::shared_ptr<submission_t> __submission){
    {
        std::lock_guard<std::mutex> lock(mtx);

        // Tokenizing current submission
        tokenizer_t tokenizer(__submission->codefile);
        std::vector<int> submission1 = tokenizer.get_tokens();
        submission_tokens_map[__submission] = submission1;

        // Add the submission to the queue
        submission_queue.push(__submission);

        // Record the timestamp for the submission
        auto timestamp = std::chrono::system_clock::now();
        submission_timestamps[__submission] = timestamp; 
    }

    // Notify the worker thread
    cv.notify_one();
}

void plagiarism_checker_t::worker_thread_function() {
    while (true) {
        std::shared_ptr<submission_t> submission;

        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this]() { return !submission_queue.empty() || terminate; });

            if (terminate && submission_queue.empty()) {
                break;
            }

            // Dequeue the next submission
            submission = submission_queue.front();
            submission_queue.pop();
        }

        // Process the submission outside the critical section
        checkPlag(submission);
    }
}

bool match_submissions(std::vector<int> &submission1, 
        std::vector<int> &submission2, std::map<std::pair<int,int>, int> &match_freq) {
    // TODO: Write your code here
    int n1 = submission1.size();
    int n2 = submission2.size();
    bool exactMatchFlag = false;
    int numExactMatches = 0;
    std::vector<bool> used1(n1, false);
    std::vector<bool> used2(n2, false);

    // Function to mark indices in a given range as used
    auto mark_used = [](std::vector<bool>& used, int start, int length) {
        for (int i = start; i < start + length; ++i) {
            used[i] = true;
        }
    };

    for (size_t i = 0; i < n1; ++i) {
        for (size_t j = 0; j < n2; ++j) {
            int matchLength = 0;

            // Find the longest match starting at i and j
            while (i + matchLength < n1 && j + matchLength < n2 &&
               submission1[i + matchLength] == submission2[j + matchLength] &&
               matchLength < 75) {
                ++matchLength;
            }

            bool overlap = false;
            for (int k = 0; k < matchLength; ++k) {
                if (used1[i + k] || used2[j + k]) {
                    overlap = true;
                    break;
                }
            }

            if (!overlap) {
                // Check for exact 75-length match first
                if (matchLength == 75) {
                    exactMatchFlag = true;
                }

                // Process each 15-length sub-match within the current match
                for (int start = 0; start + 15 <= matchLength; start += 15) {
                    int subMatchStart1 = i + start;
                    int subMatchStart2 = j + start;

                    // Check if this sub-match is already used
                    bool subOverlap = false;
                    for (int k = 0; k < 15; ++k) {
                        if (used1[subMatchStart1 + k] || used2[subMatchStart2 + k]) {
                            subOverlap = true;
                            break;
                        }
                    }

                    if (!subOverlap) {
                        // Count this sub-match
                        numExactMatches++;
                        match_freq[{subMatchStart1, 15}]++;
                        mark_used(used1, subMatchStart1, 15);
                        mark_used(used2, subMatchStart2, 15);
                    }
                }

            }
        }
    }

    // If we have at least 10 matches, set the exact match flag
    if (numExactMatches >= 10) {
        exactMatchFlag = true;
    }

    return exactMatchFlag;
}

void plagiarism_checker_t::checkPlag(std::shared_ptr<submission_t> submission){
    if(submission == nullptr) return;
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<int> submission1 = submission_tokens_map[submission];

    // Ensuring different segments
    std::map<std::pair<int,int>, int> match_freq;
    
    // checking database
    for(auto& [Submission, submission2]: submission_tokens_map){
        if(Submission == nullptr) return;
        if(Submission->id == submission->id) continue;
        auto check = match_submissions(submission1,submission2,match_freq);
        auto time1 = submission_timestamps[submission];
        auto time2 = submission_timestamps[Submission];
        bool c1 = flagged_submissions.find(Submission) == flagged_submissions.end();
        bool c2 = initial_submissions.find(Submission) == initial_submissions.end();
        bool c3 = std::chrono::duration_cast<std::chrono::seconds>(abs(time1 - time2)).count() <= 1;
        bool c4 = flagged_submissions.find(submission) == flagged_submissions.end();
        if(check && c4){
            // Flagging one
            std::cerr << "Flagging submission: " << submission->student->get_name() << std::endl;
            if(submission->student != nullptr) submission->student->flag_student(submission);
            if(submission->professor != nullptr) submission->professor->flag_professor(submission);
            flagged_submissions.insert(submission);
        }
        // Flagging older file if both submitted under one second
        if(check && c1 && c2 && c3){
            std::cerr << "Flagging submission: " << Submission->student->get_name() << std::endl;
            if(Submission->student != nullptr) Submission->student->flag_student(Submission);
            if(Submission->professor != nullptr) Submission->professor->flag_professor(Submission);
            flagged_submissions.insert(Submission);
        }
    }

    bool c5 = flagged_submissions.find(submission) == flagged_submissions.end();
    int numSources = match_freq.size();
    // Patchwork plagiarism
    if(c5 && numSources >= 20){
        std::cerr << "Flagging submission: " << submission->student->get_name() << std::endl;
        if(submission->student != nullptr) submission->student->flag_student(submission);
        if(submission->professor != nullptr) submission->professor->flag_professor(submission);
        flagged_submissions.insert(submission);
    }
    return;
}
// End TODO