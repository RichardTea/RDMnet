#include "rdmnet/core/message.h"

// clang-format off

const RdmnetMessage rpt_status_with_string = {
  .vector = ACN_VECTOR_ROOT_RPT,
  .sender_cid = {
    .data = { 0x31, 0x09, 0x92, 0x97, 0x4a, 0xd7, 0x47, 0xb2, 0x8d, 0x1c, 0x85, 0xf4, 0x4b, 0x0e, 0x65, 0x34 }
  },
  .data.rpt = {
    .vector = VECTOR_RPT_STATUS,
    .header = {
      .source_uid = { 0x1234, 0x5678aaaa },
      .source_endpoint_id = 0x0000,
      .dest_uid = { 0xcba9, 0x87654321 },
      .dest_endpoint_id = 0x0000,
      .seqnum = 0x12345678
    },
    .data.status = {
      .status_code = kRptStatusRdmTimeout,
      .status_string =
        "01234567890123456789012345678901234567890123456789" // 50
        "01234567890123456789012345678901234567890123456789" // 100
        "01234567890123456789012345678901234567890123456789" // 150
        "01234567890123456789012345678901234567890123456789" // 200
        "01234567890123456789012345678901234567890123456789" // 250
        "01234567890123456789012345678901234567890123456789" // 300
        "01234567890123456789012345678901234567890123456789" // 350
        "01234567890123456789012345678901234567890123456789" // 400
        "01234567890123456789012345678901234567890123456789" // 450
        "01234567890123456789012345678901234567890123456789" // 500
        "01234567890123456789012345678901234567890123456789" // 550
        "01234567890123456789012345678901234567890123456789" // 600
        "01234567890123456789012345678901234567890123456789" // 650
        "01234567890123456789012345678901234567890123456789" // 700
        "01234567890123456789012345678901234567890123456789" // 750
        "01234567890123456789012345678901234567890123456789" // 800
        "01234567890123456789012345678901234567890123456789" // 850
        "01234567890123456789012345678901234567890123456789" // 900
        "01234567890123456789012345678901234567890123456789" // 950
        "01234567890123456789012345678901234567890123456789" // 1000
        "012345678901234567890123" // 1024
    }
  }
};