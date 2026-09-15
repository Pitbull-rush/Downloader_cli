#pragma once

#include <thread>

#include "downloader/DownloadManager.h"
#include "downloader/HttpDownloader.h"

class Worker {
public:
	Worker(DownloadManager& manager) :manager_(manager) {}

	void manager_run_download() {

		while (!manager_.isStopRequested) {
			DownloadTask current_task = std::move(manager_.takeNextTask());
			result_ = httpDownload.Get_work(current_task);


		}
	}


private:
	DownloadManager manager_;
	HttpDownloader httpDownload;
	DownloadResult result_:
};