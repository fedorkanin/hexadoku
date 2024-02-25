#!/bin/bash

PROGRAMS=("./bin/main_dev.out" "./bin/main_release.out")
TESTS_DIRS=("data/basic" "data/extra")

clean_up() {
	rm -f time.txt test_out.txt
}

run_tests() {
	local program="$1"
	local tests_dir="$2"
	echo "Testing ${program} in directory ${tests_dir}"

	for IN_FILE in "${tests_dir}"/*_in.txt; do
		REF_FILE="${IN_FILE/_in.txt/_out.txt}"
		{ time "${program}" <"${IN_FILE}" >test_out.txt; } 2>time.txt
		if ! diff "${REF_FILE}" test_out.txt >/dev/null; then
			echo "Test FAILED: ${IN_FILE}"
			echo "Reference file: ${REF_FILE}"
			diff -u "${REF_FILE}" test_out.txt || true
			clean_up
			exit 1
		else
			echo -n "Test PASSED: ${IN_FILE}"
			tr '\n' ' ' <time.txt | sed 's/ $//' || true
		fi

		echo ''
	done
}

programs_to_test=()
for prog in "${PROGRAMS[@]}"; do
	if [[ -f ${prog} ]]; then
		programs_to_test+=("${prog}")
	fi
done

if [[ ${#programs_to_test[@]} -eq 0 ]]; then
	echo "Error: No executable found to test. Please compile first."
	exit 1
fi

for prog in "${programs_to_test[@]}"; do
	for tests_dir in "${TESTS_DIRS[@]}"; do
		run_tests "${prog}" "${tests_dir}"
		echo ''
	done
done

clean_up
