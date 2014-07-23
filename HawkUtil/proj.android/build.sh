ndk-build -B

cp -v ../*.h jni/prebuild/include/
cp -v obj/local/armeabi/libhawkutil.a jni/prebuild/libs/armeabi/
cp -v obj/local/armeabi-v7a/libhawkutil.a jni/prebuild/libs/armeabi-v7a/
