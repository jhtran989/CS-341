make clean
make

echo "M = 32, N = 32"
./test-trans -M 32 -N 32

echo "M = 61, N = 67"
./test-trans -M 61 -N 67

echo "M = 64, N = 64"
./test-trans -M 64 -N 64
