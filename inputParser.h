// include guard
#ifndef __INPUTPARSER_H_INCLUDED__
#define __INPUTPARSER_H_INCLUDED__

#include <string>
#include <vector>
#include <algorithm>
#include <map> 

struct option{
    std::string optionName;
    std::string* arguements;
};

class InputParser{
    private:
        
        std::map<std::string, std::vector <std::string>> tokens;

    public:
        InputParser (int &argc, char **argv, std::vector <option> options);
        ~InputParser();
        std::string* getCmdOption(std::string &option);
        bool cmdOptionExists(std::string &option);
};

#endif // __INPUTPARSER_H_INCLUDED__