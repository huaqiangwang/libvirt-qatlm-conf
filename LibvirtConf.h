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
        VirtSetting() {items_ = new std::vector<ConfItem>;}
        std::vector<ConfItem>* GetConf(std::string&);

    private:
        std::vector<ConfItem>* items_;
    };

}

#endif //LIBVIRT_QATLM_CONF_LIBVIRT_CONF_H
