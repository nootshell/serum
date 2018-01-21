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



#include "./thread.h"



FILEID("Threading interface.");



ls_result_t
ls_thread_init(ls_thread_t *const thread, const ls_uint32_t flags) {
	return LS_E_UNSUPPORTED;
}

ls_result_t
ls_thread_clear(ls_thread_t *const thread) {
	return LS_E_UNSUPPORTED;
}



ls_result_t
ls_thread_start(ls_thread_t *const thread) {
	return LS_E_UNSUPPORTED;
}

ls_result_t
ls_thread_stop(ls_thread_t *const thread) {
	return LS_E_UNSUPPORTED;
}



ls_result_t
ls_thread_suspend(ls_thread_t *const thread) {
	return LS_E_UNSUPPORTED;
}

ls_result_t
ls_thread_resume(ls_thread_t *const thread) {
	return LS_E_UNSUPPORTED;
}
