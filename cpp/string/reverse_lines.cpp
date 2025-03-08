#include <iostream>
#include <iomanip>
#include <string>
#include <string_view>
#include <execution>
#include <numeric>
#include <algorithm>
#include <thread>
#include <filesystem>
#include <fstream>

// namespace this_thread = std::this_thread;

namespace ex = std::execution;
namespace fs = std::filesystem;

void reverse_lines(fs::path path, int offset, int size) {
    std::ifstream file{ path };
    if (!file.is_open()) {
        std::cerr << "Unable to open file" << std::endl;
        return;
    }

    file.seekg(offset, std::ios::beg);

    std::string line;
    if (offset)
        std::getline(file, line);

    while (std::getline(file, line)) {
        std::reverse(line.begin(), line.end());
        std::cout << line << std::endl;
    }

    file.close();
}

int main() {
    fs::path path("file.txt");

    if (!fs::exists(path)) {
        std::cerr << "File " << path << " does not exist" << std::endl;
        return -1;
    }

    std::uintmax_t filesize = fs::file_size(path);

    static const auto cores{ (int)std::thread::hardware_concurrency() };
    const auto chunk{ filesize / cores };

    std::vector<std::thread> threads;
    threads.reserve((size_t)cores);

    auto offset{ chunk };
    for (auto i=1; i < cores; ++i) {
        threads.emplace_back(reverse_lines, path, offset, chunk);
        offset += chunk;
    }

    reverse_lines(path, 0, chunk);

    for (auto& t : threads)
        t.join();
}
