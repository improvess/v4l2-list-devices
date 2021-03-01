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

        std::cout << "\n";
    }

}