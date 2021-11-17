//
// Created by david on 2021/11/12.
//

#include <string>
#include <vector>
#include <cinttypes>
#include <regex>

#include "ConfItem.h"


namespace LibvirtConf {
    std::vector<std::string> ConfItem::toStrings() {
        std::vector<std::string> retStrings;
        auto buf = std::make_unique<char[]>(256);
        int pos = 0;

        switch(type_) {
            case ItemType::ITEM_INT:
                std::snprintf(buf.get(), 256, "%s = %" PRId64, name_.c_str(), value_.GetInt());
                retStrings.push_back(buf.get());
                break;
            case ItemType::ITEM_STRING:
                std::snprintf(buf.get(), 256, "%s = \"%s\"", name_.c_str(), value_.GetStr().c_str());
                retStrings.push_back(buf.get());
                break;
            case ItemType::ITEM_STRING_LIST:
                std::snprintf(buf.get(), 256, "%s = [", name_.c_str());
                retStrings.push_back(buf.get());
                for (auto item: value_.GetStrList()) {
                    // output line with a length less than 80 chars
                    // 2 is size for two extra \" sign
                    if (pos + item.size() + 2 > 80) {
                        // tune " " char to a string ending char
                        buf[--pos] = 0;
                        retStrings.push_back(buf.get());
                        pos = 0;
                    }
                    buf[pos++]='\"';
                    std::memcpy(&buf[pos], item.c_str(), item.size());
                    pos += item.size();
                    buf[pos++]='\"';
                    buf[pos++] = ',';
                    buf[pos++] = ' ';
                }
                if (pos) {
                    // ending with ', '. end the string with '\0'
                    buf[pos-2] = 0;
                    retStrings.push_back(buf.get());
                }
                retStrings.push_back("]");
                break;
        }
        return retStrings;
    }


    /**
     * Get the configuration item name and value from strings.
     * @param confStrings: stripped strings
     * @return: the configuration item type find.
     * ITEM_NONE for no valid configuration found.
     */
    ItemType ConfItem::parse(std::vector<std::string> &confStrings) {
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
                std::vector<std::string> *strList = new std::vector<std::string>;
                std::smatch m;
                while (std::regex_search(strValue, m, regDevString)) {
                    strList->push_back(*new std::string(m[1].str()));
                    strValue = m.suffix();
                }
                value_.Set(*strList);
                name_ = std::string(got[1].str());
                retType = ItemType::ITEM_STRING_LIST;
                type_ = retType;
            }
        } else if (typeInt) {
            if (std::regex_search(content, got, regItemIntValue)) {
                // TODO: What about if raised an exception?
                name_ = std::string(got[1].str());
                value_.Set(std::stoi(got[2].str()));
                retType = ItemType::ITEM_INT;
                type_ = retType;
            }
        } else {
            if (std::regex_search(content, got, regItemStringValue)) {
                name_ = std::string(got[1].str());
                auto s = std::string(got[2].str());
                value_.Set(s);
                retType = ItemType::ITEM_STRING;
                type_ = retType;
            }
        }

        return retType;
    }

    std::string ConfItem::stringListJoin_(std::vector<std::string> &confStrings) {
        std::string content;
        const std::string WHITESPACE = " \n\r\t\f\v";

        // Error: std::vector<std::string::iterator> s = confStrings.begin();
        // OK for: autos s = confStrings.begin();
        for (std::vector<std::string>::const_iterator s = confStrings.begin();
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

    bool ConfItem::operator == (ConfItem one) const{
        if (one.name_ != name_)
            return false;

        if (one.type_ != type_)
            return false;

        switch(type_) {
            case ItemType::ITEM_INT:
                if (one.GetValueInt() == GetValueInt())
                    return true;
                    break;
            case ItemType::ITEM_STRING:
                if (one.GetValueStr() == GetValueStr())
                    return true;
                break;
            case ItemType::ITEM_STRING_LIST:
                if (one.GetValueStrList() == GetValueStrList())
                    return true;
                break;
            case ItemType ::ITEM_NONE:
                return true;
        }
        return false;

    }

    ConfItem::ConfItem(const ConfItem& confItem) {
        name_ = confItem.name();
        type_ = confItem.type_;
        value_ = confItem.value_;
    }

    bool ConfItem::InStrList(std::string &item) {
        if (type_ != ItemType::ITEM_STRING_LIST)
            return false;

        for (const auto & s : value_.GetStrList()) {
            if (s == item)
                return true;
        }

        return false;
    }

    bool ConfItem::InsertItemInStrList(std::string item) {
        if (type_ != ItemType::ITEM_STRING_LIST)
            return false;

        value_.GetStrList().push_back(item);
        return true;
    }
}