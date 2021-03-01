# v4l2-list-devices

A single header file containing a single function to list USB camera devices on linux using V4L2 and ioctl

## But why?

This function provides a way to list devices without brute force probing the system.

In addition, and maybe more important, the resulting data contain USB bus port identification which is useful to properly identify the right camera if you have more than one attached to the computer.

## how to use

- Copy list_devices.hpp to your project's include folder
- Include the header with `#include "list_devices.hpp"`
- call `v4l2::devices::list(devices)`
- vòi la

Check out the following example:

## Example

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

## Credits

This code is based on the `list_devices()` function from [v4l2-ctl](https://github.com/gjasny/v4l-utils/blob/master/utils/v4l2-ctl/v4l2-ctl-common.cpp#L232).

## License

This short code is shared with a very permissive MIT liceense. Check out the licensing file for more details.
