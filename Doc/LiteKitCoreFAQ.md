# FAQ

## Q1：cmake软件打不开
A：在系统偏好设置中，点击安全性与隐私，通用选项卡，最下面点击仍要打开即可。

## Q2：cmake软件如何安装到命令行？
A：打开cmake软件，在tools选项卡中，选择“how to install for command line use”，在终端中敲入第二行的提示即可。

## Q3：android studio在build时提示ndk未下载或ndk版本未匹配
A：点击android studio页面右上角SDK Manager选项卡，弹出的页面点击sdk tools，ndk一栏中选择对应版本即可

## Q4：执行sh product_build.sh build失败，报错libpaddle_api_light_bundled.a (No such file or directory) 
A：下载依赖库到指定目录，目前ios的依赖库下载链接为：https://gitee.com/paddlepaddle/LiteKit/tree/main/iOS/LiteKitCoreNativeDepencency/paddle_api_light_bundled对应版本

## Q5：.bashrc配置环境变量不生效
A：考虑使用的是其他命令行工具配置文件有所不同，例如zsh写到.zshrc文件里面。
