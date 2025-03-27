#ifndef _WINVERSION_NOSLIB_HPP_
#define _WINVERSION_NOSLIB_HPP_

/* Just to remove boosts compile error about windows version */
#ifdef WIN32
#include <SDKDDKVer.h>
#endif // WIN32

#endif // _WINVERSION_NOSLIB_HPP_