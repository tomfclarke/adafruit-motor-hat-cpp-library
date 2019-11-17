/**
 *  i2cdevice.cpp
 *
 *  MIT License
 *
 *  Copyright (c) 2018, Tom Clarke
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include "i2cdevice.h"
#include "util.h"

#include <string>

#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

int getDefaultBusNumber()
{
    // TODO: work out which revision of Raspberry PI we're
    // running on and return the correct bus number.
    // Revision 1 uses bus 0
    // Revision 2 uses bus 1
    return 1;
}

std::string getFilenameForBus (int busNumber)
{
    return "/dev/i2c-" + std::to_string (busNumber);
}

I2CDevice::I2CDevice (int i2cAddress)
    : busNumber (getDefaultBusNumber())
    , address (i2cAddress)
    , handle (-1)
{
    std::string filename (getFilenameForBus (busNumber));

    handle = open (filename.c_str(), O_RDWR);

    if (handle == -1)
    {
        log::strerror ("Couldn't open device");
    }
}

bool I2CDevice::isValid()
{
    return handle != -1;
}

void I2CDevice::write8 (int deviceRegister, int data)
{
    if (isValid())
    {
        selectDevice();

        bool success = false;

        if (!success)
        {
            log::strerror ("Failed to write to device");
        }
    }
}

int I2CDevice::read8 (int deviceRegister)
{
    if (isValid())
    {
        selectDevice();

        int data = -1;

        if (data < 0)
        {
            log::strerror ("Failed to read from device");
        }

        return data;
    }

    return -1;
}

void I2CDevice::selectDevice()
{
    if (ioctl (handle, I2C_SLAVE, address & 0x7F) < 0)
    {
        log::strerror ("Failed to select device");
    }
}
