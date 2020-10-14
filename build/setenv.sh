#!/usr/bin/env bash
#-----------------------------------------------------------------------------
#
#  TSDuck - The MPEG Transport Stream Toolkit
#  Copyright (c) 2005-2020, Thierry Lelegard
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#
#  1. Redistributions of source code must retain the above copyright notice,
#     this list of conditions and the following disclaimer.
#  2. Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
#  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
#  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
#  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
#  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
#  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
#  THE POSSIBILITY OF SUCH DAMAGE.
#
#-----------------------------------------------------------------------------
#
#  This script builds the name of the directory which contains binaries.
#  The typical usage is to 'source' it: it adds the binary directory to
#  the path. Other options:
#
#     --display : only display the binary directory, don't set PATH
#     --debug : use debug build
#
#-----------------------------------------------------------------------------

# Default options.
TARGET=release
SHOW_PATH=false

# Decode command line options.
while [[ $# -gt 0 ]]; do
    case "$1" in
        --debug)
            TARGET=debug
            ;;
        --display)
            SHOW_PATH=true
            ;;
    esac
    shift
done

# Build binary directory.
ROOTDIR=$(cd $(dirname "${BASH_SOURCE[0]}")/..; pwd)
ARCH=$(uname -m | sed -e 's/i.86/i386/' -e 's/^arm.*$/arm/')
HOST=$(hostname | sed -e 's/\..*//')
BINDIR="$ROOTDIR/bin/$TARGET-$ARCH-$HOST"
TSPYDIR="$ROOTDIR/src/libtsduck/python"

# Display or set path.
if $SHOW_PATH; then
    echo "$BINDIR"
else
    [[ ":$PATH:" != *:$BINDIR:* ]] && export PATH="$BINDIR:$PATH"
    [[ ":$TSPLUGINS_PATH:" != *:$BINDIR:* ]] && export TSPLUGINS_PATH="$BINDIR:$TSPLUGINS_PATH"
    [[ ":$LD_LIBRARY_PATH:" != *:$BINDIR:* ]] && export LD_LIBRARY_PATH="$BINDIR:$LD_LIBRARY_PATH"
    [[ ":$PYTHONPATH:" != *:$TSPYDIR:* ]] && export PYTHONPATH="$TSPYDIR:$PYTHONPATH"
    # For macOS only: LD_LIBRARY_PATH is not passed to shell-scripts for security reasons.
    # Define a backup version which can be explicitly checked in scripts (typically Python bindings).
    export LD_LIBRARY_PATH2="$LD_LIBRARY_PATH"
fi

# Make sure to exit with success status
true
