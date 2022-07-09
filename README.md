# Configure Libvirt for QAT Supported QEMU

To let `libvirt` support `QEMU` with QAT accelerated live migration feature,
some libvirt configuration files should be customized according the host
environment.

## Usage

*Looking usage of this tool*

```shell
libvirt-qatlm-conf -h
```

*Update qemu.conf for purpose of support LM qat migration*

```shell
# backup qemu.conf
sudo cp /etc/libvirt/qemu.conf /etc/libvirt/qemu.origin.conf
# Apply new settings to qemu.conf
sudo /tools/libvirt_qatlm_conf /etc/libvirt/qemu.conf /tmp/qemu.conf
sudo cp /tmp/qemu.conf /etc/libvirt/qemu.conf
sudo systemctl restart libvirtd
```

## Compile and Run the test

```shell
cd <root>
mkdir build && cd build
cmake ..
make -j10
#(or make test)
```
