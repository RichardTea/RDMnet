################################ RDMnet library ###############################

include(CMakePackageConfigHelpers)

################################## Config file ################################

if(DEFINED RDMNET_CONFIG_LOC)
  get_filename_component(RDMNET_CONFIG_LOC ${RDMNET_CONFIG_LOC}
    ABSOLUTE
    BASE_DIR ${CMAKE_BINARY_DIR}
  )
endif()

################################### Library ###################################

# RDMnet sources and headers are divided logically into groups - these groups
# are used to create interface libraries which separate portions of RDMnet for
# unit testing. They are then combined into the encompassing RDMnet library for
# external consumption.

# The "RDMnet API" layer: Top-level public API logic called by applications.

set(RDMNET_API_PUBLIC_HEADERS
  ${RDMNET_INCLUDE}/rdmnet/client.h
  ${RDMNET_INCLUDE}/rdmnet/controller.h
  ${RDMNET_INCLUDE}/rdmnet/defs.h
  ${RDMNET_INCLUDE}/rdmnet/device.h
  ${RDMNET_INCLUDE}/rdmnet/version.h
)
set(RDMNET_API_PRIVATE_HEADERS
  ${RDMNET_SRC}/rdmnet/private/client.h
  ${RDMNET_SRC}/rdmnet/private/controller.h
  ${RDMNET_SRC}/rdmnet/private/device.h
)
set(RDMNET_API_SOURCES
  ${RDMNET_SRC}/rdmnet/client.c
  ${RDMNET_SRC}/rdmnet/controller.c
  ${RDMNET_SRC}/rdmnet/device.c
)

# This target should not be used externally - it is only for use by the automated tests.
add_library(RDMnetApi INTERFACE)
target_sources(RDMnetApi INTERFACE
  ${RDMNET_API_PUBLIC_HEADERS}
  ${RDMNET_API_PRIVATE_HEADERS}
  ${RDMNET_API_SOURCES}
)
target_include_directories(RDMnetApi INTERFACE ${RDMNET_INCLUDE} ${RDMNET_SRC})

# The "RDMnet Core" layer: Lower-level logic shared by the Client APIs and the
# Broker library.

set(RDMNET_CORE_PUBLIC_HEADERS
  ${RDMNET_INCLUDE}/rdmnet/core.h
  ${RDMNET_INCLUDE}/rdmnet/core/broker_prot.h
  ${RDMNET_INCLUDE}/rdmnet/core/connection.h
  ${RDMNET_INCLUDE}/rdmnet/core/ept_prot.h
  ${RDMNET_INCLUDE}/rdmnet/core/llrp.h
  ${RDMNET_INCLUDE}/rdmnet/core/llrp_manager.h
  ${RDMNET_INCLUDE}/rdmnet/core/llrp_target.h
  ${RDMNET_INCLUDE}/rdmnet/core/message.h
  ${RDMNET_INCLUDE}/rdmnet/core/rpt_prot.h
  ${RDMNET_INCLUDE}/rdmnet/core/util.h
)
set(RDMNET_CORE_PRIVATE_HEADERS
  ${RDMNET_SRC}/rdmnet/private/broker_prot.h
  ${RDMNET_SRC}/rdmnet/private/core.h
  ${RDMNET_SRC}/rdmnet/private/connection.h
  ${RDMNET_SRC}/rdmnet/private/llrp.h
  ${RDMNET_SRC}/rdmnet/private/llrp_manager.h
  ${RDMNET_SRC}/rdmnet/private/llrp_prot.h
  ${RDMNET_SRC}/rdmnet/private/llrp_target.h
  ${RDMNET_SRC}/rdmnet/private/mcast.h
  ${RDMNET_SRC}/rdmnet/private/message.h
  ${RDMNET_SRC}/rdmnet/private/msg_buf.h
  ${RDMNET_SRC}/rdmnet/private/opts.h
  ${RDMNET_SRC}/rdmnet/private/rpt_prot.h
  ${RDMNET_SRC}/rdmnet/private/util.h
)
set(RDMNET_CORE_SOURCES
  ${RDMNET_SRC}/rdmnet/core.c
  ${RDMNET_SRC}/rdmnet/core/broker_prot.c
  ${RDMNET_SRC}/rdmnet/core/client_entry.c
  ${RDMNET_SRC}/rdmnet/core/connection.c
  ${RDMNET_SRC}/rdmnet/core/llrp.c
  ${RDMNET_SRC}/rdmnet/core/llrp_manager.c
  ${RDMNET_SRC}/rdmnet/core/llrp_prot.c
  ${RDMNET_SRC}/rdmnet/core/llrp_target.c
  ${RDMNET_SRC}/rdmnet/core/mcast.c
  ${RDMNET_SRC}/rdmnet/core/message.c
  ${RDMNET_SRC}/rdmnet/core/msg_buf.c
  ${RDMNET_SRC}/rdmnet/core/rpt_prot.c
  ${RDMNET_SRC}/rdmnet/core/util.c
)

# This target should not be used externally - it is only for use by the automated tests.
add_library(RDMnetCore INTERFACE)
target_sources(RDMnetCore INTERFACE
  ${RDMNET_CORE_PUBLIC_HEADERS}
  ${RDMNET_CORE_PRIVATE_HEADERS}
  ${RDMNET_CORE_SOURCES}
)
target_include_directories(RDMnetCore INTERFACE ${RDMNET_INCLUDE} ${RDMNET_SRC})

# Combination variables for convenience

set(RDMNET_LIB_PUBLIC_HEADERS 
  ${RDMNET_API_PUBLIC_HEADERS}
  ${RDMNET_CORE_PUBLIC_HEADERS}
)
set(RDMNET_LIB_PRIVATE_HEADERS
  ${RDMNET_API_PRIVATE_HEADERS}
  ${RDMNET_CORE_PRIVATE_HEADERS}
)
set(RDMNET_LIB_SOURCES
  ${RDMNET_API_SOURCES}
  ${RDMNET_CORE_SOURCES}
)

# The main RDMnet library
add_library(RDMnet
  ${RDMNET_LIB_PUBLIC_HEADERS}
  ${RDMNET_LIB_PRIVATE_HEADERS}
  ${RDMNET_LIB_SOURCES}
)
target_include_directories(RDMnet
  PUBLIC ${RDMNET_INCLUDE}
  PRIVATE ${RDMNET_SRC}
)
if(DEFINED RDMNET_CONFIG_LOC)
  target_include_directories(RDMnet PRIVATE ${RDMNET_CONFIG_LOC})
  target_compile_definitions(RDMnet PRIVATE RDMNET_HAVE_CONFIG_H)
endif()
target_link_libraries(RDMnet PUBLIC EtcPal RDM)

# Organize sources in IDEs
source_group(TREE ${RDMNET_SRC}/rdmnet PREFIX src FILES
  ${RDMNET_API_SOURCES}
  ${RDMNET_CORE_SOURCES}
  ${RDMNET_API_PRIVATE_HEADERS}
  ${RDMNET_CORE_PRIVATE_HEADERS}
  ${RDMNET_DISC_COMMON_SOURCES}
  ${RDMNET_DISC_PLATFORM_SOURCES}
)
source_group(TREE ${RDMNET_INCLUDE}/rdmnet PREFIX include FILES
  ${RDMNET_API_PUBLIC_HEADERS}
  ${RDMNET_CORE_PUBLIC_HEADERS}
  ${RDMNET_DISC_PUBLIC_HEADERS}
)

target_link_libraries(RDMnet PRIVATE RDMnetDiscoveryCommon RDMnetDiscoveryPlatform dnssd)

# Installation
install(TARGETS RDMnet
#  EXPORT RDMnet
  ARCHIVE DESTINATION lib
#  INCLUDES DESTINATION include
)
install(DIRECTORY ${RDMNET_ROOT}/include/rdmnet DESTINATION include FILES_MATCHING PATTERN "*.h")

#################################### Broker ###################################

if(WIN32 OR APPLE OR UNIX)
  set(RDMNET_BROKER_PUBLIC_HEADERS
    ${RDMNET_INCLUDE}/rdmnet/broker.h
    ${RDMNET_INCLUDE}/rdmnet/broker/log.h
    ${RDMNET_INCLUDE}/rdmnet/broker/settings.h
  )
  set(RDMNET_BROKER_PRIVATE_HEADERS
    ${RDMNET_SRC}/rdmnet/broker/broker_core.h
    ${RDMNET_SRC}/rdmnet/broker/broker_client.h
    ${RDMNET_SRC}/rdmnet/broker/broker_discovery.h
    ${RDMNET_SRC}/rdmnet/broker/broker_responder.h
    ${RDMNET_SRC}/rdmnet/broker/broker_socket_manager.h
    ${RDMNET_SRC}/rdmnet/broker/broker_threads.h
    ${RDMNET_SRC}/rdmnet/broker/broker_uid_manager.h
    ${RDMNET_SRC}/rdmnet/broker/broker_util.h
    ${RDMNET_SRC}/rdmnet/broker/rdmnet_conn_wrapper.h
  )
  set(RDMNET_BROKER_SOURCES
    ${RDMNET_SRC}/rdmnet/broker/broker_api.cpp
    ${RDMNET_SRC}/rdmnet/broker/broker_core.cpp
    ${RDMNET_SRC}/rdmnet/broker/broker_client.cpp
    ${RDMNET_SRC}/rdmnet/broker/broker_discovery.cpp
    ${RDMNET_SRC}/rdmnet/broker/broker_log.cpp
    ${RDMNET_SRC}/rdmnet/broker/broker_responder.cpp
    ${RDMNET_SRC}/rdmnet/broker/broker_threads.cpp
    ${RDMNET_SRC}/rdmnet/broker/broker_uid_manager.cpp
    ${RDMNET_SRC}/rdmnet/broker/broker_util.cpp
    ${RDMNET_SRC}/rdmnet/broker/rdmnet_conn_wrapper.cpp
  )

  if(WIN32)
    set(RDMNET_BROKER_PRIVATE_HEADERS ${RDMNET_BROKER_PRIVATE_HEADERS}
      ${RDMNET_SRC}/rdmnet/broker/windows/win_socket_manager.h
    )
    set(RDMNET_BROKER_SOURCES ${RDMNET_BROKER_SOURCES}
      ${RDMNET_SRC}/rdmnet/broker/windows/win_socket_manager.cpp
    )
  elseif(APPLE)
    set(RDMNET_BROKER_PRIVATE_HEADERS ${RDMNET_BROKER_PRIVATE_HEADERS}
      ${RDMNET_SRC}/rdmnet/broker/macos/macos_socket_manager.h
    )
    set(RDMNET_BROKER_SOURCES ${RDMNET_BROKER_SOURCES}
      ${RDMNET_SRC}/rdmnet/broker/macos/macos_socket_manager.cpp
    )
  elseif(UNIX)
    set(RDMNET_BROKER_PRIVATE_HEADERS ${RDMNET_BROKER_PRIVATE_HEADERS}
      ${RDMNET_SRC}/rdmnet/broker/linux/linux_socket_manager.h
    )
    set(RDMNET_BROKER_SOURCES ${RDMNET_BROKER_SOURCES}
      ${RDMNET_SRC}/rdmnet/broker/linux/linux_socket_manager.cpp
    )
  endif()

  # This target should not be used externally - it is only for use by the automated tests.
  add_library(RDMnetBrokerCore INTERFACE)
  target_sources(RDMnetBrokerCore INTERFACE
    ${RDMNET_BROKER_PUBLIC_HEADERS}
    ${RDMNET_BROKER_PRIVATE_HEADERS}
    ${RDMNET_BROKER_SOURCES}
  )
  target_include_directories(RDMnetBrokerCore INTERFACE ${RDMNET_INCLUDE} ${RDMNET_SRC})

  add_library(RDMnetBroker
    ${RDMNET_BROKER_PUBLIC_HEADERS}
    ${RDMNET_BROKER_PRIVATE_HEADERS}
    ${RDMNET_BROKER_SOURCES}
  )

  target_include_directories(RDMnetBroker PUBLIC ${RDMNET_INCLUDE} broker PRIVATE ${RDMNET_SRC})
  target_link_libraries(RDMnetBroker PUBLIC RDMnet)
  set_target_properties(RDMnetBroker PROPERTIES CXX_STANDARD 14)

  # Organize sources in IDEs
  source_group(TREE ${RDMNET_SRC}/rdmnet PREFIX src FILES
    ${RDMNET_BROKER_PRIVATE_HEADERS}
    ${RDMNET_BROKER_SOURCES}
  )
  source_group(TREE ${RDMNET_INCLUDE}/rdmnet PREFIX include FILES
    ${RDMNET_BROKER_PUBLIC_HEADERS}
  )


  install(TARGETS RDMnetBroker ARCHIVE DESTINATION lib)

endif()


# TODO

# configure_package_config_file(
#   ${RDMNET_ROOT}/tools/cmake/RDMnetConfig.cmake.in
#   ${CMAKE_CURRENT_BINARY_DIR}/RDMnetConfig.cmake
#   INSTALL_DESTINATION lib/cmake
# )
# write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/RDMnetConfigVersion.cmake
#   VERSION ${PROJECT_VERSION}
#   COMPATIBILITY SameMajorVersion
# )
# install(FILES
#   ${CMAKE_CURRENT_BINARY_DIR}/RDMnetConfig.cmake
#   ${CMAKE_CURRENT_BINARY_DIR}/RDMnetConfigVersion.cmake
#   DESTINATION lib/cmake
# )
# install(EXPORT RDMnet DESTINATION lib/cmake)
