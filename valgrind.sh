#!/bin/bash

mkdir -p valgrind-results
rm -rf valgrind-results
valgrind --suppressions=valgrind.supp --xml=yes --xml-file=valgrind-results/osal.test.xml output/debug/bin/osal.test
valgrind --suppressions=valgrind.supp --xml=yes --xml-file=valgrind-results/core.test.xml output/debug/bin/core.test
valgrind --suppressions=valgrind.supp --xml=yes --xml-file=valgrind-results/crypto.test.xml output/debug/bin/crypto.test
valgrind --suppressions=valgrind.supp --xml=yes --xml-file=valgrind-results/json.test.xml output/debug/bin/json.test
valgrind --suppressions=valgrind.supp --xml=yes --xml-file=valgrind-results/unit-test-c++.test.xml output/debug/bin/unit-test-c++.test
valgrind --suppressions=valgrind.supp --xml=yes --xml-file=valgrind-results/xml.test.xml output/debug/bin/xml.test
valgrind --suppressions=valgrind.supp --xml=yes --xml-file=valgrind-results/network.test.xml output/debug/bin/network.test

