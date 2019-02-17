#!/bin/bash

for f in csvMaps/*.csv
do
    WORLD="$(basename $f .csv).world"
    echo "$f ----->>>> $WORLD"
    rosrun equinor2018_simulator csv_to_gz_util $f "worlds/$WORLD"
done

