// include guard
#ifndef __INPUTPARSER_H_INCLUDED__
#define __INPUTPARSER_H_INCLUDED__

#include <string>
#include <vector>
#include <algorithm>

class InputParser{
    private:
        std::vector <std::string> tokens;

    public:
        InputParser (int &argc, char **argv);
        const std::string& getCmdOption(const std::string &option) const;
        bool cmdOptionExists(const std::string &option) const;
};

#endif // __INPUTPARSER_H_INCLUDED__