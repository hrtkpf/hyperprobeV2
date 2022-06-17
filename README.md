# HyperprobeV2 - Virtual Machine Extrospection

HyperprobeV2 is a POC implementation for Virtual Machine Extrospection.
It currently works for the KVM hypervisor.
Run in a virtual machine, it can detect the KVM/kernel version used by the host system.

Based on the original Hyperprobe, v2 offers multiple enhancements, especially support for more recent KVM versions.
The project was extended within the scope of my university research.


## How does it work?

Hyperprobe detects certain features and bugs of the underlying hypervisor. Based on the knowledge of when a feature was introduced and when a bug was fixed, it narrows down the possible KVM versions.
Since KVM is Open-source, its development history is publicly obtainable and can be analyzed.

Hypervisor features are used to determine a limit of the minimum KVM version while bugs are used to set an upper limit. For the exact procedure, please refer to the [Hyperprobe paper](https://www.usenix.org/system/files/conference/lisa15/lisa15-paper-xiao.pdf).


## Additions of v2

The original version of Hyperprobe (by Xiao et al.) supported KVM versions 2.6.20 through 3.14.

V2 adds support for newer versions, namely 4.2 through 5.12, by introducing new tests for multiple features and bugs of KVM.

Additionally, the limitation logic was extended so that not only bugs, but also certain features can be used to set an upper limit, increasing the efficiency of the identification.


## Running HyperprobeV2

Compile the project using `make` and run `./bin/main`.
Since many tests depend on reading and writing MSR values, the MSR kernel module is required. You can activate it with `modprobe msr`.


## Credits

HyperprobeV2 is based on the [original Hyperprobe](https://github.com/jidongxiao/hyperprobe) by Jidong Xiao, Lei Lu, Hai Huang and Haining Wang.


## Author

Jan Hartkopf (j@hartkopf.io)