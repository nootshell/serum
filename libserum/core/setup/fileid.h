/******************************************************************************
**                                                                           **
**   The MIT License                                                         **
**                                                                           **
**   Copyright 2017-2018 icecubetray                                         **
**                                                                           **
**   Permission is hereby granted, free of charge, to any person             **
**   obtaining a copy of this software and associated documentation files    **
**   (the "Software"), to deal in the Software without restriction,          **
**   including without limitation the rights to use, copy, modify, merge,    **
**   publish, distribute, sublicense, and/or sell copies of the Software,    **
**   and to permit persons to whom the Software is furnished to do so,       **
**   subject to the following conditions:                                    **
**                                                                           **
**   The above copyright notice and this permission notice shall be          **
**   included in all copies or substantial portions of the Software.         **
**                                                                           **
**   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,         **
**   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF      **
**   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  **
**   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY    **
**   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,    **
**   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE       **
**   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                  **
**                                                                           **
******************************************************************************/



#if (!defined(__LS_CORE_SETUP_FILEID_H) && LS_EXPORTING)
#define __LS_CORE_SETUP_FILEID_H



#include "./platform-setup.h"



#ifndef GIT_TAG
#	define GIT_TAG							"undefined"
#endif

#ifndef GIT_COMMIT
#	define GIT_COMMIT						" (no commit info)"
#endif



#define LS_CODENAME                         "Fresh"
#define LS_VERSION							GIT_TAG

#define LS_AUTHORS							\
			"ice cube tray"



#define FILEID_PLAIN(name, string)			static LS_ATTR_USED const char name[] = "$Id: " string " $"
#define FILEID(desc)						FILEID_PLAIN(___FILEID___, FILEPATH GIT_COMMIT " " desc)



#endif
