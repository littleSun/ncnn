fork from https://github.com/Tencent/ncnn

---------
此版本专为IOS编译，使用clang-gcd

编译步骤
1 build for iphone
```
sudo xcode-select --switch /Applications/Xcode.app/
mkdir ios-build && cd ios-build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../ios.toolchain.cmake -DIOS_PLATFORM=iPhoneOS ..
make && make install
```

2 build for simulator
```
cd ..
mkdir ios-sim-build && cd ios-sim-build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../ios.toolchain.cmake -DIOS_PLATFORM=iPhoneSimulator ..
make && make install
```
3 merge
```
cd ..
mkdir -p ncnn.framework/Versions/A/Headers
mkdir -p ncnn.framework/Versions/A/Resources
lipo -create ios-build/install/lib/libncnn.a ios-sim-build/install/lib/libncnn.a -o ncnn.framework/Versions/A/ncnn
ln -s A ncnn.framework/Versions/Current
ln -s Versions/Current/Headers ncnn.framework/Headers
ln -s Versions/Current/Resources ncnn.framework/Resources
ln -s Versions/Current/ncnn ncnn.framework/ncnn
cp -r ios-sim-build/install/include/* ncnn.framework/Versions/A/Headers/
cp Info.plist ncnn.framework/Versions/A/Resources/
```
