#!/bin/bash

for f in csvMaps/*.csv
do
    WORLD="$(basename $f .csv).world"
    echo "$f ----->>>> $WORLD"
    rosrun simulator csv_to_gz_util $f "worlds/$WORLD"
done

