/******************************************************************************
************************* IMPORTANT NOTE -- READ ME!!! ************************
*******************************************************************************
* THIS SOFTWARE IMPLEMENTS A **DRAFT** STANDARD, BSR E1.33 REV. 63. UNDER NO
* CIRCUMSTANCES SHOULD THIS SOFTWARE BE USED FOR ANY PRODUCT AVAILABLE FOR
* GENERAL SALE TO THE PUBLIC. DUE TO THE INEVITABLE CHANGE OF DRAFT PROTOCOL
* VALUES AND BEHAVIORAL REQUIREMENTS, PRODUCTS USING THIS SOFTWARE WILL **NOT**
* BE INTEROPERABLE WITH PRODUCTS IMPLEMENTING THE FINAL RATIFIED STANDARD.
*******************************************************************************
* Copyright 2018 ETC Inc.
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

/*! \file rdmnet/opts.h
 *  \brief RDMnet configuration options.
 *
 *  Default values for all of RDMnet's \ref rdmnetopts
 *  "compile-time configuration options".
 *
 *  \author Sam Kearney
 */
#ifndef _RDMNET_OPTS_H_
#define _RDMNET_OPTS_H_

/*! \defgroup rdmnetopts RDMnet Configuration Options
 *  \brief Compile-time configuration options for RDMnet.
 *
 *  Default values can be overriden by defining the option in your project's
 *  rdmnet_config.h file. Default values for platform-specific options are
 *  contained in rdmnet_platform_defaults.h, which lives with each platform in
 *  the port folder.
 */

/* User configuration options. Any non-defined ones will get their defaults in
 * this file. */
#include "rdmnet_config.h"
#include "rdmnet_platform_defaults.h"

/********************************* Global ************************************/

/*! \defgroup rdmnetopts_global Global
 *  \ingroup rdmnetopts
 *
 *  Global or library-wide configuration options.
 *  @{
 */

/*! \brief Use dynamic memory allocation.
 *
 *  If defined nonzero, RDMnet manages memory dynamically using malloc() and
 *  free() from stdlib.h. Otherwise, RDMnet uses static arrays and fixed-size
 *  pools through \ref lwpa_mempool. The size of the pools is controlled with
 *  other config options.
 *
 *  This option is typically overridden in rdmnet_platform_defaults.h;
 *  application-level platforms such as Windows define it to 1, and embedded
 *  platforms define it to 0. */
#ifndef RDMNET_DYNAMIC_MEM
#define RDMNET_DYNAMIC_MEM 0
#endif

/*! \brief The maximum number of RDMnet connections that can be created.
 *
 *  Meaningful only if #RDMNET_DYNAMIC_MEM is defined to 0.
 */
#ifndef RDMNET_MAX_CONNECTIONS
#define RDMNET_MAX_CONNECTIONS 2
#endif

/*! \brief Spawn a thread internally to call rdmnet_tick().
 *
 *  If defined nonzero, rdmnet_init() will create a thread using
 *  \ref lwpa_thread which calls rdmnet_tick() periodically. The thread will be
 *  created using #RDMNET_TICK_THREAD_PRIORITY and
 *  #RDMNET_TICK_THREAD_STACK. The thread will be stopped by
 *  rdmnet_deinit().
 *
 *  If defined zero, the function declaration of rdmnet_tick() will be exposed
 *  in rdmnet/connection.h and it must be called by the application as
 *  specified in that function's documentation. */
#ifndef RDMNET_USE_TICK_THREAD
#define RDMNET_USE_TICK_THREAD 1
#endif

/*! \brief The amount of time the tick thread sleeps between calls to
 *         rdmnet_tick().
 *
 *  Meaningful only if #RDMNET_USE_TICK_THREAD is defined to 1. */
#ifndef RDMNET_TICK_THREAD_SLEEP_MS
#define RDMNET_TICK_THREAD_SLEEP_MS 1000
#endif

/*! \brief The size of the internal receive buffer used by the RDMnet stream
 *         parser. */
#ifndef RDMNET_RECV_BUF_SIZE
#define RDMNET_RECV_BUF_SIZE 1000 /* TODO find the real number */
#else
#if (RDMNET_RECV_BUF_SIZE < 1000) /* TODO find the real number */
#undef RDMNET_RECV_BUF_SIZE
#define RDMNET_RECV_BUF_SIZE 1000
#endif
#endif

/*!@}*/

/*! \defgroup rdmnetopts_llrp LLRP
 *  \ingroup rdmnetopts
 *
 *  Configuration options for LLRP.
 *  @{
 */

/*! \brief The maximum number of LLRP sockets that can be created.
 *
 *  Meaningful only if #RDMNET_DYNAMIC_MEM is defined to 0.
 */
#ifndef LLRP_MAX_SOCKETS
#define LLRP_MAX_SOCKETS 2
#endif

/*! \brief In LLRP, whether to bind the underlying network socket directly to
 *         the LLRP multicast address.
 *
 *  Otherwise, the socket is bound to INADDR_ANY. On some systems, binding
 *  directly to a multicast address decreases traffic duplication. On other
 *  systems, it's not even allowed. Leave this option at its default value
 *  unless you REALLY know what you're doing.
 */
#ifndef LLRP_BIND_TO_MCAST_ADDRESS
#define LLRP_BIND_TO_MCAST_ADDRESS 1
#endif

/*!@} */

#endif /* _RDMNET_OPTS_H_ */