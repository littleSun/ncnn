![](https://raw.githubusercontent.com/Tencent/ncnn/master/images/256-ncnn.png)
# ncnn


ncnn is a high-performance neural network inference computing framework optimized for mobile platforms. ncnn is deeply considerate about deployment and uses on mobile phones from the beginning of design. ncnn does not have third party dependencies. it is cross-platform, and runs faster than all known open source frameworks on mobile phone cpu. Developers can easily deploy deep learning algorithm models to the mobile platform by using efficient ncnn implementation, create intelligent APPs, and bring the artificial intelligence to your fingertips. ncnn is currently being used in many Tencent applications, such as QQ, Qzone, WeChat, Pitu and so on.

ncnn 是一个为手机端极致优化的高性能神经网络前向计算框架。ncnn 从设计之初深刻考虑手机端的部署和使用。无第三方依赖，跨平台，手机端 cpu 的速度快于目前所有已知的开源框架。基于 ncnn，开发者能够将深度学习算法轻松移植到手机端高效执行，开发出人工智能 APP，将 AI 带到你的指尖。ncnn 目前已在腾讯多款应用中使用，如 QQ，Qzone，微信，天天P图等。


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
ln -s A ncnn.framework/Versions/Current
ln -s Versions/Current/Headers ncnn.framework/Headers
ln -s Versions/Current/Resources ncnn.framework/Resources
ln -s Versions/Current/ncnn ncnn.framework/ncnn
lipo -create ios-build/install/lib/libncnn.a ios-build-sim/install/lib/libncnn.a -o ncnn.framework/Versions/A/ncnn
cp -r ios-sim-build/install/include/* ncnn.framework/Versions/A/Headers/
cp Info.plist ncnn.framework/Versions/A/Resources/
```
