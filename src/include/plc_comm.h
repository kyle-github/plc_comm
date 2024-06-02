
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


#include <inttypes.h>


#define LIB_EXTERN extern


/* general definitions */

/* IDs are typed internally */
typedef int32_t plc_comm_id_t;

typedef enum {
    PLC_COMM_STATUS_ILLEGAL = 0, /* not allowed, this is a coding error if this happens. */

    /* non-error or warning status */
    PLC_COMM_STATUS_OK = -1,
    PLC_COMM_STATUS_PENDING = -2,
    PLC_COMM_STATUS_TIMEOUT = -3,
    PLC_COMM_STATUS_PARTIAL = -4,

    /* errors */
    PLC_COMM_ERR_OUT_OF_BOUNDS = -100,
    PLC_COMM_ERR_NOT_FOUND = -101,
    PLC_COMM_ERR_NO_RESOURCES = -102,
    PLC_COMM_ERR_NOT_SUPPORTED = -103,
    
} plc_comm_status_t;


typedef enum {
    PLC_COMM_ATTR_ID_SHIFT = 8,

    PLC_COMM_ATTR_TYPE_MASK = 0x0F,

    PLC_COMM_ATTR_TYPE_BUF = 0x01,
    PLC_COMM_ATTR_TYPE_INT = 0x02,
    PLC_COMM_ATTR_TYPE_PTR = 0x03,
    PLC_COMM_ATTR_TYPE_FUNC = 0x04,
    PLC_COMM_ATTR_TYPE_UNUSED0x05 = 0x05,
    PLC_COMM_ATTR_TYPE_UNUSED0x06 = 0x06,
    PLC_COMM_ATTR_TYPE_UNUSED0x07 = 0x07,
    PLC_COMM_ATTR_TYPE_UNUSED0x08 = 0x08,
    PLC_COMM_ATTR_TYPE_UNUSED0x09 = 0x09,
    PLC_COMM_ATTR_TYPE_UNUSED0x0A = 0x0A,
    PLC_COMM_ATTR_TYPE_UNUSED0x0B = 0x0B,
    PLC_COMM_ATTR_TYPE_UNUSED0x0C = 0x0C,
    PLC_COMM_ATTR_TYPE_UNUSED0x0D = 0x0D,
    PLC_COMM_ATTR_TYPE_UNUSED0x0E = 0x0E,
    PLC_COMM_ATTR_TYPE_UNUSED0x0F = 0x0F,
    
    PLC_COMM_ATTR_ACCESS_MASK = 0xF0,

    PLC_COMM_ATTR_ACCESS_READ = 0x10,
    PLC_COMM_ATTR_ACCESS_DUMMY_READ = 0x20,
    PLC_COMM_ATTR_ACCESS_WRITE = 0x30,
    PLC_COMM_ATTR_ACCESS_UNUSED0x40 = 0x40,
    PLC_COMM_ATTR_ACCESS_UNUSED0x50 = 0x50,
    PLC_COMM_ATTR_ACCESS_UNUSED0x60 = 0x60,
    PLC_COMM_ATTR_ACCESS_UNUSED0x70 = 0x70,
    PLC_COMM_ATTR_ACCESS_UNUSED0x80 = 0x80,
    PLC_COMM_ATTR_ACCESS_UNUSED0x90 = 0x90,
    PLC_COMM_ATTR_ACCESS_UNUSED0xA0 = 0xA0,
    PLC_COMM_ATTR_ACCESS_UNUSED0xB0 = 0xB0,
    PLC_COMM_ATTR_ACCESS_UNUSED0xC0 = 0xC0,
    PLC_COMM_ATTR_ACCESS_UNUSED0xD0 = 0xD0,
    PLC_COMM_ATTR_ACCESS_UNUSED0xE0 = 0xE0,
    PLC_COMM_ATTR_ACCESS_UNUSED0xF0 = 0xF0,

} plc_comm_attr_base_t;

typedef enum {
    PLC_COMM_ID_TYPE_MASK = 0x0F,
    PLC_COMM_ID_SHIFT = 4,
    PLC_COMM_ID_MIN = 0x00000001,
    PLC_COMM_ID_MAX = 0x07FFFFFF,

    PLC_COMM_ID_CONN_TYPE = 0x01,
    PLC_COMM_ID_RESULT_BATCH_TYPE = 0x02,
    PLC_COMM_ID_CONFIG_TYPE = 0x03,
    PLC_COMM_ID_REQUEST_BATCH_TYPE = 0x04,
    PLC_COMM_ID_TYPE_UNUSED0x05 = 0x05,
    PLC_COMM_ID_TYPE_UNUSED0x06 = 0x06,
    PLC_COMM_ID_TYPE_UNUSED0x07 = 0x07,
    PLC_COMM_ID_TYPE_UNUSED0x08 = 0x08,
    PLC_COMM_ID_TYPE_UNUSED0x09 = 0x09,
    PLC_COMM_ID_TYPE_UNUSED0x0A = 0x0A,
    PLC_COMM_ID_TYPE_UNUSED0x0B = 0x0B,
    PLC_COMM_ID_TYPE_UNUSED0x0C = 0x0C,
    PLC_COMM_ID_TYPE_UNUSED0x0D = 0x0D,
    PLC_COMM_ID_TYPE_UNUSED0x0E = 0x0E,
    PLC_COMM_ID_TYPE_UNUSED0x0F = 0x0F,

} plc_comm_id_type_t;


/* connection definitions */

LIB_EXTERN int32_t PLC_COMM_CONN_NULL_ID = PLC_COMM_ID_CONN_TYPE;

/* supported types of PLCs */
typedef enum {
    PLC_COMM_PLC_TYPE_NONE = 0, /* catch uninitialized PLC data */

    PLC_COMM_PLC_TYPE_CONTROLLOGIX_1756_L5x = 10,
    PLC_COMM_PLC_TYPE_CONTROLLOGIX_1756_L6x,
    PLC_COMM_PLC_TYPE_CONTROLLOGIX_1756_L7x,
    PLC_COMM_PLC_TYPE_CONTROLLOGIX_1756_L8x,

    PLC_COMM_PLC_TYPE_COMPACTLOGIX = 150,

    PLC_COMM_PLC_TYPE_PLC5_CIP_DF1 = 200,
    PLC_COMM_PLC_TYPE_PLC5_BRIDGED_DHPLUS_DF1 = 201,
    PLC_COMM_PLC_TYPE_SLC500_CIP_DF1 = 202,
    PLC_COMM_PLC_TYPE_MICROLOGIX_CIP_DF1 = 203,
    PLC_COMM_PLC_TYPE_CONTROLLOGIX_CIP_DF1 = 204,

    PLC_COMM_PLC_TYPE_MICRO8x0 = 300,

    PLC_COMM_PLC_TYPE_OMRON_NJ = 400,
    PLC_COMM_PLC_TYPE_OMRON_NX = 401,
    PLC_COMM_PLC_TYPE_OMRON_CJ = 402,

    PLC_COMM_PLC_TYPE_MODBUS_GENERIC = 500
} plc_comm_plc_type_t;


typedef enum {
    PLC_COMM_REQUEST_TYPE_NONE = 0,
    PLC_COMM_REQUEST_TYPE_RAW = 1,
    PLC_COMM_REQUEST_TYPE_DUMMY_READ = 2,
    PLC_COMM_REQUEST_TYPE_READ = 3,
    PLC_COMM_REQUEST_TYPE_WRITE = 4,
} plc_comm_request_op_t;

LIB_EXTERN plc_comm_conn_open(plc_comm_plc_type_t plc_type, const char *address, plc_comm_id_t config, int32_t timeout_ms);
LIB_EXTERN plc_comm_conn_do_request(plc_comm_id_t conn_id, const char *tag_name, int32_t num_elements, plc_comm_request_op_t op, plc_comm_id_t config, int32_t timeout_ms);
LIB_EXTERN int32_t plc_comm_conn_dispose(plc_comm_id_t conn_id);


/* request definitions */

LIB_EXTERN int32_t PLC_COMM_REQUEST_BATCH_NULL_ID = PLC_COMM_ID_REQUEST_BATCH_TYPE;


/* result and result batch definitions */

LIB_EXTERN int32_t PLC_COMM_RESULT_BATCH_NULL_ID = PLC_COMM_ID_RESULT_BATCH_TYPE;

typedef enum {
    PLC_COMM_ATTR_RESULT_BATCH_RESULT_COUNT = ((1 << PLC_COMM_ATTR_ID_SHIFT) | PLC_COMM_ATTR_TYPE_INT | PLC_COMM_ATTR_ACCESS_READ),
} plc_comm_result_batch_attr_t;

LIB_EXTERN int32_t plc_comm_result_batch_get_attr_int(plc_comm_id_t result_batch_id, plc_comm_result_batch_attr_t attr, int32_t default_val);

typedef enum {
    PLC_COMM_ATTR_RESULT_TRANSLATED_ELEMENT_COUNT = ((1 << PLC_COMM_ATTR_ID_SHIFT) | PLC_COMM_ATTR_TYPE_INT | PLC_COMM_ATTR_ACCESS_READ),
    PLC_COMM_ATTR_RESULT_TRANSLATED_ELEMENT_DATA = ((2 << PLC_COMM_ATTR_ID_SHIFT) | PLC_COMM_ATTR_TYPE_PTR | PLC_COMM_ATTR_ACCESS_READ),
} plc_comm_result_attr_t;

LIB_EXTERN int32_t plc_comm_result_get_attr_int(plc_comm_id_t result_batch_id, int32_t result_indx, plc_comm_result_attr_t attr, int32_t default_val);
LIB_EXTERN void *plc_comm_result_get_attr_ptr(plc_comm_id_t result_batch_id, int32_t result_indx, plc_comm_result_attr_t attr, void *default_val);


