#!/bin/bash

SUCCESS=0
FAIL=0
DIFF_OUTPUT=""
REGEX="--regexp=\"[0-9]\+\""
PATTERN="21"
FILE_NAME="./s21_grep.c"

for flag_1 in '' -i -v -c -l -n $REGEX
do
  if [[ "$flag_1" == "$REGEX" ]]
  then
      if [[ "$flag_1" != "-l" ]]
      then
          OPTIONS="$flag_1 $FILE_NAME"
          echo "sh: $OPTIONS"
      else
          continue;
      fi
  else
      OPTIONS="$flag_1 $PATTERN $FILE_NAME"
      echo "sh: $OPTIONS"
  fi

  ./s21_grep $OPTIONS > s21_temp.txt
  grep $OPTIONS > temp.txt

  DIFF_OUTPUT="$(diff -s s21_temp.txt temp.txt)"
  if [ "$DIFF_OUTPUT" == "Files s21_temp.txt and temp.txt are identical" ]
      then
          (( SUCCESS++ ))
      else
          (( FAIL++ ))
          echo "ERROR"
          echo "$(cmp s21_temp.txt temp.txt)"
          echo "$(diff -s s21_temp.txt temp.txt)"
  fi

  rm s21_temp.txt temp.txt
done


echo "SUCCESS: $SUCCESS"
echo "FAIL: $FAIL"