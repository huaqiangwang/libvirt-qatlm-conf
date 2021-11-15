//
// Created by david on 2021/11/12.
//

#include <string>
#include <list>
#include <regex>

#include "ConfItem.h"


namespace LibvirtConf {
    /**
     * Get the configuration item name and value from strings.
     * @param confStrings: stripped strings
     * @return: the configuration item type find.
     * ITEM_NONE for no valid configuration found.
     */
    ItemType ConfItem::parse(std::list<std::string> &confStrings) {
        std::string content;
        bool typeStringList = false;
        bool typeInt = false;
        ItemType retType = ItemType::ITEM_NONE;

        // In item.name there is no '-'
        //std::regex regItemIntValue(R"((\w+)\s*=\s*(\d+))");
        std::regex regItemIntValue("(\\w+)\\s*=\\s*(\\d+)");
        //std::regex regItemStringValue(R"((\w+)\s*=\s*\"(.*)\")");
        std::regex regItemStringValue("(\\w+)\\s*=\\s*\"(.*)\"");
        //std::regex regItemStringListValue(R"((\w+)\s*=\s*\[(.*)\])");
        std::regex regItemStringListValue("(\\w+)\\s*=\\s*\\[(.*)\\]");
        //std::regex regDevString(R"(\"(.*)\")");
        std::regex regDevString("\"([^\"]*)\"");

        std::smatch got;
        if (confStrings.empty())
            return retType;

        content = stringListJoin_(confStrings);
        if (content.empty() ||
            (content.find("=")) == std::string::npos)
            return retType;

        if (content.find("[") != std::string::npos)
            typeStringList = true;
        if (content.find("\"") == std::string::npos)
            // No '"' found, should be a Int type configuration
            typeInt = true;

        if (typeStringList) {
            if (std::regex_search(content, got, regItemStringListValue)) {
                auto strValue = got[2].str();
                value_.strList = new std::list<std::string>;
                std::smatch m;
                while (std::regex_search(strValue, m, regDevString)) {
                    value_.strList->push_back(*new std::string(m[1].str()));
                    strValue = m.suffix();
                }
                name_ = new std::string(got[1].str());
                retType = ItemType::ITEM_STRING_LIST;
                type_ = retType;
            }
        } else if (typeInt) {
            if (std::regex_search(content, got, regItemIntValue)) {
                // TODO: What about if raised an exception?
                name_ = new std::string(got[1].str());
                value_.i = std::stoi(got[2].str());
                retType = ItemType::ITEM_INT;
                type_ = retType;
            }
        } else {
            if (std::regex_search(content, got, regItemStringValue)) {
                name_ = new std::string(got[1].str());
                value_.s = new std::string(got[2].str());
                retType = ItemType::ITEM_STRING;
                type_ = retType;
            }
        }

        return retType;
    }

    std::string ConfItem::stringListJoin_(std::list<std::string> &confStrings) {
        std::string content;
        const std::string WHITESPACE = " \n\r\t\f\v";

        // Error: std::list<std::string::iterator> s = confStrings.begin();
        // OK for: autos s = confStrings.begin();
        for (std::list<std::string>::const_iterator s = confStrings.begin();
             s != confStrings.end(); ++s) {
            auto bNotLast = std::next(s) != confStrings.end();
            auto posStart = s->find_first_not_of(WHITESPACE);
            auto posEnd = s->find_last_not_of(WHITESPACE);
            if (posStart != std::string::npos && posEnd != std::string::npos) {
                content += s->substr(posStart, posEnd + 1 - posStart);
                if (bNotLast && not content.empty())
                    content += " ";
            }
        }
        // Returning a local variable will not have problem, return a reference of local
        // variable might have problem.
        return content;
    }

    bool ConfItem::operator == (const ConfItem& one) const{
        if (one.name_ != name_)
            return false;

        if (one.type_ != type_)
            return false;

        switch(type_) {
            case ItemType::ITEM_INT:
                if (one.value_.i == value_.i)
                    return true;
                    break;
            case ItemType::ITEM_STRING:
                if (one.value_.s == value_.s)
                    return true;
                break;
            case ItemType::ITEM_STRING_LIST:
                if (one.value_.strList == value_.strList)
                    return true;
                break;
            case ItemType ::ITEM_NONE:
                return true;
        }
        return false;

    }
}