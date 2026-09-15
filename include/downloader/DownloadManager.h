#pragma once

#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <queue>
#include <stdexcept>

#include "downloader/DownloadResult.h"
#include "downloader/DownloadTask.h"

class DownloadManager {
public:
    DownloadManager() = default;

    ~DownloadManager() {
        requestStop();
    }

    void addTask(const DownloadTask& task) {
        {
            std::lock_guard<std::mutex> lock(mutex_);

            if (stopRequested_) {
                throw std::runtime_error(
                    "Нельзя добавить задачу: менеджер остановлен"
                );
            }

            tasks_.push(task);
            state_.queueCount++;
        }

        tasksCondition_.notify_one();
    }

    void infoResult(const DownloadResult& result) {
        {
            std::lock_guard<std::mutex> lock(mutex_);

            if (state_.activeCount == 0) {
                throw std::logic_error(
                    "Получен результат при отсутствии активных задач"
                );
            }

            state_.activeCount--;
            state_.completedCount++;
            results_.push(result);
        }

        tasksCondition_.notify_all();
    }

    DownloadTask takeNextTask() {
        std::unique_lock<std::mutex> lock(mutex_);

        tasksCondition_.wait(lock, [this] {
            return !tasks_.empty() || stopRequested_;
            });

        if (tasks_.empty() && stopRequested_) {
            throw std::runtime_error(
                "Менеджер остановлен, новых задач нет"
            );
        }

        DownloadTask task = std::move(tasks_.front());
        tasks_.pop();

        state_.queueCount--;
        state_.activeCount++;

        return task;
    }

    DownloadResult takeNextResult() {
        std::unique_lock<std::mutex> lock(mutex_);

        tasksCondition_.wait(lock, [this] {
            return !results_.empty() || allTasksCompleted();
            });

        if (results_.empty()) {
            throw std::runtime_error(
                "Новых результатов нет, все задачи завершены"
            );
        }

        DownloadResult result = std::move(results_.front());
        results_.pop();

        return result;
    }

    void requestStop() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            stopRequested_ = true;
        }

        tasksCondition_.notify_all();
    }

    bool hasTask() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return !tasks_.empty();
    }

    std::size_t taskCount() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return state_.queueCount;
    }

    std::size_t activeTaskCount() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return state_.activeCount;
    }

    std::size_t completedTaskCount() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return state_.completedCount;
    }

    bool isStopRequested() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return stopRequested_;
    }

private:
    bool allTasksCompleted() const {
        return tasks_.empty() &&
            state_.activeCount == 0;
    }

private:
    mutable std::mutex mutex_;
    std::condition_variable tasksCondition_;

    bool stopRequested_ = false;

    std::queue<DownloadTask> tasks_;
    std::queue<DownloadResult> results_;

    struct ManagerState {
        std::size_t queueCount = 0;
        std::size_t activeCount = 0;
        std::size_t completedCount = 0;
    } state_;
};