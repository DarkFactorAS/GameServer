#ifndef _VOLATILEARG_H
#define _VOLATILEARG_H

#ifndef VOLATILE_ARG

// Use this macro to disable unreferenced variable/argument warnings.
#define VOLATILE_ARG(arg) AvoidVolatileArgWarn(arg)
#define VOLATILE_ARGS(args) AvoidVolatileArgWarn args

template <class T>
inline void AvoidVolatileArgWarn(const T&, ...) {}

#ifdef PLATFORM_LINUX
  #define VARIABLE_IS_NOT_USED __attribute__ ((unused))
#else
  #define VARIABLE_IS_NOT_USED
#endif

#endif
#endif

