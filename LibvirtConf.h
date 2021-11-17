//
// Created by david on 2021/11/12.
//

#include <vector>
#include <string>

#include "ConfItem.h"

#ifndef LIBVIRT_QATLM_CONF_LIBVIRT_CONF_H
#define LIBVIRT_QATLM_CONF_LIBVIRT_CONF_H

namespace LibvirtConf {
    class VirtSetting {
    public:
        VirtSetting() {
            items_ = new std::vector<ConfItem>;
            devs_ = new std::vector<std::string>;

        }
        ~VirtSetting() {
            delete items_;
            delete devs_;
        }
        std::vector<ConfItem>* GetConf(std::string&);
        int GenerateConfFile(std::string&);
        std::vector<std::string>* GetQATDevList();
        void UpdateQATDevices();

    private:
        std::vector<ConfItem>* items_;
        std::vector<std::string> *devs_;
    };
}

#endif //LIBVIRT_QATLM_CONF_LIBVIRT_CONF_H
