//
// Created by david on 2021/11/12.
//

#include <string>
#include <list>
#include <iterator>
#include <regex>

#include "ConfItem.h"


namespace LibvirtConf {
    /**
     * Get the configuration item name and value from strings.
     * @param confStrings: stripped strings
     * @return: the configuration item type find.
     * ITEM_NONE for no valid configuration found.
     */
    ItemType ConfItem::parse(std::list<std::string>& confStrings) {
        const std::string WHITESPACE = " \n\r\t\f\v";
        std::string content;
        bool typeStringList = false;
        bool typeInt = false;
        ItemType retType = ItemType::ITEM_NONE;

        std::smatch match;
        // In item.name there is no '-'
        std::regex regItemIntValue(R"((\w+)\s*=\s*(\d+))");
        std::regex regItemStringValue( R"((\w+)\s*=\s*\"(.*)\")");
        std::regex regItemStringListValue(R"((\w+)\s*=\s*\[(.*)\])");

        if (confStrings.empty)
            return retType;

        content = stringListJoin_(confStrings);
        if (content.empty() ||
            (posEqualSign = content.find("=")) == std::string::npos)
            return retType;

        if (content.find("[") == std::string::npos)
            typeStringList = true;
        if (content.find("\"") != std::string::npos)
            typeInt = true;

        if (typeStringList){
            if (std::regex_search(content, match, regItemStringListValue)) {
                auto strValue = match[1];
                std::smatch m;
                std::list<std::string> strList;
                while (std::regex_search(strValue, m, R"(\"(.*)\")")) {
                    strList.push_back(m[0]);
                    strValue = m.suffix();
                }
                name_ = match[0];
                value_.strList = strList;
                retType = ItemType::ITEM_STRING_LIST;
            }
        } else if (typeInt) {
            if (std::regex_search(content, match, regItemIntValue)) {
                // TODO: What about if raised an exception?
                name_ = match[0];
                value_.i = std::stoi(match[1]);
                retType = ItemType::ITEM_INT;
            }
        } else {
            if (std::regex_search(content, match, regItemStringValue)) {
                name_ = match[0];
                value_.s = match[1];
                retType = ItemType::ITEM_STRING;
            }
        }

        return retType;
    }

    std::string ConfItem::stringListJoin_(std::list<std::string>& confStrings) {
        std::string content;

        for (std::list<std::string::const_iterator s = confStrings.begin();
             s != confStrings.end(); ++s) {
            auto posStart= s->find_first_not_of(WHITESPACE);
            auto posEnd = s->find_last_not_of(WHITESPACE);
            if (posStart != std::string::npos && posEnd != std::string::npos) {
                content += subs->substr(posStart, posEnd);
            }
        }
        // Returning a local variable will not have problem, return a reference of local
        // variable might have problem.
        return content;
    }
}