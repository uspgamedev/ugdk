

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

    Reader(FILE *file) : file_path_("<unknown>"), file_(file), line_(1) {}

    Reader(std::string &file_path);

    ~Reader() {}

    std::string& file_path() { return file_path_; }

    size_t line() { return line_; }

    bool Begin() {
        file_ = fopen(file_path_.c_str(), "r");
        return file_ != NULL;
    }

    int Next() {
        int token = fgetc(file_);
        if (token == '\n')
            line_++;
        return token;
    }

    void SkipComment()  {
        int token = 0;
        while ((token = fgetc(file_)) != '\n' && token != EOF);
        line_++;
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

    bool UntilNextTag() {
        int token = 0;
        while ((token = fgetc(file_)) == ' ' || token == '\t');
        ungetc(token, file_);
        return !IsReserved(token) && token != '\n' && token != '\r' && token != EOF;
    }

    bool Name(std::string &name)  {
        int token = 0;
        token = fgetc(file_);
        ASSERT_PARSE(VALID_NAME_BEGIN(token),
                     ERR_BAD_NAME_BEGIN((*this)),
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
        while (UntilNextTag()) {
            std::string value = "";
            Value(value);
            if (value.length() == 0) break;
            values.push_back(value);
        }
    }

  private:

    std::string file_path_;
    FILE        *file_;
    size_t      line_;

};

} /* namespace gdd */

} /* namespace ugdk */

#endif /* UGDK_UTIL_GDD_READER_H_ */
