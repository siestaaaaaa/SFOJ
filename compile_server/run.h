#pragma once

#include <fcntl.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/wait.h>

#include "../include/util.h"

namespace run {
using namespace util;

#define LOG(level) log_util::log(#level, __FILE__, __LINE__)

class runner {
  static void set_limit(int cpu, int mem) {
    struct rlimit cpu_rlimit;
    cpu_rlimit.rlim_max = RLIM_INFINITY;
    cpu_rlimit.rlim_cur = cpu;
    setrlimit(RLIMIT_CPU, &cpu_rlimit);

    struct rlimit mem_rlimit;
    mem_rlimit.rlim_max = RLIM_INFINITY;
    mem_rlimit.rlim_cur = mem * 1024;
    setrlimit(RLIMIT_AS, &mem_rlimit);
  }

 public:
  // < 0: syscall failed
  // == 0: ok
  // > 0: signal
  static int start(const std::string& filename, int cpu_limit,
                   int mem_limit /*KB*/) {
    auto _exe = path_util::exe(filename);
    auto _stdin = path_util::_stdin(filename);
    auto _stdout = path_util::_stdout(filename);
    auto _stderr = path_util::_stderr(filename);

    auto _stdin_fd = open(_stdin.c_str(), O_CREAT | O_RDONLY, 0644);
    auto _stdout_fd = open(_stdout.c_str(), O_CREAT | O_WRONLY, 0644);
    auto _stderr_fd = open(_stderr.c_str(), O_CREAT | O_WRONLY, 0644);
    if (_stdin_fd < 0 || _stdout_fd < 0 || _stderr_fd < 0) {
      LOG(WARNING) << "open() failed\n";
      return -1;
    }

    auto pid = fork();
    if (pid < 0) {
      close(_stdin_fd);
      close(_stdout_fd);
      close(_stderr_fd);
      LOG(ERROR) << "fork() failed\n";
      return -2;
    } else if (pid == 0) {
      dup2(_stdin_fd, 0);
      dup2(_stdout_fd, 1);
      dup2(_stderr_fd, 2);
      set_limit(cpu_limit, mem_limit);

      execl(_exe.c_str(), _exe.c_str(), nullptr);

      LOG(ERROR) << "execl() failed\n";
      exit(1);
    } else {
      close(_stdin_fd);
      close(_stdout_fd);
      close(_stderr_fd);

      int status = 0;
      waitpid(pid, &status, 0);
      LOG(INFO) << "run() finished\n";
      return status & 0x7f;
    }
  }
};

#undef LOG
}  // namespace run