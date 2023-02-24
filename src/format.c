#include "format.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <stdio.h>

const char * format_strerror(FormatCode code) {
    switch (code) {
      case kFormatOK:
        return "No error.";
      case kFormatInvalidArg:
        return "Invalid value for argument(s) provided.";
      case kFormatNoArgsLeft:
        return "Attempted to set argument while no slots were available.";
      default:
        break;
    }
    return "Value provided is not a valid Format error code.";
}

static uint64_t get_args_count(const char *str) {
    uint64_t count = 0;
    while (*str) {
        count += 1 * (*str == '{' && *(str + 1) == '}');
        str += 1;
    }
    return count;
}

FormatCode format_init(struct format *fmt, const char *str) {
    if (!fmt || !str || !*str) {
        return kFormatInvalidArg;
    }

    fmt->length = strlen(str);
    fmt->args_left = get_args_count(str);
    fmt->value = (char *) calloc(fmt->length, sizeof(char));
    strcpy(fmt->value, str);

    return kFormatOK;
}

FormatCode format_free(struct format *fmt) {
    if (!fmt) {
        return kFormatInvalidArg;
    }

    free(fmt->value);
    *fmt->value = 0;
    return kFormatOK;
}

FormatCode format_set_argument(struct format *fmt, const char *arg) {
    if (!fmt->args_left) {
        return kFormatNoArgsLeft;
    }

    if (!arg || !*arg) {
        return kFormatInvalidArg;
    }

    uint64_t new_len = fmt->length + strlen(arg);
    char *new_value = (char *) calloc(new_len, sizeof(char));
    char *dst = &new_value[0];

    // We can only update one argument per call
    bool once = false;

    for (char *src = fmt->value; *src; src += 1) {
        switch (*src) {
          case '{':
            if (*(src + 1) == '}' && !once) {
                while (*arg) {
                    *dst++ = *arg++;
                }
                src += 1;
                fmt->args_left -= 1;
                once |= true;
                continue;
            } else if (*(src + 1) == '{') {
                *src += 1;
                continue;
            }
            break;

          case '}':
            if (*(src + 1) == '}') {
                *src += 1;
                continue;
            }
            break;

          default:
            break;
        }
        *dst++ = *src;
    }

    free(fmt->value);
    fmt->value = new_value;
    fmt->length = new_len;
    return kFormatOK;
}

const char * format_get_str(struct format *fmt) {
    return (const char *) fmt->value;
}
