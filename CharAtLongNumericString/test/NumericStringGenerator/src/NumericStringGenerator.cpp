#include <iostream>
#include <cstdlib>
#include <fstream>

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
  int i=1;
  while (i < length) {
    outfile << i++;
  }
}

int main (int argc, const char **argv) {
  if (argc < 3)
    // wrong number of arguments
    return 0;
  const char *seriesLength = argv[1];
  const char *outputFilename = argv[2];

  int length = itoa(seriesLength);
  if (length == 0)
    // integer could not be generated from input
    return 0;

  NumericStringGenerator generator(length, outputFilename);
  generator.generate();

}
