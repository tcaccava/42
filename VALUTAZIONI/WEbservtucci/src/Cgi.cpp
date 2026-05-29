#include "Cgi.hpp"


Cgi::Cgi() {}

Cgi::~Cgi() {}

std::string Cgi::execute(const std::string& scriptPath) {
	if (scriptPath.empty())
		throw CgiException("Cgi: empty script path");
	return runForkExec(scriptPath.c_str());
}

// static helper (private) inside CgiExecutor.cpp

static void createPipeOrThrow(int tubes[2]) {
	if (pipe(tubes) == -1)
		throw CgiException(std::string("pipe failed: ") + strerror(errno));
}

// static helper
static pid_t spawnChildProcess(const char* script, int tubes[2], char** envp) {
	pid_t pid = fork();
	if (pid < 0) {
		close(tubes[0]);
		close(tubes[1]);
		throw CgiException(std::string("fork failed: ") + strerror(errno));
	}
	if (pid == 0) {
		// Child process
		close(tubes[0]); // Close read end
		if (dup2(tubes[1], STDOUT_FILENO) < 0)
			_exit(1);
		close(tubes[1]);
		const char* args[] = { script, NULL };
		execve(script, const_cast<char* const*>(args), envp);
		_exit(1);
	}
	return (pid); // Parent process returns child PID
}

// static helper
static std::string readFromPipe(int fd) {
	std::string result;
	const int BUF = 256;
	char buf[BUF];
	ssize_t n;

	while ((n = read(fd, buf, BUF - 1)) > 0) {
		buf[n] = '\0';
		result += buf;
	}
	if (n < 0)
		throw CgiException(std::string("read failed: ") + strerror(errno));
	return result;
}

std::string	Cgi::runForkExec(const char* script) {
	int	tubes[2];
	createPipeOrThrow(tubes);
	pid_t pid = spawnChildProcess(script, tubes, envp_);
	close(tubes[1]);  // parent closes write-end
	std::string result = readFromPipe(tubes[0]);
	close(tubes[0]);  // now safe to close read-end

	// Reap child
	int status = 0;
	waitpid(pid, &status, 0);
	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
		std::ostringstream oss;
		oss << "CGI script `" << script
			<< "` exited with status " << WEXITSTATUS(status);
		throw CgiException(oss.str());
	}
	return (result);
}

Cgi::CgiSpawn Cgi::spawn(const std::string &scriptPath) {
	std::cout << RED << "Cgi::CgiSpawn Cgi::spawn" << RESET << std::endl;
	std::cout << YELLOW << "inside the cgi:\tpath: " << RESET << scriptPath << std::endl;
	if (scriptPath.empty())
		throw CgiException("Cgi: empty script path");

	int tubes[2];
	if (pipe(tubes) < 0)
		throw CgiException(std::string("pipe failed: ") + strerror(errno));

	pid_t pid = fork();
	if (pid < 0) {
		close(tubes[0]);
		close(tubes[1]);
		throw CgiException(std::string("fork failed: ") + strerror(errno));
	}
	if (pid == 0) {
		close(tubes[0]);
		dup2(tubes[1], STDOUT_FILENO);
		close(tubes[1]);
		const char* argv[] = { scriptPath.c_str(), NULL };
		extern char** environ;
		execve(scriptPath.c_str(), const_cast<char* const*>(argv), environ);
		throw CgiException(std::string("execve failed: ") + strerror(errno));
		//_exit(1);  // if exec failed
	}
	// PARENT
	close(tubes[1]); // close write end
	 //TODO: this is key
	 // Set read end to non-blocking mode
	int flags = fcntl(tubes[0], F_GETFL, 0);
	if (flags == -1) {
		close(tubes[0]); // Clean up before throwing
		throw CgiException(std::string("fcntl get failed: ") + strerror(errno));
	}
	if (fcntl(tubes[0], F_SETFL, flags | O_NONBLOCK) == -1) {
		close(tubes[0]); // Clean up before throwing
		throw CgiException(std::string("fcntl set nonblock failed: ") + strerror(errno));
	}
	CgiSpawn result = { pid, tubes[0] };
	
	return result;
}

