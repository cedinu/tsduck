#!/usr/bin/env python3
#----------------------------------------------------------------------------
#
# TSDuck sample Python application running a chain of plugins on the long
# run with CPU and memory monitoring.
#
#----------------------------------------------------------------------------

import ts
import os
import tempfile

# Create an asynchronous report to log multi-threaded messages.
rep = ts.AsyncReport(severity = ts.Report.VERBOSE)

# Build a temporary file name to download a real TS file.
url = "https://tsduck.io/streams/france-dttv/tnt-uhf30-546MHz-2019-01-22.ts"
tsfile = tempfile.gettempdir() + os.path.sep + tempfile.gettempprefix() + str(os.getpid()) + ".ts"

# First phase: Download the TS file:
print("Downloading %s to %s ..." % (url, tsfile))

tsp = ts.TSProcessor(rep)
tsp.input = ['http', url]
tsp.output = ['file', tsfile]
tsp.start()
tsp.waitForTermination()

# Second phase: Play the file at regulated speed a large number of times.
# Must use another instance of ts.TSProcessor.
print("Playing %s ..." % (tsfile))

tsp = ts.TSProcessor(rep)
tsp.monitor = True
tsp.input = ['file', tsfile, '--repeat', '100']
tsp.plugins = [ ['regulate'] ]
tsp.output = ['drop']
tsp.start()
tsp.waitForTermination()

# Terminate asynchronous log.
tsp = None
rep.terminate()

# Delete temporary TS file.
os.remove(tsfile)
