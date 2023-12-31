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

// ============================================================================
//  dir
// ============================================================================

static
int lt_posix_dir_type(const link_t* link) {
	return 0;
}

static
int lt_posix_dir_state(const link_t* link){
	return 0;
}

static
size_t lt_posix_dir_size(const link_t* link, int type){
	return 0;
}

static
int lt_posix_dir_boot(link_t* link, const lt_args_t* args) {
	const char* path_arg = lt_args_gets(args, "@path", NULL);
    if ( path_arg == NULL ) {
        return EINVAL;
    }

    const size_t path_strlen = strlen(path_arg);
    char* path = malloc(path_strlen+1);
    if ( path == NULL ) {
        return ENOMEM;
    }
    strcpy(path, path_arg);
    link->gw_shr = path;

    return LT_OK;
}

static
int lt_posix_dir_start(link_t* link, int type, const lt_args_t* args) {
	return LT_OK;
}

static
void lt_posix_dir_stop(link_t* link, int type) {
    char* path = link->gw_shr;
    if ( path ) {
        free(path);
    }
    link->gw_shr = NULL;
}

static
int lt_posix_dir_copy(link_t* link, link_t* out) {
	return 0;
}

static
size_t lt_posix_dir_read(link_t* link, char* buffer, size_t length) {
	return 0;
}

static
size_t lt_posix_dir_write(link_t* link, const char* buffer, size_t length) {
    return 0;
}

static
void lt_posix_dir_recv(link_t* link) {
}

static
bool lt_posix_dir_recv_async(link_t* link) {
    return false;
}

static
bool lt_posix_dir_open(link_t* link, int a) {
    return false;
}

static
lt_api_t lt_posix_dir = {
	.type = lt_posix_dir_type,
	.state = lt_posix_dir_state,
	.size = lt_posix_dir_size,
	.boot = lt_posix_dir_boot,
	.start = lt_posix_dir_start,
	.stop = lt_posix_dir_stop,
	.copy = lt_posix_dir_copy,
	.read = lt_posix_dir_read,
	.write = lt_posix_dir_write,
    .recv = lt_posix_dir_recv,
    .recv_async = lt_posix_dir_recv_async
};

int lt_new_posix_dir(link_t* link, const lt_args_t* args) {
	link->gw_api = &lt_posix_dir;
	lt_posix_dir_boot(link, args);
	return LT_OK;
}
