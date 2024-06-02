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

#include "utils.h"

#define NUM_ELEMENTS (10)


/**
 * @brief This tests the simplest multiple tag synchronous use of the library
 * 
 * This function shows an example of how to read multiple tags.  The results are placed into the 
 * local array of data.
 * 
 * Functions used:
 * 
 * plc_comm_id_t plc_comm_conn_open(plc_comm_plc_type_t plc_type, const char *address, plc_comm_config_t *config, int32_t timeout_ms);
 * plc_comm_id_t plc_comm_request_batch_init(plc_comm_id_t conn_id, int32_t num_requests, plc_comm_id_t config_id); 
 * int32_t plc_comm_request_init(plc_comm_id_t request_batch_id, int32_t request_indx, const char *tag_name, int32_t num_elements, plc_comm_request_op_t op, void *translated_data, int32_t translated_data_size, plc_comm_id_t config_id);
 * plc_comm_id_t plc_comm_conn_do_request_batch(plc_comm_id_t conn_id, plc_comm_id_t request_batch_id, plc_comm_id_t config_id, int32_t timeout_ms);
 * int32_t plc_comm_result_batch_get_status(plc_comm_id_t result_batch_id);
 * int32_t plc_comm_conn_dispose(plc_comm_id_t conn_id, int32_t timeout_ms);
 * 
 * @return int - status
 */
int main(void)
{
    plc_comm_id_t conn_id = PLC_COMM_CONFIG_NULL_ID;
    plc_comm_id_t request_batch_id = PLC_COMM_REQUEST_BATCH_NULL_ID;
    plc_comm_id_t result_batch_id = PLC_COMM_RESULT_BATCH_NULL_ID;
    int32_t rc = PLC_COMM_STATUS_OK;
    int64_t end_time = (int64_t)0;
    int32_t num_results = 0;
    int32_t tag_elements[NUM_ELEMENTS] = {0};

    do {
        conn_id = plc_comm_conn_open(PLC_COMM_PLC_TYPE_COMPACTLOGIX, "10.1.2.3", PLC_COMM_CONFIG_NULL_ID, 5000);
        if(conn_id < 0) break;

        /* build a batch of requests. */
        request_batch_id = plc_comm_request_batch_init(conn_id, NUM_ELEMENTS, PLC_COMM_CONFIG_NULL_ID);
        if(request_batch_id < 0) break;
    
        /* map each tag to local data. */
        rc = plc_comm_request_init(request_batch_id, 0, "MyDINTTag[0]", 1, PLC_COMM_REQUEST_TYPE_READ, &tag_elements[0], sizeof(tag_elements[0]), PLC_COMM_CONFIG_NULL_ID);
        if(rc != PLC_COMM_STATUS_OK) break;
        
        rc = plc_comm_request_init(request_batch_id, 1, "MyDINTTag[1]", 1, PLC_COMM_REQUEST_TYPE_READ, &tag_elements[1], sizeof(tag_elements[1]), PLC_COMM_CONFIG_NULL_ID);
        if(rc != PLC_COMM_STATUS_OK) break;
        
        rc = plc_comm_request_init(request_batch_id, 2, "MyDINTTag[2]", 1, PLC_COMM_REQUEST_TYPE_READ, &tag_elements[2], sizeof(tag_elements[2]), PLC_COMM_CONFIG_NULL_ID);
        if(rc != PLC_COMM_STATUS_OK) break;
        
        rc = plc_comm_request_init(request_batch_id, 3, "MyDINTTag[3]", 1, PLC_COMM_REQUEST_TYPE_READ, &tag_elements[3], sizeof(tag_elements[3]), PLC_COMM_CONFIG_NULL_ID);
        if(rc != PLC_COMM_STATUS_OK) break;
        
        rc = plc_comm_request_init(request_batch_id, 4, "MyDINTTag[4]", 1, PLC_COMM_REQUEST_TYPE_READ, &tag_elements[4], sizeof(tag_elements[4]), PLC_COMM_CONFIG_NULL_ID);
        if(rc != PLC_COMM_STATUS_OK) break;
        
        rc = plc_comm_request_init(request_batch_id, 5, "MyDINTTag[5]", 1, PLC_COMM_REQUEST_TYPE_READ, &tag_elements[5], sizeof(tag_elements[5]), PLC_COMM_CONFIG_NULL_ID);
        if(rc != PLC_COMM_STATUS_OK) break;
        
        rc = plc_comm_request_init(request_batch_id, 6, "MyDINTTag[6]", 1, PLC_COMM_REQUEST_TYPE_READ, &tag_elements[6], sizeof(tag_elements[6]), PLC_COMM_CONFIG_NULL_ID);
        if(rc != PLC_COMM_STATUS_OK) break;
        
        rc = plc_comm_request_init(request_batch_id, 7, "MyDINTTag[7]", 1, PLC_COMM_REQUEST_TYPE_READ, &tag_elements[7], sizeof(tag_elements[7]), PLC_COMM_CONFIG_NULL_ID);
        if(rc != PLC_COMM_STATUS_OK) break;
        
        rc = plc_comm_request_init(request_batch_id, 8, "MyDINTTag[8]", 1, PLC_COMM_REQUEST_TYPE_READ, &tag_elements[8], sizeof(tag_elements[8]), PLC_COMM_CONFIG_NULL_ID);
        if(rc != PLC_COMM_STATUS_OK) break;
        
        rc = plc_comm_request_init(request_batch_id, 9, "MyDINTTag[9]", 1, PLC_COMM_REQUEST_TYPE_READ, &tag_elements[9], sizeof(tag_elements[9]), PLC_COMM_CONFIG_NULL_ID);
        if(rc != PLC_COMM_STATUS_OK) break;
        
        /* a timeout of zero tell the library to queue up the batch of requests and then return. */
        result_batch_id = plc_comm_conn_do_request_batch(conn_id, request_batch_id, PLC_COMM_CONFIG_NULL_ID, 5000);
        if(result_batch_id < 0) break;

        rc = plc_comm_result_batch_get_status(result_batch_id);

        /* check the status, all good or partially good is OK */
        if((rc != PLC_COMM_STATUS_OK) || (rc == PLC_COMM_STATUS_PARTIAL)) break;

        for(int result_indx=0; result_indx < NUM_ELEMENTS; result_indx++) {
            /* if the individual result is good then use the data */
            rc = plc_comm_result_get_status(result_batch_id, result_indx);
            if(rc == PLC_COMM_STATUS_OK) {
                printf("data[%d] = %"PRId32" (%08"PRIx32")\n", result_indx, tag_elements[result_indx], tag_elements[result_indx]);
            } else {
                printf("data[%d] was not retrived correctly with error %"PRId32"!\n", result_indx, rc);
            }
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

