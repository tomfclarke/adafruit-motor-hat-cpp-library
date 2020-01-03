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

#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

int getDefaultBusNumber()
{
    // Revision 1 uses bus 0
    // Revision 2 or greater uses bus 1
    return util::getRaspberryPiRevisionNumber() > 1 ? 1 : 0;
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
    openHandle();
}
I2CDevice::~I2CDevice()
{
    closeHandle();
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
        writeByteData (deviceRegister, data);
    }
}

int I2CDevice::read8 (int deviceRegister)
{
    if (isValid())
    {
        selectDevice();
        return readByteData (deviceRegister);
    }
    return -1;
}

void I2CDevice::openHandle()
{
    closeHandle();

    std::string filename (getFilenameForBus (busNumber));

    handle = open (filename.c_str(), O_RDWR);

    if (!isValid())
    {
        log::strerror ("Couldn't open device");
    }
}

void I2CDevice::closeHandle()
{
    if (isValid())
    {
        if (close (handle) < 0)
        {
            log::strerror ("Couldn't close device");
        }
    }
}

void I2CDevice::selectDevice()
{
    if (ioctl (handle, I2C_SLAVE, address & 0x7F) < 0)
    {
        log::strerror ("Failed to select device");
    }
}

void I2CDevice::writeByteData (int deviceRegister, int data)
{
    char buffer[2] =
    {
        static_cast<char>(deviceRegister & 0xFF),
        static_cast<char>(data & 0xFF)
    };

    if (write (handle, buffer, 2) != 2)
    {
        log::strerror ("Failed to write to device");
    }
}

int I2CDevice::readByteData (int deviceRegister)
{
    char buffer { 0 };

    if (read (handle, &buffer, 1) != 1)
    {
        log::strerror ("Failed to read from device");
        return -1;
    }

    return static_cast<int>(buffer);
}