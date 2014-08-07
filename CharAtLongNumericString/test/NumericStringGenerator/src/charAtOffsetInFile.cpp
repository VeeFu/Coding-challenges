#include <fstream>

char charAtOffsetInFile(const char *filename, long offset) {
  std::ifstream ifs(filename);
  // doing 0 based offset, right?
  char retval = '\0'
  for (int i = 0; i <= offset; ++i ) {
    ifs >> retval;
  }
  return retval;
}
