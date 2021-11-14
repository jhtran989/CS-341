#gcc -o csim_test main.c

# test different combinations of s and b
#./csim_test -s 4 -E 1 -b 4 -t traces/yi.trace
#./csim_test -v -s 2 -E 1 -b 2 -t traces/yi.trace
#./csim_test -v -s 4 -E 1 -b 2 -t traces/yi.trace
#./csim_test -v -s 2 -E 1 -b 4 -t traces/yi.trace

# test different E values (s and b stay the same at 4 for easy checking -- 4
# bits for s and b means one hexadecimal place for each)
#./csim_test -v -s 4 -E 2 -b 4 -t traces/yi.trace
#./csim_test -v -s 4 -E 4 -b 4 -t traces/yi.trace
#./csim_test -v -s 4 -E 8 -b 4 -t traces/yi.trace
#./csim_test -v -s 4 -E 16 -b 4 -t traces/yi.trace

# remember to test the other trace files...

# official test cases
test_case_dir="test_case_files"

num_test_cases=8

personal_exec="csim"
ref_exec="csim-ref"

personal_file_prefix="test_case_personal_"
ref_file_prefix="test_case_ref_"
diff_output="test_case_diff.txt"

text_ext=".txt"
exec_prefix="./"
up="../"

s_array=("1" "4" "2" "2" "2" "2" "5" "5")
E_array=(1 2 1 1 2 4 1 1)
b_array=(1 4 4 3 3 3 5 5)
trace_files_array=("traces/yi2.trace" "traces/yi.trace" "traces/dave.trace" "traces/trans.trace" 
"traces/trans.trace" "traces/trans.trace" "traces/trans.trace" "traces/long.trace")  

mkdir -p ${test_case_dir}
cd ${test_case_dir}

for (( i=0; i<${num_test_cases}; i++))
do
	mkdir -p ${i}
	cd ${i}
	
	personal_file=${personal_file_prefix}${i}${text_ext}
	ref_file=${ref_file_prefix}${i}${text_ext}
	${exec_prefix}${personal_exec} -v -s ${s_array[i]} -E ${E_array[i]} -b ${b_array[i]} -t ${traces_files_array[i]} > ${personal_file}
	${exec_prefix}${ref_exec} -v -s ${s_array[i]} -E ${E_array[i]} -b ${b_array[i]} -t ${traces_files_array[i]} > ${ref_file}
	diff ${personal_file} ${ref_file} > ${diff_output}
	
	cd ${up}
done

# ./csim -v -s 1 -E 1 -b 1 -t traces/yi2.trace > test_case_personal_1.txt
# ./csim-ref -v -s 1 -E 1 -b 1 -t traces/yi2.trace > test_case_ref_1.txt
# diff test_case_personal_1.txt test_case_ref_1.txt > test_case_output.txt

# find differences (verbose)
