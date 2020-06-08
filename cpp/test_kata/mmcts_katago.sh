#!/usr/bin/env bash
../katago gtp -model ../models/g170e-b15c192-s1672170752-d466197061.txt.gz -config <(cat base.cfg mmcts_extra.cfg)
