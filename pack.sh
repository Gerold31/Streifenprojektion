DIRS=('prototype1' 'plot' 'tools')
PACK="pack.zip"

cd "$(dirname "$0")"

for DIR in "${DIRS[@]}"
do
	git clean -Xdi "$DIR"
	zip -r "$PACK" "$DIR"
done
