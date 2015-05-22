DIRS=('prototype1' 'plot' 'tools' 'hardware/main.ino')
PACK="pack.zip"

cd "$(dirname "$0")"

cp hardware/servoctl-device/main.ino hardware/main.ino

rm "$PACK"
for DIR in "${DIRS[@]}"
do
	git clean -Xdi "$DIR"
	zip -r "$PACK" "$DIR" -x "**/"
done

rm hardware/main.ino
