#!/bin/bash
# simple_test.sh
# ./simple_test.sh

ARGS="5 610 200 200 10"
LOG_FILE="test.log"
DEAD_LOG_FILE="dead.log"

> "$LOG_FILE"
> "$DEAD_LOG_FILE"

for i in $(seq 1 10); do
    OUTPUT=$(./philo $ARGS 2>&1)

    LAST_LINE=$(echo "$OUTPUT" | tail -n 1)

    echo "Run #$i: $LAST_LINE"
    echo "Run #$i: $LAST_LINE" >> "$LOG_FILE"

    if echo "$OUTPUT" | grep -q "died"; then
        echo "----- Run #$i -----" >> "$DEAD_LOG_FILE"
        echo "$OUTPUT" >> "$DEAD_LOG_FILE"
        echo "------------------" >> "$DEAD_LOG_FILE"
    fi
done

echo "All runs finished. Output saved in $LOG_FILE."
echo "Deaths logged in $DEAD_LOG_FILE."
