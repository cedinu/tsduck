//----------------------------------------------------------------------------
//
// TSDuck - The MPEG Transport Stream Toolkit
// Copyright (c) 2005-2020, Thierry Lelegard
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
// THE POSSIBILITY OF SUCH DAMAGE.
//
//----------------------------------------------------------------------------
//!
//!  @file
//!  Information about version identification of TSDuck.
//!
//----------------------------------------------------------------------------

#pragma once
#include "tsEnumeration.h"
#include "tsReport.h"
#include "tsThread.h"

//!
//! TSDuck namespace, containing all TSDuck classes and functions.
//!
namespace ts {
    //!
    //! Information about version identification of TSDuck.
    //! @ingroup app
    //!
    class TSDUCKDLL VersionInfo : private Thread
    {
        TS_NOBUILD_NOCOPY(VersionInfo);
    public:
        //!
        //! Default constructor.
        //! @param [in,out] report Reference to the report object through which new versions will be reported.
        //!
        VersionInfo(Report& report);

        //!
        //! Destructor.
        //!
        virtual ~VersionInfo();

        //!
        //! Start a thread which checks the availability of a new TSDuck version.
        //!
        //! If a new version is found, it is reported through the Report object that was specified in the constructor.
        //! This can be done only once, further calls are ignored.
        //! The destructor of this object waits for the completion of the thread.
        //!
        //! If the environment variable TSDUCK_NO_VERSION_CHECK is not empty, do not start the new version check.
        //! To debug new version checking, define the environment variable TS_DEBUG_NEW_VERSION to any non-empty
        //! value; debug and errors messages will be reported through the same Report object.
        //!
        void startNewVersionDetection();

        //!
        //! Types of version formatting, for predefined option -\-version.
        //!
        enum class Format {
            SHORT,    //!< Short format X.Y-R.
            LONG,     //!< Full explanatory format.
            INTEGER,  //!< Integer format XXYYRRRRR.
            DATE,     //!< Build date.
            NSIS,     //!< Output NSIS @c !define directives.
            DEKTEC,   //!< Version of embedded Dektec DTAPI and detected Dektec drivers.
            HTTP,     //!< Version of HTTP library which is used.
            COMPILER, //!< Version of the compiler which was used to build the code.
            SRT,      //!< Version of SRT library which is used.
            ALL,      //!< Multi-line output with full details.
        };

        //!
        //! Enumeration description of class Format.
        //! Typically used to implement the -\-version command line option.
        //!
        static const Enumeration FormatEnum;

        //!
        //! Get the TSDuck formatted version number.
        //! @param [in] format Type of output, short by default.
        //! @param [in] applicationName Name of the application to prepend to the long format.
        //! @return The formatted version string.
        //!
        static UString GetVersion(Format format = Format::SHORT, const UString& applicationName = UString());

        //!
        //! Compare two version strings.
        //! @param [in] v1 First version string.
        //! @param [in] v2 First version string.
        //! @return One of -1, 0, 1 when @a v1 < @a v2, @a v1 == @a v2, @a v1 > @a v2.
        //!
        static int CompareVersions(const UString& v1, const UString& v2);

    private:
        Report& _report;
        Report& _debug;
        bool    _started;

        // Inherited from Thread.
        virtual void main() override;

        // Build a string representing the version of the compiler which built this module.
        static ts::UString GetCompilerVersion();

        // Convert a version string into a vector of integers.
        static void VersionToInts(std::vector<int>& ints, const ts::UString& version);
    };
}

extern "C" {
    //! Major version of the TSDuck library.
    TSDUCKDLL extern const int tsduckLibraryVersionMajor;
    //! Minor version of the TSDuck library.
    TSDUCKDLL extern const int tsduckLibraryVersionMinor;
    //! Commit version of the TSDuck library.
    TSDUCKDLL extern const int tsduckLibraryVersionCommit;
}
