#include "uploader.h"

//static void print_shard_info(storj_upload_state_t *state, int index) {
//    shard_tracker_t *shard = &state->shard[index];
//    shard_meta_t *shard_meta = state->shard[index].meta;
//    farmer_pointer_t *p = state->shard[index].pointer;
//
//    printf("\n================\n");
//
//    printf("Shard index [%d]\n", index);
//
//    printf("=== Shard Tracker ===\n");
//    printf("progress: %d\n", shard->progress);
//    printf("push_frame_request_count: %d\n", shard->push_frame_request_count);
//    printf("push_shard_request_count: %d\n", shard->push_shard_request_count);
//    printf("index: %d\n", shard->index);
//    printf("uploaded_size: %"PRIu64"\n", shard->uploaded_size);
//
//    printf("\n=== Shard Pointer ===\n");
//    printf("token: %s\n", p->token);
//    printf("farmer_user_agent: %s\n", p->farmer_user_agent);
//    printf("farmer_protocol: %s\n", p->farmer_protocol);
//    printf("farmer_address: %s\n", p->farmer_address);
//    printf("farmer_port: %s\n", p->farmer_port);
//    printf("farmer_node_id: %s\n", p->farmer_node_id);
//
//    printf("\n=== Shard Meta ===\n");
//    printf("hash: %s\n", shard_meta->hash);
//    printf("index: %d\n", shard_meta->index);
//    printf("size:  %"PRIu64"\n", shard_meta->size);
//    printf("is_parity: %d\n", shard_meta->is_parity);
//    for (int i = 0; i < STORJ_SHARD_CHALLENGES; i++ ) {
//        printf("Challenge [%d]: %s\n", i, (char *)shard_meta->challenges_as_str[i]);
//    }
//    for (int i = 0; i < STORJ_SHARD_CHALLENGES; i++ ) {
//        printf("Leaf [%d]: %s\n", i, (char *)shard_meta->tree[i]);
//    }
//
//    printf("================\n");
//
//    return;
//
//}

static uv_work_t *uv_work_new()
{
    uv_work_t *work = malloc(sizeof(uv_work_t));
    return work;
}

//static uv_work_t *frame_work_new(int *index, storj_upload_state_t *state)
//{
//    uv_work_t *work = uv_work_new();
//    if (!work) {
//        return NULL;
//    }
//
//    frame_request_t *req = malloc(sizeof(frame_request_t));
//    if (!req) {
//        return NULL;
//    }
//
//    req->http_options = state->env->http_options;
//    req->options = state->env->bridge_options;
//    req->upload_state = state;
//    req->error_status = 0;
//    req->status_code = 0;
//    req->log = state->log;
//
//    if (index != NULL) {
//        req->shard_meta_index = *index;
//        req->farmer_pointer = farmer_pointer_new();
//    }
//
//    work->data = req;
//
//    return work;
//}
//
//static uv_work_t *shard_meta_work_new(int index, storj_upload_state_t *state)
//{
//    uv_work_t *work = uv_work_new();
//    if (!work) {
//        return NULL;
//    }
//    frame_builder_t *req = malloc(sizeof(frame_builder_t));
//    if (!req) {
//        return NULL;
//    }
//    req->shard_meta = malloc(sizeof(shard_meta_t));
//    if (!req->shard_meta) {
//        return NULL;
//    }
//    req->upload_state = state;
//    req->log = state->log;
//
//    // make sure we switch between parity and data shards files.
//    // When using Reed solomon must also read from encrypted file
//    // rather than the original file for the data
//    if (index + 1 > state->total_data_shards) {
//        req->shard_file = state->parity_file;
//    } else if (state->rs) {
//        req->shard_file = state->encrypted_file;
//    } else {
//        req->shard_file = state->original_file;
//    }
//    // Reset shard index when using parity shards
//    req->shard_meta->index = (index + 1 > state->total_data_shards) ? index - state->total_data_shards: index;
//
//    // Position on shard_meta array
//    req->shard_meta_index = index;
//
//    req->error_status = 0;
//    req->status_code = 0;
//
//    work->data = req;
//
//    return work;
//}
//
//static storj_exchange_report_t *storj_exchange_report_new()
//{
//    storj_exchange_report_t *report = malloc(sizeof(storj_exchange_report_t));
//    if (!report) {
//        return NULL;
//    }
//    report->data_hash = NULL;
//    report->reporter_id = NULL;
//    report->farmer_id = NULL;
//    report->client_id = NULL;
//    report->message = NULL;
//
//    report->send_status = STORJ_REPORT_NOT_PREPARED; // not sent
//    report->start = 0;
//    report->end = 0;
//    report->code = 0;
//    report->send_count = 0;
//
//    return report;
//}
//
//static farmer_pointer_t *farmer_pointer_new()
//{
//    farmer_pointer_t *pointer = calloc(sizeof(farmer_pointer_t), sizeof(char));
//    if (!pointer) {
//        return NULL;
//    }
//    pointer->token = NULL;
//    pointer->farmer_user_agent = NULL;
//    pointer->farmer_protocol = NULL;
//    pointer->farmer_address = NULL;
//    pointer->farmer_port = NULL;
//    pointer->farmer_node_id = NULL;
//
//    return pointer;
//}
//
//static shard_meta_t *shard_meta_new()
//{
//    shard_meta_t *meta = calloc(sizeof(shard_meta_t), sizeof(char));
//    if (!meta) {
//        return NULL;
//    }
//    meta->hash = NULL;
//
//    return meta;
//}
//
//static storj_encryption_ctx_t *prepare_encryption_ctx(uint8_t *ctr, uint8_t *pass)
//{
//    storj_encryption_ctx_t *ctx = calloc(sizeof(storj_encryption_ctx_t), sizeof(char));
//    if (!ctx) {
//        return NULL;
//    }
//
//    ctx->ctx = calloc(sizeof(struct aes256_ctx), sizeof(char));
//    if (!ctx->ctx) {
//        return NULL;
//    }
//
//    ctx->encryption_ctr = calloc(AES_BLOCK_SIZE, sizeof(char));
//    if (!ctx->encryption_ctr) {
//        return NULL;
//    }
//
//    memcpy(ctx->encryption_ctr, ctr, AES_BLOCK_SIZE);
//
//    aes256_set_encrypt_key(ctx->ctx, pass);
//
//    return ctx;
//}
//
//static void shard_meta_cleanup(shard_meta_t *shard_meta)
//{
//    if (shard_meta->hash != NULL) {
//        free(shard_meta->hash);
//    }
//
//    free(shard_meta);
//}
//
//static void pointer_cleanup(farmer_pointer_t *farmer_pointer)
//{
//    if (farmer_pointer->token != NULL) {
//        free(farmer_pointer->token);
//    }
//
//    if (farmer_pointer->farmer_user_agent != NULL) {
//        free(farmer_pointer->farmer_user_agent);
//    }
//
//    if (farmer_pointer->farmer_protocol != NULL) {
//        free(farmer_pointer->farmer_protocol);
//    }
//
//    if (farmer_pointer->farmer_address != NULL) {
//        free(farmer_pointer->farmer_address);
//    }
//
//    if (farmer_pointer->farmer_port != NULL) {
//        free(farmer_pointer->farmer_port);
//    }
//
//    if (farmer_pointer->farmer_node_id != NULL) {
//        free(farmer_pointer->farmer_node_id);
//    }
//
//    free(farmer_pointer);
//}

static void cleanup_work(uv_work_t *work, int status)
{
    storj_upload_state_t *state = work->data;

    cleanup_state(state);
    free(work);
}

static void cleanup_state(storj_upload_state_t *state)
{
    if (state->original_file) {
        fclose(state->original_file);
    }

    if (state->encrypted_file_name) {
        free((char *)state->encrypted_file_name);
    }

    state->finished_cb(state->error_status, state->info, state->handle);

    free(state);
}

//static void free_encryption_ctx(storj_encryption_ctx_t *ctx)
//{
//    if (ctx->encryption_ctr) {
//        free(ctx->encryption_ctr);
//    }
//
//    if (ctx->encryption_key) {
//        free(ctx->encryption_key);
//    }
//
//    if (ctx->ctx) {
//        free(ctx->ctx);
//    }
//
//    free(ctx);
//}
//
//static void after_create_bucket_entry(uv_work_t *work, int status)
//{
//    post_to_bucket_request_t *req = work->data;
//    storj_upload_state_t *state = req->upload_state;
//
//    state->pending_work_count -= 1;
//
//    if (status == UV_ECANCELED) {
//        state->add_bucket_entry_count = 0;
//        state->creating_bucket_entry = false;
//        goto clean_variables;
//    }
//
//    state->add_bucket_entry_count += 1;
//    state->creating_bucket_entry = false;
//
//    if (req->error_status) {
//        state->error_status = req->error_status;
//        goto clean_variables;
//    }
//
//    // Check if we got a 200 status and token
//    if (req->status_code == 200 || req->status_code == 201) {
//
//        req->log->info(state->env->log_options, state->handle,
//                       "Successfully Added bucket entry");
//
//        req->log->debug(state->env->log_options, state->handle,
//                        "fn[after_create_bucket_entry] - JSON Response: %s", json_object_to_json_string(req->response));
//
//        state->add_bucket_entry_count = 0;
//        state->completed_upload = true;
//
//        state->info = malloc(sizeof(storj_file_meta_t));
//        state->info->created = NULL;
//        state->info->filename = state->file_name;
//        state->info->mimetype = NULL;
//        state->info->erasure = NULL;
//        state->info->size = state->file_size;
//        state->info->hmac = state->hmac_id;
//        state->info->id = NULL;
//        state->info->bucket_id = state->bucket_id;
//        state->info->decrypted = true;
//        state->info->index = strdup(state->index);
//
//        struct json_object *file_id_value = NULL;
//        struct json_object *created_value = NULL;
//        struct json_object *mimetype_value = NULL;
//
//        if (json_object_object_get_ex(req->response, "id", &file_id_value)) {
//        	state->info->id = strdup((char *)json_object_get_string(file_id_value));
//        }
//        if (json_object_object_get_ex(req->response, "created", &created_value)) {
//        	state->info->created = strdup((char *)json_object_get_string(created_value));
//        }
//        if (json_object_object_get_ex(req->response, "mimetype", &mimetype_value)) {
//        	state->info->mimetype = strdup((char *)json_object_get_string(mimetype_value));
//        }
//
//    } else if (state->add_bucket_entry_count == 6) {
//        state->error_status = STORJ_BRIDGE_REQUEST_ERROR;
//    }
//
//clean_variables:
//    queue_next_work(state);
//    if (req->response) {
//        json_object_put(req->response);
//    }
//    free(req);
//    free(work);
//}
//
//static int prepare_bucket_entry_hmac(storj_upload_state_t *state)
//{
//    struct hmac_sha512_ctx hmac_ctx;
//    hmac_sha512_set_key(&hmac_ctx, SHA256_DIGEST_SIZE, state->encryption_key);
//
//    for (int i = 0; i < state->total_shards; i++) {
//
//        shard_tracker_t *shard = &state->shard[i];
//
//        if (!shard->meta ||
//            !shard->meta->hash ||
//            strlen(shard->meta->hash) != RIPEMD160_DIGEST_SIZE * 2) {
//            return 1;
//        }
//
//        struct base16_decode_ctx base16_ctx;
//        base16_decode_init(&base16_ctx);
//
//        size_t decode_len = 0;
//        uint8_t hash[RIPEMD160_DIGEST_SIZE];
//        if (!base16_decode_update(&base16_ctx, &decode_len, hash,
//                                  RIPEMD160_DIGEST_SIZE * 2,
//                                  (uint8_t *)shard->meta->hash)) {
//            return 1;
//
//        }
//        if (!base16_decode_final(&base16_ctx) ||
//            decode_len != RIPEMD160_DIGEST_SIZE) {
//            return 1;
//        }
//        hmac_sha512_update(&hmac_ctx, RIPEMD160_DIGEST_SIZE, hash);
//    }
//
//    uint8_t digest_raw[SHA512_DIGEST_SIZE];
//    hmac_sha512_digest(&hmac_ctx, SHA512_DIGEST_SIZE, digest_raw);
//
//    size_t digest_len = BASE16_ENCODE_LENGTH(SHA512_DIGEST_SIZE);
//    state->hmac_id = calloc(digest_len + 1, sizeof(char));
//    if (!state->hmac_id) {
//        return 1;
//    }
//
//    base16_encode_update((uint8_t *)state->hmac_id, SHA512_DIGEST_SIZE, digest_raw);
//
//    return 0;
//}
//
//static void free_push_shard_work(uv_handle_t *progress_handle)
//{
//    uv_work_t *work = progress_handle->data;
//    push_shard_request_t *req = work->data;
//
//    if (req) {
//        free(req);
//    }
//
//    if (work) {
//        free(work);
//    }
//}
//
//static void after_push_shard(uv_work_t *work, int status)
//{
//    push_shard_request_t *req = work->data;
//    storj_upload_state_t *state = req->upload_state;
//    uv_handle_t *progress_handle = (uv_handle_t *) &req->progress_handle;
//    shard_tracker_t *shard = &state->shard[req->shard_meta_index];
//
//    // free the upload progress
//    free(progress_handle->data);
//
//    // assign work so that we can free after progress_handle is closed
//    progress_handle->data = work;
//
//    state->pending_work_count -= 1;
//
//    if (status == UV_ECANCELED) {
//        shard->push_shard_request_count = 0;
//        shard->progress = AWAITING_PUSH_FRAME;
//        shard->report->send_status = STORJ_REPORT_NOT_PREPARED;
//        goto clean_variables;
//    }
//
//    // Update times on exchange report
//    shard->report->start = req->start;
//    shard->report->end = req->end;
//
//    // Check if we got a 200 status and token
//    if (!req->error_status &&
//        (req->status_code == 200 ||
//         req->status_code == 201 ||
//         req->status_code == 304)) {
//
//        req->log->info(state->env->log_options, state->handle,
//                       "Successfully transferred shard index %d",
//                       req->shard_meta_index);
//
//        shard->progress = COMPLETED_PUSH_SHARD;
//        state->completed_shards += 1;
//        shard->push_shard_request_count = 0;
//
//        // Update the uploaded size outside of the progress async handle
//        shard->uploaded_size = shard->meta->size;
//
//        // Update the exchange report with success
//        shard->report->code = STORJ_REPORT_SUCCESS;
//        shard->report->message = STORJ_REPORT_SHARD_UPLOADED;
//        shard->report->send_status = STORJ_REPORT_AWAITING_SEND;
//
//    } else if (!state->canceled){
//
//        // Update the exchange report with failure
//        shard->report->code = STORJ_REPORT_FAILURE;
//        shard->report->message = STORJ_REPORT_UPLOAD_ERROR;
//        shard->report->send_status = STORJ_REPORT_AWAITING_SEND;
//
//        if (shard->push_shard_request_count == 6) {
//
//            req->log->error(state->env->log_options, state->handle,
//                            "Failed to push shard %d\n", req->shard_meta_index);
//
//            state->error_status = STORJ_FARMER_REQUEST_ERROR;
//        } else {
//            req->log->warn(state->env->log_options, state->handle,
//                           "Failed to push shard %d... Retrying...",
//                           req->shard_meta_index);
//
//            // We go back to getting a new pointer instead of retrying push with same pointer
//            shard->progress = AWAITING_PUSH_FRAME;
//            shard->push_shard_request_count += 1;
//
//            // Add pointer to exclude for future calls
//            if (state->exclude == NULL) {
//                state->exclude = calloc(strlen(shard->pointer->farmer_node_id) + 1, sizeof(char));
//                if (!state->exclude) {
//                    state->error_status = STORJ_MEMORY_ERROR;
//                    return;
//                }
//                strcpy(state->exclude, shard->pointer->farmer_node_id);
//            } else {
//                int new_len = strlen(state->exclude) + strlen(shard->pointer->farmer_node_id) + 1;
//                state->exclude = realloc(state->exclude, new_len + 1);
//                strcat(state->exclude, ",");
//                strcat(state->exclude, shard->pointer->farmer_node_id);
//                state->exclude[new_len] = '\0';
//            }
//        }
//    }
//
//clean_variables:
//    queue_next_work(state);
//    // close the async progress handle
//    uv_close(progress_handle, free_push_shard_work);
//}
//
//static void push_shard(uv_work_t *work)
//{
//    push_shard_request_t *req = work->data;
//    storj_upload_state_t *state = req->upload_state;
//    shard_tracker_t *shard = &state->shard[req->shard_meta_index];
//
//    req->log->info(state->env->log_options, state->handle,
//                   "Transfering Shard index %d... (retry: %d)",
//                   req->shard_meta_index,
//                   state->shard[req->shard_meta_index].push_shard_request_count);
//
//    int status_code = 0;
//    int read_code = 0;
//
//    req->start = get_time_milliseconds();
//
//    uint64_t file_position = req->shard_index * state->shard_size;
//
//    storj_encryption_ctx_t *encryption_ctx = NULL;
//    if (!state->rs) {
//        // Initialize the encryption context
//        encryption_ctx = prepare_encryption_ctx(state->encryption_ctr,
//                                                                        state->encryption_key);
//        if (!encryption_ctx) {
//            state->error_status = STORJ_MEMORY_ERROR;
//            goto clean_variables;
//        }
//        // Increment the iv to proper placement because we may be reading from the middle of the file
//        increment_ctr_aes_iv(encryption_ctx->encryption_ctr, req->shard_meta_index * state->shard_size);
//    }
//
//    int req_status = put_shard(req->http_options,
//                               shard->pointer->farmer_node_id,
//                               "http",
//                               shard->pointer->farmer_address,
//                               atoi(shard->pointer->farmer_port),
//                               shard->meta->hash,
//                               shard->meta->size,
//                               req->shard_file,
//                               file_position,
//                               encryption_ctx,
//                               shard->pointer->token,
//                               &status_code,
//                               &read_code,
//                               &req->progress_handle,
//                               req->canceled);
//
//    if (read_code != 0) {
//        req->log->error(state->env->log_options, state->handle,
//                        "Put shard read error: %i", read_code);
//    }
//
//    if (req_status) {
//        req->error_status = req_status;
//        req->log->error(state->env->log_options, state->handle,
//                        "Put shard request error code: %i", req_status);
//    }
//
//    req->end = get_time_milliseconds();
//
//    req->status_code = status_code;
//
//clean_variables:
//    if (encryption_ctx) {
//        free_encryption_ctx(encryption_ctx);
//    }
//}
//
//static void progress_put_shard(uv_async_t* async)
//{
//
//    shard_upload_progress_t *progress = async->data;
//
//    storj_upload_state_t *state = progress->state;
//
//    state->shard[progress->pointer_index].uploaded_size = progress->bytes;
//
//    uint64_t uploaded_bytes = 0;
//    uint64_t total_bytes = 0;
//
//    for (int i = 0; i < state->total_shards; i++) {
//
//        shard_tracker_t *shard = &state->shard[i];
//
//        uploaded_bytes += shard->uploaded_size;
//        total_bytes += shard->meta->size;
//    }
//
//    double total_progress = (double)uploaded_bytes / (double)total_bytes;
//
//    if (state->progress_finished) {
//        return;
//    }
//
//    if (uploaded_bytes == total_bytes) {
//        state->progress_finished = true;
//    }
//
//    state->progress_cb(total_progress,
//                       uploaded_bytes,
//                       total_bytes,
//                       state->handle);
//
//
//}
//
//static void queue_push_shard(storj_upload_state_t *state, int index)
//{
//    uv_work_t *work = uv_work_new();
//    if (!work) {
//        state->error_status = STORJ_MEMORY_ERROR;
//        return;
//    }
//
//    push_shard_request_t *req = malloc(sizeof(push_shard_request_t));
//    if (!req) {
//        state->error_status = STORJ_MEMORY_ERROR;
//        return;
//    }
//
//    req->http_options = state->env->http_options;
//    req->options = state->env->bridge_options;
//    req->upload_state = state;
//    req->error_status = 0;
//    req->log = state->log;
//
//    // Reset shard index when using parity shards
//    req->shard_index = (index + 1 > state->total_data_shards) ? index - state->total_data_shards: index;
//
//    // make sure we switch between parity and data shards files.
//    // When using Reed solomon must also read from encrypted file
//    // rather than the original file for the data
//    if (index + 1 > state->total_data_shards) {
//        req->shard_file = state->parity_file;
//    } else if (state->rs) {
//        req->shard_file = state->encrypted_file;
//    } else {
//        req->shard_file = state->original_file;
//    }
//
//    // Position on shard_meta array
//    req->shard_meta_index = index;
//
//    req->status_code = 0;
//
//    req->canceled = &state->canceled;
//
//    // setup upload progress reporting
//    shard_upload_progress_t *progress =
//        malloc(sizeof(shard_upload_progress_t));
//
//    if (!progress) {
//        state->error_status = STORJ_MEMORY_ERROR;
//        return;
//    }
//
//    progress->pointer_index = index;
//    progress->bytes = 0;
//    progress->state = state;
//
//    req->progress_handle.data = progress;
//
//    uv_async_init(state->env->loop, &req->progress_handle,
//                  progress_put_shard);
//
//    work->data = req;
//
//    state->pending_work_count += 1;
//    int status = uv_queue_work(state->env->loop, (uv_work_t*) work,
//                               push_shard, after_push_shard);
//
//    if (status) {
//        state->error_status = STORJ_QUEUE_ERROR;
//        return;
//    }
//
//    state->shard[index].progress = PUSHING_SHARD;
//
//    if (state->shard[index].report->farmer_id != NULL) {
//        free(state->shard[index].report);
//        state->shard[index].report = storj_exchange_report_new();
//    }
//
//    if (!state->shard[index].report) {
//        state->error_status = STORJ_MEMORY_ERROR;
//        return;
//    }
//
//    // setup the exchange report
//    storj_exchange_report_t *report = state->shard[index].report;
//    report->data_hash = state->shard[index].meta->hash;
//    report->reporter_id = (char *)state->env->bridge_options->user;
//    report->farmer_id = state->shard[index].pointer->farmer_node_id;
//    report->client_id = (char *)state->env->bridge_options->user;
//    report->pointer_index = index;
//    report->start = 0;
//    report->end = 0;
//    report->code = 0;
//    report->message = NULL;
//    report->send_status = 0; // not sent
//    report->send_count = 0;
//
//    state->shard[index].work = work;
//}
//
//static void after_push_frame(uv_work_t *work, int status)
//{
//    frame_request_t *req = work->data;
//    storj_upload_state_t *state = req->upload_state;
//    farmer_pointer_t *pointer = req->farmer_pointer;
//
//    state->pending_work_count -= 1;
//
//    if (status == UV_ECANCELED) {
//        state->shard[req->shard_meta_index].push_frame_request_count = 0;
//        state->shard[req->shard_meta_index].progress = AWAITING_PUSH_FRAME;
//        goto clean_variables;
//    }
//
//    // Increment request count every request for retry counts
//    state->shard[req->shard_meta_index].push_frame_request_count += 1;
//
//    if (req->status_code == 429 || req->status_code == 420) {
//
//        state->error_status = STORJ_BRIDGE_RATE_ERROR;
//
//    } else if ((req->status_code == 200 || req->status_code == 201) &&
//        pointer->token != NULL) {
//        // Check if we got a 200 status and token
//
//        // Reset for if we need to get a new pointer later
//        state->shard[req->shard_meta_index].push_frame_request_count = 0;
//        state->shard[req->shard_meta_index].progress = AWAITING_PUSH_SHARD;
//
//        farmer_pointer_t *p = state->shard[req->shard_meta_index].pointer;
//
//        // Add token to shard[].pointer
//        p->token = calloc(strlen(pointer->token) + 1, sizeof(char));
//        if (!p->token) {
//            state->error_status = STORJ_MEMORY_ERROR;
//            goto clean_variables;
//        }
//        memcpy(p->token, pointer->token, strlen(pointer->token));
//
//        // Add farmer_user_agent to shard[].pointer
//        p->farmer_user_agent = calloc(strlen(pointer->farmer_user_agent) + 1,
//                                      sizeof(char));
//        if (!p->farmer_user_agent) {
//            state->error_status = STORJ_MEMORY_ERROR;
//            goto clean_variables;
//        }
//        memcpy(p->farmer_user_agent, pointer->farmer_user_agent,
//               strlen(pointer->farmer_user_agent));
//
//        // Add farmer_address to shard[].pointer
//        p->farmer_address = calloc(strlen(pointer->farmer_address) + 1,
//                                   sizeof(char));
//        if (!p->farmer_address) {
//            state->error_status = STORJ_MEMORY_ERROR;
//            goto clean_variables;
//        }
//        memcpy(p->farmer_address, pointer->farmer_address,
//               strlen(pointer->farmer_address));
//
//        // Add farmer_port to shard[].pointer
//        p->farmer_port = calloc(strlen(pointer->farmer_port) + 1, sizeof(char));
//        if (!p->farmer_port) {
//            state->error_status = STORJ_MEMORY_ERROR;
//            goto clean_variables;
//        }
//        memcpy(p->farmer_port, pointer->farmer_port,
//               strlen(pointer->farmer_port));
//
//        // Add farmer_protocol to shard[].pointer
//        p->farmer_protocol = calloc(strlen(pointer->farmer_protocol) + 1,
//                                    sizeof(char));
//        if (!p->farmer_protocol) {
//            state->error_status = STORJ_MEMORY_ERROR;
//            goto clean_variables;
//        }
//        memcpy(p->farmer_protocol, pointer->farmer_protocol,
//               strlen(pointer->farmer_protocol));
//
//        // Add farmer_node_id to shard[].pointer
//        p->farmer_node_id = calloc(strlen(pointer->farmer_node_id) + 1,
//                                   sizeof(char));
//        if (!p->farmer_node_id) {
//            state->error_status = STORJ_MEMORY_ERROR;
//            goto clean_variables;
//        }
//        memcpy(p->farmer_node_id, pointer->farmer_node_id,
//               strlen(pointer->farmer_node_id));
//
//        state->log->info(
//            state->env->log_options,
//            state->handle,
//            "Contract negotiated with: "
//            "{ "
//            "\"userAgent: \"%s\", "
//            "\"protocol:\" \"%s\", "
//            "\"port\": \"%s\", "
//            "\"nodeID\": \"%s\""
//            "}",
//            p->farmer_user_agent,
//            p->farmer_protocol,
//            p->farmer_port,
//            p->farmer_node_id
//        );
//
//    } else if (state->shard[req->shard_meta_index].push_frame_request_count ==
//               STORJ_MAX_PUSH_FRAME_COUNT) {
//        state->error_status = STORJ_BRIDGE_OFFER_ERROR;
//    } else {
//        state->shard[req->shard_meta_index].progress = AWAITING_PUSH_FRAME;
//    }
//
//clean_variables:
//    queue_next_work(state);
//    if (pointer) {
//        pointer_cleanup(pointer);
//    }
//
//    free(req);
//    free(work);
//}
//
//static void push_frame(uv_work_t *work)
//{
//    frame_request_t *req = work->data;
//    storj_upload_state_t *state = req->upload_state;
//    shard_meta_t *shard_meta = state->shard[req->shard_meta_index].meta;
//
//    req->log->info(state->env->log_options, state->handle,
//                   "Pushing frame for shard index %d... (retry: %d)",
//                   req->shard_meta_index,
//                   state->shard[req->shard_meta_index].push_frame_request_count);
//
//    char resource[strlen(state->frame_id) + 9];
//    memset(resource, '\0', strlen(state->frame_id) + 9);
//    strcpy(resource, "/frames/");
//    strcat(resource, state->frame_id);
//
//    // Prepare the body
//    struct json_object *body = json_object_new_object();
//
//    // Add shard hash
//    json_object *shard_hash = json_object_new_string(shard_meta->hash);
//    json_object_object_add(body, "hash", shard_hash);
//
//    // Add shard size
//    json_object *shard_size = json_object_new_int64(shard_meta->size);
//    json_object_object_add(body, "size", shard_size);
//
//    // Add shard index
//    json_object *shard_index = json_object_new_int(req->shard_meta_index);
//    json_object_object_add(body, "index", shard_index);
//
//    json_object *parity_shard = NULL;
//    if (req->shard_meta_index + 1 > state->total_data_shards) {
//        parity_shard = json_object_new_boolean(true);
//    } else {
//        parity_shard = json_object_new_boolean(false);
//    }
//    json_object_object_add(body, "parity", parity_shard);
//
//    // Add challenges
//    json_object *challenges = json_object_new_array();
//    for (int i = 0; i < STORJ_SHARD_CHALLENGES; i++ ) {
//        json_object_array_add(challenges,
//                              json_object_new_string(
//                                  (char *)shard_meta->challenges_as_str[i]));
//    }
//    json_object_object_add(body, "challenges", challenges);
//
//    // Add Tree
//    json_object *tree = json_object_new_array();
//    for (int i = 0; i < STORJ_SHARD_CHALLENGES; i++ ) {
//        json_object_array_add(tree,
//                              json_object_new_string(
//                                  (char *)shard_meta->tree[i]));
//    }
//    json_object_object_add(body, "tree", tree);
//
//    // Add exclude (Don't try to upload to farmers that have failed before)
//    json_object *exclude = json_object_new_array();
//    if (state->exclude) {
//        char *exclude_list = calloc(strlen(state->exclude) + 1, sizeof(char));
//        if (!exclude_list) {
//            req->error_status = STORJ_MEMORY_ERROR;
//            goto clean_variables;
//        }
//        strcpy(exclude_list, state->exclude);
//
//        char *node_id = strtok(exclude_list, ",");
//        while (node_id != NULL) {
//            json_object_array_add(exclude, json_object_new_string(node_id));
//            node_id = strtok (NULL, ",");
//        }
//        free(exclude_list);
//    }
//
//    json_object_object_add(body, "exclude", exclude);
//
//    req->log->debug(state->env->log_options, state->handle,
//                    "fn[push_frame] - JSON body: %s", json_object_to_json_string(body));
//
//    int status_code;
//    struct json_object *response = NULL;
//    int request_status = fetch_json(req->http_options,
//                                    req->options,
//                                    "PUT",
//                                    resource,
//                                    body,
//                                    true,
//                                    &response,
//                                    &status_code);
//
//    req->log->debug(state->env->log_options, state->handle,
//                    "fn[push_frame] - JSON Response: %s",
//                    json_object_to_json_string(response));
//
//    if (request_status) {
//        req->log->warn(state->env->log_options, state->handle,
//                       "Push frame error: %i", request_status);
//        req->error_status = STORJ_BRIDGE_REQUEST_ERROR;
//        goto clean_variables;
//    }
//
//    struct json_object *obj_token;
//    if (!json_object_object_get_ex(response, "token", &obj_token)) {
//        req->error_status = STORJ_BRIDGE_JSON_ERROR;
//        goto clean_variables;
//    }
//
//    struct json_object *obj_farmer;
//    if (!json_object_object_get_ex(response, "farmer", &obj_farmer)) {
//        req->error_status = STORJ_BRIDGE_JSON_ERROR;
//        goto clean_variables;
//    }
//
//    struct json_object *obj_farmer_address;
//    if (!json_object_object_get_ex(obj_farmer, "address",
//                                   &obj_farmer_address)) {
//
//        req->error_status = STORJ_BRIDGE_JSON_ERROR;
//        goto clean_variables;
//    }
//
//    struct json_object *obj_farmer_port;
//    if (!json_object_object_get_ex(obj_farmer, "port", &obj_farmer_port)) {
//        req->error_status = STORJ_BRIDGE_JSON_ERROR;
//        goto clean_variables;
//    }
//
//    struct json_object *obj_farmer_user_agent;
//    if (!json_object_object_get_ex(obj_farmer, "userAgent",
//                                   &obj_farmer_user_agent)) {
//
//        req->error_status = STORJ_BRIDGE_JSON_ERROR;
//        goto clean_variables;
//    }
//
//    struct json_object *obj_farmer_protocol;
//    if (!json_object_object_get_ex(obj_farmer, "protocol",
//                                   &obj_farmer_protocol)) {
//
//        req->error_status = STORJ_BRIDGE_JSON_ERROR;
//        goto clean_variables;
//    }
//
//    struct json_object *obj_farmer_node_id;
//    if (!json_object_object_get_ex(obj_farmer, "nodeID",
//                                   &obj_farmer_node_id)) {
//
//        req->error_status = STORJ_BRIDGE_JSON_ERROR;
//        goto clean_variables;
//    }
//
//    if (!json_object_is_type(obj_token, json_type_string)) {
//
//        req->error_status = STORJ_BRIDGE_JSON_ERROR;
//        goto clean_variables;
//    }
//
//    // Token
//    char *token = (char *)json_object_get_string(obj_token);
//    req->farmer_pointer->token = calloc(strlen(token) + 1, sizeof(char));
//    if (!req->farmer_pointer->token) {
//        req->error_status = STORJ_MEMORY_ERROR;
//        goto clean_variables;
//    }
//    memcpy(req->farmer_pointer->token, token, strlen(token));
//
//    // Farmer user agent
//    char *farmer_user_agent =
//        (char *)json_object_get_string(obj_farmer_user_agent);
//    req->farmer_pointer->farmer_user_agent =
//        calloc(strlen(farmer_user_agent) + 1, sizeof(char));
//    if (!req->farmer_pointer->farmer_user_agent) {
//        req->error_status = STORJ_MEMORY_ERROR;
//        goto clean_variables;
//    }
//    memcpy(req->farmer_pointer->farmer_user_agent,
//           farmer_user_agent,
//           strlen(farmer_user_agent));
//
//    // Farmer protocol
//    char *farmer_protocol = (char *)json_object_get_string(obj_farmer_protocol);
//    req->farmer_pointer->farmer_protocol =
//        calloc(strlen(farmer_protocol) + 1, sizeof(char));
//    if (!req->farmer_pointer->farmer_protocol) {
//        req->error_status = STORJ_MEMORY_ERROR;
//        goto clean_variables;
//    }
//    memcpy(req->farmer_pointer->farmer_protocol,
//           farmer_protocol,
//           strlen(farmer_protocol));
//
//    // Farmer address
//    char *farmer_address = (char *)json_object_get_string(obj_farmer_address);
//    req->farmer_pointer->farmer_address =
//        calloc(strlen(farmer_address) + 1, sizeof(char));
//    if (!req->farmer_pointer->farmer_address) {
//        req->error_status = STORJ_MEMORY_ERROR;
//        goto clean_variables;
//    }
//    memcpy(req->farmer_pointer->farmer_address,
//           farmer_address,
//           strlen(farmer_address));
//
//    // Farmer port
//    char *farmer_port = (char *)json_object_get_string(obj_farmer_port);
//    req->farmer_pointer->farmer_port = calloc(strlen(farmer_port) + 1, sizeof(char));
//    if (!req->farmer_pointer->farmer_port) {
//        req->error_status = STORJ_MEMORY_ERROR;
//        goto clean_variables;
//    }
//    memcpy(req->farmer_pointer->farmer_port, farmer_port, strlen(farmer_port));
//
//    // Farmer node id
//    char *farmer_node_id = (char *)json_object_get_string(obj_farmer_node_id);
//    req->farmer_pointer->farmer_node_id =
//        calloc(strlen(farmer_node_id) + 1, sizeof(char));
//    if (!req->farmer_pointer->farmer_node_id) {
//        req->error_status = STORJ_MEMORY_ERROR;
//        goto clean_variables;
//    }
//    memcpy(req->farmer_pointer->farmer_node_id,
//           farmer_node_id,
//           strlen(farmer_node_id));
//
//    // Status code
//    req->status_code = status_code;
//
//clean_variables:
//    if (response) {
//        json_object_put(response);
//    }
//    if (body) {
//        json_object_put(body);
//    }
//}
//
//static void queue_push_frame(storj_upload_state_t *state, int index)
//{
//    if (state->shard[index].pointer->token != NULL) {
//        pointer_cleanup(state->shard[index].pointer);
//        state->shard[index].pointer = farmer_pointer_new();
//        if (!state->shard[index].pointer) {
//            state->error_status = STORJ_MEMORY_ERROR;
//            return;
//        }
//    }
//
//    uv_work_t *shard_work = frame_work_new(&index, state);
//    if (!shard_work) {
//        state->error_status = STORJ_MEMORY_ERROR;
//        return;
//    }
//
//    state->pending_work_count += 1;
//    int status = uv_queue_work(state->env->loop, (uv_work_t*) shard_work,
//                               push_frame, after_push_frame);
//    if (status) {
//        state->error_status = STORJ_QUEUE_ERROR;
//        return;
//    }
//
//    state->shard[index].progress = PUSHING_FRAME;
//}
//
//static void after_prepare_frame(uv_work_t *work, int status)
//{
//    frame_builder_t *req = work->data;
//    shard_meta_t *shard_meta = req->shard_meta;
//    storj_upload_state_t *state = req->upload_state;
//
//    state->pending_work_count -= 1;
//
//    if (status == UV_ECANCELED) {
//        state->shard[shard_meta->index].progress = AWAITING_PREPARE_FRAME;
//        goto clean_variables;
//    }
//
//    if (req->error_status) {
//        state->error_status = req->error_status;
//        goto clean_variables;
//    }
//
//    /* set the shard_meta to a struct array in the state for later use. */
//
//    // Add Hash
//    state->shard[req->shard_meta_index].meta->hash =
//        calloc(RIPEMD160_DIGEST_SIZE * 2 + 1, sizeof(char));
//
//    if (!state->shard[req->shard_meta_index].meta->hash) {
//        state->error_status = STORJ_MEMORY_ERROR;
//        goto clean_variables;
//    }
//
//    memcpy(state->shard[req->shard_meta_index].meta->hash,
//           shard_meta->hash,
//           RIPEMD160_DIGEST_SIZE * 2);
//
//    req->log->info(state->env->log_options, state->handle,
//                  "Shard (%d) hash: %s", req->shard_meta_index,
//                  state->shard[req->shard_meta_index].meta->hash);
//
//    // Add challenges_as_str
//    state->log->debug(state->env->log_options, state->handle,
//                      "Challenges for shard index %d",
//                      req->shard_meta_index);
//
//    for (int i = 0; i < STORJ_SHARD_CHALLENGES; i++ ) {
//        memcpy(state->shard[req->shard_meta_index].meta->challenges_as_str[i],
//               shard_meta->challenges_as_str[i],
//               64);
//
//        state->log->debug(state->env->log_options, state->handle,
//                          "Shard %d Challenge [%d]: %s",
//                        req->shard_meta_index,
//                          i,
//                          state->shard[req->shard_meta_index].meta->challenges_as_str[i]);
//    }
//
//    // Add Merkle Tree leaves.
//    state->log->debug(state->env->log_options, state->handle,
//                      "Tree for shard index %d",
//                      req->shard_meta_index);
//
//    for (int i = 0; i < STORJ_SHARD_CHALLENGES; i++ ) {
//        memcpy(state->shard[req->shard_meta_index].meta->tree[i],
//               shard_meta->tree[i],
//               40);
//
//        state->log->debug(state->env->log_options, state->handle,
//                          "Shard %d Leaf [%d]: %s", req->shard_meta_index, i,
//                          state->shard[req->shard_meta_index].meta->tree[i]);
//    }
//
//    // Add index
//    state->shard[req->shard_meta_index].meta->index = shard_meta->index;
//
//    // Add size
//    state->shard[req->shard_meta_index].meta->size = shard_meta->size;
//
//    state->log->info(state->env->log_options, state->handle,
//                     "Successfully created frame for shard index %d",
//                     req->shard_meta_index);
//
//    state->shard[req->shard_meta_index].progress = AWAITING_PUSH_FRAME;
//
//clean_variables:
//    queue_next_work(state);
//    if (shard_meta) {
//        shard_meta_cleanup(shard_meta);
//    }
//
//    free(req);
//    free(work);
//}
//
//static void prepare_frame(uv_work_t *work)
//{
//    frame_builder_t *req = work->data;
//    shard_meta_t *shard_meta = req->shard_meta;
//    storj_upload_state_t *state = req->upload_state;
//
//    // Set the challenges
//    uint8_t buff[32];
//    for (int i = 0; i < STORJ_SHARD_CHALLENGES; i++ ) {
//        memset_zero(buff, 32);
//
//        random_buffer(buff, 32);
//        memcpy(shard_meta->challenges[i], buff, 32);
//
//        // Convert the uint8_t challenges to character arrays
//        char *challenge_as_str = hex2str(32, buff);
//        if (!challenge_as_str) {
//            req->error_status = STORJ_MEMORY_ERROR;
//            goto clean_variables;
//        }
//        memcpy(shard_meta->challenges_as_str[i], challenge_as_str, strlen(challenge_as_str));
//        free(challenge_as_str);
//    }
//
//    // Hash of the shard_data
//    shard_meta->hash = calloc(RIPEMD160_DIGEST_SIZE*2 + 2, sizeof(char));
//    if (!shard_meta->hash) {
//        req->error_status = STORJ_MEMORY_ERROR;
//        goto clean_variables;
//    }
//
//    req->log->info(state->env->log_options, state->handle,
//                   "Creating frame for shard index %d",
//                   req->shard_meta_index);
//
//    // Sha256 of encrypted data for calculating shard has
//    uint8_t prehash_sha256[SHA256_DIGEST_SIZE];
//
//    // Initialize context for sha256 of encrypted data
//    struct sha256_ctx shard_hash_ctx;
//    sha256_init(&shard_hash_ctx);
//
//    // Calculate the merkle tree with challenges
//    struct sha256_ctx first_sha256_for_leaf[STORJ_SHARD_CHALLENGES];
//    for (int i = 0; i < STORJ_SHARD_CHALLENGES; i++ ) {
//        sha256_init(&first_sha256_for_leaf[i]);
//        sha256_update(&first_sha256_for_leaf[i], 32, (uint8_t *)&shard_meta->challenges[i]);
//    }
//
//    storj_encryption_ctx_t *encryption_ctx = NULL;
//    if (!state->rs) {
//        // Initialize the encryption context
//        encryption_ctx = prepare_encryption_ctx(state->encryption_ctr, state->encryption_key);
//        if (!encryption_ctx) {
//            state->error_status = STORJ_MEMORY_ERROR;
//            goto clean_variables;
//        }
//        // Increment the iv to proper placement because we may be reading from the middle of the file
//        increment_ctr_aes_iv(encryption_ctx->encryption_ctr, req->shard_meta_index * state->shard_size);
//    }
//
//    uint8_t cphr_txt[AES_BLOCK_SIZE * 256];
//    memset_zero(cphr_txt, AES_BLOCK_SIZE * 256);
//    char read_data[AES_BLOCK_SIZE * 256];
//    memset_zero(read_data, AES_BLOCK_SIZE * 256);
//    unsigned long int read_bytes = 0;
//    uint64_t total_read = 0;
//
//    do {
//        if (state->canceled) {
//            goto clean_variables;
//        }
//
//        read_bytes = pread(fileno(req->shard_file),
//                           read_data, AES_BLOCK_SIZE * 256,
//                           shard_meta->index*state->shard_size + total_read);
//
//        if (read_bytes == -1) {
//            req->log->warn(state->env->log_options, state->handle,
//                           "Error reading file: %d",
//                           errno);
//            req->error_status = STORJ_FILE_READ_ERROR;
//            goto clean_variables;
//        }
//
//        total_read += read_bytes;
//
//        if (!state->rs) {
//            // Encrypt data
//            ctr_crypt(encryption_ctx->ctx, (nettle_cipher_func *)aes256_encrypt,
//                      AES_BLOCK_SIZE, encryption_ctx->encryption_ctr, read_bytes,
//                      (uint8_t *)cphr_txt, (uint8_t *)read_data);
//        } else {
//            // Just use the already encrypted data
//            memcpy(cphr_txt, read_data, AES_BLOCK_SIZE*256);
//        }
//
//        sha256_update(&shard_hash_ctx, read_bytes, cphr_txt);
//
//        for (int i = 0; i < STORJ_SHARD_CHALLENGES; i++ ) {
//            sha256_update(&first_sha256_for_leaf[i], read_bytes, cphr_txt);
//        }
//
//        memset_zero(read_data, AES_BLOCK_SIZE * 256);
//        memset_zero(cphr_txt, AES_BLOCK_SIZE * 256);
//
//    } while(total_read < state->shard_size && read_bytes > 0);
//
//    shard_meta->size = total_read;
//
//    sha256_digest(&shard_hash_ctx, SHA256_DIGEST_SIZE, prehash_sha256);
//
//    uint8_t prehash_ripemd160[RIPEMD160_DIGEST_SIZE];
//    memset_zero(prehash_ripemd160, RIPEMD160_DIGEST_SIZE);
//    ripemd160_of_str(prehash_sha256, SHA256_DIGEST_SIZE, prehash_ripemd160);
//
//    // Shard Hash
//    char *hash = hex2str(RIPEMD160_DIGEST_SIZE, prehash_ripemd160);
//    if (!hash) {
//        req->error_status = STORJ_MEMORY_ERROR;
//        goto clean_variables;
//    }
//    memcpy(shard_meta->hash, hash, strlen(hash));
//    free(hash);
//
//    uint8_t preleaf_sha256[SHA256_DIGEST_SIZE];
//    memset_zero(preleaf_sha256, SHA256_DIGEST_SIZE);
//    uint8_t preleaf_ripemd160[RIPEMD160_DIGEST_SIZE];
//    memset_zero(preleaf_ripemd160, RIPEMD160_DIGEST_SIZE);
//    char leaf[RIPEMD160_DIGEST_SIZE*2 +1];
//    memset(leaf, '\0', RIPEMD160_DIGEST_SIZE*2 +1);
//    for (int i = 0; i < STORJ_SHARD_CHALLENGES; i++ ) {
//        // finish first sha256 for leaf
//        sha256_digest(&first_sha256_for_leaf[i], SHA256_DIGEST_SIZE, preleaf_sha256);
//
//        // ripemd160 result of sha256
//        ripemd160_of_str(preleaf_sha256, SHA256_DIGEST_SIZE, preleaf_ripemd160);
//
//        // sha256 and ripemd160 again
//        ripemd160sha256_as_string(preleaf_ripemd160, RIPEMD160_DIGEST_SIZE, leaf);
//
//        memcpy(shard_meta->tree[i], leaf, RIPEMD160_DIGEST_SIZE*2 + 1);
//    }
//
//clean_variables:
//    if (encryption_ctx) {
//        free_encryption_ctx(encryption_ctx);
//    }
//}
//
//static void after_create_encrypted_file(uv_work_t *work, int status)
//{
//    encrypt_file_req_t *req = work->data;
//    storj_upload_state_t *state = req->upload_state;
//
//    state->pending_work_count -= 1;
//    state->create_encrypted_file_count += 1;
//
//    uint64_t encrypted_file_size = 0;
//    #ifdef _WIN32
//        struct _stati64 st;
//        _stati64(state->encrypted_file_path, &st);
//        encrypted_file_size = st.st_size;
//    #else
//        struct stat st;
//        stat(state->encrypted_file_path, &st);
//        encrypted_file_size = st.st_size;
//    #endif
//
//    if (req->error_status != 0 || state->file_size != encrypted_file_size) {
//        state->log->warn(state->env->log_options, state->handle,
//                       "Failed to encrypt data.");
//
//        if (state->create_encrypted_file_count == 6) {
//            state->error_status = STORJ_FILE_ENCRYPTION_ERROR;
//        }
//    } else {
//        state->log->info(state->env->log_options, state->handle,
//                       "Successfully encrypted file");
//
//        state->encrypted_file = fopen(state->encrypted_file_path, "r");
//        if (!state->encrypted_file) {
//            state->error_status = STORJ_FILE_READ_ERROR;
//        }
//    }
//
//    state->creating_encrypted_file = false;
//
//clean_variables:
//    queue_next_work(state);
//    free(work->data);
//    free(work);
//}
//
//static void create_encrypted_file(uv_work_t *work)
//{
//    encrypt_file_req_t *req = work->data;
//    storj_upload_state_t *state = req->upload_state;
//
//    state->log->info(state->env->log_options, state->handle, "Encrypting file...");
//
//    // Initialize the encryption context
//    storj_encryption_ctx_t *encryption_ctx = prepare_encryption_ctx(state->encryption_ctr,
//                                                                    state->encryption_key);
//    if (!encryption_ctx) {
//        state->error_status = STORJ_MEMORY_ERROR;
//        goto clean_variables;
//    }
//
//    uint8_t cphr_txt[AES_BLOCK_SIZE * 256];
//    memset_zero(cphr_txt, AES_BLOCK_SIZE * 256);
//    char read_data[AES_BLOCK_SIZE * 256];
//    memset_zero(read_data, AES_BLOCK_SIZE * 256);
//    unsigned long int read_bytes = 0;
//    unsigned long int written_bytes = 0;
//    uint64_t total_read = 0;
//
//    FILE *encrypted_file = fopen(state->encrypted_file_path, "w+");
//
//    if (encrypted_file == NULL) {
//      state->log->error(state->env->log_options, state->handle,
//                     "Can't create file for encrypted data [%s]",
//                     state->encrypted_file_path);
//        goto clean_variables;
//    }
//
//    do {
//        if (state->canceled) {
//            goto clean_variables;
//        }
//
//        read_bytes = pread(fileno(state->original_file),
//                           read_data, AES_BLOCK_SIZE * 256,
//                           total_read);
//
//        if (read_bytes == -1) {
//            state->log->warn(state->env->log_options, state->handle,
//                           "Error reading file: %d",
//                           errno);
//            req->error_status = STORJ_FILE_READ_ERROR;
//            goto clean_variables;
//        }
//
//        // Encrypt data
//        ctr_crypt(encryption_ctx->ctx, (nettle_cipher_func *)aes256_encrypt,
//                  AES_BLOCK_SIZE, encryption_ctx->encryption_ctr, read_bytes,
//                  (uint8_t *)cphr_txt, (uint8_t *)read_data);
//
//        written_bytes = pwrite(fileno(encrypted_file), cphr_txt, read_bytes, total_read);
//
//        memset_zero(read_data, AES_BLOCK_SIZE * 256);
//        memset_zero(cphr_txt, AES_BLOCK_SIZE * 256);
//
//        total_read += read_bytes;
//
//        if (written_bytes != read_bytes) {
//            goto clean_variables;
//        }
//
//    } while(total_read < state->file_size && read_bytes > 0);
//
//clean_variables:
//    if (encrypted_file) {
//        fclose(encrypted_file);
//    }
//    if (encryption_ctx) {
//        free_encryption_ctx(encryption_ctx);
//    }
//}
//
//static void after_request_frame_id(uv_work_t *work, int status)
//{
//    frame_request_t *req = work->data;
//    storj_upload_state_t *state = req->upload_state;
//
//    state->requesting_frame = false;
//    state->pending_work_count -= 1;
//
//    if (status == UV_ECANCELED) {
//        state->frame_request_count = 0;
//        goto clean_variables;
//    }
//
//    state->frame_request_count += 1;
//
//    if (req->status_code == 429 || req->status_code == 420) {
//
//        state->error_status = STORJ_BRIDGE_RATE_ERROR;
//
//    } else if (req->error_status == 0 && req->status_code == 200 && req->frame_id) {
//
//        state->log->info(state->env->log_options, state->handle,
//                         "Successfully retrieved frame id: %s", req->frame_id);
//
//        state->frame_id = req->frame_id;
//
//    } else if (state->frame_request_count == 6) {
//        state->error_status = STORJ_BRIDGE_FRAME_ERROR;
//    }
//
//clean_variables:
//    queue_next_work(state);
//    free(req);
//    free(work);
//}
//
//static void request_frame_id(uv_work_t *work)
//{
//    frame_request_t *req = work->data;
//    storj_upload_state_t *state = req->upload_state;
//
//    req->log->info(state->env->log_options,
//                   state->handle,
//                   "[%s] Requesting file staging frame... (retry: %d)",
//                   state->file_name,
//                   state->frame_request_count);
//
//    // Prepare the body
//    struct json_object *body = json_object_new_object();
//
//    int status_code;
//    struct json_object *response = NULL;
//    int request_status = fetch_json(req->http_options,
//                                    req->options,
//                                    "POST",
//                                    "/frames",
//                                    body,
//                                    true,
//                                    &response,
//                                    &status_code);
//
//
//    if (request_status) {
//        req->log->warn(state->env->log_options, state->handle,
//                       "Request frame id error: %i", request_status);
//    }
//
//    req->log->debug(state->env->log_options,
//                    state->handle,
//                    "fn[request_frame_id] - JSON Response: %s",
//                    json_object_to_json_string(response));
//
//    struct json_object *frame_id;
//    if (!json_object_object_get_ex(response, "id", &frame_id)) {
//        req->error_status = STORJ_BRIDGE_JSON_ERROR;
//        goto cleanup;
//    }
//
//    if (!json_object_is_type(frame_id, json_type_string)) {
//        req->error_status = STORJ_BRIDGE_JSON_ERROR;
//        goto cleanup;
//    }
//
//    char *frame_id_str = (char *)json_object_get_string(frame_id);
//    req->frame_id = calloc(strlen(frame_id_str) + 1, sizeof(char));
//    if (!req->frame_id) {
//        req->error_status = STORJ_MEMORY_ERROR;
//        goto cleanup;
//    }
//
//    strcpy(req->frame_id, frame_id_str);
//
//cleanup:
//    req->status_code = status_code;
//
//    json_object_put(response);
//    json_object_put(body);
//}
//
//static void after_create_parity_shards(uv_work_t *work, int status)
//{
//    parity_shard_req_t *req = work->data;
//    storj_upload_state_t *state = req->upload_state;
//
//    state->pending_work_count -= 1;
//
//    // TODO: Check if file was created
//    if (req->error_status != 0) {
//        state->log->warn(state->env->log_options, state->handle,
//                       "Failed to create parity shards");
//
//        state->awaiting_parity_shards = true;
//
//        state->error_status = STORJ_FILE_PARITY_ERROR;
//    } else {
//        state->log->info(state->env->log_options, state->handle,
//                       "Successfully created parity shards");
//
//        state->parity_file = fopen(state->parity_file_path, "r");
//
//        if (!state->parity_file) {
//            state->error_status = STORJ_FILE_READ_ERROR;
//        }
//
//    }
//
//clean_variables:
//    queue_next_work(state);
//    free(work->data);
//    free(work);
//}
//
//static void create_parity_shards(uv_work_t *work)
//{
//    parity_shard_req_t *req = work->data;
//    storj_upload_state_t *state = req->upload_state;
//
//    state->log->info(state->env->log_options, state->handle,
//                   "Creating parity shards");
//
//    // ???
//    fec_init();
//
//    uint8_t **data_blocks = NULL;
//    uint8_t **fec_blocks = NULL;
//
//    uint8_t *map = NULL;
//    int status = 0;
//
//    FILE *encrypted_file = fopen(state->encrypted_file_path, "r");
//
//    if (!encrypted_file) {
//        req->error_status = 1;
//        state->log->error(state->env->log_options, state->handle,
//                          "Unable to open encrypted file");
//        goto clean_variables;
//    }
//
//    status = map_file(fileno(encrypted_file), state->file_size, &map, true);
//
//    if (status) {
//        req->error_status = 1;
//        state->log->error(state->env->log_options, state->handle,
//                          "Could not create mmap original file: %d", status);
//        goto clean_variables;
//    }
//
//    uint64_t parity_size = state->total_shards * state->shard_size - state->file_size;
//
//    // determine parity shard location
//    char *tmp_folder = NULL;
//    if (!state->parity_file_path) {
//        req->error_status = 1;
//        state->log->error(state->env->log_options, state->handle,
//                          "No temp folder set for parity shards");
//        goto clean_variables;
//    }
//
//    FILE *parity_file = fopen(state->parity_file_path, "w+");
//    if (!parity_file) {
//        req->error_status = 1;
//        state->log->error(state->env->log_options, state->handle,
//                          "Could not open parity file [%s]", state->parity_file_path);
//        goto clean_variables;
//    }
//
//    int falloc_status = allocatefile(fileno(parity_file), parity_size);
//
//    if (falloc_status) {
//        req->error_status = 1;
//        state->log->error(state->env->log_options, state->handle,
//                          "Could not allocate space for mmap parity " \
//                          "shard file: %i", falloc_status);
//        goto clean_variables;
//    }
//
//    uint8_t *map_parity = NULL;
//    status = map_file(fileno(parity_file), parity_size, &map_parity, false);
//
//    if (status) {
//        req->error_status = 1;
//        state->log->error(state->env->log_options, state->handle,
//                       "Could not create mmap parity shard file: %d", status);
//        goto clean_variables;
//    }
//
//    data_blocks = (uint8_t**)malloc(state->total_data_shards * sizeof(uint8_t *));
//    if (!data_blocks) {
//        req->error_status = 1;
//        state->log->error(state->env->log_options, state->handle,
//                       "memory error: unable to malloc");
//        goto clean_variables;
//    }
//
//    for (int i = 0; i < state->total_data_shards; i++) {
//        data_blocks[i] = map + i * state->shard_size;
//    }
//
//    fec_blocks = (uint8_t**)malloc(state->total_parity_shards * sizeof(uint8_t *));
//    if (!fec_blocks) {
//        req->error_status = 1;
//        state->log->error(state->env->log_options, state->handle,
//                       "memory error: unable to malloc");
//        goto clean_variables;
//    }
//
//    for (int i = 0; i < state->total_parity_shards; i++) {
//        fec_blocks[i] = map_parity + i * state->shard_size;
//    }
//
//    state->log->debug(state->env->log_options, state->handle,
//                      "Encoding parity shards, data_shards: %i, "       \
//                      "parity_shards: %i, shard_size: %" PRIu64 ", "    \
//                      "file_size: %" PRIu64,
//                      state->total_data_shards,
//                      state->total_parity_shards,
//                      state->shard_size,
//                      state->file_size);
//
//
//    reed_solomon *rs = reed_solomon_new(state->total_data_shards,
//                                        state->total_parity_shards);
//    reed_solomon_encode2(rs, data_blocks, fec_blocks, state->total_shards,
//                         state->shard_size, state->file_size);
//    reed_solomon_release(rs);
//
//clean_variables:
//    if (data_blocks) {
//        free(data_blocks);
//    }
//
//    if (fec_blocks) {
//        free(fec_blocks);
//    }
//
//    if (tmp_folder) {
//        free(tmp_folder);
//    }
//
//    if (map) {
//        unmap_file(map, state->file_size);
//    }
//
//    if (map_parity) {
//        unmap_file(map_parity, parity_size);
//    }
//
//    if (parity_file) {
//        fclose(parity_file);
//    }
//
//    if (encrypted_file) {
//        fclose(encrypted_file);
//    }
//}
//
//
//static void after_send_exchange_report(uv_work_t *work, int status)
//{
//    shard_send_report_t *req = work->data;
//
//    req->state->pending_work_count -= 1;
//
//    if (status == UV_ECANCELED) {
//        req->report->send_count = 0;
//        req->report->send_status = STORJ_REPORT_AWAITING_SEND;
//
//        goto clean_variables;
//    }
//
//    req->report->send_count += 1;
//
//    if (req->status_code == 201) {
//        req->state->env->log->info(req->state->env->log_options,
//                        req->state->handle,
//                         "Successfully sent exchange report for shard %d",
//                         req->report->pointer_index);
//
//        req->report->send_status = STORJ_REPORT_NOT_PREPARED; // report has been sent
//    } else if (req->report->send_count == 6) {
//        req->report->send_status = STORJ_REPORT_NOT_PREPARED; // report failed retries
//    } else {
//        req->report->send_status = STORJ_REPORT_AWAITING_SEND; // reset report back to unsent
//    }
//
//clean_variables:
//    queue_next_work(req->state);
//    free(work->data);
//    free(work);
//
//}
//
//static void send_exchange_report(uv_work_t *work)
//{
//    shard_send_report_t *req = work->data;
//    storj_upload_state_t *state = req->state;
//
//    struct json_object *body = json_object_new_object();
//
//    json_object_object_add(body, "dataHash",
//                           json_object_new_string(req->report->data_hash));
//
//    json_object_object_add(body, "reporterId",
//                           json_object_new_string(req->report->reporter_id));
//
//    json_object_object_add(body, "farmerId",
//                           json_object_new_string(req->report->farmer_id));
//
//    json_object_object_add(body, "clientId",
//                           json_object_new_string(req->report->client_id));
//
//    json_object_object_add(body, "exchangeStart",
//                           json_object_new_int64(req->report->start));
//
//    json_object_object_add(body, "exchangeEnd",
//                           json_object_new_int64(req->report->end));
//
//    json_object_object_add(body, "exchangeResultCode",
//                           json_object_new_int(req->report->code));
//
//    json_object_object_add(body, "exchangeResultMessage",
//                           json_object_new_string(req->report->message));
//
//    int status_code = 0;
//
//    // there should be an empty object in response
//    struct json_object *response = NULL;
//    int request_status = fetch_json(req->http_options,
//                                    req->options, "POST",
//                                    "/reports/exchanges", body,
//                                    true, &response, &status_code);
//
//
//    if (request_status) {
//        state->log->warn(state->env->log_options, state->handle,
//                         "Send exchange report error: %i", request_status);
//    }
//
//    req->status_code = status_code;
//
//    // free all memory for body and response
//    json_object_put(response);
//    json_object_put(body);
//}
//
//static void verify_bucket_id_callback(uv_work_t *work_req, int status)
//{
//    get_bucket_request_t *req = work_req->data;
//    storj_upload_state_t *state = req->handle;
//
//    state->log->info(state->env->log_options, state->handle,
//                     "Checking if bucket id [%s] exists", state->bucket_id);
//
//    state->pending_work_count -= 1;
//    state->bucket_verify_count += 1;
//
//    if (req->status_code == 200) {
//        state->bucket_verified = true;
//        goto clean_variables;
//    } else if (req->status_code == 404 || req->status_code == 400) {
//        state->log->error(state->env->log_options, state->handle,
//                         "Bucket [%s] doesn't exist", state->bucket_id);
//        state->error_status = STORJ_BRIDGE_BUCKET_NOTFOUND_ERROR;
//    } else {
//        state->log->error(state->env->log_options, state->handle,
//                         "Request failed with status code: %i", req->status_code);
//
//         if (state->bucket_verify_count == 6) {
//             state->error_status = STORJ_BRIDGE_REQUEST_ERROR;
//             state->bucket_verify_count = 0;
//         }
//
//         goto clean_variables;
//    }
//    state->bucket_verified = true;
//
//clean_variables:
//    queue_next_work(state);
//
//    storj_free_get_bucket_request(req);
//    free(work_req);
//}
//
//static void verify_file_name_callback(uv_work_t *work_req, int status)
//{
//    json_request_t *req = work_req->data;
//    storj_upload_state_t *state = req->handle;
//
//    state->pending_work_count -= 1;
//    state->file_verify_count += 1;
//
//    if (req->status_code == 404) {
//        state->file_verified = true;
//        goto clean_variables;
//    } else if (req->status_code == 200) {
//        state->log->error(state->env->log_options, state->handle,
//                          "File [%s] already exists", state->file_name);
//        state->error_status = STORJ_BRIDGE_BUCKET_FILE_EXISTS;
//    } else {
//        state->log->error(state->env->log_options, state->handle,
//                          "Request failed with status code: %i", req->status_code);
//
//        if (state->file_verify_count == 6) {
//            state->error_status = STORJ_BRIDGE_REQUEST_ERROR;
//            state->file_verify_count = 0;
//        }
//
//        goto clean_variables;
//    }
//
//    state->file_verified = true;
//
//clean_variables:
//    queue_next_work(state);
//
//    json_object_put(req->response);
//    free(req->path);
//    free(req);
//    free(work_req);
//}
//
//static void verify_file_name(uv_work_t *work)
//{
//    json_request_t *req = work->data;
//    storj_upload_state_t *state = req->handle;
//    int status_code = 0;
//
//    state->log->info(state->env->log_options, state->handle,
//                     "Checking if file name [%s] already exists...", state->file_name);
//
//    req->error_code = fetch_json(req->http_options,
//                                 req->options, req->method, req->path, req->body,
//                                 req->auth, &req->response, &status_code);
//
//    req->status_code = status_code;
//}

static void queue_get_file_info(uv_work_t *work)
{
    // TODO: call get_file_info
    // TODO: set state->info fields

//    get_file_info()
}

static void store_file(storj_upload_state_t *state)
{
    BucketRef bucket_ref = open_bucket(state->env->project_ref,
                                     strdup(state->bucket_id),
                                     strdup(state->encryption_access),
                                     STORJ_LAST_ERROR);
    STORJ_RETURN_SET_STATE_ERROR_IF_LAST_ERROR;

    UploaderRef uploader_ref = upload(bucket_ref, strdup(state->file_name),
                                      state->upload_opts, STORJ_LAST_ERROR);
    STORJ_RETURN_SET_STATE_ERROR_IF_LAST_ERROR;

    size_t buf_len;
    uint8_t *buf;
    while (state->uploaded_bytes < state->file_size) {
        size_t remaining_size = state->file_size - state->uploaded_bytes;
        if (remaining_size >= buf_len) {
            buf_len = state->buffer_size;
        } else {
            buf_len = remaining_size;
        }

        buf = malloc(buf_len);
        size_t read_size = fread(buf, sizeof(char), buf_len, state->original_file);
        // TODO: what if read_size != buf_len!?

        int written_size = upload_write(uploader_ref, buf, buf_len, STORJ_LAST_ERROR);
        STORJ_RETURN_SET_STATE_ERROR_IF_LAST_ERROR;

        // TODO: use uv_async_init/uv_async_send instead of calling cb directly?
        // TODO: what if written_byte != buf_len!?
        state->uploaded_bytes += written_size;
        double progress = state->uploaded_bytes / state->file_size;
        state->progress_cb(progress, state->uploaded_bytes,
                           state->file_size, state->handle);
        free(buf);
    }

    state->progress_finished = true;
    state->completed_upload = true;

    upload_commit(uploader_ref, STORJ_LAST_ERROR);
    STORJ_RETURN_SET_STATE_ERROR_IF_LAST_ERROR;
}

static void begin_work_queue(uv_work_t *work, int status)
{
    storj_upload_state_t *state = work->data;

    // TODO: fix segfault
    // Load progress bar
//    state->progress_cb(0, 0, 0, state->handle);

    // TODO: do we care about status before this point?
    store_file(state);
    status = uv_queue_work(state->env->loop, (uv_work_t*) work,
                               queue_get_file_info, cleanup_work);

    if (status) {
        state->error_status = STORJ_QUEUE_ERROR;
        return;
    }
}

static void prepare_upload_state(uv_work_t *work)
{
    storj_upload_state_t *state = work->data;

    // Get the file size, expect to be up to 10tb
#ifdef _WIN32
    struct _stati64 st;

    if(_fstati64(fileno(state->original_file), &st) != 0) {
        state->error_status = STORJ_FILE_INTEGRITY_ERROR;
        return;
    }
#else
    struct stat st;
    if(fstat(fileno(state->original_file), &st) != 0) {
        state->error_status = STORJ_FILE_INTEGRITY_ERROR;
        return;
    }
#endif

    state->file_size = st.st_size;
    state->info = malloc(sizeof(storj_file_meta_t));
    state->info->created = NULL;
    state->info->filename = state->file_name;
    state->info->mimetype = NULL;
    state->info->size = state->file_size;
    state->info->id = NULL;
    state->info->bucket_id = state->bucket_id;
    state->info->decrypted = true;
}

//char *create_tmp_name(storj_upload_state_t *state, char *extension)
//{
//    char *tmp_folder = strdup(state->env->tmp_path);
//    int encode_len = BASE16_ENCODE_LENGTH(SHA256_DIGEST_SIZE);
//    int file_name_len = strlen(state->encrypted_file_name);
//    int extension_len = strlen(extension);
//    int tmp_folder_len = strlen(tmp_folder);
//    if (tmp_folder[tmp_folder_len - 1] == separator()) {
//        tmp_folder[tmp_folder_len - 1] = '\0';
//        tmp_folder_len -= 1;
//    }
//
//    char *path = calloc(
//        tmp_folder_len + 1 + encode_len + extension_len + 2,
//        sizeof(char)
//    );
//
//    // hash and encode name for filesystem use
//    struct sha256_ctx ctx;
//    uint8_t digest[SHA256_DIGEST_SIZE];
//    uint8_t digest_encoded[encode_len + 1];
//    sha256_init(&ctx);
//    sha256_update(&ctx, file_name_len, state->encrypted_file_name);
//    sha256_digest(&ctx, SHA256_DIGEST_SIZE, digest);
//    base16_encode_update(digest_encoded, SHA256_DIGEST_SIZE, digest);
//    digest_encoded[encode_len] = '\0';
//
//    sprintf(path,
//            "%s%c%s%s%c",
//            tmp_folder,
//            separator(),
//            digest_encoded,
//            extension,
//            '\0');
//
//    free(tmp_folder);
//    return path;
//}
//
//STORJ_API int storj_bridge_store_file_cancel(storj_upload_state_t *state)
//{
//    if (state->canceled) {
//        return 0;
//    }
//
//    state->canceled = true;
//
//    state->error_status = STORJ_TRANSFER_CANCELED;
//
//    // loop over all shards, and cancel any that are queued to be uploaded
//    // any uploads that are in-progress will monitor the state->canceled
//    // status and exit when set to true
//    for (int i = 0; i < state->total_shards; i++) {
//        shard_tracker_t *shard = &state->shard[i];
//        if (shard->progress == PUSHING_SHARD) {
//            uv_cancel((uv_req_t *)shard->work);
//        }
//    }
//
//    return 0;
//}

STORJ_API storj_upload_state_t *storj_bridge_store_file(storj_env_t *env,
                            storj_upload_opts_t *opts,
                            void *handle,
                            storj_progress_cb progress_cb,
                            storj_finished_upload_cb finished_cb)
{
    if (!opts->fd) {
        env->log->error(env->log_options, handle, "Invalid File descriptor");
        return NULL;
    }

    storj_upload_state_t *state = malloc(sizeof(storj_upload_state_t));
    if (!state) {
        return NULL;
    }

    int default_size = 1024 * 1024 * 4 * sizeof(char);
    // TODO: fix this
//    state->buffer_size = (opts->buffer_size == 0) ? STORJ_DEFAULT_UPLOAD_BUFFER_SIZE : opts->buffer_size;
    state->buffer_size = (opts->buffer_size == 0) ? default_size : opts->buffer_size;

    state->upload_opts = malloc(sizeof(UploadOptions));
//    state->upload_opts->content_type = strdup(opts->content_type);
    state->upload_opts->expires = opts->expires;

    state->env = env;
    // TODO: strdup(opts->file_name)?
    state->file_name = strdup(opts->file_name);
    state->encryption_access = strdup(opts->encryption_access);
    state->file_size = 0;
    state->bucket_id = strdup(opts->bucket_id);
    state->encrypted_file_name = strdup(opts->file_name);
    state->buffer_size = opts->buffer_size;

    state->original_file = opts->fd;

    state->progress_finished = false;

    state->finished_cb = finished_cb;
    state->error_status = 0;
    state->log = env->log;
    state->handle = handle;

    uv_work_t *work = uv_work_new();
    work->data = state;

    int status = uv_queue_work(env->loop, (uv_work_t*) work,
                               prepare_upload_state, begin_work_queue);
    if (status) {
        state->error_status = STORJ_QUEUE_ERROR;
    }
    return state;
}

STORJ_API void storj_free_uploaded_file_info(storj_file_meta_t *file)
{
    if (file) {
        free((char *)file->id);
        free((char *)file->created);
        free((char *)file->mimetype);
    }
    free(file);
}
