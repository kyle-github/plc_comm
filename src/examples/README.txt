This provides an overview of the examples.

example1.c
    This example shows the most ergonomic and simplest use of the API of the library.
    The library is responsible for handling almost everything but the translated data from the
    PLC is automatically copied into directly usable form for the application.

    The tag is retrived from the PLC synchronously.

    The following functions from the library are used:

    plc_comm_id_t plc_comm_conn_open(plc_comm_plc_type_t plc_type, const char *address, plc_comm_config_t *config, int32_t timeout_ms);
    plc_comm_id_t plc_comm_conn_do_request(plc_comm_id_t conn_id, const char *tag_name, int32_t num_elements, plc_comm_request_type_t op, void *translated_data, int32_t translated_data_size, plc_comm_config_t *config, int32_t timeout_ms);
    int32_t plc_comm_result_batch_get_status(plc_comm_id_t result_batch_id);
    int32_t plc_comm_conn_dispose(plc_comm_id_t conn_id, int32_t timeout_ms);


example2.c
    This example show how to trigger a request to the PLC in the background and continue to do
    processing in the application thread.  In this example, a simple wait is done to simulate
    work.

    The following functions from the library are used:

    plc_comm_id_t plc_comm_conn_open(plc_comm_plc_type_t plc_type, const char *address, plc_comm_config_t *config, int32_t timeout_ms);
    plc_comm_id_t plc_comm_conn_do_request(plc_comm_id_t conn_id, const char *tag_name, int32_t num_elements, plc_comm_request_type_t op, void *translated_data, int32_t translated_data_size, plc_comm_config_t *config, int32_t timeout_ms);
    int32_t plc_comm_result_batch_get_status(plc_comm_id_t result_batch_id);
    int32_t plc_comm_conn_dispose(plc_comm_id_t conn_id, int32_t timeout_ms);


example3.c
    This example shows how to use additional configuration data to change the configuration of a request.
    All entities exposed by the library have additional attributes that can be changed at time of
    creation or initialization and some can be changed at runtime.

    This example shows how to set up a locally-owned buffer for the raw data coming back from the PLC.

    The following functions from the library are used:

    plc_comm_id_t plc_comm_conn_open(plc_comm_plc_type_t plc_type, const char *address, plc_comm_config_t *config, int32_t timeout_ms);
    plc_comm_id_t plc_comm_conn_request_config_init(plc_comm_id_t conn_id, void *app_config_data, int32_t app_config_data_size);
    plc_comm_id_t plc_comm_conn_do_request(plc_comm_id_t conn_id, const char *tag_name, int32_t num_elements, plc_comm_request_type_t op, plc_comm_config_t *config, int32_t timeout_ms);
    int32_t plc_comm_result_batch_get_status(plc_comm_id_t result_batch_id);
    int32_t plc_comm_conn_dispose(plc_comm_id_t conn_id, int32_t timeout_ms);


example4.c
     This function shows an example of how to read a single tag where the library allocates all buffers.
     All memory and other resourcess are managed by the library.

    The following functions from the library are used:

    plc_comm_id_t plc_comm_conn_open(plc_comm_plc_type_t plc_type, const char *address, plc_comm_config_t *config, int32_t timeout_ms);
    plc_comm_id_t plc_comm_conn_do_request(plc_comm_id_t conn_id, const char *tag_name, int32_t num_elements, plc_comm_request_type_t op, void *translated_data, int32_t translated_data_size, plc_comm_config_t *config, int32_t timeout_ms);
    int32_t plc_comm_result_batch_get_status(plc_comm_id_t result_batch_id);
    int32_t plc_comm_result_batch_get_attr_int(plc_comm_id_t result_batch_id, int32_t result_indx, plc_comm_result_batch_attr_t attr, int32_t default_val);
    void *plc_comm_result_batch_get_attr_ptr(plc_comm_id_t result_batch_id, int32_t result_indx, plc_comm_result_batch_attr_t attr, void *default_val);
    int32_t plc_comm_conn_dispose(plc_comm_id_t conn_id, int32_t timeout_ms);


example5.c
    This example shows how to read multiple tags in one batch.  The batch as a whole is done 
    synchronously.

    The following functions from the library are used:

    plc_comm_id_t plc_comm_conn_open(plc_comm_plc_type_t plc_type, const char *address, plc_comm_config_t *config, int32_t timeout_ms);
    plc_comm_id_t plc_comm_request_batch_init(plc_comm_id_t conn_id, int32_t num_requests, plc_comm_id_t config_id); 
    int32_t plc_comm_request_init(plc_comm_id_t request_batch_id, int32_t request_indx, const char *tag_name, int32_t num_elements, plc_comm_request_op_t op, void *translated_data, int32_t translated_data_size, plc_comm_id_t config_id);
    plc_comm_id_t plc_comm_conn_do_request_batch(plc_comm_id_t conn_id, plc_comm_id_t request_batch_id, plc_comm_id_t config_id, int32_t timeout_ms);
    int32_t plc_comm_result_batch_get_status(plc_comm_id_t result_batch_id);
    int32_t plc_comm_conn_dispose(plc_comm_id_t conn_id, int32_t timeout_ms);

