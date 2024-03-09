#include <iostream>
#include <string>
#include <sstream>

using namespace std;

namespace markdown_decode {
     std::string markdownToHTML(std::string markdown) {
        for(int i = 0; i < markdown.length() ; i++) {
            if(markdown[i] == '~')
                if(markdown[i+1] == 'n') {
                    markdown[i] = ' ';
                    markdown[i+1] = '\n';
                    i++;
                }
        }
        std::istringstream mdStream(markdown);
        std::string line;
        std::string html = "";

        string buffer = "";
        bool bufferEmpty = true;

        while (std::getline(mdStream, line)) {
            if(!bufferEmpty) {
                if (line[0] == '=') {
                    bool isLine = true;
                    for(const auto c : line) {
                        if(c != '=')
                            if(c != '\n' && c != ' ')
                                isLine = false;
                            else
                                break;
                    }
                    if(isLine) {
                        html += "<h1>";
                        html += buffer;
                        html += "</h1>\n";
                        buffer = "";
                        bufferEmpty = true;
                    }
                    continue;
                }
                if (line[0] == '-') {
                    bool isLine = true;
                    for(const auto c : line) {
                        if(c != '-')
                            if(c != '\n' && c != ' ')
                                isLine = false;
                            else
                                break;
                    }
                    if(isLine) {
                        html += "<h2>";
                        html += buffer;
                        html += "</h2>\n";
                        buffer = "";
                        bufferEmpty = true;
                    }
                    continue;
                }
                else {
                    html += "<p>";
                    html += buffer;
                    html += "</p>\n";
                    buffer = "";
                    bufferEmpty = true;
                }
            }

            // 标题语法
            if (line[0] == '#') {
                int count = 0;
                while (line[count] == '#') { count++; }
                html += "<h" + std::to_string(count) + ">" + line.substr(count) + "</h" + std::to_string(count) + ">\n";
            }
            else if (line[0] == '-') {
                bool isLine = true;
                for(const auto c : line) {
                    if(c != '-')
                        if(c != '\n' && c != ' ')
                            isLine = false;
                        else
                            break;
                }
                if(isLine) {
                    html += "<hr/>";
                    html += "\n";
                }
            }
            // 段落语法
            else {
                buffer += line;
                bufferEmpty = false;
            }
        }

        return html;
    }
}

