#!/bin/sh
# Run a test with a timeout. Convert timeouts/segfaults to non-zero exit so
# ctest's WILL_FAIL property can handle them as "expected failure".
TEST=$1
TIMEOUT=${2:-30}
timeout -k 2 $TIMEOUT $TEST >/dev/null 2>&1
EXIT=$?
# 124 = timeout from `timeout` command
# 137 = killed by signal 9 (OOM kill or kill -9)
# 139 = killed by signal 11 (segfault)  
# 134 = killed by signal 6 (abort)
# Convert these to a normal failure exit code
if [ $EXIT -eq 124 ] || [ $EXIT -ge 128 ]; then
    exit 1
fi
exit $EXIT
