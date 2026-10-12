#!/usr/bin/env bash
#
# Quick local test for lemipc.
#
#   ./test.sh                  3 vs 3 (teams 1 and 2), with the viewer
#   ./test.sh 1 1 2 2 3 3      any list of team numbers, one per player
#   ./test.sh -q [team ...]    no viewer, just run the game
#
# Ctrl-C stops everything cleanly. At the end the script prints what each
# player did and checks that no IPC object was left behind.

KEY=0x4c454d49

cd "$(dirname "$0")" || exit 1

viewer=1
if [ "$1" = "-q" ]; then
	viewer=0
	shift
fi
teams=("$@")
[ ${#teams[@]} -eq 0 ] && teams=(1 1 1 2 2 2)

make -s || exit 1

if ipcs | grep -qi "$KEY"; then
	echo "A lemipc game is already running or was left over."
	echo "Stop it, or clean up with: ipcrm -M $KEY -S $KEY -Q $KEY"
	exit 1
fi

log=$(mktemp)
trap 'rm -f "$log"' EXIT
# Ctrl-C reaches the players and the viewer directly (same process group),
# so the script only needs to survive it and go on to the summary.
trap ':' INT

pids=()
for t in "${teams[@]}"; do
	./lemipc "$t" >> "$log" 2>&1 &
	pids+=($!)
done

if [ $viewer -eq 1 ]; then
	./lemipc -v
else
	echo "Started ${#pids[@]} players (teams: ${teams[*]}). Ctrl-C to stop."
fi

# wait is interrupted by Ctrl-C, so keep waiting until every player is gone
for p in "${pids[@]}"; do
	while kill -0 "$p" 2>/dev/null; do
		wait "$p" 2>/dev/null
	done
done

echo
echo "=== Players"
cat "$log"

echo
echo "=== Summary"
winner=$(grep -m1 -oE 'team [0-9]+: victory' "$log" | grep -oE '[0-9]+')
if [ -n "$winner" ]; then
	result="team $winner won"
elif grep -q "draw" "$log"; then
	result="draw"
else
	result="stopped before the end"
fi
echo "Players: $(grep -c 'joined' "$log")   Killed: $(grep -c 'killed' "$log")   Result: $result"

if ipcs | grep -qi "$KEY"; then
	echo "IPC cleanup: FAILED, objects left behind:"
	ipcs | grep -i "$KEY"
	exit 1
fi
echo "IPC cleanup: OK, nothing left behind"
