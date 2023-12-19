/* BSD 2-Clause License
 * 
 * Copyright (c) 2023, Felipe Bombardelli
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
	
// ============================================================================
//  Header
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
 #include <errno.h>

#include "lt_api.h"

typedef struct {
    int count;
    int mode;
    char path[];
} ll_shr_t;

// ============================================================================
//  File
// ============================================================================

static
int lt_posix_file_type(const link_t* link) {
	return 0;
}

static
int lt_posix_file_state(const link_t* link){
	return 0;
}

static
size_t lt_posix_file_size(const link_t* link, int type){
	return 0;
}

static
int lt_posix_file_boot(link_t* link, const lt_args_t* args) {
	const char* path = lt_args_gets(args, "@path", NULL);
    if ( path == NULL ) {
        lt_error(link, EINVAL, "Parameter @path is not present");
        return EINVAL;
    }

    // create shared data
    const size_t size = strlen(path);
    ll_shr_t* shr = malloc( sizeof(ll_shr_t) + size + 1 );
    shr->count = 1;
    strncpy(shr->path, path, size);
    link->gw_shr = shr;

    // start the link, case mode is present
	const char* mode = lt_args_gets(args, "@mode", NULL);
    if ( mode != NULL ) {
        FILE* fd = fopen(shr->path, mode);
        if ( fd == NULL ) {
            lt_error(link, EINVAL, "File not found");
            return EINVAL;
        }
        link->gw_obj = fd;
    }

    // success
	return LT_OK;
}

static
int lt_posix_file_start(link_t* link, int type, const lt_args_t* args) {
    if ( type == LT_START_PUBLISHER ) {
        ll_shr_t* shr = (ll_shr_t*) link->gw_shr;
        FILE* fd = fopen(shr->path, "w");
        if ( fd == NULL ) {
            lt_error(link, EINVAL, "File not found");
            return EINVAL;
        }
        link->gw_obj = fd;
    } else if ( type == LT_START_SUBSCRIBER ) {
        ll_shr_t* shr = (ll_shr_t*) link->gw_shr;
        FILE* fd = fopen(shr->path, "r");
        if ( fd == NULL ) {
            lt_error(link, EINVAL, "File not found");
            return EINVAL;
        }
        link->gw_obj = fd;
    }

    // success
	return LT_OK;
}

static
void lt_posix_file_stop(link_t* link, int type) {
    if ( link->gw_obj != NULL ) {
        FILE* fd = link->gw_obj;
        fclose(fd);
        link->gw_obj = NULL;
    }

    if ( link->gw_shr != NULL ) {
        ll_shr_t* shr = (ll_shr_t*) link->gw_shr;
        if ( shr->count <= 1 ) {
            shr->count = 0;
            free(shr);
            link->gw_shr = NULL;
        } else {
            shr->count -= 1;
        }
    }
}

static
int lt_posix_file_copy(link_t* link, link_t* out) {
	return 0;
}

static
size_t lt_posix_file_read(link_t* link, char* buffer, size_t length) {
	FILE* fd = link->gw_obj;
	return fread(buffer, 1, length, fd);
}

static
size_t lt_posix_file_write(link_t* link, const char* buffer, size_t length) {
	FILE* fd = link->gw_obj;
    return fwrite(buffer, 1, length, fd);
}

static
lt_api_t lt_posix_file = {
    .name = "Posix:File",
	.type = lt_posix_file_type,
	.state = lt_posix_file_state,
	.size = lt_posix_file_size,
	.boot = lt_posix_file_boot,
	.start = lt_posix_file_start,
	.stop = lt_posix_file_stop,
	.copy = lt_posix_file_copy,
	.read = lt_posix_file_read,
	.write = lt_posix_file_write,
};

int lt_new_posix_file(link_t* link, const lt_args_t* args) {
    link->gw_shr = NULL;
    link->gw_obj = NULL;
    link->dec_obj = NULL;
    link->enc_obj = NULL;

	link->gw_api = &lt_posix_file;
	if ( lt_posix_file_boot(link, args) != LT_OK ) {
        fprintf(stderr, "%s\n", link->errstr);
        exit(1);
    }

	// const char* format = lt_args_gets(args, "@format");
	// lt_load_encoder(link, format, args);
	return LT_OK;
}

// ============================================================================
//  Public Functions
// ============================================================================

const char* lt_posix_list() {
    return "file";
}
