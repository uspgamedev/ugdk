

#ifndef UGDK_UTIL_GDD_READER_H_
#define UGDK_UTIL_GDD_READER_H_

#include <cstdio>
#include <string>
#include <vector>
#include <ugdk/util/gdd/parserutility.h>

namespace ugdk {

namespace gdd {

class Reader {

  public:

    Reader(FILE *file) : file_(file) {}

    ~Reader() {}

    int Next() {
        return fgetc(file_);
    }

    void SkipComment()  {
        int token = 0;
        while ((token = fgetc(file_)) != '\n' && token != EOF);
    }

    void SkipSpace() {
        int token = 0;
        while ((token = fgetc(file_)) == ' ' || token == '\t');
        ungetc(token, file_);
    }

    bool IsReserved(int token)  {
        switch (token) {
          case '#':
          case '$':
          case '%':
          case '@':
          case '+':
          case '[':
          case ']':
              return true;
          default:
              return false;
        }
    }

    bool untilNext() {
        int token = 0;
        while ((token = fgetc(file_)) == ' ' || token == '\t');
        ungetc(token, file_);
        return !IsReserved(token) && token != '\n' && token != EOF;
    }

    bool Name(std::string &name)  {
        int token = 0;
        token = fgetc(file_);
        ASSERT_PARSE(VALID_NAME_BEGIN(token),
                     ERR_BAD_NAME_BEGIN(),
                     false);
        name.push_back(token);
        for (token = fgetc(file_); VALID_NAME_TOKEN(token); token = fgetc(file_))
            name.push_back(token);
        ungetc(token, file_);
        return true;
    }

    void Value(std::string &value) {
        int token = 0;
        for (token = fgetc(file_); VALID_VALUE_TOKEN(token); token = fgetc(file_))
            value.push_back(token);
        ungetc(token, file_);
    }

    void ValueSequence(std::vector<std::string> &values) {
        while (untilNext()) {
            std::string value = "";
            Value(value);
            if (value.length() == 0) break;
            values.push_back(value);
        }
    }

  private:

    FILE *file_;

};

} /* namespace gdd */

} /* namespace ugdk */

#endif /* UGDK_UTIL_GDD_READER_H_ */
