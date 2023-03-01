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

	local bin=da_lab2

	log_info "Stage #1. Compiling..."

	if ! make ; then
		log_error "Failed to compile da_lab2.cpp"
		return 1
	fi

	log_info "Compiling OK"


	log_info "Stage #2. Test generating..."
	rm -rf ${TESTS_DIR}
	mkdir ${TESTS_DIR}

	local count_of_tests=10
	if ! python3 generator.py ${TESTS_DIR} ${count_of_tests} ; then
		log_error "Failed to test generating tests"
		return 1
	fi 
	log_info "Test generating OK"


	log_info "Stage #3. Checking..."
	for test_file in $(ls ${TESTS_DIR}/*.t) ;do
		local tmp_output=tmp
		if ! ./${bin} < ${test_file} > ${tmp_output} ; then
			log_error "Failed to run test"
			return 1
		fi

		local file_line_cnt=; file_line_cnt=$(cat ${test_file} | wc -l | sed -e 's/ *//g')
		local answer_file=${test_file%.*}.a
		if ! diff -u ${tmp_output} ${answer_file} ; then
			log_error "Failed to check test ${test_file}."
			return 1
		fi
		log_info "${test_file}, lines = ${file_line_cnt} OK"
	done

	log_info "Checking OK"
}

main $@