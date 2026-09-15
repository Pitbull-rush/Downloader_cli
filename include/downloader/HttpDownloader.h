#include "downloader/DownloadTask.h"
#include "downloader/DownloadResult.h"

class HttpDownloader {
public:
	HttpDownloader() {}
	~HttpDownloader() {}

	DownloadResult work(DownloadTask& task_) {
		
		return task_;
	}
private:
	

};