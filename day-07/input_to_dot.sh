cat input.txt | awk '
BEGIN { print "digraph {" };
END { print "}\n"};

/[a-z][a-z]? [A-Z]+ [0-9]+ ->/ {
	printf "{ %s %d0%s } -> %s;\n", $1,NR,$3,$5
};

/[a-z][a-z]? [A-Z]+ [a-z][a-z]? ->/ {
	printf "{ %s %s } -> %s;\n", $1,$3,$5
};

/^NOT [a-z][a-z]? ->/ {
	printf "{ not%d %s } -> %s;\n", NR,$2,$4
};

/^[a-z][a-z]? ->/ {
	printf "%s -> %s;\n", $1, $3
};

'
