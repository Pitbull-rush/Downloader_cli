#include <iostream>
#include "downloader/DownloadManager.h"


using std::cout;
using std::cin;
using std::endl;

int main() {
	DownloadManager manager;

    DownloadTask task(
        "https://example.com/file.zip",
        "downloads/file.zip",
        1
    );

    manager.addTask(task);

    std::cout << "Task in queue: "
        << manager.taskCount()
        << '\n';

    DownloadTask nextTask = manager.takeNextTask();

    std::cout << "Get task nomber: "
        << nextTask.getId()
        << '\n';
};