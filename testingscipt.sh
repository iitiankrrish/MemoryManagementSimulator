
EXECUTABLE="./memsim"
TEST_DIR="tests/randomgenerator"
mkdir -p $TEST_DIR
get_rand_pow2() {
    local powers=(256 512 1024 2048 4096)
    echo "${powers[$((RANDOM % 5))]}"
}
get_rand_policy() {
    local policies=("first_fit" "best_fit" "worst_fit")
    echo "${policies[$((RANDOM % 3))]}"
}

run_test() {
    local name=$1
    local input_file="$TEST_DIR/${name}_input.txt"
    local output_file="$TEST_DIR/${name}_output.out"
    local log_file="$TEST_DIR/${name}_trace.log"

    echo "Running Highly Random Test: $name..."
    $EXECUTABLE < "$input_file" > "$output_file" 
    cp "$output_file" "$log_file"
}
NAME="standard_random"
MEM_SIZE=$(( (RANDOM % 4 + 1) * 1024 ))
{
    echo "init memory $MEM_SIZE"
    echo "set allocator $(get_rand_policy)"
    for i in {1..10}; do
        echo "malloc $(( RANDOM % (MEM_SIZE / 10) + 10 ))"
    done
    echo "free $(( RANDOM % 5 + 1 ))"
    echo "free $(( RANDOM % 5 + 5 ))"
    echo "stats"
    echo "dump"
    echo "exit"
} > "$TEST_DIR/${NAME}_input.txt"
run_test $NAME
NAME="buddy_random"
BUDDY_SIZE=$(get_rand_pow2)
{
    echo "init buddy $BUDDY_SIZE 16"
    for i in {1..8}; do
        echo "malloc $(( RANDOM % (BUDDY_SIZE / 8) + 5 ))"
    done
    echo "stats"
    echo "dump"
    echo "free 0x0"
    echo "stats"
    echo "exit"
} > "$TEST_DIR/${NAME}_input.txt"
run_test $NAME
NAME="cache_random"
{
    echo "cache init L1 $(( (RANDOM % 4 + 1) * 64 )) 32 $(( RANDOM % 2 + 1 ))"
    ADDR1="0x$(( RANDOM % 500 ))"
    ADDR2="0x$(( RANDOM % 500 + 500 ))"
    ADDR3="0x$(( RANDOM % 500 + 1000 ))"
    POOL=($ADDR1 $ADDR2 $ADDR3)
    for i in {1..15}; do
        echo "cache access ${POOL[$(( RANDOM % 3 ))]}"
    done
    echo "cache stats"
    echo "exit"
} > "$TEST_DIR/${NAME}_input.txt"
run_test $NAME
NAME="integration_random"
INT_SIZE=$(( (RANDOM % 2 + 1) * 1024 ))
{
    echo "init memory $INT_SIZE"
    echo "cache init L1 128 32 2"
    for i in {1..5}; do
        echo "malloc $(( RANDOM % 100 + 20 ))"
        echo "cache access 0x0" 
    done
    echo "stats"
    echo "cache stats"
    echo "exit"
} > "$TEST_DIR/${NAME}_input.txt"
run_test $NAME

echo "----------------------------------------------------"
echo "All Highly Random Artifacts Generated in '$TEST_DIR'"