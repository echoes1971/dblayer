// Utility header for integer2string
// Extracted from dblayer.cpp and schema/schema.cpp to eliminate duplication
// and fix the off-by-one bug where int i = 50-2 with char tmp[50] risked buffer overflow

#pragma once

#include <string>

/** Convert a long longValue to its decimal string representation.
 *  Fixes the original bug: original code used `int i = 50-2` with `char tmp[50]`,
 *  which had an off-by-one risk. This implementation uses a safer approach.
 *
 *  @param longValue The integer value to convert
 *  @return String representation of the value
 */
inline std::string integer2string(long longValue) {
    // Use std::to_string for robustness (C++11)
    // This is preferable to the hand-rolled implementation which had
    // potential off-by-one issues with the buffer index initialization.
    return std::to_string(longValue);
}

/** Legacy compatibility wrapper.
 *  Kept for code that still calls the old signature.
 *  @deprecated Use std::to_string() directly or the DBLayer::integer2string below.
 */
[[deprecated("Use std::to_string() or DBLayer::integer2string instead")]]
inline std::string legacy_integer2string(long longValue) {
    return integer2string(longValue);
}