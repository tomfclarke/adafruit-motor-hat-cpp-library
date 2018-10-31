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

#include <cerrno>
#include <cstring>
#include <iostream>

I2CDevice::I2CDevice (int deviceAddress)
    : address (deviceAddress)
    , handle (wiringPiI2CSetup (address))
{
    if (handle == -1)
    {
        std::cerr << "Couldn't open device: ";
        std::cerr << std::strerror (errno) << std::endl;
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
        if (wiringPiI2CWriteReg8 (handle, deviceRegister, data) < 0)
        {
            std::cerr << "Failed to write to device: ";
            std::cerr << std::strerror (errno) << std::endl;
        }
    }
}

int I2CDevice::read8 (int deviceRegister)
{
    if (isValid())
    {
        int data = wiringPiI2CReadReg8 (handle, deviceRegister);

        if (data < 0)
        {
            std::cerr << "Failed to read from device: ";
            std::cerr << std::strerror (errno) << std::endl;
        }

        return data;
    }

    return -1;
}
