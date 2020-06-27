#!/usr/bin/env bash

: ${NUM_GAMES:=16}
: ${NUM_THREADS:=1}

NOW=$(date +"%T")
CFG_FILENAME="mmcts_extra.cfg"
echo 'CONFIG_FILE:' $CFG_FILENAME
echo 'NUM_THREADS:' $NUM_THREADS
echo 'NUM_GAMES:' $NUM_GAMES

WHITE="./orig_katago.sh"
BLACK="./mmcts_katago.sh ${CFG_FILENAME}"

gogui-twogtp -black "$BLACK" -white "$WHITE" -verbose -threads $NUM_THREADS -alternate -games ${NUM_GAMES} -size 9 -auto -sgffile result-${NOW} 2>&1 | gnomon
gogui-twogtp -analyze result-${NOW}.dat
