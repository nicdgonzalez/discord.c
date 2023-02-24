#ifndef NDG_FORMAT_H_
#define NDG_FORMAT_H_

#ifdef __cplusplus
extern "C" {
#endif  // !__cplusplus

#include <stdint.h>

/// @brief Represents return values for various Format related functions.
typedef enum format_code {
    kFormatOK,
    kFormatInvalidArg,
    kFormatNoArgsLeft
} FormatCode;

/// @brief A string template with replaceable placeholder symbols.
struct format {
    char *value;
    uint64_t length;
    uint64_t args_left;
};

/// @brief Retrieves a readable string version of an error code.
/// @param code The error code returned by a format_* function.
 /// @return const char pointer containing a read-only string error.
const char * format_strerror(FormatCode);

/// @brief Constructor for Format structs.
/// @param fmt Pointer to an uninitialized Format struct.
/// @param str The string template to store.
FormatCode format_init(struct format *, const char *);

/// @brief Destructor for Format structs.
/// @param fmt Pointer to a Format struct initialized with format_init().
FormatCode format_free(struct format *);

/// @brief Replace the first placeholder symbol with a value.
/// @param fmt Pointer to a Format struct initialized with format_init().
/// @param arg The value to swap with the placeholder symbol.
FormatCode format_set_argument(struct format *, const char *);

/// @brief Retrieves the modified string template provided to format_init().
/// @param fmt Pointer to a Format struct initialized with format_init().
/// @return const char pointer containing the held string value.
const char * format_get_str(struct format *fmt);

#ifdef __cplusplus
}
#endif  // !__cplusplus

#endif  // !NDG_FORMAT_H_
