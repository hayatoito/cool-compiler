#!/bin/bash

testfiles=(expressions.cl)
TESTDIR=../tests/semanticanalyzer

echo -e "======= Testing Semantic Analyzer =======\n"

cd ../../src
make >/dev/null 2>&1
cp cooc $TESTDIR
cd $TESTDIR

for testfile in $testfiles
do
    echo "*** Running test for ${testfile} ***"

    base=$(basename -s .cl $testfile)
    ./cooc $testfile > ${base}.out

    output=$(diff ${base}.out ${base}.ast)

    if [ $? -eq 0 ]
    then
        echo -e "Test ${base} Passed!"
        rm ${base}.out
    else
        echo -e "Test ${base} Failed! Examine ${base}.out for more details.\n"
        echo -e "Diff output:\n$output\n"
    fi

    echo "*** Test for ${testfile} ended ***"
done

rm cooc

