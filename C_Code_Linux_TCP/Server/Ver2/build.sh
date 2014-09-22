BUILD_PATH=`pwd`
OUTPUT_IMAGE="Server"
SOURCE_DIR="Source"
TARGET_DIR="Target"
OUTPUT_DIR="Output"

if [ "$#" != "0" ]; then
	for ((index=0;index<$#;))
	do
		if [ "$1" == "clean" ]; then
			echo "rm -rf $TARGET_DIR $OUTPUT_DIR"
			rm -rf $TARGET_DIR $OUTPUT_DIR
			exit	
		fi
		shift
	done
fi

rm -rf $BUILD_PATH/$TARGET_DIR
cp -r $BUILD_PATH/$SOURCE_DIR $BUILD_PATH/$TARGET_DIR

cd $TARGET_DIR

sed -i '1a #include "GlobalDefine.h"' *.c
make

cd $BUILD_PATH

mkdir -p $BUILD_PATH/$OUTPUT_DIR

mv $BUILD_PATH/$TARGET_DIR/${OUTPUT_IMAGE} $BUILD_PATH/$OUTPUT_DIR
