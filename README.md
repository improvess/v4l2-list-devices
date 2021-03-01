# v4l2-list-devices

A single C++ header file containing a single function to list USB camera devices on linux using V4L2 and ioctl.

## TL; DR;

Copy file `include/list_devices.hpp` to your project and use it:

```c++
#include "list_devices.hpp"

std::vector<v4l2::devices::DEVICE_INFO> devices;
v4l2::devices::list(devices);
```

## But why?

This function provides a way to list devices without brute force probing the system.

In addition, and maybe more important, the resulting data contain USB bus port identification which is useful to properly identify the right camera if you have more than one attached to the computer.

## how to use

- Copy `include/list_devices.hpp` to your project's include folder
- Include the header with `#include "list_devices.hpp"`
- call `v4l2::devices::list(devices)`
- vòi la!

Note that this repo contains a CMakeLists.txt file which is useful only for build the usage example. It is noteworthy that you needn't build nothing in order to use the header `list_devices.hpp` in your projects! Duh!

## Example

This is basically the contents of file `main.cpp` 

```c++
#include <iostream>

#include "list_devices.hpp"

int main() {

    std::vector<v4l2::devices::DEVICE_INFO> devices;

    v4l2::devices::list(devices);

    for (const auto & device : devices) 
    {
    
        std::cout << device.device_description <<  " at " << device.bus_info << " is attached to\n";

        for (const auto & path : device.device_paths) {
            std::cout << path << "\n";
        }
        
    }

}
```

I just ran this example on my system (Ubuntu 20.04 running on Virtualbox) with two attached webcams and this is the output:

```bash
$ ./list-devices 
VirtualBox Webcam - Integrated  at usb-0000:00:0c.0-2 is attached to
/dev/video0
/dev/video1

VirtualBox Webcam - HD Pro Webc at usb-0000:00:0c.0-3 is attached to
/dev/video2
/dev/video3

```

Which is basically the same output of v4l2-ctl:

```bash
$ v4l2-ctl --list-devices
VirtualBox Webcam - Integrated  (usb-0000:00:0c.0-2):
        /dev/video0
        /dev/video1

VirtualBox Webcam - HD Pro Webc (usb-0000:00:0c.0-3):
        /dev/video2
        /dev/video3

```

## Credits

This code is based on the `list_devices()` function from [v4l2-ctl](https://github.com/gjasny/v4l-utils/blob/master/utils/v4l2-ctl/v4l2-ctl-common.cpp#L232).

## License

This short code is shared with a very permissive MIT license. Check out the licensing section for more details.
