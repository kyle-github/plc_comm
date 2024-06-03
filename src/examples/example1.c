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
 * @brief This tests the simplest use of the library
 * 
 * This function shows an example of how to read a single tag.   All memory and other resourcess
 * are managed by the library.   Only the basic functions are used:
 * 
 * plc_comm_id_t plc_comm_conn_open(plc_comm_plc_type_t plc_type, const char *address, plc_comm_config_t *config, int32_t timeout_ms);
 * plc_comm_id_t plc_comm_conn_do_request(plc_comm_id_t conn_id, const char *tag_name, int32_t num_elements, plc_comm_request_type_t op, void *translated_data, int32_t translated_data_size, plc_comm_config_t *config, int32_t timeout_ms);
 * int32_t plc_comm_result_batch_get_status(plc_comm_id_t result_batch_id);
 * int32_t plc_comm_conn_dispose(plc_comm_id_t conn_id, int32_t timeout_ms);
 * 
 * @return int - status
 */
int main(void)
{
    plc_comm_id_t conn_id = PLC_COMM_CONFIG_NULL_ID;
    plc_comm_id_t result_batch_id = PLC_COMM_RESULT_BATCH_NULL_ID;
    int32_t rc = PLC_COMM_STATUS_OK;
    int32_t tag_elements[NUM_ELEMENTS] = {0};
    int32_t num_elements = 0;

    do {
        /*
         * Open a connection the PLC.  This has the most common arguments split
         * out as parameters.   Less common arguments can be set via a config object.
         * 
         * The call is done synchronously.  This is indicated by the timeout value, 5000,
         * which is the number of milliseconds to wait for the connection to complete or
         * fail.  
         * 
         * The connection ID returned is just an integer handle.  A positive value is a
         * valid handle and a negative one is an error response.
         * 
         * All error, warning and status return values are negative. 
         */
        conn_id = plc_comm_conn_open(PLC_COMM_PLC_TYPE_COMPACTLOGIX, "10.1.2.3", PLC_COMM_CONFIG_NULL_ID, 5000);
        if(conn_id < 0) break;

        /*
         * This does a single request on a tag.   It returns a batch of results to make
         * sure the API does not expand too much.  In this case, the batch has only
         * a single result in it.
         * 
         * There are three levels of error reporting here.   The first is the batch ID itself.
         * If that is negative, something went wrong with the creation of the batch.  Perhaps
         * memory was not available or a required argument was missing.
         * 
         * The second level of error handling is the status of the request batch.  If the batch
         * has multiple requests in it, one of them might have failed an other succeeded.  In
         * that case, the batch as a whole has not been fully successful and a status of partial
         * success will be found.
         * 
         * The arguments include references to the C array tag_elements where the result will
         * be placed after the raw data is received and translated to host PC format.
         */
        result_batch_id = plc_comm_conn_do_request(conn_id, "MyDINTTag", NUM_ELEMENTS, PLC_COMM_REQUEST_TYPE_READ, &tag_elements[0], (int32_t)(uint32_t)sizeof(tag_elements), PLC_COMM_CONFIG_NULL_ID, 5000);
        if(result_batch_id < 0) break;

        /*
         * Check the status of the request batch.  This is actually skipping the batch-level
         * status check.
         *
         * We know that there is only one result so we just check that status.   This is 
         * only a possible short-cut when we have just one result.
         */
        rc = plc_comm_result_get_status(result_batch_id, 0);
        if(rc != PLC_COMM_STATUS_OK) break;

        /*
         * Print out the resulting data.   Note that the data is in a local C array
         * in the host format.
         *
         * The library will handle all the translation and management of the raw data
         * from the PLC.
         * 
         * The array tag_elements is just a normal C array.  All regular C code can be used
         * with it.
         */
        for(int i=0; i < num_elements; i++) {
            printf("data[%d] = %"PRId32" (%08"PRIx32")\n", i, tag_elements[i], tag_elements[i]);
        }
    } while(0);

    if(conn_id < 0) {
        printf("Connection opening failed with status %"PRId32"!\n", (int32_t)conn_id);
        rc = conn_id;
    } else {
        if(conn_id != PLC_COMM_CONFIG_NULL_ID) {
            /* 
             * The library will clean up all the managed resources including memory, sockets,
             * threads etc. when dispose is called.  If the connection is still active, it will
             * be closed first.   This is why there is a timeout to this operation.
             */
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


