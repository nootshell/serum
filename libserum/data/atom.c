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



#include "./atom.h"



ls_result_t
ls_atom32_init(ls_atom32_t *const atom) {
	if (atom == NULL) {
		return LS_E_NULL;
	}

	if (LS_MAGIC32_VALID(atom->lock.flags)) {
		return LS_E_MAGIC;
	}

	atom->timeout_millis = 0;
	atom->value = 0;
	return ls_mutex_init(&atom->lock);
}



ls_result_t
ls_atom32_clear(ls_atom32_t *const atom) {
	if (atom == NULL) {
		return LS_E_NULL;
	}

	if (!LS_MAGIC32_VALID(atom->lock.flags)) {
		return LS_E_MAGIC;
	}

	atom->timeout_millis = 0;
	atom->value = 0;
	return ls_mutex_clear(&atom->lock);
}



ls_result_t
ls_atom32_flag_set(ls_atom32_t *const atom, const ls_uint32_t flag) {
	if (atom == NULL) {
		return LS_E_NULL;
	}

	if (flag == 0) {
		return LS_E_INVALID;
	}

	if (!LS_MAGIC32_VALID(atom->lock.flags)) {
		return LS_E_MAGIC;
	}

	{
		ls_result_t result = LS_E_UNINITIALIZED;
		if (atom->timeout_millis == 0) {
			result = ls_mutex_lock(&atom->lock);
		} else {
			result = ls_mutex_timedlock_millis(&atom->lock, atom->timeout_millis);
		}
		if (result != LS_E_SUCCESS) {
			return result;
		}
	}

	atom->value |= flag;

	return ls_mutex_unlock(&atom->lock);
}



ls_result_t
ls_atom32_flag_unset(ls_atom32_t *const atom, const ls_uint32_t flag) {
	if (atom == NULL) {
		return LS_E_NULL;
	}

	if (flag == 0) {
		return LS_E_INVALID;
	}

	if (!LS_MAGIC32_VALID(atom->lock.flags)) {
		return LS_E_MAGIC;
	}

	{
		ls_result_t result = LS_E_UNINITIALIZED;
		if (atom->timeout_millis == 0) {
			result = ls_mutex_lock(&atom->lock);
		} else {
			result = ls_mutex_timedlock_millis(&atom->lock, atom->timeout_millis);
		}
		if (result != LS_E_SUCCESS) {
			return result;
		}
	}

	atom->value &= ~flag;

	return ls_mutex_unlock(&atom->lock);
}



ls_result_t
ls_atom32_flag_get(ls_atom32_t *const restrict atom, const ls_uint32_t flag, ls_bool_t *const restrict out_state) {
	if (atom == NULL) {
		return LS_E_NULL;
	}

	if (flag == 0) {
		return LS_E_INVALID;
	}

	if (!LS_MAGIC32_VALID(atom->lock.flags)) {
		return LS_E_MAGIC;
	}

	{
		ls_result_t result = LS_E_UNINITIALIZED;
		if (atom->timeout_millis == 0) {
			result = ls_mutex_lock(&atom->lock);
		} else {
			result = ls_mutex_timedlock_millis(&atom->lock, atom->timeout_millis);
		}
		if (result != LS_E_SUCCESS) {
			return result;
		}
	}

	*out_state = LS_FLAG(atom->value, flag);

	return ls_mutex_unlock(&atom->lock);
}
