#! /usr/bin/env bash

set -o errexit
set -o pipefail
set -o nounset

readonly TESTS_DIR=tests

function log_info()
{
	log_ "info" "$@"
}

function log_error()
{
	log_ "error" "$@" 
}

function log_()
{
	local type=$1
	local messange=$2
	local date_str=; date_str=$(date)
	echo "[${type}][${date_str}] ${messange}"
}

function main() {

	local bin_prog=da_lab4
    local bin_bench=benchmark
    local bench_name="benchmark_test"

	log_info "Stage #1. Compiling..."

	if ! make ; then
		log_error "Failed to compile da_lab4.cpp"
		return 1
	fi

	log_info "Compiling OK"


	log_info "Stage #2. Benchmark generating..."

	local count_of_num_occurrences=10
	if ! python3 generator.py ${count_of_num_occurrences} ; then
		log_error "Failed benchmark generating"
		return 1
	fi 
	log_info "Benchmark generating OK"


	log_info "Stage #3. Benchmark results:"
	if ! ./${bin_bench} < ${bench_name}; then
        log_error "Failed to run benchmark test"
        return 1
    fi
	log_info "Benchmark OK"
}

main $@