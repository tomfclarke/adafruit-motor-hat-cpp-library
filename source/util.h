/**
 *  util.h
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

#pragma once

#include <cerrno>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

namespace log
{
    /**
        Writes a message to stdout.
    */
    template<typename MessageType>
    void output (MessageType message)
    {
        std::cout << message << std::endl;
    }

    /**
        Writes a message to stderr.
    */
    template<typename MessageType>
    void error (MessageType message)
    {
        std::cerr << message << std::endl;
    }

    /**
        Writes a message to stderr with the errno string appended.
    */
    template<typename MessageType>
    void strerror (MessageType message)
    {
        std::cerr << message << ": ";
        std::cerr << std::strerror (errno) << std::endl;
    }
}

namespace util
{
    /**
        Reads the file and attempts to find the first line that matches the
        supplied regular expression.
    */
    inline bool matchInFile (const std::string& filename, std::string& line, std::smatch& match_result, const std::regex& match_regex)
    {
        std::ifstream filestream (filename, std::ios::in);

        if (filestream.is_open())
        {
            while (std::getline (filestream, line))
            {
                if (std::regex_match (line, match_result, match_regex))
                    return true;
            }
            return false;
        }
        log::error ("Failed to open " + filename);
        return false;
    }

    /**
        Returns the revision number of the Raspberry Pi
        the program is running on, or -1 if not a Pi.
    */
    inline int getRaspberryPiRevisionNumber()
    {
        std::regex revision_regex { "^Revision\\s+:\\s+(\\w+)$",
                                    std::regex::icase };
        std::smatch revision_match;
        std::string line;

        if (matchInFile ("/proc/cpuinfo", line, revision_match, revision_regex))
        {
            if (revision_match.size() == 2)
            {
                std::string revision { revision_match.str (1) };

                if (!revision.empty())
                {
                    if (revision == "0000" || revision == "0002" || revision == "0003")
                        return 1;
                    else
                        return 2;
                }
            }
        }

        log::error ("Couldn\'t find Raspberry Pi revision number.");
        return -1;
    }

    /**
        Returns the version number of the Raspberry Pi
        the program is running on, or -1 if not a Pi.
    */
    inline int getRaspberryPiVersionNumber()
    {
        std::regex hardware_regex { "^Hardware\\s+:\\s+(\\w+)$",
                                    std::regex::icase };
        std::smatch hardware_match;
        std::string line;

        if (matchInFile ("/proc/cpuinfo", line, hardware_match, hardware_regex))
        {
            if (hardware_match.size() == 2)
            {
                std::string model { hardware_match.str (1) };

                if (model == "BCM2708")
                {
                    return 1; // Pi 1
                }
                else if (model == "BCM2709")
                {
                    return 2; // Pi 2
                }
                else if (model == "BCM2835")
                {
                    return 3; // Pi 3 or Pi on 4.9.x kernel
                }
            }
        }

        log::error ("Unrecognised Raspberry Pi device.");
        return -1;
    }
}