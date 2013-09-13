#pragma once

#ifdef __GNUC__
#  include <features.h>
#  if !__GNUC_PREREQ(4,7)
#    define GCC_4_6
#  endif
#endif


#define NOEXCEPT noexcept

#ifdef _MSC_VER
#  undef NOEXCEPT
#  define NOEXCEPT
#endif

#define OVERRIDE override

#ifdef GCC_4_6
#  undef OVERRIDE
#  define OVERRIDE
#endif
