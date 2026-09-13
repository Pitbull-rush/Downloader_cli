#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

enum class Status {
	Success,
	Failed
};

class DownloadResult {
public:
	DownloadResult()
		: id(0),
		status(Status::Failed),
		bytesDownloaded(0) {
	}

	DownloadResult(int id, std::string url, std::string targetPath,Status status, std::uint64_t bytesDownloaded, std::string  errorMessage)
	:id(id)
	,url(url)
	,targetPath(targetPath)
	, status(status)
	, bytesDownloaded(bytesDownloaded)
	, errorMessage(errorMessage){}

	int getId() {
		return id;
	}
	const std::string& getURL() const{
		return url;
	}
	const std::string& getTargetPath() const{
		return targetPath;
	}
	Status getStatus() const{
		return status;
	}
	std::uint64_t getByteDownloaded() const{
		return bytesDownloaded;
	}
	
	const std::string& geterrorMessage() const{
		return errorMessage;
	}

	void SetStatus(Status status_) {
		status = status_;
	}

	void setBytesDownloaded(std::uint64_t bytes) {
		bytesDownloaded = bytes;
	}

	void setErrorMessage(const std::string& msg) {
		errorMessage = msg;
	}

private:
	int id;
	std::string url;
	std::string targetPath;
	Status status;
	std::uint64_t bytesDownloaded;
	std::string  errorMessage;
};