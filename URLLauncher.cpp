#include "URLLauncher.h"
#include <cerrno>
#include <cstring>
#include <iostream>
#include <unistd.h>

void URLLauncher::openURL(const std::string& url) {
    // Prefer Chrome when available; fall back to Chromium or xdg-open.
    pid_t pid = fork();
    if (pid == 0) {
        execlp("google-chrome", "google-chrome", "--new-tab", url.c_str(), (char*)nullptr);
        execlp("chromium", "chromium", "--new-tab", url.c_str(), (char*)nullptr);
        execlp("xdg-open", "xdg-open", url.c_str(), (char*)nullptr);
        std::cerr << "URLを開けませんでした: " << std::strerror(errno) << std::endl;
        _exit(127);
    }
    if (pid < 0) {
        std::cerr << "プロセス生成に失敗しました: " << std::strerror(errno) << std::endl;
    }
}
