#!/bin/bash
DEBUG=""
if [ -n "$1" ] && [ $1 = "-v" ]; then
    DEBUG=$1
fi
case $DEBUG in
    -v) PROG=$2 && DIR=$3;;
    *) PROG=$1 && DIR=$2;;
esac

if [ "$(find $PROG)" = "$PROG" ]; then
    for i in $DIR/*.in; do
        FILENAME=${i%.in}
        DIFO="diff /tmp/$PROG.out $FILENAME.out"
        DIFE="diff /tmp/$PROG.err $FILENAME.err"
		EXEC="./$PROG $DEBUG <$i 1>/tmp/$PROG.out 2>/tmp/$PROG.err"
        if [ $1 = "-v" ]; then
			OUTEX="$(eval "$EXEC")"
            OUTDIFO="$(eval "$DIFO")"
            OUTDIFE="$(eval "$DIFE")"
			if [ -n "$OUTDIFO" ] || [ -n "$OUTDIFE" ]; then
				echo "$FILENAME FAILED"
			else
				echo "$FILENAME PASSED"
			fi
        else
            OUTEX="$(eval "./"$PROG" <"$i" 1>/tmp/"$PROG".out 2>/tmp/"$PROG".err")"
            OUTDIFO="$(eval "$DIFO")"
			if [ -n "$OUTDIFO" ]; then
				echo "$FILENAME FAILED"
			else
				echo "$FILENAME PASSED"
			fi
	    fi
		rm /tmp/$PROG.*
    done
fi
