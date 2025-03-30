#!/bin/bash

# Philosopher Dining Problem Enhanced Test Script
# This script runs comprehensive test scenarios for the Philosophers project

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Test executable name
PHILO="./philo"

# Check if the executable exists
if [ ! -f "$PHILO" ]; then
    echo -e "${RED}Error: $PHILO not found. Please compile your program first.${NC}"
    exit 1
fi

# Function to run a test with timeout
run_test() {
    local test_name="$1"
    local cmd="$2"
    local expected="$3"
    local timeout_seconds="$4"

    echo -e "\n${BLUE}Running Test: ${test_name}${NC}"
    echo -e "Command: ${cmd}"
    echo -e "Expected: ${expected}"
    echo -e "Timeout: ${timeout_seconds} seconds"

    # Create a temporary file for output
    output_file=$(mktemp)

    # Run the command with timeout
    timeout $timeout_seconds $cmd > "$output_file" 2>&1

    exit_code=$?
    if [ $exit_code -eq 124 ]; then
        echo -e "${YELLOW}Test completed (timed out as expected)${NC}"
    elif [ $exit_code -ne 0 ]; then
        echo -e "${RED}Test failed with exit code $exit_code${NC}"
    else
        echo -e "${GREEN}Test completed successfully${NC}"
    fi

    # Display the last few lines of output
    echo -e "\n${YELLOW}Last 10 lines of output:${NC}"
    tail -n 10 "$output_file"

    # Save output for later analysis if needed
    mv "$output_file" "./test_${test_name// /_}.log"
    echo -e "${CYAN}Full output saved to ./test_${test_name// /_}.log${NC}"

    # Add a separator
    echo -e "\n${BLUE}----------------------------------------${NC}"
}

# Function to run a test and analyze death timing
analyze_death_timing() {
    local test_name="$1"
    local cmd="$2"
    local timeout_seconds="$3"

    echo -e "\n${PURPLE}Running Death Timing Analysis: ${test_name}${NC}"
    echo -e "Command: ${cmd}"
    echo -e "Timeout: ${timeout_seconds} seconds"

    # Create a temporary file for output
    output_file=$(mktemp)

    # Run the command with timeout
    timeout $timeout_seconds $cmd > "$output_file" 2>&1

    # Check if the philosopher died and analyze timing
    if grep -q "died" "$output_file"; then
        echo -e "${YELLOW}Death detected, analyzing timing...${NC}"

        # Get the last meal time and death time
        last_meal=$(grep -E "[0-9]+ [0-9]+ is eating" "$output_file" | tail -n 1 | awk '{print $1}')
        death_time=$(grep -E "[0-9]+ [0-9]+ died" "$output_file" | awk '{print $1}')
        philo_num=$(grep -E "[0-9]+ [0-9]+ died" "$output_file" | awk '{print $2}')
        time_to_die=$(echo "$cmd" | awk '{print $3}')

        if [ -n "$last_meal" ] && [ -n "$death_time" ]; then
            actual_time=$((death_time - last_meal))
            expected_time=$time_to_die
            difference=$((actual_time - expected_time))

            echo -e "Philosopher $philo_num last meal: ${last_meal}ms"
            echo -e "Philosopher $philo_num death: ${death_time}ms"
            echo -e "Time to die parameter: ${expected_time}ms"
            echo -e "Actual time until death: ${actual_time}ms"
            echo -e "Difference: ${difference}ms"

            if [ $difference -lt 10 ]; then
                echo -e "${GREEN}Death timing is accurate! (within 10ms)${NC}"
            else
                echo -e "${RED}Death timing is off by more than 10ms!${NC}"
            fi
        else
            echo -e "${RED}Could not analyze timing - missing meal or death logs${NC}"
        fi
    else
        echo -e "${YELLOW}No death detected in the output${NC}"
    fi

    # Save output for later analysis
    mv "$output_file" "./test_${test_name// /_}.log"
    echo -e "${CYAN}Full output saved to ./test_${test_name// /_}.log${NC}"

    # Add a separator
    echo -e "\n${BLUE}----------------------------------------${NC}"
}

# Title banner
echo -e "${GREEN}╔════════════════════════════════════════════════════════╗${NC}"
echo -e "${GREEN}║                                                        ║${NC}"
echo -e "${GREEN}║        PHILOSOPHER DINING PROBLEM TEST SUITE           ║${NC}"
echo -e "${GREEN}║                                                        ║${NC}"
echo -e "${GREEN}╚════════════════════════════════════════════════════════╝${NC}"

# Required tests from the assignment
echo -e "${GREEN}=== MANDATORY TESTS FROM ASSIGNMENT ===${NC}"
analyze_death_timing "Test 1 800 200 200" "$PHILO 1 800 200 200" 3
run_test "Test 5 800 200 200" "$PHILO 5 800 200 200" "No philosopher should die" 5
run_test "Test 5 800 200 200 7" "$PHILO 5 800 200 200 7" "No deaths, simulation stops after all philosophers eat 7 times" 10
run_test "Test 4 410 200 200" "$PHILO 4 410 200 200" "No philosopher should die" 5
analyze_death_timing "Test 4 310 200 100" "$PHILO 4 310 200 100" 5

# Death timing tests
echo -e "${GREEN}=== DEATH TIMING PRECISION TESTS ===${NC}"
analyze_death_timing "Death Timing Test 1" "$PHILO 2 800 200 200" 5
analyze_death_timing "Death Timing Test 2" "$PHILO 3 600 150 150" 5

# Special edge cases
echo -e "${GREEN}=== EDGE CASE TESTS ===${NC}"
run_test "Even/Odd Philosophers Test 1" "$PHILO 4 800 200 200" "Even number of philosophers, should survive" 5
run_test "Even/Odd Philosophers Test 2" "$PHILO 5 800 200 200" "Odd number of philosophers, should survive" 5
run_test "Balanced Timing" "$PHILO 5 600 200 200" "Balanced eat/sleep times" 5
run_test "Unbalanced Timing 1" "$PHILO 5 600 300 100" "More eating than sleeping" 5
run_test "Unbalanced Timing 2" "$PHILO 5 600 100 300" "More sleeping than eating" 5

# Stress tests (within limits)
echo -e "${GREEN}=== STRESS TESTS ===${NC}"
run_test "Medium Load Test" "$PHILO 100 800 200 200" "100 philosophers, should handle without issues" 8
run_test "Higher Load Test" "$PHILO 199 800 200 200" "199 philosophers, approaching limit" 10
run_test "Tight Timing Test" "$PHILO 5 400 150 150" "Tighter timing that still allows survival" 5
run_test "Threshold Test" "$PHILO 4 310 150 150" "Near death threshold" 5

# Behavior verification tests
echo -e "${GREEN}=== BEHAVIOR VERIFICATION TESTS ===${NC}"
run_test "Meal Termination Test 1" "$PHILO 5 800 200 200 1" "Terminate after 1 meal each" 5
run_test "Meal Termination Test 2" "$PHILO 5 800 200 200 10" "Terminate after 10 meals each" 15
run_test "High Meal Count" "$PHILO 3 800 200 200 20" "Terminate after 20 meals each" 20

# Error handling
echo -e "${GREEN}=== ERROR HANDLING TESTS ===${NC}"
run_test "Missing Arguments" "$PHILO 5 800 200" "Should display error for missing arguments" 2
run_test "Too Many Arguments" "$PHILO 5 800 200 200 7 extra" "Should display error for too many arguments" 2
run_test "Invalid Philosopher Count" "$PHILO 0 800 200 200" "Should reject 0 philosophers" 2
run_test "Negative Values" "$PHILO 5 -800 200 200" "Should reject negative values" 2
run_test "Non-numeric Arguments" "$PHILO five 800 200 200" "Should reject non-numeric input" 2

# Specific scenario tests
echo -e "${GREEN}=== SPECIFIC SCENARIO TESTS ===${NC}"
run_test "Minimum Valid Philo Count" "$PHILO 2 800 200 200" "Minimum valid number of philosophers" 5
run_test "Nearly Starving Test" "$PHILO 5 250 200 200" "Philosophers nearly starve but survive" 5
run_test "Exact Death Test" "$PHILO 3 210 200 200" "Death should occur right after time_to_die" 5

# Bonus section (if applicable)
echo -e "${GREEN}=== BONUS TESTS (if implemented) ===${NC}"
echo -e "${YELLOW}The following tests are for the bonus part and will only be relevant if you implemented it${NC}"
run_test "Bonus Check 1" "$PHILO_BONUS 5 800 200 200" "Bonus version with processes instead of threads" 5
run_test "Bonus Check 2" "$PHILO_BONUS 5 800 200 200 7" "Bonus version with meal limit" 10

# Memory and race condition tests recommendation
echo -e "${GREEN}=== MEMORY & RACE CONDITION TESTS ===${NC}"
echo -e "${YELLOW}It's recommended to run the following commands manually:${NC}"
echo -e "valgrind --leak-check=full ./philo 5 800 200 200"
echo -e "valgrind --tool=helgrind ./philo 5 800 200 200"
echo -e "valgrind --tool=drd ./philo 5 800 200 200"

# Display summary
echo -e "\n${GREEN}╔════════════════════════════════════════════════════════╗${NC}"
echo -e "${GREEN}║                    TEST SUMMARY                        ║${NC}"
echo -e "${GREEN}╚════════════════════════════════════════════════════════╝${NC}"
echo -e "${YELLOW}All tests have been executed. Log files have been saved for detailed analysis.${NC}"
echo -e "Crucial checks to ensure your project is perfect:"

echo -e "\n${BLUE}1. Timing Verification:${NC}"
echo -e "   - Death messages must appear within 10ms of actual death time"
echo -e "   - Philosophers must die exactly time_to_die ms after their last meal"

echo -e "\n${BLUE}2. Resource Management:${NC}"
echo -e "   - No philosopher should ever hold more than two forks"
echo -e "   - Forks should never be used by more than one philosopher simultaneously"
echo -e "   - All allocated resources should be properly freed (no memory leaks)"

echo -e "\n${BLUE}3. Synchronization:${NC}"
echo -e "   - No data races (verify with helgrind/drd)"
echo -e "   - No deadlocks or livelocks"
echo -e "   - Messages should never overlap in the output"

echo -e "\n${BLUE}4. Performance:${NC}"
echo -e "   - Program should handle the maximum number of philosophers efficiently"
echo -e "   - CPU usage should be reasonable (not excessive spinning)"

echo -e "\n${PURPLE}For a perfect project, manually verify log files for:${NC}"
echo -e "1. Consistent timestamps (monotonically increasing)"
echo -e "2. Proper sequencing of actions (take forks → eating → sleeping → thinking)"
echo -e "3. Correct fork assignment (philosophers only use their designated forks)"
echo -e "4. Proper termination (clean exit when required)"

exit 0


Test: [5 800 200 200 7] | no one should die, simulation should stop after 7 eats

