#!/usr/bin/env bash

../katago gtp -model ../models/g170-b6c96-s175395328-d26788732.bin.gz -config <(cat base.cfg $1)