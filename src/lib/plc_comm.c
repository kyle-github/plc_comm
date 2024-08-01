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


#include <string.h>
#include <plc_comm.h>
#include <libplctag.h>
#include "utils.h"

typedef struct {
    value_type_t val_type;
    value_p parent;

    uint8_t *buf_data;
    uint32_t buf_data_size;
    uint32_t offset;
    uint32_t value_size;
    uint32_t index; /* for array elements */

    /* FIXME - these need to be atomic */
    bool dirty;
    bool busy;
} value_base;

typedef struct value_bool {
    value_base;

    uint8_t bit;
    int32_t (*encoder)(struct value_bool *bool_val, bool val);
    int32_t (*decoder)(struct value_bool *bool_val, bool *val);
} value_bool;


typedef struct value_uint {
    value_base;

    int32_t (*encoder)(struct value_uint *uint_val, uint64_t val);
    int32_t (*decoder)(struct value_uint *uint_val, uint64_t *val);
} value_uint;


typedef struct value_int {
    value_base;

    int32_t (*encoder)(struct value_int *int_val, int64_t val);
    int32_t (*decoder)(struct value_int *int_val, int64_t *val);
} value_int;


typedef struct value_float {
    value_base;

    int32_t (*encoder)(struct value_float *float_val, double val);
    int32_t (*decoder)(struct value_float *float_val, double *val);
} value_float;


#define VAL_TYPE_DEVICE (0x10000 + VAL_TYPE_STRUCTURE)

/*
 * multi-dimensional arrays are modeled as arrays of arrays (of arrays)
 * of the element type;
 */

typedef struct {
    value_base;

    uint32_t num_elements;
    value_type_t element_type;
    value_p *elements; /* note: pointer-to-pointer */
} value_array;


typedef struct {
    value_base;

    const char *name;
    uint32_t num_fields;
    struct {
        const char *field_name;
        value_p field_val;
    } *fields;
} value_structure;





typedef struct device {
    value_structure;

    int32_t device_raw_tag;
} device;


static value_status_t mark_dirty(value_base *val);
static value_status_t clear_dirty(value_base *val);
static value_status_t find_first_dirty(value_base *val, value_base *value_stack[], uint32_t stack_index);
static value_status_t get_tags(device *plc);


value_status_t value_open_device(const char *plc_connect_attrs, value_p **device_val)
{
    value_status_t rc = VAL_OK;
    device *plc = NULL;

    plc = calloc(1, sizeof(*plc));
    if(plc) {
        plc->device_raw_tag = plc_tag_create("protocol=ab-eip&gateway=10.206.1.39&path=1,0&plc=ControlLogix&name=@raw", 5000);
        if(plc->device_raw_tag > 0) {
            rc = get_tags(plc);
            if(rc != VAL_OK) {
                info("WARN: Error getting tag information from PLC!");
                rc = VAL_ERR_CONNECTION_ERROR;
            }
        } else {
            info("WARN: Unable to open PLC device handle! Error %s.", plc_tag_decode_error(plc->device_raw_tag));
            rc = VAL_ERR_CONNECTION_ERROR;
        }
    } else {
        info("WARN: Unable to allocate the device structure memory!");
        rc = VAL_ERR_NO_RESOURCES;
    }

    return rc;
}

value_status_t value_close_device(value_p plc_tags)


#define VALUE_STACK_DEPTH (50)


value_status_t value_sync(value_p value)
{
    value_status_t rc = VAL_OK;

    if(value) {
        /* chain up to the root tag then come back down looking for dirty values */
        value_base *val_base = (value_base *)value;
        value_base *parent = (value_base *)val_base->parent;

        while(parent && parent->val_type != VAL_TYPE_DEVICE) {
            parent = (value_base *)(parent->parent);
        }

        if(parent) {
            /* now we traverse the tree looking for the first dirty marker */
            value_base *value_stack[VALUE_STACK_DEPTH] = {0};

            rc = find_first_dirty(parent, value_stack, 0);
        }
    } else {
        rc = VAL_ERR_NULL_PTR;
    }

    return rc;
}

value_status_t value_get_type(value_p val, value_type_t *val_type)
{
    value_status_t rc = VAL_OK;

    if(val && val_type) {
        /* mask off extra parts like the device type */
        *val_type = (0xFFFF & val->val_type);
    } else {
        rc = VAL_ERR_NULL_PTR;
    }

    return rc;
}


value_status_t value_get_size(value_p val_arg, uint32_t *val_size)
{
    value_status_t rc = VAL_OK;

    if(val_arg && val_size) {
        value_base *val = (value_base *)val_arg;

        *val_size = val->value_size;
    } else {
        rc = VAL_ERR_NULL_PTR;
    }

    return rc;
}


#define DECODE(VAL_STRUCT_TYPE, VAL_ARG, RESULT)                \
    do {                                                        \
        if(VAL_ARG && RESULT) {                                 \
            VAL_STRUCT_TYPE *val = (VAL_STRUCT_TYPE *)VAL_ARG;  \
            if(val->decoder) {                                  \
                rc = val->decoder(val, RESULT);                 \
            } else {                                            \
                rc = VAL_ERR_NOT_IMPLEMENTED;                   \
            }                                                   \
        } else {                                                \
            rc = VAL_ERR_NULL_PTR;                              \
        }                                                       \
    } while(0)

#define ENCODE(VAL_STRUCT_TYPE, VAL_ARG, NEW_VAL)               \
    do {                                                        \
        if(VAL_ARG) {                                           \
            VAL_STRUCT_TYPE *val = (VAL_STRUCT_TYPE *)VAL_ARG;  \
            if(val->encoder) {                                  \
                rc = val->encoder(val, NEW_VAL);                \
                if(rc == VAL_OK) {                              \
                    rc = mark_dirty((value_base *)val);          \
                }                                               \
            } else {                                            \
                rc = VAL_ERR_NOT_IMPLEMENTED;                   \
            }                                                   \
        } else {                                                \
            rc = VAL_ERR_NULL_PTR;                              \
        }                                                       \
    } while(0)



value_status_t value_bool_get_value(value_p val_arg, bool *result)
{
    value_status_t rc = VAL_OK;

    DECODE(value_bool, val_arg, result);

    return rc;
}


value_status_t value_bool_set_value(value_p val_arg, bool new_value)
{
    value_status_t rc = VAL_OK;

    ENCODE(value_bool, val_arg, new_value);

    return rc;
}




value_status_t value_uint_get_value(value_p val_arg, uint64_t *result)
{
    value_status_t rc = VAL_OK;

    DECODE(value_uint, val_arg, result);

    return rc;
}


value_status_t value_uint_set_value(value_p val_arg, uint64_t new_val)
{
    value_status_t rc = VAL_OK;

    ENCODE(value_uint, val_arg, new_val);

    return rc;
}


value_status_t value_int_get_value(value_p val_arg, int64_t *result)
{
    value_status_t rc = VAL_OK;

    DECODE(value_int, val_arg, result);

    return rc;
}


value_status_t value_int_set_value(value_p val_arg, int64_t new_val)
{
    value_status_t rc = VAL_OK;

    ENCODE(value_int, val_arg, new_val);

    return rc;
}


value_status_t value_float_get_value(value_p val_arg, double *result)
{
    value_status_t rc = VAL_OK;

    DECODE(value_float, val_arg, result);

    return rc;
}


value_status_t value_float_set_value(value_p val_arg, double new_val)
{
    value_status_t rc = VAL_OK;

    ENCODE(value_float, val_arg, new_val);

    return rc;
}


value_status_t value_array_get_num_elements(value_p array_arg, uint32_t *num_elements)
{
    value_status_t rc = VAL_OK;

    if(array_arg && num_elements) {
        if(array_arg->val_type == VAL_TYPE_ARRAY) {
            value_array *val = (value_array *)array_arg;

            *num_elements = val->num_elements;
        } else {
            rc = VAL_ERR_TYPE_MISMATCH;
        }
    } else {
        rc = VAL_ERR_NULL_PTR;
    }

    return rc;
}



value_status_t value_array_get_element_type(value_p array_arg, value_type_t *element_type)
{
    value_status_t rc = VAL_OK;

    if(array_arg && element_type) {
        if(array_arg->val_type == VAL_TYPE_ARRAY) {
            value_array *val = (value_array *)array_arg;

            *element_type = val->element_type;
        } else {
            rc = VAL_ERR_TYPE_MISMATCH;
        }
    } else {
        rc = VAL_ERR_NULL_PTR;
    }

    return rc;
}


value_status_t value_array_get_element(value_p array_arg, uint32_t index, value_p *element_val)
{
    value_status_t rc = VAL_OK;

    if(array_arg && element_val) {
        if(array_arg->val_type == VAL_TYPE_ARRAY) {
            value_array *array = (value_array *)array_arg;

            if(index < array->num_elements) {
                *element_val = array->elements[index];
            } else {
                rc = VAL_ERR_OUT_OF_BOUNDS;
            }
        } else {
            rc = VAL_ERR_TYPE_MISMATCH;
        }
    } else {
        rc = VAL_ERR_NULL_PTR;
    }

    return rc;
}



value_status_t value_structure_get_name(value_p structure_arg, const char **struct_name)
{
    value_status_t rc = VAL_OK;

    if(structure_arg && struct_name) {
        if((structure_arg->val_type & 0xFFFF) == VAL_TYPE_STRUCTURE) {
            value_structure *structure = (value_structure *)structure_arg;

            *struct_name = structure->name;
        } else {
            rc = VAL_ERR_TYPE_MISMATCH;
        }
    } else {
        rc = VAL_ERR_NULL_PTR;
    }


    return rc;
}


value_status_t value_structure_get_num_fields(value_p structure_arg, uint32_t *num_fields)
{
    value_status_t rc = VAL_OK;

    if(structure_arg && num_fields) {
        if((structure_arg->val_type & 0xFFFF) == VAL_TYPE_STRUCTURE) {
            value_structure *structure = (value_structure *)structure_arg;

            *num_fields = structure->num_fields;
        } else {
            rc = VAL_ERR_TYPE_MISMATCH;
        }
    } else {
        rc = VAL_ERR_NULL_PTR;
    }


    return rc;
}


value_status_t value_structure_get_field_name(value_p structure_arg, uint32_t field_index, const char **field_name)
{
    value_status_t rc = VAL_OK;

    if(structure_arg && field_name) {
        if((structure_arg->val_type & 0xFFFF) == VAL_TYPE_STRUCTURE) {
            value_structure *structure = (value_structure *)structure_arg;

            if(field_index < structure->num_fields) {
                *field_name = structure->fields[field_index].field_name;
            } else {
                rc = VAL_ERR_OUT_OF_BOUNDS;
            }
        } else {
            rc = VAL_ERR_TYPE_MISMATCH;
        }
    } else {
        rc = VAL_ERR_NULL_PTR;
    }


    return rc;
}



value_status_t value_structure_get_field_by_index(value_p structure_arg, uint32_t field_index, value_p *field_val)
{
    value_status_t rc = VAL_OK;

    if(structure_arg && field_val) {
        if((structure_arg->val_type & 0xFFFF) == VAL_TYPE_STRUCTURE) {
            value_structure *structure = (value_structure *)structure_arg;

            if(field_index < structure->num_fields) {
                *field_val = structure->fields[field_index].field_val;
            } else {
                rc = VAL_ERR_OUT_OF_BOUNDS;
            }
        } else {
            rc = VAL_ERR_TYPE_MISMATCH;
        }
    } else {
        rc = VAL_ERR_NULL_PTR;
    }


    return rc;
}



value_status_t value_structure_get_field_by_name(value_p structure_arg, const char *field_name, value_p *field_val)
{
    value_status_t rc = VAL_OK;

    if(structure_arg && field_name && field_val) {
        if((structure_arg->val_type & 0xFFFF) == VAL_TYPE_STRUCTURE) {
            value_structure *structure = (value_structure *)structure_arg;

            /* find the field by name */
            rc = VAL_ERR_NOT_FOUND;
            for(uint32_t i=0; i < structure->num_fields; i++) {
                if(strcmp(field_name, structure->fields[i].field_name) == 0) {
                    *field_val = structure->fields[i].field_val;
                    rc = VAL_OK;
                    break;
                }
            }
        } else {
            rc = VAL_ERR_TYPE_MISMATCH;
        }
    } else {
        rc = VAL_ERR_NULL_PTR;
    }

    return rc;
}



/* private helper functions */
value_status_t mark_dirty(value_base *val)
{
    value_status_t rc = VAL_OK;

    if(val) {
        val->dirty = true;

        // /* chain downward */
        // if(val->val_type == VAL_TYPE_ARRAY) {
        //     for(uint32_t i = 0; i < ((value_array *)val)->num_elements && rc == VAL_OK; i++) {
        //         rc = mark_dirty(((value_array *)val)->elements[i]);
        //     }
        // }

        // if(val->val_type == VAL_TYPE_STRUCTURE) {
        //     for(uint32_t i = 0; i < ((value_structure *)val)->num_fields && rc == VAL_OK; i++) {
        //         rc = mark_dirty(((value_structure *)val)->fields[i].field_val);
        //     }
        // }
    } else {
        rc = VAL_ERR_NULL_PTR;
    }

    return rc;
}


value_status_t clear_dirty(value_base *val)
{
    value_status_t rc = VAL_OK;

    if(val) {
        val->dirty = false;

        /* chain downward */
        if(val->val_type == VAL_TYPE_ARRAY) {
            for(uint32_t i = 0; i < ((value_array *)val)->num_elements && rc == VAL_OK; i++) {
                rc = clear_dirty((value_base *)((value_array *)val)->elements[i]);
            }
        }

        if(val->val_type == VAL_TYPE_STRUCTURE) {
            for(uint32_t i = 0; i < ((value_structure *)val)->num_fields && rc == VAL_OK; i++) {
                rc = clear_dirty((value_base *)((value_structure *)val)->fields[i].field_val);
            }
        }
    } else {
        rc = VAL_ERR_NULL_PTR;
    }

    return rc;
}



value_status_t find_first_dirty(value_base *val, value_base *value_stack[], uint32_t stack_index)
{
    value_status_t rc =VAL_ERR_NOT_FOUND;

    if(!val->dirty) {
        /* can we go down? */
        if(val->val_type == VAL_TYPE_ARRAY) {
            if(stack_index < VALUE_STACK_DEPTH) {
                for(uint32_t i=0; i < ((value_array *)val)->num_elements && rc == VAL_ERR_NOT_FOUND; i++) {
                    rc = find_first_dirty((value_base *)((value_array *)val)->elements[i], value_stack, stack_index + 1);
                }
            } else {
                rc = VAL_ERR_OUT_OF_BOUNDS;
            }
        }

        if(val->val_type == VAL_TYPE_STRUCTURE) {
            if(stack_index < VALUE_STACK_DEPTH) {
                for(uint32_t i=0; i < ((value_structure *)val)->num_fields && rc == VAL_ERR_NOT_FOUND; i++) {
                    rc = find_first_dirty((value_base *)((value_structure *)val)->fields[i].field_val, value_stack, stack_index + 1);
                }
            } else {
                rc = VAL_ERR_OUT_OF_BOUNDS;
            }
        }
    } else {
        rc = VAL_OK;
    }

    if(rc == VAL_OK) {
        /* store this node since a dirty node was found along the way. */
        value_stack[stack_index] = val;
    } else {
        /* returning without finding anything, clear the entry */
        value_stack[stack_index] = NULL;
    }

    return rc;
}



value_status_t build_minimal_tag_data_request(device *plc, uint32_t last_id)
{
    value_status_t rc = VAL_OK;
    int32_t tag_rc = PLCTAG_STATUS_OK;
    uint8_t request[] = {0x55,0x03,0x20,0x6b,0x25,0x00,0x00,0x00,0x03,0x00,0x02,0x00,0x07,0x00,0x01,0x00};

    request[6] = last_id & 0xFF;
    request[7] = (last_id >> 8) & 0xFF;

    tag_rc = plc_tag_set_size(plc->device_raw_tag, (int)sizeof(request));
    if(tag_rc == PLCTAG_STATUS_OK) {
        for(size_t i=0; i < sizeof(request); i++) {
            plc_tag_set_uint8(plc->device_raw_tag, i, request[i]);
        }
    } else {
        info("WARN: Unable to set tag size!");
        rc = VAL_ERR_NO_RESOURCES;
    }

    return rc;
}



value_status_t parse_minimal_tag_data(device *plc, uint32_t *last_id)
{
    value_status_t rc = VAL_OK;
    int offset = 0;

    if(plc_tag_get_size(plc->device_raw_tag) >= 4) {
        uint8_t cip_cmd = plc_tag_get_uint8(plc->device_raw_tag, 0);
        uint8_t cip_unused1 = plc_tag_get_uint8(plc->device_raw_tag, 1);
        uint8_t cip_status = plc_tag_get_uint8(plc->device_raw_tag, 2);
        uint8_t cip_extended_status_size = plc_tag_get_uint8(plc->device_raw_tag, 3);

        offset = 4;

        if(cip_status == 0 || cip_status == 0x06) {
            while(rc = VAL_OK && offset < plc_tag_get_size(plc->device_raw_tag)) {
                uint32_t tag_instance_id = plc_tag_get_uint32(plc->device_raw_tag, offset);
                uint16_t tag_type_word = plc_tag_get_uint16(plc->device_raw_tag, offset + 4);
                uint16_t tag_size = plc_tag_get_uint16(plc->device_raw_tag, offset + 6);
                uint16_t tag_name_size = plc_tag_get_uint16(plc->device_raw_tag, offset + 8);
                offset += 10;

                *last_id = tag_instance_id + 1;

                /* get the tag name */
                char *tag_name = calloc(1, tag_name_size + 1);

                for(uint16_t i=0; i < tag_name_size; i++) {
                    tag_name[i] = plc_tag_get_uint8(plc->device_raw_tag, offset + i);
                }

                offset += tag_name_size;

                /* allocate a new top level value */
                uint16_t element_type = tag_type_word & 0xfff;
                bool is_system = (tag_type_word & 0x0100) ? true : false;
                uint8_t num_dims = (tag_type_word >> 13) & 0x03;
                bool is_structure = (tag_type_word & 0x8000) ? true : false;

                printf("Tag %s (%04x) element type %04x, is system %s, is array %s, number of dimensions %d, is structure %s, size in bytes %u.\n",
                        tag_name,
                        tag_instance_id,
                        element_type,
                        is_system ? "true" : "false",
                        num_dims > 0 ? "true" : "false",
                        num_dims, is_structure ? "true" : "false",
                        tag_size);
            }

        } else {
            info("WARN: CIP error %02x", cip_status);
            rc = VAL_ERR_CONNECTION_ERROR;
        }
    } else {
        rc = VAL_ERR_CONNECTION_ERROR;
    }

    return rc;
}

value_status_t get_minimal_symbol_instance_data(device *plc)
{
    value_status_t rc = VAL_OK;
    uint32_t last_tag_instance_id = 0;
    value_status_t last_rc;
    int32_t tag_rc = PLCTAG_STATUS_OK;

    do {
        rc = build_minimal_tag_data_request(plc, last_tag_instance_id);
        if(rc == VAL_OK || rc == VAL_ERR_PARTIAL) {
            last_rc = rc;

            tag_rc = plc_tag_write(plc->device_raw_tag, 5000);
            if(tag_rc == PLCTAG_STATUS_OK) {
                rc = parse_minimal_tag_data(plc, &last_tag_instance_id);
                if(rc == VAL_OK) {
                    if(last_rc == VAL_ERR_PARTIAL)
                }
            } else {
                rc = VAL_ERR_CONNECTION_ERROR;
            }
        }
    } while(rc = VAL_ERR_PARTIAL);

    return rc;
}


value_status_t get_tags(device *plc)
{
    value_status_t rc = VAL_OK;

    rc = get_minimal_symbol_instance_data(plc);

    return rc;
}