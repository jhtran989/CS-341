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
./csim_test -v -s 4 -E 16 -b 4 -t traces/yi.trace

# remember to test the other trace files...