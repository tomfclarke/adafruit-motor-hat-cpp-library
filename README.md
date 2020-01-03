# C++ Library for Adafruit DC + Stepper Motor HAT

C++ port of the official [Adafruit-Motor-HAT-Python-Library](https://github.com/adafruit/Adafruit-Motor-HAT-Python-Library) for use with Raspberry Pi.

Uses the [I2C Device Interface](https://www.kernel.org/doc/html/latest/i2c/dev-interface.html) to control the HAT.

Example usage:

```c++
#include <thread>
#include <chrono>
#include "source/adafruitmotorhat.h"

int main()
{
    using namespace std::chrono_literals;

    // connect using the default device address 0x60
    AdafruitMotorHAT hat;

    // get the motor connected to port 1
    if (auto motor { hat.getMotor (1) })
    {
        // speed must be set before running commands
        motor->setSpeed (255);

        motor->run (AdafruitDCMotor::kForward);
        std::this_thread::sleep_for (1s);

        motor->run (AdafruitDCMotor::kBackward);
        std::this_thread::sleep_for (1s);

        // release the motor after use
        motor->run (AdafruitDCMotor::kRelease);
    }

    return 0;
}
```

Written by Tom Clarke. MIT licence.
