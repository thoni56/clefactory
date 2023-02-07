# Coverage for tests to Coveralls, but only for linux
lcov -q -d src -c -o coverage.info
lcov -q --remove coverage.info '*.mock' '/usr/*' -o coverage.info
coveralls-lcov coverage.info
