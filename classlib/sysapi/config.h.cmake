#ifndef CLASSLIB_CONFIG_H
# define CLASSLIB_CONFIG_H 1

/* Define to 1 if you have <dlfcn.h> header. */
#cmakedefine CLASSLIB_HAVE_DLFCN_H ${CLASSLIB_HAVE_DLFCN_H}

/* Define to 1 if you have __bss_start symbol. */
#cmakedefine CLASSLIB_HAVE_SYM_BSS_START ${CLASSLIB_HAVE_SYM_BSS_START}

/* Define to 1 if you have __data_start symbol. */
#cmakedefine CLASSLIB_HAVE_SYM_DATA_START ${CLASSLIB_HAVE_SYM_DATA_START}

/* Define to 1 if you have _edata symbol. */
#cmakedefine CLASSLIB_HAVE_SYM_EDATA ${CLASSLIB_HAVE_SYM_EDATA}

/* Define to 1 if you have _etext symbol. */
#cmakedefine CLASSLIB_HAVE_SYM_ETEXT ${CLASSLIB_HAVE_SYM_ETEXT}

/* Define to 1 if you have _end symbol. */
#cmakedefine CLASSLIB_HAVE_SYM_END ${CLASSLIB_HAVE_SYM_END}

#endif /* CLASSLIB_CONFIG_H */
