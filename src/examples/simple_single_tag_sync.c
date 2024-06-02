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
#include <stdio.h>

#include <plc_comm.h>



/**
 * @brief This tests the simplest use of the library
 * 
 * This function shows an example of how to read a single tag.   All memory and other resourcess
 * are managed by the library.   Only the basic functions are used:
 * 
 * plc_comm_id_t plc_comm_conn_open(plc_comm_plc_type_t plc_type, const char *address, plc_comm_config_t *config, int32_t timeout_ms);
 * plc_comm_id_t plc_comm_conn_do_request(plc_comm_id_t conn_id, const char *tag_name, int32_t num_elements, plc_comm_request_type_t op, plc_comm_config_t *config, int32_t timeout_ms);
 * int32_t plc_comm_result_batch_get_attr_int(plc_comm_id_t result_batch_id, int32_t result_indx, plc_comm_result_batch_attr_t attr, int32_t default_val);
 * void *plc_comm_result_batch_get_attr_ptr(plc_comm_id_t result_batch_id, int32_t result_indx, plc_comm_result_batch_attr_t attr, void *default_val);
 * int32_t plc_comm_conn_dispose(plc_comm_id_t conn_id);
 * 
 * @return int - status
 */
int main(void)
{
    plc_comm_id_t conn_id = PLC_COMM_CONFIG_NULL_ID;
    plc_comm_id_t result_batch_id = PLC_COMM_RESULT_BATCH_NULL_ID;
    int32_t rc = PLC_COMM_STATUS_OK;
    int32_t *tag_elements = NULL;
    int32_t num_elements = 0;

    do {
        conn_id = plc_comm_conn_open(PLC_COMM_PLC_TYPE_COMPACTLOGIX, "10.1.2.3", PLC_COMM_CONFIG_NULL_ID, 5000);
        if(conn_id < 0) break;

        result_batch_id = plc_comm_conn_do_request(conn_id, "MyDINTTag", 10, PLC_COMM_REQUEST_TYPE_READ, PLC_COMM_CONFIG_NULL_ID, 5000);
        if(result_batch_id < 0) break;

        rc = plc_comm_result_batch_get_status(result_batch_id);
        if(rc != PLC_COMM_STATUS_OK) break;

        num_elements = plc_comm_result_get_attr_int(result_batch_id, 0, PLC_COMM_ATTR_RESULT_TRANSLATED_ELEMENT_COUNT, -1);
        tag_elements = (int32_t *)plc_comm_result_get_attr_ptr(result_batch_id, 0, PLC_COMM_ATTR_RESULT_TRANSLATED_ELEMENT_DATA, NULL);

        for(int i=0; i < num_elements; i++) {
            printf("data[%d] = %d (%08x)\n", i, tag_elements[i], tag_elements[i]);
        }
    } while(0);

    if(conn_id < 0) {
        printf("Connection opening failed with status %"PRId32"!\n", (int32_t)conn_id);
        rc = conn_id;
    } else {
        if(conn_id != PLC_COMM_CONFIG_NULL_ID) {
            plc_comm_conn_dispose(conn_id, 5000);
        }
    }

    if(result_batch_id < 0) {
        printf("Request starting failed with status %"PRId32"!\n", (int32_t)result_batch_id);
        rc = result_batch_id;
    }

    if(rc != PLC_COMM_STATUS_OK) {
        printf("Request read failed with status %"PRId32"!\n", rc);
    } else {
        rc = 0;
    }

    return rc;
}


