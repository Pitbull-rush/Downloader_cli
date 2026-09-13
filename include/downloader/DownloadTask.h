#include <iostream>
#include <string>


class DownloadTask {
public:
	DownloadTask(const std::string &url, const std::string &targetPath, int id) :url(url),targetPath(targetPath), id(id){
		if (!url.size()) {
			throw std::runtime_error("URL is empty");
		}
	}
	DownloadTask(const DownloadTask&) = default;


	std::string getUrl() {
		return url;
	}

	std::string getTargetPath() {
		return targetPath;
	}

	int getId() {
		return id;
	}

private:
	std::string url;
	std::string targetPath;
	int id;
};