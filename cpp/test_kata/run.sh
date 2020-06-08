#!/usr/bin/env bash
: ${WHITE:=./orig_katago.sh}
: ${BLACK:=./mmcts_katago.sh}
: ${NUM_GAMES:=100}
: ${NUM_THREADS:=1}

NOW=$(date +"%T")
echo 'NUM_THREADS:' $NUM_THREADS
echo 'NUM_GAMES:' $NUM_GAMES
gogui-twogtp -black $BLACK -white $WHITE -verbose -threads $NUM_THREADS -alternate -games ${NUM_GAMES} -size 9 -auto -sgffile result-${NOW} 2>&1 | gnomon
gogui-twogtp -analyze result-${NOW}.dat
