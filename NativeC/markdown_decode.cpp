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
        std::string html = "<!DOCTYPE html>\n<html>\n";
        html += "<head>\n";
        html += " <meta charset=\"UTF-8\">\n";
        html += "</head>\n";
        html += "<body>\n";

        string buffer = "";
        bool bufferEmpty = true;

        while (std::getline(mdStream, line)) {
            if(!bufferEmpty) {
                if(buffer[0] == ' ' || buffer[0] == '\n') {
                    html += "<p>";
                    html += buffer;
                    html += "</p>\n";
                    buffer = "";
                    bufferEmpty = true;
                }
                else if (line[0] == '=') {
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
            // 分割线
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
                // 不是横线，那么应该就是列表了
                // 等一下，这里不是很有思路，先放着
                else {
                    html += "<li>";
                    html += line;
                    html += "</li>";
                }
            }
            // 一般段落
            else {
                string buffer2 = line;
                bufferEmpty = false;
                //加粗和倾斜使用
                int count = 0;
                bool end = false;
                bool DUOC = false;
                bool start = true;
                //记号笔使用
                int count2 = 0;
                bool end2 = false;
                bool start2 = true;
                for(const auto c : buffer2) {
                    // 加粗与倾斜
                    if(c == '*') {
                        if(end) {
                            count--;
                            if(count == 0) {
                                end = false;
                                if(DUOC)
                                    buffer += "</b>";
                                else
                                    buffer += "</i>";
                            }
                        }
                        else {
                            count ++;
                            start = true;
                        }
                    }
                    else if(count == 1) {
                        if(start) {
                            buffer += "<i>";
                            buffer += c;
                            DUOC = false;
                            start = false;
                        }
                        else
                            buffer += c;
                        end = true;
                    }
                    else if(count == 2) {
                        if(start) {
                            buffer += "<b>";
                            buffer += c;
                            DUOC = true;
                            start = false;
                        }
                        else
                            buffer += c;
                        end = true;
                    }
                    // 记号笔
                    else if(c == '=') {
                        if(end2) {
                            count2--;
                            if(count2 == 0) {
                                end2 = false;
                                buffer += "</a>";
                            }
                        }
                        else {
                            count2++;
                            start2 = true;
                        }
                    }
                    else if(count2 == 2) {
                        if(start2) {
                            start2 = false;
                            end2 = true;
                            buffer += "<a style=\"background-color: #FFFF00\">";
                            buffer += c;
                        }
                        else
                            buffer += c;
                    }
                    else {
                        buffer += c;
                    }
                }
            }
        }

        //清空缓存区
        if (!bufferEmpty) {
            html += "<p>";
            html += buffer;
            html += "</p>";
            buffer = "";
            bufferEmpty = true;
        }

        html += "</body>\n</html>\n";

        return html;
    }
}

