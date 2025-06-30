
# ulimit is neccesary to increase stack space for the recursive algo

ulimit -s 20000 && ./a.out 1 2 4 8 16 32 64 128 256 512 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 | tee time_99_07_mac.csv
