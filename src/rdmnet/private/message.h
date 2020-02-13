/******************************************************************************
 * Copyright 2019 ETC Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************
 * This file is a part of RDMnet. For more information, go to:
 * https://github.com/ETCLabs/RDMnet
 *****************************************************************************/

#ifndef RDMNET_PRIVATE_MESSAGE_H_
#define RDMNET_PRIVATE_MESSAGE_H_

#include <assert.h>
#include "etcpal/error.h"
#include "rdmnet/core/message.h"
#include "rdmnet/private/opts.h"

#if RDMNET_DYNAMIC_MEM
#include <stdlib.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define RPT_CLIENT_ENTRIES_MAX_SIZE (RDMNET_MAX_CLIENT_ENTRIES * sizeof(RptClientEntry))
#define EPT_CLIENT_ENTRIES_MAX_SIZE (RDMNET_MAX_CLIENT_ENTRIES * sizeof(EptClientEntry))
#define DYNAMIC_UID_REQUESTS_MAX_SIZE (RDMNET_MAX_DYNAMIC_UID_ENTRIES * sizeof(BrokerDynamicUidRequest))
#define DYNAMIC_UID_MAPPINGS_MAX_SIZE (RDMNET_MAX_DYNAMIC_UID_ENTRIES * sizeof(BrokerDynamicUidMapping))
#define FETCH_UID_ASSIGNMENTS_MAX_SIZE (RDMNET_MAX_DYNAMIC_UID_ENTRIES * sizeof(RdmUid))
#define RDM_BUFFERS_MAX_SIZE (RDMNET_MAX_RECEIVED_ACK_OVERFLOW_RESPONSES * sizeof(RdmBuffer))

typedef union
{
  RptClientEntry rpt_client_entries[RPT_CLIENT_ENTRIES_MAX_SIZE];
  EptClientEntry ept_client_entries[EPT_CLIENT_ENTRIES_MAX_SIZE];
  BrokerDynamicUidRequest dynamic_uid_requests[DYNAMIC_UID_REQUESTS_MAX_SIZE];
  BrokerDynamicUidMapping dynamic_uid_mappings[DYNAMIC_UID_MAPPINGS_MAX_SIZE];
  RdmUid fetch_uid_assignments[FETCH_UID_ASSIGNMENTS_MAX_SIZE];
  RdmBuffer rdm_buffers[RDM_BUFFERS_MAX_SIZE];
} StaticMessageBuffer;

extern StaticMessageBuffer rdmnet_static_msg_buf;
extern char rpt_status_string_buffer[RPT_STATUS_STRING_MAXLEN + 1];

#undef RDMNET_DYNAMIC_MEM
#define RDMNET_DYNAMIC_MEM 0

#if RDMNET_DYNAMIC_MEM

#define ALLOC_RPT_CLIENT_ENTRY() malloc(sizeof(RptClientEntry))
#define ALLOC_EPT_CLIENT_ENTRY() malloc(sizeof(EptClientEntry))
#define ALLOC_DYNAMIC_UID_REQUEST_ENTRY() malloc(sizeof(BrokerDynamicUidRequest))
#define ALLOC_DYNAMIC_UID_MAPPING() malloc(sizeof(BrokerDynamicUidMapping))
#define ALLOC_FETCH_UID_ASSIGNMENT() malloc(sizeof(RdmUid))
#define ALLOC_RDM_BUFFER() malloc(sizeof(RdmBuffer))

#define REALLOC_RPT_CLIENT_ENTRY(ptr, new_size) realloc((ptr), ((new_size) * sizeof(RptClientEntry)))
#define REALLOC_EPT_CLIENT_ENTRY(ptr, new_size) realloc((ptr), ((new_size) * sizeof(EptClientEntry)))
#define REALLOC_DYNAMIC_UID_REQUEST_ENTRY(ptr, new_size) realloc((ptr), ((new_size) * sizeof(BrokerDynamicUidRequest)))
#define REALLOC_DYNAMIC_UID_MAPPING(ptr, new_size) realloc((ptr), ((new_size) * sizeof(BrokerDynamicUidMapping)))
#define REALLOC_FETCH_UID_ASSIGNMENT(ptr, new_size) realloc((ptr), ((new_size) * sizeof(RdmUid)))
#define REALLOC_RDM_BUFFER(ptr, new_size) realloc((ptr), ((new_size) * sizeof(RdmBuffer)))

#define ALLOC_EPT_SUBPROT_LIST() malloc(sizeof(EptSubProtocol))
#define REALLOC_EPT_SUBPROT_LIST(ptr, new_size) realloc((ptr), ((new_size) * sizeof(EptSubProtocol)))
#define FREE_EPT_SUBPROT_LIST(ptr) free(ptr)

#define ALLOC_RPT_STATUS_STR(size) malloc(size)

#define FREE_MESSAGE_BUFFER(ptr) free(ptr)

#else

// Static buffer space for RDMnet messages is held in a union. Only one field can be used at a
// time.
#define ALLOC_FROM_ARRAY(array, array_size) rdmnet_static_msg_buf.array
#define REALLOC_FROM_ARRAY(ptr, new_size, array, array_size) \
  (assert((ptr) == (rdmnet_static_msg_buf.array)), ((new_size) <= (array_size) ? rdmnet_static_msg_buf.array : NULL))

#define ALLOC_RPT_CLIENT_ENTRY() ALLOC_FROM_ARRAY(rpt_client_entries, RPT_CLIENT_ENTRIES_MAX_SIZE)
#define ALLOC_EPT_CLIENT_ENTRY() ALLOC_FROM_ARRAY(ept_client_entries, EPT_CLIENT_ENTRIES_MAX_SIZE)
#define ALLOC_DYNAMIC_UID_REQUEST_ENTRY() ALLOC_FROM_ARRAY(dynamic_uid_requests, DYNAMIC_UID_REQUESTS_MAX_SIZE)
#define ALLOC_DYNAMIC_UID_MAPPING() ALLOC_FROM_ARRAY(dynamic_uid_mappings, DYNAMIC_UID_MAPPINGS_MAX_SIZE)
#define ALLOC_FETCH_UID_ASSIGNMENT() ALLOC_FROM_ARRAY(fetch_uid_assignments, FETCH_UID_ASSIGNMENTS_MAX_SIZE)
#define ALLOC_RDM_BUFFER() ALLOC_FROM_ARRAY(rdm_buffers, RDM_BUFFERS_MAX_SIZE)

#define REALLOC_RPT_CLIENT_ENTRY(ptr, new_size) \
  REALLOC_FROM_ARRAY(ptr, new_size, rpt_client_entries, RPT_CLIENT_ENTRIES_MAX_SIZE)
#define REALLOC_EPT_CLIENT_ENTRY(ptr, new_size) \
  REALLOC_FROM_ARRAY(ptr, new_size, ept_client_entries, EPT_CLIENT_ENTRIES_MAX_SIZE)
#define REALLOC_DYNAMIC_UID_REQUEST_ENTRY(ptr, new_size) \
  REALLOC_FROM_ARRAY(ptr, new_size, dynamic_uid_requests, DYNAMIC_UID_REQUESTS_MAX_SIZE)
#define REALLOC_DYNAMIC_UID_MAPPING(ptr, new_size) \
  REALLOC_FROM_ARRAY(ptr, new_size, dynamic_uid_mappings, DYNAMIC_UID_MAPPINGS_MAX_SIZE)
#define REALLOC_FETCH_UID_ASSIGNMENT(ptr, new_size) \
  REALLOC_FROM_ARRAY(ptr, new_size, fetch_uid_assignments, FETCH_UID_ASSIGNMENTS_MAX_SIZE)
#define REALLOC_RDM_BUFFER(ptr, new_size) REALLOC_FROM_ARRAY(ptr, new_size, rdm_buffers, RDM_BUFFERS_MAX_SIZE)

#define ALLOC_RPT_STATUS_STR(size) rpt_status_string_buffer

// TODO
#define ALLOC_EPT_SUBPROT_LIST() NULL
#define REALLOC_EPT_SUBPROT_LIST() NULL
#define FREE_EPT_SUBPROT_LIST(ptr)

#define FREE_MESSAGE_BUFFER(ptr)

#endif

#ifdef __cplusplus
}
#endif

#endif /* RDMNET_MESSAGE_PRIV_H_ */
