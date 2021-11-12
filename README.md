# Configure Libvirt for QAT Supported QEMU

To let `libvirt` support `QEMU` with QAT accelerated live migration feature,
some libvirt configuration files should be customized according the host
environment.

## Usage

*Looking usage of this tool*

>
> libvirt-qat-conf -h
>

*Validate current configuration*

>
> libvirt-qat-conf -v
>

or

>
> libvirt-qat-conf --validate
> 

*Configure libvirt*

>
> libvirt-qat-conf
> 
