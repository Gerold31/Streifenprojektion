DIRS=('prototype1' 'plot' 'tools')
PACK="pack.zip"

cd "$(dirname "$0")"

rm "$PACK"
for DIR in "${DIRS[@]}"
do
	git clean -Xdi "$DIR"
	zip -r "$PACK" "$DIR" -x "**/"
done
