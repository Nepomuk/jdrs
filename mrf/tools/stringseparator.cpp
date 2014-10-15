#include "stringseparator.h"
#include <iostream>

StringSeparator::StringSeparator(std::string AInput, std::string ADelimiter)
        :fStartPos(0), fStrings(), fInput(AInput), fDelimiter(ADelimiter), fOutput(), fFirstDel(false),fLastDel(false)
{
}

std::string StringSeparator::GetString (void)
{
  std::string::size_type pos;
  std::string aString;
  pos = fInput.find_first_of(fDelimiter.c_str(), fStartPos);
  if (pos-fStartPos == 0) {                //first value at fStartPos is a fDelimiter
    fStartPos += 1;
    return "";
  }
  else if (pos != std::string::npos){           // a delimiter was found after fStartPos
    aString = fInput.substr(fStartPos, pos-fStartPos);
    fStartPos = pos + 1;
    return aString;
  }
  else {                          //no delimiter was found after fStartPos
    aString = fInput.substr(fStartPos, pos-fStartPos);
    fStartPos = pos;
    return aString;
  }
}

std::vector<std::string> StringSeparator::GetStringVector(void)
{
  fStartPos = 0;
  std::string value;
  ResetVector();
  TestFirst();
  TestLast();
  while (fStartPos != std::string::npos){
    value = GetString();
    if (value.length() > 0)
      fStrings.push_back(value);
  }
  return fStrings;
}

void StringSeparator::Print()
{
  std::cout << "StringSeparator for: " << fInput << std::endl;
  for (unsigned int i = 0; i < fStrings.size(); i++)
    std::cout << i << ": " << fStrings[i] << std::endl;
}
