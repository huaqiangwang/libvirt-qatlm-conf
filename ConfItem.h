//
// Created by david on 2021/11/12.
//

#ifndef LIBVIRT_QATLM_CONF_CONFITEM_H
#define LIBVIRT_QATLM_CONF_CONFITEM_H

#include <string>
#include <list>

namespace LibvirtConf {
/**
 * Libvirt configuration item type
 */
    enum class ItemType {
        ITEM_NONE,
        ITEM_INT,
        ITEM_STRING,
        ITEM_STRING_LIST
    };

    union ItemValue {
        std::int64_t i;
        std::string s;
        std::list<std::string> strList;
    };

/**
 * Represents one libvirt style configuration Item.
 *
 * Possible configure type is (currently):
 * 1. Int value: Value is a integer number
 * 2. String
 * 3. String List
 */
    class ConfItem {
    public:
        ItemType parse(std::list<std::string> &);
        std::string &name() { return name_ };
        ItemValue &value() { return value_ };

    private:
        std::string stringListJoin_(std::list<std::string>&);
        std::string name_;
        ItemType type_;
        ItemValue value_;
    };

}
#endif //LIBVIRT_QATLM_CONF_CONFITEM_H
