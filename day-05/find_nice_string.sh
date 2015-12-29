# Many thanks to stackoverflow answer http://superuser.com/a/769482
# As usual, sed and awk are the answer for filtering a file and then processing it

# Also, http://www.regexr.com/ is ESSENTIAL. I appreciate (and will donate to) sites like this. (there's one that explains bash commands and their flags in human-readable english)

# (\w)\1{1} Create capture group that matches any word character. Backreference to match the result of capture group 1 (\1), and match 1 of the preceeding token {1}. This matches double characters.
# n{1} Match character "n" and match 1 of preceeding token

# A nice string is one with all of the following properties:
#
#     It contains at least three vowels (aeiou only), like aei, xazegov, or aeiouaeiouaeiou.
#     It contains at least one letter that appears twice in a row, like xx, abcdde (dd), or aabbccdd (aa, bb, cc, or dd).
#     It does not contain the strings ab, cd, pq, or xy, even if they are part of one of the other requirements.
#

# explanation
# print lines that do not have the matches ab cd pq xy
# print lines that have a double letter ("(\w)" a single alphanumeric character followed by itself "\1")
# remove any non-vowel character from each line
# print a 1 for each line that has at least 3 vowels
# count the number of resulting lines
echo "Answer for part 1:"
cat input.txt | grep -v -e 'ab' -e 'cd' -e 'pq' -e 'xy' | egrep '(\w)\1' | sed 's/[^[aeiou]//g' | awk '{if (length > 2) print 1; }' | wc -l

# print lines that have letter, other-letter, letter (xyx)
# print lines that have two characters and those same two letters separated by none or more characters
echo "Answer for part 2:"
cat input.txt | egrep '(\w)\w\1' | egrep '(\w\w).*\1' | wc -l
