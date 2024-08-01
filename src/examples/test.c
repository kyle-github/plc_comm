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
    value_status_t rc = VAL_OK;
    value_p plc = NULL;
    do {
        value_status_t rc = VAL_OK;
        uint32_t num_tags = 0;

        rc = value_open_device("protocol=ab-eip&gateway=10.206.1.39&path=1,0&plc=ControlLogix", &plc);
        if(rc != VAL_OK) {
            info("WARN: Unable to open connection to the PLC!");
            break;
        }

        rc = value_structure_get_num_fields(plc, &num_tags);
        if(rc != VAL_OK) {
            info("WARN: Unable to get the number of tags!");
            break;
        }

        for(uint32_t i=0; i < num_tags && rc = VAL_OK; i++)  {
            const char *tag_name = NULL;
            value_p tag_val;
            value_type_t tag_type = VAL_TYPE_NONE;

            rc = value_structure_get_field_name(plc, i, &tag_name);
            if(rc != VAL_OK) {
                info("WARN: Unable to get the number of tags!");
                break;
            }

            rc = value_structure_get_field_by_index(plc, i, &tag_val);
            if(rc != VAL_OK) {
                info("WARN: Unable to get the field %u by index!", i);
                break;
            }

            rc = value_get_type(tag_val, &tag_type);
            if(rc != VAL_OK) {
                info("WARN: Unable to get the number of tags!");
                break;
            }

            printf("Tag %s is of type ");
            switch(tag_type) {
                case VAL_TYPE_BOOL:
                    printf(" Bool.\n");
                    break;

                case VAL_TYPE_U8:
                    printf(" U8.\n");
                    break;

                case VAL_TYPE_U16:
                    printf(" U16.\n");
                    break;

                case VAL_TYPE_U32:
                    printf(" U32.\n");
                    break;

                case VAL_TYPE_U64:
                    printf(" U64.\n");
                    break;

                case VAL_TYPE_I8:
                    printf(" I8.\n");
                    break;

                case VAL_TYPE_I16:
                    printf(" I16.\n");
                    break;

                case VAL_TYPE_I32:
                    printf(" I32.\n");
                    break;

                case VAL_TYPE_I64:
                    printf(" I64.\n");
                    break;

                case VAL_TYPE_F32:
                    printf(" Float32.\n");
                    break;

                case VAL_TYPE_F64:
                    printf(" Float64.\n");
                    break;

                case VAL_TYPE_ARRAY: {
                        uint32_t num_elements = 0;
                        value_type_t element_type = VAL_TYPE_NONE;

                        printf(" Array");

                        rc = value_array_get_num_elements(tag_val, &num_elements);
                        if(rc != VAL_OK) {
                            info("WARN: Unable to get the number of tags!");
                            break;
                        }

                        printf("[%u]", num_elements);

                        rc = value_array_get_element_type(tag_val, &element_type);
                        if(rc != VAL_OK) {
                            info("WARN: Unable to get the element type of the array!");
                            break;
                        }

                        printf(" elements of type %u.\n", element_type);
                    }

                    break;

                case VAL_TYPE_STRUCTURE: {
                        uint32_t num_fields = 0;

                        printf(" Structure ");

                        rc = value_structure_get_num_fields(tag_val, &num_fields);
                        if(rc != VAL_OK) {
                            info("WARN: Unable to get the number of tags!");
                            break;
                        }

                        printf("of %u fields.\n", num_fields);
                    }

                    break;

                default:
                    printf("Unknown type!\n");
                    break;
            }

        }
    } while(0);

    return rc;
}
