#include <iostream>
#include <string>
#include <functional>

std::string header(const std::string& text, int level) {
    return "<h" + std::to_string(level) + ">" + text + "</h" + std::to_string(level) + ">";
}

std::string paragraph(const std::string& text) {
    return "<p>" + text + "</p>";
}

std::string markdownToHtml(const std::string& markdown) {
    std::string html;

    auto lines = [&markdown]{
        std::vector<std::string> lines;
        std::string line;
        std::istringstream iss(markdown);
        while (std::getline(iss, line)) {
            lines.push_back(line);
        }
        return lines;
    }();

    for (const auto& line : lines) {
        if (line.substr(0, 2) == "# ") {
            html += header(line.substr(2), 1);
        } else {
            html += paragraph(line);
        }
        html += "\n";
    }

    return html;
}

int main() {
    std::string markdown = "# Hello, world!\nThis is a paragraph.";
    std::string html = markdownToHtml(markdown);
    std::cout << html << std::endl;
    return 0;
}

