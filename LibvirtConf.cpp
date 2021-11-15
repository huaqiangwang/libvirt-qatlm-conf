//
// Created by david on 2021/11/12.
//

#include <vector>
#include <string>
#include <fstream>
#include <list>
#include <regex>

#include "ConfItem.h"
#include "LibvirtConf.h"

namespace LibvirtConf {

    std::vector<ConfItem>* VirtSetting::GetConf(std::string& conFile) {
        std::ifstream fConf(conFile, std::ios::in);
        if (!fConf.is_open()) {
            // How to get error
            return NULL;
        }

        std::vector<std::list<std::string>> confTextSections;
        std::list<std::string>* textSection = new std::list<std::string>;


        std::string line;
        std::regex regComment("\\s*#");
        std::smatch m;
        const std::string WHITESPACE = " \n\r\t\f\v";

        while (std::getline(fConf, line)) {
            // skip empty line or line with only blanks
            if (line.find_first_not_of(WHITESPACE) == std::string::npos)
                continue;

            if (std::regex_search(line, m, regComment))
                continue;

            if (line.find("=") != std::string::npos) {
                if (!textSection->empty()) {
                    confTextSections.push_back(*textSection);
                    textSection = new std::list<std::string>;
                }
            }
            //TODO: Pay attention to big projcts's exception handling
            textSection->push_back(line);
        }
        fConf.close();

        if (!textSection->empty())
            confTextSections.push_back(*textSection);

        for (auto section: confTextSections) {
            auto item = new ConfItem();
            item->parse(section);
            items_->push_back(*item);
        }

        return items_;
    }
}