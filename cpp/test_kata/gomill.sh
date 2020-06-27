 #!/usr/bin/env bash
: ${NUM_GAMES:=10}
: ${NUM_THREADS:=8}
: ${BOARD_SIZE:=9}

echo 'NUM_GAMES:' $NUM_GAMES
echo 'NUM_THREADS:' $NUM_THREADS

ringmaster gomill.ctl run -j ${NUM_THREADS}