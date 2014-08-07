
#include <iostream>
#include <fstream>
#include "charatlongstring.h"

class testApp {
  private:
    std::istream &solutionStream;

    char charInSolution(int64_t offset) {
      char retval = '\0';
      solutionStream.seekg(offset);
      solutionStream.get(retval);
      return retval;
    }

    void testFor (int64_t offset) {
     
      char testResult = calcCharAt(offset);
      char solutionResult = charInSolution(offset);
      std::cout << "calcCharAt( " << offset << " ) = " << testResult << " "
                << " solution file = " << solutionResult << " : "
                << ((testResult == solutionResult) ? "SUCCESS" : "FAILURE")
                << std::endl;
    }

  public:
    testApp(std::istream &solution) : solutionStream(solution) {}
    void run() {
      int64_t i = 0;/*
      for (i = 0; i < 15; ++i) {
        testFor(i);
      }
      for (i = 0; i < 15; ++i) {
        testFor(1000000LL + i );
      }
      for (i = 0; i < 35; ++i) {
        testFor(1000000000LL + i );
      }*/
      for (i = 0; i < 25; ++i) {
        testFor(1000000000000LL + i );
      }
    }
};


int main(const int argc, const char **argv) {
  std::cout << "opening file " << argv[1] << std::endl;
  const char *solutionfile = argv[1];
  std::ifstream ifs(solutionfile);
  testApp app(ifs);
  app.run();

}


