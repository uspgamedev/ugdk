
#ifndef UGDK_UTIL_GDD_PARSERUTILITY_H_
#define UGDK_UTIL_GDD_PARSERUTILITY_H_

// Does nothing.
#define NO_MSG {}

// Asserts 'test'. If false, executes 'print_err' and returns 'err_value'.
#define ASSERT_PARSE(test, print_err, err_value) \
    do if (!(test)) { \
        print_err; \
        return err_value; \
    } while(0)

// Checks if 'token' is a valid character for the beginning of a field name.
#define VALID_NAME_BEGIN(token) ((isalnum((token))) || ((token) == '_'))

#define ERR_LOCATION(read) \
    fprintf(stderr, "%s:%u: ", read.file_path().c_str(), (unsigned)(read.line()));

// Prints a bad name beginning error message.
#define ERR_BAD_NAME_BEGIN(read) \
    fprintf(stderr, \
            "%s:%u: Names and types must begin with an alphanumeric character or '_'.", \
            read.file_path().c_str(), \
            (unsigned)(read.line()))

// Checks if 'token' is a valid field name character.
#define VALID_NAME_TOKEN(token) ((isalnum((token))) || ((token) == '_'))

// Checks if 'token' is a valid value character.
#define VALID_VALUE_TOKEN(token) ((isalnum((token))) || ((token) == '_') || ((token) == '.') || ((token) == '-'))

// Prints an empty field error, where the field is of type 'type'.
#define ERR_EMPTY_FIELD(read, type) \
    fprintf(stderr, \
            "%s:%u: syntax error: '%s' must not be empty.\n", \
            read.file_path().c_str(), \
            (unsigned)(read.line()), \
            type)

#endif /* UGDK_UTIL_GDD_PARSERUTILITY_H_ */
