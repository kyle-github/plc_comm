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

#include <stdio.h>
#include "plc_comm.h"

#include "utils.h"


/**
 * @brief This tests the simplest multiple tag async use of the library
 * 
 * This function shows an example of how to read multiple tags with the network operations taking place
 * in the background asynchronously.   All memory and other resourcess are managed by the library.   Only the basic functions are used:
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
    plc_comm_id_t conn_id = PLC_COMM_CONN_NULL_ID;
    plc_comm_id_t request_batch_id = PLC_COMM_REQUEST_BATCH_NULL_ID;
    plc_comm_id_t result_batch_id = PLC_COMM_RESULT_BATCH_NULL_ID;
    int32_t rc = PLC_COMM_STATUS_OK;
    int64_t end_time = (int64_t)0;
    int32_t num_results = 0;
    int32_t *tag_elements = NULL;
    int32_t num_elements = 0;

    do {
        conn_id = plc_comm_conn_open(PLC_COMM_PLC_TYPE_COMPACTLOGIX, "10.1.2.3", NULL, 5000);
        if(conn_id < 0) break;

        /* build a batch of requests. */
        request_batch_id = plc_comm_request_batch_init(conn_id, 10, PLC_COMM_CONN_NULL_ID);
        if(request_batch_id < 0) break;
    
        /* fill in the requests */
        rc = plc_comm_request_init(request_batch_id, 0, "MyDINTTag[0]", 1, PLC_COMM_REQUEST_TYPE_READ, PLC_COMM_CONN_NULL_ID);
        if(rc != PLC_COMM_STATUS_OK) break;
        
        rc = plc_comm_request_init(request_batch_id, 1, "MyDINTTag[1]", 1, PLC_COMM_REQUEST_TYPE_READ, PLC_COMM_CONN_NULL_ID);
        if(rc != PLC_COMM_STATUS_OK) break;
        
        rc = plc_comm_request_init(request_batch_id, 2, "MyDINTTag[2]", 1, PLC_COMM_REQUEST_TYPE_READ, PLC_COMM_CONN_NULL_ID);
        if(rc != PLC_COMM_STATUS_OK) break;
        
        rc = plc_comm_request_init(request_batch_id, 3, "MyDINTTag[3]", 1, PLC_COMM_REQUEST_TYPE_READ, PLC_COMM_CONN_NULL_ID);
        if(rc != PLC_COMM_STATUS_OK) break;
        
        rc = plc_comm_request_init(request_batch_id, 4, "MyDINTTag[4]", 1, PLC_COMM_REQUEST_TYPE_READ, PLC_COMM_CONN_NULL_ID);
        if(rc != PLC_COMM_STATUS_OK) break;
        
        rc = plc_comm_request_init(request_batch_id, 5, "MyDINTTag[5]", 1, PLC_COMM_REQUEST_TYPE_READ, PLC_COMM_CONN_NULL_ID);
        if(rc != PLC_COMM_STATUS_OK) break;
        
        rc = plc_comm_request_init(request_batch_id, 6, "MyDINTTag[6]", 1, PLC_COMM_REQUEST_TYPE_READ, PLC_COMM_CONN_NULL_ID);
        if(rc != PLC_COMM_STATUS_OK) break;
        
        rc = plc_comm_request_init(request_batch_id, 7, "MyDINTTag[7]", 1, PLC_COMM_REQUEST_TYPE_READ, PLC_COMM_CONN_NULL_ID);
        if(rc != PLC_COMM_STATUS_OK) break;
        
        rc = plc_comm_request_init(request_batch_id, 8, "MyDINTTag[8]", 1, PLC_COMM_REQUEST_TYPE_READ, PLC_COMM_CONN_NULL_ID);
        if(rc != PLC_COMM_STATUS_OK) break;
        
        rc = plc_comm_request_init(request_batch_id, 9, "MyDINTTag[9]", 1, PLC_COMM_REQUEST_TYPE_READ, PLC_COMM_CONN_NULL_ID);
        if(rc != PLC_COMM_STATUS_OK) break;
        
        /* a timeout of zero tell the library to queue up the request and then return. */
        result_batch_id = plc_comm_conn_do_request(conn_id, "MyDINTTag", 10, PLC_COMM_REQUEST_TYPE_READ, PLC_COMM_CONN_NULL_ID, 0);
        if(result_batch_id < 0) break;

        /*
         * The request is being processed in the background.   Network operations usually take multiple milliseconds,
         * so we have a lot of time to do computation or other work if we want.
         * 
         * A real application would queue up many operations and then do other work while waiting for them
         * all to complete.
         */

        /* Loop while we have time left and the request is still processing. */
        end_time = util_time_ms() + (int64_t)5000; 
        while(((rc = plc_comm_result_batch_get_status(result_batch_id)) == PLC_COMM_STATUS_PENDING) && (util_time_ms() < end_time)) {
            /* wait for a bit.  This is simulating doing work. */
            util_sleep_ms(20);
        }

        if(rc == PLC_COMM_STATUS_PENDING) {
            /* we timed out. */
            rc = PLC_COMM_STATUS_TIMEOUT;
        }

        /* check the status, all good or partially good is OK */
        if((rc != PLC_COMM_STATUS_OK) || (rc == PLC_COMM_STATUS_PARTIAL)) break;

        /* get the number of results */
        num_results = plc_comm_result_batch_get_attr_int(request_batch_id, PLC_COMM_ATTR_RESULT_BATCH_RESULT_COUNT, -1);

        for(int result_indx=0; i < num_results; result_indx++) {
            num_elements = plc_comm_result_get_attr_int(result_batch_id, result_indx, PLC_COMM_ATTR_RESULT_TRANSLATED_ELEMENT_COUNT, -1);
            tag_elements = (int32_t *)plc_comm_result_get_attr_ptr(result_batch_id, result_indx, PLC_COMM_ATTR_RESULT_TRANSLATED_ELEMENT_DATA, NULL);

            for(int i=0; i < num_elements; i++) {
                printf("data[%d] = %d (%08x)\n", i, tag_elements[i], tag_elements[i]);
            }
        }
    } while(0);

    if(conn_id < 0) {
        printf("Connection opening failed with status %"PRId32"!\n", (int32_t)conn_id);
        rc = conn_id;
    } else {
        if(conn_id != PLC_COMM_CONN_NULL_ID) {
            plc_comm_conn_dispose(conn_id);
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


