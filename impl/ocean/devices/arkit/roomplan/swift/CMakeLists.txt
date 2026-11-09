# Copyright (c) Meta Platforms, Inc. and affiliates.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

cmake_minimum_required(VERSION 3.26)

if (IOS)

    set(OCEAN_TARGET_NAME "ocean_devices_arkit_roomplan_swift")

    find_library(ARKIT_LIB ARKit)
    find_library(COREMOTION_LIB CoreMotion)
    find_library(ROOMPLAN_LIB RoomPlan)

    # Source files
    file(GLOB OCEAN_TARGET_HEADER_FILES "${CMAKE_CURRENT_LIST_DIR}/*.h")
    file(GLOB OCEAN_TARGET_SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/*.mm")
    file(GLOB OCEAN_TARGET_SWIFT_FILES "${CMAKE_CURRENT_LIST_DIR}/*.swift")

    # The Swift code sees the Objective-C classes through the bridging header,
    # and the Objective-C++ code sees the Swift classes through the generated interface header
    set(OCEAN_SWIFT_BRIDGING_HEADER "${CMAKE_CURRENT_LIST_DIR}/AKRoomPlanTracker6DOF_Objc.h")
    set(OCEAN_SWIFT_INTERFACE_HEADER_NAME "roomplan_swiftApple-Swift.h")

    if (CMAKE_GENERATOR STREQUAL "Xcode")

        # Xcode compiles the Swift files first and writes the interface header into the target's derived sources
        add_library(${OCEAN_TARGET_NAME} ${OCEAN_TARGET_SOURCE_FILES} ${OCEAN_TARGET_SWIFT_FILES} ${OCEAN_TARGET_HEADER_FILES})

        set_target_properties(${OCEAN_TARGET_NAME} PROPERTIES
            XCODE_ATTRIBUTE_SWIFT_OBJC_BRIDGING_HEADER "${OCEAN_SWIFT_BRIDGING_HEADER}"
            XCODE_ATTRIBUTE_SWIFT_OBJC_INTERFACE_HEADER_NAME "${OCEAN_SWIFT_INTERFACE_HEADER_NAME}"
        )

    else()

        # Other generators do not know that the Objective-C++ files need the interface header of the Swift files,
        # so a separate command writes the header, which the Objective-C++ files then depend on
        set(OCEAN_SWIFT_INTERFACE_HEADER "${CMAKE_CURRENT_BINARY_DIR}/${OCEAN_SWIFT_INTERFACE_HEADER_NAME}")

        set(OCEAN_SWIFT_TARGET_OPTIONS "")
        if (CMAKE_Swift_COMPILER_TARGET)
            set(OCEAN_SWIFT_TARGET_OPTIONS -target ${CMAKE_Swift_COMPILER_TARGET})
        endif()

        add_custom_command(
            OUTPUT "${OCEAN_SWIFT_INTERFACE_HEADER}"
            COMMAND "${CMAKE_Swift_COMPILER}" -typecheck -parse-as-library -module-name ${OCEAN_TARGET_NAME} ${OCEAN_SWIFT_TARGET_OPTIONS} -sdk "${CMAKE_OSX_SYSROOT}" -I "${OCEAN_IMPL_DIR}" -import-objc-header "${OCEAN_SWIFT_BRIDGING_HEADER}" -emit-objc-header-path "${OCEAN_SWIFT_INTERFACE_HEADER}" ${OCEAN_TARGET_SWIFT_FILES}
            DEPENDS ${OCEAN_TARGET_SWIFT_FILES} ${OCEAN_TARGET_HEADER_FILES}
            COMMENT "Generating the Objective-C interface header ${OCEAN_SWIFT_INTERFACE_HEADER_NAME}"
            VERBATIM
        )

        add_library(${OCEAN_TARGET_NAME} ${OCEAN_TARGET_SOURCE_FILES} ${OCEAN_TARGET_SWIFT_FILES} ${OCEAN_TARGET_HEADER_FILES} "${OCEAN_SWIFT_INTERFACE_HEADER}")

        target_compile_options(${OCEAN_TARGET_NAME} PRIVATE "$<$<COMPILE_LANGUAGE:Swift>:-import-objc-header;${OCEAN_SWIFT_BRIDGING_HEADER}>")

        target_include_directories(${OCEAN_TARGET_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")

    endif()

    target_include_directories(${OCEAN_TARGET_NAME} PUBLIC "${OCEAN_IMPL_DIR}")

    target_compile_definitions(${OCEAN_TARGET_NAME} PUBLIC ${OCEAN_PREPROCESSOR_FLAGS})

    target_compile_options(${OCEAN_TARGET_NAME} PUBLIC "${OCEAN_COMPILER_FLAGS}")

    if (NOT WIN32)
        target_compile_options(${OCEAN_TARGET_NAME} PRIVATE "$<$<NOT:$<COMPILE_LANGUAGE:Swift>>:-fexceptions>")
    endif()

    # Dependencies
    target_link_libraries(${OCEAN_TARGET_NAME}
        PUBLIC
            ocean_base
            ocean_devices
            ocean_devices_arkit_roomplan_native
            ocean_math
            ocean_media_avfoundation
    )

    target_link_libraries(${OCEAN_TARGET_NAME}
        PUBLIC
            ${ARKIT_LIB}
            ${COREMOTION_LIB}
            ${ROOMPLAN_LIB}
    )

    # Installation
    install(TARGETS ${OCEAN_TARGET_NAME}
            DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            COMPONENT lib
    )

    install(FILES ${OCEAN_TARGET_HEADER_FILES}
            DESTINATION include/ocean/devices/arkit/roomplan/swift
            COMPONENT include
    )

endif()
