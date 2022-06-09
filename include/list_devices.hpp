/* ---------------------------------------------------------------------
 * Luiz Carlos doleron @ 2021
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Affero Public License for more details.
 * ----------------------------------------------------------------------
 */

#ifndef LIST_DEVICES_H_
#define LIST_DEVICES_H_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include <string>
#include <vector>
#include <map>
#include <algorithm> //for sorting

#include <linux/types.h>
#include <linux/v4l2-common.h>
#include <linux/v4l2-controls.h>
#include <linux/videodev2.h>
#include <linux/media.h>
#include <sys/ioctl.h>

#include <dirent.h> //to list dir's content

namespace v4l2
{
    namespace devices
    {

        struct DEVICE_INFO
        {
            std::string device_description;
            std::string bus_info;
            std::vector<std::string> device_paths;
        };

        inline void list(std::vector<DEVICE_INFO> &devices)
        {

            std::vector<std::string> files;

            const std::string dev_folder = "/dev/";

            DIR *dir;
            struct dirent *ent;
            if ((dir = opendir(dev_folder.c_str())) != NULL)
            {
                while ((ent = readdir(dir)) != NULL)
                {
                    if (strlen(ent->d_name) > 5 && !strncmp("video", ent->d_name, 5)) {
                        
                        std::string file = dev_folder + ent->d_name;

                        const int fd = open(file.c_str(), O_RDWR);
                        v4l2_capability capability;
                        if (fd >= 0) {
                            if (ioctl(fd, VIDIOC_QUERYCAP, &capability) >= 0)
                            {
                                files.push_back(file);
                            }
                            close(fd);
                        }
                    }
                }
                closedir(dir);
            }
            else
            {
                std::string msg = "Cannot list " + dev_folder + " contents!";
                throw std::runtime_error(msg);
            }

            std::sort(files.begin(), files.end());

            struct v4l2_capability vcap;

            std::map<std::string, DEVICE_INFO> device_map;

            for (const auto &file : files)
            {
                int fd = open(file.c_str(), O_RDWR);
                std::string bus_info;
                std::string card;

                if (fd < 0)
                    continue;
                int err = ioctl(fd, VIDIOC_QUERYCAP, &vcap);
                if (err)
                {
                    struct media_device_info mdi;

                    err = ioctl(fd, MEDIA_IOC_DEVICE_INFO, &mdi);
                    if (!err)
                    {
                        if (mdi.bus_info[0])
                            bus_info = mdi.bus_info;
                        else
                            bus_info = std::string("platform:") + mdi.driver;

                        if (mdi.model[0])
                            card = mdi.model;
                        else
                            card = mdi.driver;
                    }
                }
                else
                {
                    bus_info = reinterpret_cast<const char *>(vcap.bus_info);
                    card = reinterpret_cast<const char *>(vcap.card);
                }
                close(fd);

                if (!bus_info.empty() && !card.empty())
                {

                    if (device_map.find(bus_info) != device_map.end())
                    {
                        DEVICE_INFO &device = device_map.at(bus_info);
                        device.device_paths.emplace_back(file);
                    }
                    else
                    {
                        DEVICE_INFO device;
                        device.device_paths.emplace_back(file);
                        device.bus_info = bus_info;
                        device.device_description = card;
                        device_map.insert(std::pair<std::string, DEVICE_INFO>(bus_info, device));
                    }
                }
            }

            for (const auto &row : device_map)
            {
                devices.emplace_back(row.second);
            }
            
        }

        inline DEVICE_INFO resolve_path(const std::string & target_usb_bus_id)
        {
            DEVICE_INFO result;
            result.bus_info = target_usb_bus_id;

            std::vector<std::string> files;
            std::vector<std::string> paths;

            const std::string dev_folder = "/dev/";

            DIR *dir;
            struct dirent *ent;
            if ((dir = opendir(dev_folder.c_str())) != NULL)
            {
                //duplicate entries for the same path requires full loop
                while ((ent = readdir(dir)) != NULL)
                {
                    if (strlen(ent->d_name) > 5 && !strncmp("video", ent->d_name, 5)) {
                        std::string file = dev_folder + ent->d_name;

                        const int fd = open(file.c_str(), O_RDWR);
                        v4l2_capability capability;
                        if (fd >= 0)
                        {
                            int err = ioctl(fd, VIDIOC_QUERYCAP, &capability);
                            if (err >= 0)
                            {
                                std::string bus_info;
                                struct media_device_info mdi;

                                err = ioctl(fd, MEDIA_IOC_DEVICE_INFO, &mdi);
                                if (!err)
                                {
                                    if (mdi.bus_info[0])
                                        bus_info = mdi.bus_info;
                                    else
                                        bus_info = std::string("platform:") + mdi.driver;
                                }
                                else
                                {
                                    bus_info = reinterpret_cast<const char *>(capability.bus_info);
                                }

                                if (!bus_info.empty())
                                {   
                                    if (bus_info.compare(target_usb_bus_id) == 0) {

                                        paths.push_back(file);
                                    }
                                }
                            }
                            close(fd);
                        }
                    }
                }
                if (!paths.empty()) {
                    std::sort(paths.begin(), paths.end());
                    result.device_paths = paths;
                }
                closedir(dir);
            }
            else
            {
                std::string msg = "Cannot list " + dev_folder + " contents!";
                throw std::runtime_error(msg);
            }
            return result;
        }
    }

}

#endif
