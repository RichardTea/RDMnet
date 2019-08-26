/******************************************************************************
************************* IMPORTANT NOTE -- READ ME!!! ************************
*******************************************************************************
* THIS SOFTWARE IMPLEMENTS A **DRAFT** STANDARD, BSR E1.33 REV. 77. UNDER NO
* CIRCUMSTANCES SHOULD THIS SOFTWARE BE USED FOR ANY PRODUCT AVAILABLE FOR
* GENERAL SALE TO THE PUBLIC. DUE TO THE INEVITABLE CHANGE OF DRAFT PROTOCOL
* VALUES AND BEHAVIORAL REQUIREMENTS, PRODUCTS USING THIS SOFTWARE WILL **NOT**
* BE INTEROPERABLE WITH PRODUCTS IMPLEMENTING THE FINAL RATIFIED STANDARD.
*******************************************************************************
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
*******************************************************************************
* This file is a part of RDMnet. For more information, go to:
* https://github.com/ETCLabs/RDMnet
******************************************************************************/

#include <string>
#include <iostream>
#include <ctime>

#include "lwpa/uuid.h"
#include "manager.h"

extern "C" {
static void manager_log_callback(void *context, const LwpaLogStrings *strings)
{
  (void)context;
  std::cout << strings->human_readable << "\n";
}

static void manager_time_callback(void *context, LwpaLogTimeParams *time_params)
{
  time_t t = time(NULL);
  struct tm *local_time = localtime(&t);
  time_params->year = local_time->tm_year + 1900;
  time_params->month = local_time->tm_mon + 1;
  time_params->day = local_time->tm_mday;
  time_params->hour = local_time->tm_hour;
  time_params->minute = local_time->tm_min;
  time_params->second = local_time->tm_sec;
  time_params->msec = 0;
  time_params->utc_offset = (int)(local_time->tm_gmtoff / 60);
}
}

int main(int /*argc*/, char * /*argv*/ [])
{
  LwpaUuid manager_cid;
  lwpa_generate_os_preferred_uuid(&manager_cid);

  LwpaLogParams params;
  params.action = kLwpaLogCreateHumanReadableLog;
  params.log_fn = manager_log_callback;
  params.log_mask = LWPA_LOG_UPTO(LWPA_LOG_INFO);
  params.time_fn = manager_time_callback;
  params.context = nullptr;

  lwpa_validate_log_params(&params);
  LLRPManager mgr(manager_cid, &params);
  printf("Discovered network interfaces:\n");
  mgr.PrintNetints();
  mgr.PrintCommandList();

  std::string input;
  while (true)
  {
    std::getline(std::cin, input);
    if (!mgr.ParseCommand(input))
      break;
  }
  return 0;
}