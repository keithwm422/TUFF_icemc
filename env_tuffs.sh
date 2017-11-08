#!/bin/bash

export ANITA_UTIL_INSTALL_DIR=/home/mcbride.342/anitaBuildTool/Utils
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ANITA_UTIL_INSTALL_DIR/lib
export ICEMC_SRC_DIR=/home/mcbride.342/TUFF_icemc/icemc
export ICEMC_BUILD_DIR=/home/mcbride.342/TUFF_icemc/icemc
export LD_LIBRARY_PATH=${ICEMC_BUILD_DIR}:${LD_LIBRARY_PATH}
export ANITA_ROOT_DATA=/home/mcbride.342/TUFF_icemc/icemc/

