//
// Created by david on 2021/11/15.
//

#include <string>
#include <vector>
#include <gtest/gtest.h>
#include <cstring>
#include <iostream>

#include "LibvirtConf.h"
#include "ConfItem.h"

using namespace LibvirtConf;

TEST(TestLivirtConf, testReadConf) {
    struct testItems_ {
        char* confFile;
        std::vector<ConfItem> confItems;
    };

    std::vector<testItems_> items = {
        {
            "qemu.conf",
            {
                ConfItem("clear_emulator_capabilities",
                         ItemType::ITEM_INT,
                         ItemValue(0, "", {"",""})),
                ConfItem("clear_emulator_capabilities",
                         ItemType::ITEM_INT,
                         ItemValue(0, "", {"",""})),
            },
        },
    };

    for (auto item : items) {
        auto conf = new VirtSetting();
        char fileNameBuf[256] = "/home/david/2021/libvirt-qatlm-conf/test/data/testLibvirtConf/";
        std::strcat(fileNameBuf, item.confFile);
        std::string fileName(fileNameBuf);

        auto results = conf->GetConf(fileName);
        for (auto i = 0; i < sizeof(*results); i++) {
            EXPECT_EQ((*results)[i], item.confItems[i]);
        }
    }
}

TEST(TestLivirtConf, testWriteConfFile) {
    auto conf = new VirtSetting();
    char fileNameBuf[256] = "/home/david/2021/libvirt-qatlm-conf/test/data/testLibvirtConf/";
    std::strcat(fileNameBuf, "qemu.conf");
    std::string fileName(fileNameBuf);
    conf->GetConf(fileName);

    conf->GetQATDevList();
    conf->UpdateQATDevices();

    std::string fileOut("qemu-auto.conf");
    conf->GenerateConfFile(fileOut);
}

TEST(TestLivirtConf, testGetDevList) {
    auto conf = new VirtSetting();

    auto devs = conf->GetQATDevList();
    EXPECT_TRUE(!devs->empty());
}
