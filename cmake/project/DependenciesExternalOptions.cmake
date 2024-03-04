# Copyright 2019-present, Joseph Garnier
# All rights reserved.
#
# This source code is licensed under the license found in the
# LICENSE file in the root directory of this source tree.
# =============================================================================
# What Is This?
# -------------
# See README file in the root directory of this source tree.


#------------------------------------------------------------------------------
# Import and link external libraries from here.
#------------------------------------------------------------------------------

include(CPM)

CPMAddPackage("gh:glfw/glfw#3.3.9")
CPMAddPackage(
  NAME assimp
  GITHUB_REPOSITORY assimp/assimp
  GIT_TAG v5.3.1
  OPTIONS "ASSIMP_WARNINGS_AS_ERRORS off"
)
CPMAddPackage("gh:nlohmann/json@3.10.5")

target_link_libraries("${${PROJECT_NAME}_MAIN_BIN_TARGET}"
  PUBLIC
    "$<BUILD_INTERFACE:glfw;assimp;nlohmann_json::nlohmann_json>"
    "$<INSTALL_INTERFACE:glfw;assimp;nlohmann_json::nlohmann_json>"
)
