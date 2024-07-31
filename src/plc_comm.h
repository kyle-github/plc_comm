
#pragma once

/***************************************************************************
 *   Copyright (C) 2024 by Kyle Hayes                                      *
 *   Author Kyle Hayes  kyle.hayes@gmail.com                               *
 *                                                                         *
 * This software is available under either the Mozilla Public License      *
 * version 2.0 or the GNU LGPL version 2 (or later) license, whichever     *
 * you choose.                                                             *
 *                                                                         *
 * MPL 2.0:                                                                *
 *                                                                         *
 *   This Source Code Form is subject to the terms of the Mozilla Public   *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this   *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.              *
 *                                                                         *
 *                                                                         *
 * LGPL 2:                                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include <stdbool.h>
#include <stdint.h>

typedef enum {
    VAL_OK,
    VAL_ERR_TYPE_MISMATCH,
    VAL_ERR_NOT_FOUND,
    VAL_ERR_BUSY,
    VAL_ERR_NULL_PTR,
    VAL_ERR_NOT_IMPLEMENTED,
    VAL_ERR_OUT_OF_BOUNDS,
    VAL_ERR_DIRTY_COUNT_CORRUPT,
} value_status_t;

typedef enum {
    VAL_TYPE_BOOL,
    VAL_TYPE_U8,
    VAL_TYPE_U16,
    VAL_TYPE_U32,
    VAL_TYPE_U64,
    VAL_TYPE_I8,
    VAL_TYPE_I16,
    VAL_TYPE_I32,
    VAL_TYPE_I64,
    VAL_TYPE_F32,
    VAL_TYPE_F64,
    VAL_TYPE_STR, /* open this can of worms? */
    VAL_TYPE_ARRAY,
    VAL_TYPE_STRUCTURE,
} value_type_t;

typedef struct {
    value_type_t val_type;
} *value_any;


extern value_status_t value_open_device(const char *plc_connect_attrs, value_any *plc_tags);
extern value_status_t value_close_device(value_any plc_tags);

extern value_status_t value_sync(value_any value);

extern value_status_t value_get_type(value_any var, value_type_t *val_type);
extern value_status_t value_get_size(value_any var, uint32_t *val_size);

extern value_status_t value_bool_get_value(value_any var, bool *val);
extern value_status_t value_bool_set_value(value_any var, bool val);

extern value_status_t value_uint_get_value(value_any var, uint64_t *val);
extern value_status_t value_uint_set_value(value_any var, uint64_t val);

extern value_status_t value_int_get_value(value_any var, int64_t *val);
extern value_status_t value_int_set_value(value_any var, int64_t val);

extern value_status_t value_float_get_value(value_any var, double *val);
extern value_status_t value_float_set_value(value_any var, double val);

extern value_status_t value_array_get_element_type(value_any array, value_type_t *element_type);
extern value_status_t value_array_get_element(value_any array, uint32_t index, value_any *element_val);

extern value_status_t value_structure_get_name(value_any structure, const char **struct_name);
extern value_status_t value_structure_get_num_fields(value_any structure, uint32_t *num_fields);
extern value_status_t value_structure_get_field_name(value_any structure, uint32_t field_index, const char **field_name);
extern value_status_t value_structure_get_field_by_index(value_any structure, uint32_t field_index, value_any *field_val);
extern value_status_t value_structure_get_field_by_name(value_any structure, const char *field_name, value_any *field_val);
