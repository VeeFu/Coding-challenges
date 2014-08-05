#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

class NumericStringGenerator {
  private:
    int length;
    const char *filename;
  public:
    NumericStringGenerator(int _length, const char *_filename);
    void generate();
};

NumericStringGenerator::NumericStringGenerator(int _length, const char *_filename) :
  length(_length), filename(_filename) {
  }

void NumericStringGenerator::generate() {
  ofstream outfile(filename);
  int digit = 1;
  while (digit < length) {
    outfile << digit++;
  }
}

int main (int argc, const char **argv) {
  if (argc < 3)
    // wrong number of arguments
    return 0;
  const char *seriesLength = argv[1];
  const char *outputFilename = argv[2];

  int length = atoi(seriesLength);
  if (length == 0)
    // integer could not be generated from input
    return 0;

  NumericStringGenerator generator(length, outputFilename);
  generator.generate();

}
