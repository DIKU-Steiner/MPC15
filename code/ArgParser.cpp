#include "ArgParser.h"

#include <algorithm>
#include <iostream>
#include <stdlib.h>


ArgParser::ArgParser(int argc_, char** argv_): begin(argv_), end(argv_+argc_)
{
}

std::string ArgParser::getOption(const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

double ArgParser::getOption_double(const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return atof(*itr);
    }
    return 0;
}

int ArgParser::getOption_int(const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return atoi(*itr);
    }
    return 0;
}

bool ArgParser::optionExists(const std::string& option)
{
    return std::find(begin, end, option) != end;
}

void ArgParser::printArgs()
{
    std::cout<<"Arguments: ";
    for(int i=1;i<end-begin;i++){
        std::cout<<*(begin+i)<<" ";
    }
    std::cout<<std::endl;

}
