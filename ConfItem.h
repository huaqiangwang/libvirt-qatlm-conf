//
// Created by david on 2021/11/12.
//

#ifndef LIBVIRT_QATLM_CONF_CONFITEM_H
#define LIBVIRT_QATLM_CONF_CONFITEM_H

#include <string>
#include <vector>

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

    class ItemValue {
    private:
        std::int64_t i_;
        std::string s_;
        std::vector<std::string> strList_;
    public:
        int64_t & GetInt() const {
            return const_cast<int64_t &>(i_);
        }
        std::string & GetStr() const {
            return const_cast<std::string &>(s_);
        }
        std::vector<std::string> & GetStrList() const {
            return const_cast<std::vector<std::string> &>(strList_);
        }

        void Set(std::int64_t i) {
            i_ = i;
        }
        void Set(std::string& s) {
            s_ = s;
        }
        void Set(std::vector<std::string>& strList) {
            strList_ = strList;
        }
        ItemValue(std::int64_t vi, std::string s, std::vector<std::string> strList){
            i_ = vi;
            s_ = s;
            strList_ = strList;
        }
        ItemValue(const ItemValue& obj) {
            i_ = obj.GetInt();
            s_ = obj.GetStr();
            strList_ = obj.GetStrList();
        }
        ItemValue(): i_(0){};
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
        ConfItem(std::string n, ItemType t, ItemValue v) {
            name_ = n;
            type_ = t;
            value_ = v;
        }

        ConfItem(const ConfItem& confItem);

        bool operator == (ConfItem) const;
        ConfItem() {};
        ItemType parse(std::vector<std::string> &);
        std::vector<std::string> toStrings();
        std::string &name() const{ return const_cast<std::string &>(name_); }

        ItemType &type() {return type_;}
        int64_t & GetValueInt() const {return value_.GetInt();}
        std::string & GetValueStr() const {return value_.GetStr();}
        std::vector<std::string> & GetValueStrList() const {return value_.GetStrList();}
        void SetValue(std::int64_t &i) {value_.Set(i);}
        void SetValue(std::string &s) {value_.Set(s);}
        void SetValue(char*s) {auto str=std::string(s);value_.Set(str);}
        void SetValue(std::vector<std::string> & strList) {value_.Set(strList);}

    protected:
        std::string stringListJoin_(std::vector<std::string>&);
    private:
        std::string name_;
        ItemType type_;
        ItemValue value_;
    };

}
#endif //LIBVIRT_QATLM_CONF_CONFITEM_H
