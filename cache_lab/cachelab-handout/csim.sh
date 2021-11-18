gcc -o csim_test main.c

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
#./csim -v -s 1 -E 1 -b 1 -t traces/yi2.trace > test_case_personal_1.txt
#./csim-ref -v -s 1 -E 1 -b 1 -t traces/yi2.trace > test_case_ref_1.txt
#diff test_case_personal_1.txt test_case_ref_1.txt > test_case_output.txt

# 1st (index 0) OK
#./csim_test -v -s 1 -E 1 -b 1 -t traces/yi2.trace

# 2nd (index 1) OK
#./csim_test -v -s 4 -E 2 -b 4 -t traces/yi.trace

# 3rd (index 2) OK
#./csim_test -v -s 2 -E 1 -b 4 -t traces/dave.trace

# 4rth (index 3) OK
#./csim_test -v -s 2 -E 1 -b 3 -t traces/trans.trace

# 5th (index 4) OK
#./csim_test -v -s 2 -E 2 -b 3 -t traces/trans.trace

# 6th (index 5) OK
#./csim_test -v -s 2 -E 4 -b 3 -t traces/trans.trace

# 7th (index 6) OK
#./csim_test -v -s 5 -E 1 -b 5 -t traces/trans.trace

# FINAL ONE
# 8th (index 7)
./csim_test -v -s 5 -E 1 -b 5 -t traces/long.trace

# find differences (verbose)

# array=("1" "2" "3")
#
# for (( i=0; i<3; i++ ))
# do
# 	echo ${array[i]}
# done