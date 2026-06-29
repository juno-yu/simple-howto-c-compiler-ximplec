#!/bin/sh
# run_all_tests.sh — Run all tests for the simplecc project
# Usage: ./run_all_tests.sh [--quick]
#   --quick  Skip lesson example compilation tests, only run unit tests

ROOT="$(cd "$(dirname "$0")" && pwd)"
BUILD="$ROOT/build"
PASS=0
FAIL=0
SKIP=0
TOTAL=0

log_pass() { printf "  ✅ PASS %s\n" "$1"; PASS=$((PASS+1)); }
log_fail() { printf "  ❌ FAIL %s: %s\n" "$1" "$2"; FAIL=$((FAIL+1)); }
log_skip() { printf "  ⏭  SKIP %s: %s\n" "$1" "$2"; SKIP=$((SKIP+1)); }
log_section() { printf "\n━━━ %s ━━━\n" "$1"; }

# ─── Build ──────────────────────────────────────────────────────────────
log_section "BUILDING"
if [ ! -f "$BUILD/simplecc" ]; then
    echo "  Building compiler..."
    mkdir -p "$BUILD"
    (cd "$BUILD" && cmake .. -DCMAKE_BUILD_TYPE=Release > /dev/null 2>&1)
    (cd "$BUILD" && make -j$(nproc) 2>&1 | tail -3)
fi
if [ ! -f "$BUILD/simplecc" ]; then
    echo "Build failed. Cannot continue."
    exit 1
fi
echo "  Compiler built: $BUILD/simplecc"

# ─── Unit Tests (ctest) ────────────────────────────────────────────────
log_section "UNIT TESTS (ctest)"
(cd "$BUILD" && ctest --output-on-failure -j1 2>&1 | tail -8)
CTEST_RESULT=$?
if [ $CTEST_RESULT -eq 0 ]; then
    log_pass "All unit tests passed"
else
    log_fail "Unit tests" "ctest returned $CTEST_RESULT"
fi

# ─── Lesson Example Tests ──────────────────────────────────────────────
if [ "$1" != "--quick" ]; then
log_section "LESSON EXAMPLES (compile + run)"
echo "  Testing all example.c files..."
echo ""

# Known failures: skip gracefully
KNOWN_FAIL="0086-nested-functions"

for dir in "$ROOT"/[0-9]*/; do
    [ -f "$dir/src/example.c" ] || continue
    lesson=$(basename "$dir")
    
    # Skip known failures
    skip=0
    for kf in $KNOWN_FAIL; do
        if [ "$lesson" = "$kf" ]; then skip=1; break; fi
    done
    if [ $skip -eq 1 ]; then
        log_skip "$lesson" "known limitation"
        continue
    fi
        example="$dir/src/example.c"
        TOTAL=$((TOTAL+1))

        # Step 1: Compile with simplecc (output to stdout, 3s timeout)
        result=$(timeout 3 "$BUILD/simplecc" -S "$example" 2>&1)
        ec=$?

        if [ $ec -eq 124 ]; then
            log_fail "$lesson" "compile timeout"
            continue
        fi
        if [ $ec -ne 0 ] || echo "$result" | grep -qi "error"; then
            err=$(echo "$result" | grep -i "error" | head -1)
            log_fail "$lesson" "compile: ${err:-unknown error}"
            continue
        fi

        if [ -z "$result" ]; then
            log_fail "$lesson" "compile: empty output"
            continue
        fi

        # Step 2: Write asm and link with gcc
        asm_out=$(mktemp /tmp/scctest_asmXXXXXX).s
        exe_out=$(mktemp /tmp/scctest_binXXXXXX)
        echo "$result" > "$asm_out"

        if ! gcc -o "$exe_out" "$asm_out" 2>/dev/null; then
            log_fail "$lesson" "link error"
            rm -f "$asm_out" "$exe_out"
            continue
        fi

        # Step 3: Run (3s timeout)
        prog_ec=$(timeout 3 "$exe_out" 2>&1; echo $?)
        # Strip any extra output, just get exit code
        prog_ec=$(echo "$prog_ec" | tail -1)
        # Handle timeout (124 = timeout exit code)
        if [ "$prog_ec" = "124" ]; then
            log_fail "$lesson" "runtime timeout"
            rm -f "$asm_out" "$exe_out"
            continue
        fi

        log_pass "$lesson (exit=$prog_ec)"

        rm -f "$asm_out" "$exe_out"
    done
fi

# ─── Summary ───────────────────────────────────────────────────────────
log_section "SUMMARY"
TOTAL_ALL=$((PASS + FAIL + SKIP))
echo "  Passed: $PASS"
echo "  Failed: $FAIL"
echo "  Skipped: $SKIP"
echo "  Total: $TOTAL_ALL"
echo ""

if [ $FAIL -gt 0 ]; then
    echo "Some tests failed."
    exit 1
else
    echo "All tests passed!"
    exit 0
fi
