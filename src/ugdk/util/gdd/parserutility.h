
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
#define VALID_NAME_BEGIN(token) ((!isalpha((token))) && ((token) != '_'))

// Prints a bad name beginning error message.
#define ERR_BAD_NAME_BEGIN() \
    fputs("Data name must begin with an alphabetic character or '_'.", stderr)

// Checks if 'token' is a valid field name character.
#define VALID_NAME_TOKEN(token) ((isalnum((token))) || ((token) == '_'))

// Checks if 'token' is a valid value character.
#define VALID_VALUE_TOKEN(token) ((isalnum((token))) || ((token) == '_') || ((token) == '.'))

// Prints an empty field error, where the field is of type 'type'.
#define ERR_EMPTY_FIELD(type) \
    fprintf(stderr, "Syntax Error: %s name must not be empty.\n", type)

#endif /* UGDK_UTIL_GDD_PARSERUTILITY_H_ */
