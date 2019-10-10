// Copyright 2007 Google Inc. All Rights Reserved.

#include "supersonic/utils/strings/human_readable.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <glog/logging.h>
#include "supersonic/utils/logging-inl.h"
#include "supersonic/utils/stringprintf.h"
#include "supersonic/utils/strings/strip.h"

namespace {

template <typename T>
const char* GetNegStr(T* value) {
  if (*value < 0) {
    *value = -(*value);
    return "-";
  } else {
    return "";
  }
}

}  // namespace

bool HumanReadableNumBytes::LessThan(const string &a, const string &b) {
  int64 a_bytes, b_bytes;
  if (!HumanReadableNumBytes::ToInt64(a, &a_bytes))
    a_bytes = 0;
  if (!HumanReadableNumBytes::ToInt64(b, &b_bytes))
    b_bytes = 0;
  return (a_bytes < b_bytes);
}

bool HumanReadableNumBytes::ToInt64(const string &str, int64 *num_bytes) {
  char *end;
  double d = strtod(str.c_str(), &end);
  // If this didn't consume the entire string, fail.
  if ((end - str.c_str()) + 1 < str.size())
    return false;
  int64 scale = 1;
  switch (*end) {
    // NB: an int64 can only go up to <8 EB.
    case 'E':
      scale <<= 10;
      FALLTHROUGH_INTENDED;
    case 'P':
      scale <<= 10;
      FALLTHROUGH_INTENDED;
    case 'T':
      scale <<= 10;
      FALLTHROUGH_INTENDED;
    case 'G':
      scale <<= 10;
      FALLTHROUGH_INTENDED;
    case 'M':
      scale <<= 10;
      FALLTHROUGH_INTENDED;
    case 'K':
    case 'k':
      scale <<= 10;
      FALLTHROUGH_INTENDED;
    case 'B':
    case '\0':
      break;
    default:
      return false;
  }
  d *= scale;
  if (d > 0) {
    // Note that double(kint64max) is 2^63 exactly, so we cannot convert such
    // a value back to an int64.
    if (d + 0.5 >= kint64max)
      return false;
    *num_bytes = static_cast<int64>(d + 0.5);
  } else {
    if (d - 0.5 < kint64min)
      return false;
    *num_bytes = static_cast<int64>(d - 0.5);
  }
  return true;
}

bool HumanReadableNumBytes::ToDouble(const string &str, double *num_bytes) {
  char *end;
  double d = strtod(str.c_str(), &end);
  // If this didn't consume the entire string, fail.
  if ((end - str.c_str()) + 1 < str.size())
    return false;
  const char scale = *end;
  switch (scale) {
    case 'Y':
      d *= 1024.0;  // That's a yotta bytes!
      FALLTHROUGH_INTENDED;
    case 'Z':
      d *= 1024.0;
      FALLTHROUGH_INTENDED;
    case 'E':
      d *= 1024.0;
      FALLTHROUGH_INTENDED;
    case 'P':
      d *= 1024.0;
      FALLTHROUGH_INTENDED;
    case 'T':
      d *= 1024.0;
      FALLTHROUGH_INTENDED;
    case 'G':
      d *= 1024.0;
      FALLTHROUGH_INTENDED;
    case 'M':
      d *= 1024.0;
      FALLTHROUGH_INTENDED;
    case 'K':
    case 'k':
      d *= 1024.0;
      FALLTHROUGH_INTENDED;
    case 'B':
    case '\0':
      break;  // to here.
    default:
      return false;
  }
  *num_bytes = d;
  return true;
}

string HumanReadableNumBytes::DoubleToString(double num_bytes) {
  const char *neg_str = GetNegStr(&num_bytes);
  static const char units[] = "BKMGTPEZY";
  double scaled = num_bytes;
  int i = 0;
  for (; i < arraysize(units) && scaled >= 1024.0; ++i) {
    scaled /= 1024.0;
  }
  if (i == arraysize(units)) {
    return StringPrintf("%s%g", neg_str, num_bytes);
  } else {
    return StringPrintf("%s%.2f%c", neg_str, scaled, units[i]);
  }
}

string HumanReadableNumBytes::ToString(int64 num_bytes) {
  if (num_bytes == kint64min) {
    // Special case for number with not representable nagation.
    return "-8E";
  }

  const char *neg_str = GetNegStr(&num_bytes);

  // Special case for bytes.
  if (num_bytes < GG_LONGLONG(1024)) {
    // No fractions for bytes.
    return StringPrintf("%s%" GG_LL_FORMAT "dB", neg_str, num_bytes);
  }

  static const char units[] = "KMGTPE";  // int64 only goes up to E.
  const char* unit = units;
  while (num_bytes >= GG_LONGLONG(1024) * GG_LONGLONG(1024)) {
    num_bytes /= GG_LONGLONG(1024);
    ++unit;
    CHECK(unit < units + arraysize(units));
  }

  return StringPrintf(((*unit == 'K')
                       ? "%s%.1f%c"
                       : "%s%.2f%c"), neg_str, num_bytes / 1024.0, *unit);
}

string HumanReadableNumBytes::ToStringWithoutRounding(int64 num_bytes) {
  if (num_bytes == kint64min) {
    // Special case for number with not representable nagation.
    return "-8E";
  }

  const char *neg_str = GetNegStr(&num_bytes);
  static const char units[] = "BKMGTPE";  // int64 only goes up to E.

  int64 num_units = num_bytes;
  int unit_type = 0;
  for (; unit_type < arraysize(units); unit_type++) {
    if (num_units % 1024 != 0) {
      // Not divisible by the next unit.
      break;
    }

    int64 next_units = num_units >> 10;
    if (next_units == 0) {
      // Less than the next unit.
      break;
    }

    num_units = next_units;
  }
  return StringPrintf("%s%lld%c", neg_str, num_units, units[unit_type]);
}

string HumanReadableInt::ToString(int64 value) {
  string s;
  if (value < 0) {
    s += "-";
    value = -value;
  }
  if (value < GG_LONGLONG(1000)) {
    StringAppendF(&s, "%" GG_LL_FORMAT "d", value);
  } else if (value >= GG_LONGLONG(1000000000000000)) {
    // Number bigger than 1E15; use that notation.
    StringAppendF(&s, "%0.3G", static_cast<double>(value));
  } else {
    static const char units[] = "kMBT";
    const char *unit = units;
    while (value >= GG_LONGLONG(1000000)) {
      value /= GG_LONGLONG(1000);
      ++unit;
      CHECK(unit < units + arraysize(units));
    }
    StringAppendF(&s, "%.2f%c", value / 1000.0, *unit);
  }
  return s;
}

string HumanReadableNum::ToString(int64 value) {
  return HumanReadableInt::ToString(value);
}

string HumanReadableNum::DoubleToString(double value) {
  string s;
  if (value < 0) {
    s += "-";
    value = -value;
  }
  if (value < 1.0) {
    StringAppendF(&s, "%.3f", value);
  } else if (value < 10) {
    StringAppendF(&s, "%.2f", value);
  } else if (value < 1e2) {
    StringAppendF(&s, "%.1f", value);
  } else if (value < 1e3) {
    StringAppendF(&s, "%.0f", value);
  } else if (value >= 1e15) {
    // Number bigger than 1E15; use that notation.
    StringAppendF(&s, "%0.3G", value);
  } else {
    static const char units[] = "kMBT";
    const char *unit = units;
    while (value >= 1e6) {
      value /= 1e3;
      ++unit;
      CHECK(unit < units + arraysize(units));
    }
    StringAppendF(&s, "%.2f%c", value / 1000.0, *unit);
  }
  return s;
}

bool HumanReadableNum::ToDouble(const string &str, double *value) {
  char *end;
  double d = strtod(str.c_str(), &end);
  // Allow the string to contain at most one extra character:
  if ((end - str.c_str()) + 1 < str.size())
    return false;
  const char scale = *end;
  if ((scale == 'k') || (scale == 'K')) {
    d *= 1e3;
  } else if (scale == 'M') {
    d *= 1e6;
  } else if (scale == 'B') {
    d *= 1e9;
  } else if (scale == 'T') {
    d *= 1e12;
  } else if (scale != '\0') {
    return false;
  }
  *value = d;
  return true;
}

bool HumanReadableInt::ToInt64(const string &str, int64 *value) {
  char *end;
  double d = strtod(str.c_str(), &end);
  if (d > kint64max || d < kint64min)
    return false;
  if (*end == 'k') {
    d *= 1000;
  } else if (*end == 'M') {
    d *= 1e6;
  } else if (*end == 'B') {
    d *= 1e9;
  } else if (*end == 'T') {
    d *= 1e12;
  } else if (*end != '\0') {
    return false;
  }
  *value = static_cast<int64>(d < 0 ? d - 0.5 : d + 0.5);
  return true;
}

// Abbreviations used here are acceptable English abbreviations
// without the ending period (".") for brevity, except for uncommon
// abbreviations, in which case the entire word is spelled out. ("mo"
// and "mos" are not good abbreviations for "months" -- with or
// without the period). If needed, one can add a
// HumanReadableTime::ToStringShort() for shorter abbreviations or one
// for always spelling out the unit, HumanReadableTime::ToStringLong().
string HumanReadableElapsedTime::ToShortString(double seconds) {
  string human_readable;

  if (seconds < 0) {
    human_readable = "-";
    seconds = -seconds;
  }

  // Start with us and keep going up to years.
  // The comparisons must account for rounding to prevent the format breaking
  // the tested condition and returning, e.g., "1e+03 us" instead of "1 ms".
  const double microseconds = seconds * 1.0e6;
  if (microseconds < 999.5) {
    StringAppendF(&human_readable, "%0.3g us", microseconds);
    return human_readable;
  }
  double milliseconds = seconds * 1e3;
  if (milliseconds >= .995 && milliseconds < 1) {
    // Round half to even in StringAppendF would convert this to 0.999 ms.
    milliseconds = 1.0;
  }
  if (milliseconds < 999.5) {
    StringAppendF(&human_readable, "%0.3g ms", milliseconds);
    return human_readable;
  }
  if (seconds < 60.0) {
    StringAppendF(&human_readable, "%0.3g s", seconds);
    return human_readable;
  }
  seconds /= 60.0;
  if (seconds < 60.0) {
    StringAppendF(&human_readable, "%0.3g min", seconds);
    return human_readable;
  }
  seconds /= 60.0;
  if (seconds < 24.0) {
    StringAppendF(&human_readable, "%0.3g h", seconds);
    return human_readable;
  }
  seconds /= 24.0;
  if (seconds < 30.0) {
    StringAppendF(&human_readable, "%0.3g days", seconds);
    return human_readable;
  }
  if (seconds < 365.2425) {
    StringAppendF(&human_readable, "%0.3g months", seconds / 30.436875);
    return human_readable;
  }
  seconds /= 365.2425;
  StringAppendF(&human_readable, "%0.3g years", seconds);
  return human_readable;
}

bool HumanReadableElapsedTime::ToDouble(const string& str, double* value) {
  struct TimeUnits {
    const char* unit;  // unit name
    double seconds;    // number of seconds in that unit (minutes => 60)
  };

  // These must be sorted in decreasing length.  In particulary, a
  // string must exist before and of its substrings or the substring
  // will match;
  static const TimeUnits kUnits[] = {
    // Long forms
    { "microsecond", 0.000001 },
    { "millisecond", 0.001 },
    { "second", 1.0 },
    { "minute", 60.0 },
    { "hour", 3600.0 },
    { "day", 86400.0 },
    { "week", 7 * 86400.0 },
    { "month", 30 * 86400.0 },
    { "year", 365 * 86400.0 },

    // Abbreviated forms
    { "microsec", 0.000001 },
    { "millisec", 0.001 },
    { "sec", 1.0 },
    { "min", 60.0 },
    { "hr", 3600.0 },
    { "dy", 86400.0 },
    { "wk", 7 * 86400.0 },
    { "mon", 30 * 86400.0 },
    { "yr", 365 * 86400.0 },

    // micro -> u
    { "usecond", 0.000001 },
    { "usec", 0.000001 },
    // milli -> m
    { "msecond", 0.001 },
    { "msec", 0.001 },

    // Ultra-short form
    { "us", 0.000001 },
    { "ms", 0.001 },
    { "s", 1.0 },
    { "m", 60.0 },
    { "h", 3600.0 },
    { "d", 86400.0 },
    { "w", 7 * 86400.0 },
    { "M", 30 * 86400.0 },  // upper-case M to disambiguate with minute
    { "y", 365 * 86400.0 }
  };

  char* unit_start;     // Start of unit name.
  double work_value = 0;
  int sign = 1;
  const char* interval_start = SkipLeadingWhiteSpace(str.c_str());
  if (*interval_start == '-') {
    sign = -1;
    interval_start = SkipLeadingWhiteSpace(interval_start + 1);
  } else if (*interval_start == '+') {
    interval_start = SkipLeadingWhiteSpace(interval_start + 1);
  }
  if (!*interval_start) {
    // Empty string and strings with just a sign are illegal.
    return false;
  }
  do {
    // Leading signs on individual values are not allowed.
    if (*interval_start == '-' || *interval_start == '+') {
      return false;
    }
    double factor = strtod(interval_start, &unit_start);
    if (interval_start == unit_start) {
      // Illegally formatted value, no values consumed by strtod.
      return false;
    }
    unit_start = SkipLeadingWhiteSpace(unit_start);
    bool found_unit = false;
    for (int i = 0; !found_unit && i < ARRAYSIZE(kUnits); ++i) {
      const size_t unit_len = strlen(kUnits[i].unit);
      if (strncmp(unit_start, kUnits[i].unit, unit_len) == 0) {
        work_value += factor * kUnits[i].seconds;
        interval_start = unit_start + unit_len;
        // Allowing pluralization of any unit (except empty string)
        if (unit_len > 0 && *interval_start == 's') {
            interval_start++;
        }
        found_unit = true;
      }
    }
    if (!found_unit) {
      return false;
    }
    interval_start = SkipLeadingWhiteSpace(interval_start);
  } while (*interval_start);

  *value = sign * work_value;
  return true;
}
