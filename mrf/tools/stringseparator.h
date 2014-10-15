/// -----------------------------------------------------------------
///
///          PndStringSeparator
///
///          Version 1.0
///            by
///          Tobias Stockmanns
///  Seperates an input string into substrings and stores them in a
///  string vector. The seperation criteria is a string of characters.
///  If no delimiters are given the default delimiter " " is used.
///
///  Example:
///    std::vector<std::string> fOutput
///    TPndStringVector fInput("Column:Row Type: Test ", ": ");
///    fOutput = fInput.GetStringVector();
///
///    fOutput[0] = "Column"
///    fOutput[1] = "Row"
///    fOutput[2] = "Type"
///    fOutput[3] = "Test"
///
///  Original can be found here:
///   .cxx - https://subversion.gsi.de/trac/fairroot/browser/pandaroot/trunk/PndTools/generalTools/PndStringSeparator.cxx
///   .h   - https://subversion.gsi.de/trac/fairroot/browser/pandaroot/trunk/PndTools/generalTools/PndStringSeparator.h
///
/// -----------------------------------------------------------------

#ifndef STRINGSEPARATOR_H
#define STRINGSEPARATOR_H


#include <string>
#include <vector>


class StringSeparator
{
  public :
    StringSeparator() : fStartPos(0), fStrings(), fInput(), fDelimiter(), fOutput(), fFirstDel(false), fLastDel(false) {}
    ~StringSeparator() {}
    StringSeparator(std::string AInput, std::string ADelimiter = " ");

    // input functions
    void SetInput ( std::string AInput ) { fInput = AInput; }
    void SetDelimiter ( std::string ADelimiter ) { fDelimiter = ADelimiter; }
    void ResetVector() { fStrings.clear(); }

    // get the splitted string
    std::vector<std::string> GetStringVector (void);

    // access functions
    void TestFirst()    { if (fInput.find_first_of(fDelimiter)==0) fFirstDel=true; else fFirstDel = false; }
    void TestLast()     { if (fInput.find_last_of(fDelimiter) == fInput.size()-1) fLastDel = true; else fLastDel = false; }
    bool GetIfFirst()   { return fFirstDel; }
    bool GetIfLast()    { return fLastDel; }
    //std::string Replace(std::string from, std::string to);
    void Print();

  private :
    std::string::size_type fStartPos;
    std::vector<std::string> fStrings;
    std::string fInput;
    std::string fDelimiter;
    std::string fOutput;
    std::string GetString (void);
    bool fFirstDel;  //first element in the string was a delimiter
    bool fLastDel;   //last element in the string was a delimiter
};

#endif // STRINGSEPARATOR_H
