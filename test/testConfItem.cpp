//
// Created by wanghuaq on 11/14/2021.
//

#include <string>
#include <list>
#include <gtest/gtest.h>

#include <ConfItem.h>

namespace LibvirtConf {
    TEST(TestLibvirtConf, smock) {
        EXPECT_EQ(1,1);
    }

    // Using multiple
    class TestConfItem: public ::testing::Test, public ConfItem {
    public:
        std::string joinList(std::list<std::string> &list) {
          return stringListJoin_(list);
        }
    };


    TEST_F(TestConfItem, testJoinString){
        struct TestItem_{
            std::list<std::string> strList;
            std::string result;
        };
        std::vector<TestItem_> items {
                {
                        {"", ""},
                        ""
                },
                {
                        {"  ", "  "},
                        ""
                },
                {
                        {"Hello", "friend"},
                        "Hello friend"
                },
                {
                        {" Hello   ", "   friend  "},
                        "Hello friend"
                },
                {
                        {" \t \n Hello \n \t  ", "   \t friend\n  \n"},
                        "Hello friend"
                },
                {
                        {" \t Hello \n", "my", "  friend \n"},
                        "Hello my friend"
                }
        };
      for (auto item : items) {
          auto ret = joinList(item.strList);
          EXPECT_EQ(item.result, ret);
      }
    }

    TEST_F(TestConfItem, testParse) {
        struct TestItem_ {
            std::list<std::string> strList;
            ItemType result;
            std::string name;
            int iValue;
            std::string sValue;
        };
        std::vector<TestItem_> items{
                {
                        {"tls_allowed_dn_list = [\"DN1\", \"DN2\"]"},
                        ItemType::ITEM_STRING_LIST,
                        "tls_allowed_dn_list",
                        0,
                        "DN1 DN2"
                },
                {
                        {"test = 1",},
                        ItemType::ITEM_INT,
                        "test",
                        1,
                        ""
                },
                {
                        {"test = \"s\""},
                        ItemType::ITEM_STRING,
                        "test",
                        0,
                        "s"
                },
                {
                        {"test = ", "\"s\""},
                        ItemType::ITEM_STRING,
                        "test",
                        0,
                        "s"
                },

                {
                        {
                         "cgroup_device_acl = [",
                                    "\"/dev/null\", \"/dev/full\", \"/dev/zero\",",
                                "\"/dev/random\", \"/dev/urandom\",",
                                "\"/dev/ptmx\", \"/dev/kvm\", \"/dev/kqemu\",",
                                "\"/dev/rtc\", \"/dev/hpet\", \"/dev/sev\",",
                                "\"/dev/usdm_drv\", \"/dev/qat_dev_processes\",",
                                "\"/dev/qat_adf_ctl\"",
                                "]"
                        },
                        ItemType::ITEM_STRING_LIST,
                        "cgroup_device_acl",
                        0,
                        "/dev/null /dev/full /dev/zero /dev/random /dev/urandom "
                        "/dev/ptmx /dev/kvm /dev/kqemu /dev/rtc /dev/hpet /dev/sev "
                        "/dev/usdm_drv /dev/qat_dev_processes /dev/qat_adf_ctl"
                },
        };
        for (auto item : items) {
            auto ret = parse(item.strList);
            EXPECT_EQ(item.result, ret);
            EXPECT_EQ(*name(), item.name);
            switch(ret) {
                case ItemType::ITEM_INT:
                    EXPECT_EQ(value().i, item.iValue);
                    break;

                case ItemType::ITEM_STRING:
                    EXPECT_EQ(*(value().s), item.sValue);
                    break;

                case ItemType::ITEM_STRING_LIST:
                    EXPECT_EQ(joinList(*(value().strList)), item.sValue);
                    break;
            }
        }
    }
}
