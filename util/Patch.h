/*
 * Patch.h
 *
 *  Created on: 3 de mar. de 2016
 *      Author: angel
 */

#ifndef SRC_UTIL_PATCH_H_
#define SRC_UTIL_PATCH_H_

#include <string>
#include <climits>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <cerrno>

/*
 * CORRECTS BUGS OF CYGWIN:
 *
 * for the moment cygwin does not allows the use of the functions at patch name space:
 * All functions are the implementations of c++11 functions, in the future when this functions are
 * available in cygwin is better to change them for the standard ones.
 */
namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }

    inline bool stol (int &i, char const *s, int base = 0)
    {
        char *end;
        long  l;
        errno = 0;
        l = strtol(s, &end, base);
        if ((errno == ERANGE && l == LONG_MAX) || l > INT_MAX) {
            return false;
        }
        if ((errno == ERANGE && l == LONG_MIN) || l < INT_MIN) {
            return false;
        }
        if (*s == '\0' || *end != '\0') {
            return false;
        }
        i = l;
        return true;
    }

    inline bool stod (float &i, char const *s)
        {
            char *end;
            double  l;
            errno = 0;
            l = strtod(s, &end);
            if ((errno == ERANGE && l == LONG_MAX) || l > INT_MAX) {
                return false;
            }
            if ((errno == ERANGE && l == LONG_MIN) || l < INT_MIN) {
                return false;
            }
            if (*s == '\0' || *end != '\0') {
                return false;
            }
            i = l;
            return true;
        }
}

#endif /* SRC_UTIL_PATCH_H_ */
