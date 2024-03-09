#include <iostream>
#include <string>
#include <sstream>

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

        while (std::getline(mdStream, line)) {
            // 标题语法
            if (line[0] == '#') {
                int count = 0;
                while (line[count] == '#') { count++; }
                html += "<h" + std::to_string(count) + ">" + line.substr(count) + "</h" + std::to_string(count) + ">\n";
            }
            // 段落语法
            else {
                html += "<p>" + line + "</p>\n";
            }
        }

        return html;
    }
}

