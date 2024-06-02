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

#define NUM_ELEMENTS (10)

/**
 * @brief This tests the simplest use of the library with configuration.
 * 
 * This function shows an example of how to read the off-the-network, raw, data from the PLC
 * into a local buffer that the application controls.
 *    
 * All other memory and other resourcess are managed by the library.   Only the basic functions are used:
 * 
 * plc_comm_id_t plc_comm_conn_open(plc_comm_plc_type_t plc_type, const char *address, plc_comm_config_t *config, int32_t timeout_ms);
 * plc_comm_id_t plc_comm_conn_request_config_init(plc_comm_id_t conn_id, void *app_config_data, int32_t app_config_data_size);
 * plc_comm_id_t plc_comm_conn_do_request(plc_comm_id_t conn_id, const char *tag_name, int32_t num_elements, plc_comm_request_type_t op, plc_comm_config_t *config, int32_t timeout_ms);
 * int32_t plc_comm_conn_dispose(plc_comm_id_t conn_id, int32_t timeout_ms);
 * 
 * @return int - status
 */

/* Rockwell's definition of a 32-bit integer. */
typedef int32_t DINT;

int main(void)
{
    plc_comm_id_t conn_id = PLC_COMM_CONFIG_NULL_ID;
    plc_comm_id_t config_id = PLC_COMM_CONFIG_NULL_ID;
    plc_comm_id_t result_batch_id = PLC_COMM_RESULT_BATCH_NULL_ID;
    int32_t rc = PLC_COMM_STATUS_OK;
    int32_t tag_elements[NUM_ELEMENTS]  = {0};
    uint8_t raw_elements[NUM_ELEMENTS * sizeof(DINT)] = {0};

    do {
        conn_id = plc_comm_conn_open(PLC_COMM_PLC_TYPE_COMPACTLOGIX, "10.1.2.3", PLC_COMM_CONFIG_NULL_ID, 5000);
        if(conn_id < 0) break;

        /* get a config object for a request */
        config_id = plc_comm_request_config_init(conn_id, NULL, 0);
        if(config_id < 0) break;

        /* set up config for the input raw data buffer.  Pass a pointer and a size IN BYTES. */
        rc = plc_comm_config_set_attr_buf(config_id, PLC_COMM_ATTR_RESULT_RAW_DATA_BUF, &raw_elements[0], (int32_t)(uint32_t)sizeof(raw_elements));
        if(rc != PLC_COMM_STATUS_OK) break;

        /* now the library will use the space we provided for the raw data from the PLC */

        /* Pass the config so that the request knows to use the raw_element array directly. */
        result_batch_id = plc_comm_conn_do_request(conn_id, "MyDINTTag", NUM_ELEMENTS, PLC_COMM_REQUEST_TYPE_READ, &tag_elements[0], (int32_t)(uint32_t)sizeof(tag_elements), config_id, 5000);
        if(result_batch_id < 0) break;

        rc = plc_comm_result_batch_get_status(result_batch_id);
        if(rc != PLC_COMM_STATUS_OK) break;

        /* we can access the tag_elements array directly.   The mapping set up in the result config*/
        for(int i=0; i < NUM_ELEMENTS; i++) {
            printf("data[%d] = %"PRId32" (%08"PRIx32")\n", i, tag_elements[i], tag_elements[i]);
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


