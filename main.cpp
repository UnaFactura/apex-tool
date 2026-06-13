#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <numeric>
#include <thread>
#include <future>
#include <chrono>
#include <variant>
#include <optional>

// ============================================================================
// 1. ENUMS AND CONSTANTS (Modern scoped enums)
// ============================================================================
enum class TaskPriority {
    Low,
    Medium,
    High,
    Critical
};

// Helper function to convert enum to string for clean output
std::string priorityToString(TaskPriority priority) {
    switch (priority) {
        case TaskPriority::Low:      return "Low";
        case TaskPriority::Medium:   return "Medium";
        case TaskPriority::High:     return "High";
        case TaskPriority::Critical: return "Critical";
    }
    return "Unknown";
}

// ============================================================================
// 2. DATA STRUCTURES & POLYMORPHISM (Object-Oriented Programming)
// ============================================================================
class Task {
private:
    unsigned int id;
    std::string title;
    TaskPriority priority;
    bool completed;

public:
    // Constructor utilizing member initializer lists
    Task(unsigned int t_id, std::string t_title, TaskPriority t_priority)
        : id(t_id), title(std::move(t_title)), priority(t_priority), completed(false) {}

    // Virtual destructor for safe polymorphic deletion
    virtual ~Task() = default;

    // Getters and Setters
    unsigned int getId() const { return id; }
    std::string getTitle() const { return title; }
    TaskPriority getPriority() const { return priority; }
    bool isCompleted() const { return completed; }
    
    void markComplete() { completed = true; }

    // Pure virtual method making this an abstract base class
    virtual void executeWork() = 0;

    // Const-correct print utility
    virtual void printDetails() const {
        std::cout << "[" << id << "] " << title 
                  << " | Priority: " << priorityToString(priority)
                  << " | Status: " << (completed ? "Completed" : "Pending");
    }
};

// Derived Class 1: Modeling Compute-Intensive Work
class CodingTask : public Task {
private:
    unsigned int linesOfCode;

public:
    CodingTask(unsigned int id, std::string title, TaskPriority priority, unsigned int loc)
        : Task(id, std::move(title), priority), linesOfCode(loc) {}

    void executeWork() override {
        // Simulating heavy string manipulation or calculation
        std::string buffer;
        for (unsigned int i = 0; i < linesOfCode; ++i) {
            buffer += "CodeLine_" + std::to_string(i) + " ";
        }
        markComplete();
    }

    void printDetails() const override {
        Task::printDetails();
        std::cout << " (Coding Project: " << linesOfCode << " lines)" << std::endl;
    }
};

// Derived Class 2: Modeling IO / Analytical Work
class AnalyticalTask : public Task {
private:
    std::vector<double> datasets;

public:
    AnalyticalTask(unsigned int id, std::string title, TaskPriority priority, std::vector<double> data)
        : Task(id, std::move(title), priority), datasets(std::move(data)) {}

    void executeWork() override {
        // Calculate the sum via Standard Library numeric algorithms
        double sum = std::accumulate(datasets.begin(), datasets.end(), 0.0);
        // Suppress unused variable warning while keeping simulation logic
        (void)sum; 
        markComplete();
    }

    void printDetails() const override {
        Task::printDetails();
        std::cout << " (Analytics: Processing " << datasets.size() << " data nodes)" << std::endl;
    }
};

// ============================================================================
// 3. ENGINE CLASS (Demonstrating Smart Pointers and Generic STL Algorithms)
// ============================================================================
class TaskManager {
private:
    // Using std::unique_ptr to manage polymorphic memory lifecycles safely
    std::vector<std::unique_ptr<Task>> taskRegistry;

public:
    void addTask(std::unique_ptr<Task> task) {
        if (task) {
            taskRegistry.push_back(std::move(task));
        }
    }

    void listAllTasks() const {
        std::cout << "\n--- System Task Registry ---" << std::endl;
        for (const auto& task : taskRegistry) {
            task->printDetails();
        }
    }

    // Modern Lambda usage inside an STL find structure
    Task* findTaskById(unsigned int id) {
        auto it = std::find_if(taskRegistry.begin(), taskRegistry.end(), 
            [id](const std::unique_ptr<Task>& task) {
                return task->getId() == id;
            });
        
        return (it != taskRegistry.end()) ? it->get() : nullptr;
    }

    // Modern C++ standard sorting algorithm with inline predicate logic
    void sortByPriorityDescending() {
        std::sort(taskRegistry.begin(), taskRegistry.end(), 
            [](const std::unique_ptr<Task>& a, const std::unique_ptr<Task>& b) {
                return static_cast<int>(a->getPriority()) > static_cast<int>(b->getPriority());
            });
    }

    // Expose registry reference safely for parallel processing routines
    const std::vector<std::unique_ptr<Task>>& getRegistry() const {
        return taskRegistry;
    }
};

// ============================================================================
// 4. MULTITHREADING (Asynchronous workers using futures)
// ============================================================================
void asyncWorkerRoutine(Task* taskPtr) {
    if (!taskPtr) return;
    // Simulate complex latency delays safely 
    std::this_thread::sleep_for(std::chrono::milliseconds(400)); 
    taskPtr->executeWork();
}

// ============================================================================
// 5. ENTRY POINT (main)
// ============================================================================
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    std::cout << "Initializing Advanced Runtime Architecture Example..." << std::endl;

    TaskManager manager;

    // Adding items using memory-safe factory patterns (std::make_unique)
    manager.addTask(std::make_unique<CodingTask>(101, "Deploy Engine Upgrade", TaskPriority::Critical, 5000));
    manager.addTask(std::make_unique<AnalyticalTask>(102, "Process Telemetry Metrics", TaskPriority::Medium, std::vector<double>{12.4, 88.1, 43.9, 104.2}));
    manager.addTask(std::make_unique<CodingTask>(103, "Refactor Core Allocator", TaskPriority::High, 1200));
    manager.addTask(std::make_unique<AnalyticalTask>(104, "Parse Financial Audits", TaskPriority::Low, std::vector<double>{1.5, 9.9}));

    std::cout << "\n[State 1] Initial Tasks Ingested:";
    manager.listAllTasks();

    std::cout << "\n[Action] Re-ordering registry payload via priority matrices...";
    manager.sortByPriorityDescending();
    manager.listAllTasks();

    // Finding structural properties cleanly using safety pointers
    std::cout << "\n[Action] Extracting record metadata for Task 103...";
    if (Task* targetedTask = manager.findTaskById(103); targetedTask != nullptr) {
        std::cout << "\nTarget Found Successfully: ";
        targetedTask->printDetails();
    }

    // Modern Multithreaded execution using std::async and std::future
    std::cout << "\n[Action] Bootstrapping Parallel Task Processing Pool (Asynchronous)..." << std::endl;
    auto startClock = std::chrono::high_resolution_clock::now();

    std::vector<std::future<void>> workerPool;
    
    // Dispatch every task onto a background thread processing pipeline
    for (const auto& task : manager.getRegistry()) {
        workerPool.push_back(
            std::async(std::launch::async, asyncWorkerRoutine, task.get())
        );
    }

    // Await thread convergence (join operations handled implicitly via futures)
    for (auto& futureHandle : workerPool) {
        futureHandle.get();
    }

    auto endClock = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> executionDuration = endClock - startClock;

    std::cout << "\n[State 2] Parallel Pipeline Complete. Computation Runtime: " 
              << executionDuration.count() << " ms";
    manager.listAllTasks();

    std::cout << "\nDeallocating runtime instances safely. Execution termination clean." << std::endl;
    return 0;
}
