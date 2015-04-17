#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <string>


class ArgParser
{
public:
  ArgParser(int argc, char** argv);
  std::string getOption(const std::string & option);
  double getOption_double(const std::string & option);
  int getOption_int(const std::string & option);
  bool optionExists(const std::string& option);

  void printArgs();

private:
  char **begin, **end;
};

#endif // ARGPARSER_H
