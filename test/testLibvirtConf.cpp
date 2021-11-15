//
// Created by david on 2021/11/15.
//

#include <string>
#include <vector>
#include <gtest/gtest.h>
#include <iostream>

#include "LibvirtConf.h"
#include "ConfItem.h"

using namespace LibvirtConf;

TEST(TestLivirtConf, testReadConf) {
    struct testItems_ {
        std::string confFile;
        std::vector<ConfItem> confItems;
    };

    std::vector<testItems_> items = {
        {
            "qemu.conf",
            {
                ConfItem("clear_emulator_capabilities",
                         ItemType::ITEM_INT,
                         ItemValue(0, NULL, NULL)),
                ConfItem("clear_emulator_capabilities",
                         ItemType::ITEM_INT,
                         ItemValue(0, NULL, NULL)),
            },
        },
    };

    for (auto item : items) {
        auto conf = new VirtSetting();
        auto fileName = "/home/david/2021/libvirt-qatlm-conf/test/data/testLibvirtConf/" + item.confFile;

        auto results = conf->GetConf(fileName);
        for (auto i = 0; i < sizeof(*results); i++) {
		std::cout << (*results)[i].name()<< std::endl;
            EXPECT_EQ((*results)[i], item.confItems[i]);
        }
    }
}
