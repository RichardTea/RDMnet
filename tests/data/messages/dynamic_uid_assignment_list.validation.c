#include "rdmnet/core/message.h"

// clang-format off

static BrokerDynamicUidMapping mappings[] = {
  {
    .status_code = kRdmnetDynamicUidStatusOk,
    .uid = {
      .manu = 0xa592,
      .id = 0x00000037
    },
    .rid = {
      .data = { 0xf9, 0x71, 0xcd, 0x96, 0xa2, 0xcf, 0x4c, 0xed, 0xaa, 0xd6, 0x1d, 0x8d, 0x63, 0xf7, 0xd0, 0x77 }
    }
  },
  {
    .status_code = kRdmnetDynamicUidStatusOk,
    .uid = {
      .manu = 0xa592,
      .id = 0x00000038
    },
    .rid = {
      .data = { 0x27, 0xf3, 0x3a, 0xfc, 0x99, 0x6a, 0x4e, 0xc3, 0xa6, 0x03, 0x01, 0x5c, 0xf9, 0x06, 0x23, 0x68 }
    }
  },
  {
    .status_code = kRdmnetDynamicUidStatusDuplicateRid,
    .uid = {
      .manu = 0,
      .id = 0
    },
    .rid = {
      .data = { 0x60, 0xd3, 0x7e, 0x0d, 0x34, 0xef, 0x45, 0x27, 0x86, 0x95, 0xfe, 0x85, 0xde, 0x08, 0xe9, 0xe0 }
    }
  }
};

const RdmnetMessage dynamic_uid_assignment_list = {
  .vector = ACN_VECTOR_ROOT_BROKER,
  .sender_cid = {
    .data = { 0xf9, 0xc7, 0xc7, 0x14, 0xf4, 0x7d, 0x41, 0x61, 0x9b, 0x1d, 0x28, 0xf4, 0xfa, 0xae, 0xfd, 0xcc }
  },
  .data.broker = {
    .vector = VECTOR_BROKER_ASSIGNED_DYNAMIC_UIDS,
    .data.dynamic_uid_assignment_list = {
      .mappings = mappings,
      .num_mappings = 3,
      .more_coming = false
    }
  }
};
