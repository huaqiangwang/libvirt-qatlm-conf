#include <vector>
#include <string>
#include <iostream>

#include "LibvirtConf.h"

using namespace LibvirtConf;

void usage() {
    std::cout << "----------------------------------------------------------------------" << std::endl;
    std::cout << "Applet to create qemu.conf by detecting UIO devices." << std::endl;
    std::cout << "Usage: " << std::endl;
    std::cout << "\tlibvirt-qatlm-conf <source qemu conf file> <target qemu conf file>" << std::endl;
    std::cout << std::endl;
}


int main(int argc, char **argv) {
    std::vector<std::string> args(argv+1, argv+argc);
    if (argc != 3) {
        usage();
        return 0;
    }
    std::cout << "Applet to create qemu.conf by detecting UIO devices." << std::endl;
    std::cout << "\tSource File: " << args[0] << std::endl;
    std::cout << "\tOutput File: " << args[1] << std::endl;

    std::string fileName(args[0]);
    std::string fileOut(args[1]);

    auto conf = VirtSetting();
    conf.GetConf(fileName);
    conf.GetQATDevList();
    conf.UpdateQATDevices();
    conf.GenerateConfFile(fileOut);
    std::cout << std::endl << "Done" << std::endl;

    return 0;
}
