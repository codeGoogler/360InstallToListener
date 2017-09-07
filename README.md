
#### NDK项目实战仿360手机助手卸载监听提交报告。


效果图如下：


![效果图](http://upload-images.jianshu.io/upload_images/4614633-12cb0be0690df358.gif?imageMogr2/auto-orient/strip)


### 目的

需要监听什么时候卸载，卸载后执行部分操作

### 针对于Android的系统，我们可以试想有一下策略


- ****1、监听系统卸载广播****

    只能监听到其他应用的卸载广播，无法监听到自己是否被卸载。

        内存  --》  监听  别人
        . ACTION_PACKAGE_REMOVED
        2.ACTION_PACKAGE_REMOVED




- 2、****卸载****

    通过系统log   （正在被安装的包程序不能接收到这个广播）


    监听自身

- 3、****Java线程****

    轮训 监听  监听/data/data/{package-name}目录是否存在


- 4、****C进程****

    监听/data/data/{package-name}目录是否存在  跳转到网页


![image](http://s14.sinaimg.cn/orignal/49b22c67045842f500f0d)

-  5、****静默安装另外的apk****

    监听自己是否被卸载  可以，但是前提需要（root）

从上面分析来看

- 如果开启线程 ，则耗资源比较大
- File 的监听 可以试试


### 360卸载监听的方式：

    监听  /data/data/{package-name}目录是否存在

#### 总结

从前四种方案可以看到，单纯的Java层代码是无法监听自身卸载的。既然Java层无法实现，我们试着使用C语言在底层实现。
****借助Linux进程fork出来的C进程在应用被卸载后不会被销毁****，监听/data/data/{package-name}目录是否存在，如果不存在，就证明应用被卸载了。


## 说干就干,开始撸码

  1. ****fork()子进程****
  2. ****创建监听文件****
  3. ****初始化inotify实例****
  4. ****注册监听事件****
  5. ****调用read函数开始监听****
  6. ****卸载反馈统计****

### 实现原理

是仿照FileObserve监听文件的方式，在本地方式中进行拦截和操作

接下老我么来看下FileObserve的源码

FileObserve一进来的时候就开启了一个线程，如下：

![FileObserve进来初始化.png](http://upload-images.jianshu.io/upload_images/4614633-a3d118624f3fd0cb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

利用本利方法进行监听：


![FileObserve.png](http://upload-images.jianshu.io/upload_images/4614633-08b0f4cbd6d5185f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)


在线程中进行操作：

![执行操作.png](http://upload-images.jianshu.io/upload_images/4614633-78b15015fa45a698.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)


C中fork代码如下：
```
JNIEXPORT
void JNICALL
Java_unstall_yyh_com_a360installtolistener_MainActivity_callUnInstallListener(JNIEnv *env,jobject obj,jint versionSdk,jstring path){
    LOGD("------------------------");
    LOGF("------------------------");
    const char * path_str = env->GetStringUTFChars(path,0);
        pid_t pid =  fork();
        if(pid < 0){
               LOGD("克隆失败");
        }else if(pid > 0){
            LOGD("父进程");
        }else{
            LOGD("子进程！");
            //*******************在这里进程操作*****************
            LOGD("你好，终端研发部");
            int fuileDescript = inotify_init();
            int watch = inotify_add_watch(fuileDescript,path_str,IN_DELETE_SELF);
           void * p =  malloc(sizeof(struct inotify_event));
            read(fuileDescript,p, sizeof(struct inotify_event));
            inotify_rm_watch(fuileDescript,watch);
            LOGD(LOG_TAG,"接下来进行操作，来条状网页!!!");
            if(versionSdk< 17){
                //am start -a android.intent.action.VIEW -d  http://gityuan.com
                execlp("am","am","start","-a","android.intent.action.VIEW","-d","https://mp.weixin.qq.com/s?__biz=MzI3OTU0MzI4MQ==&mid=2247484366&idx=2&sn=a015497277d2a6380a80fdc9031ca51c&chksm=eb476f50dc30e64620fbb8a7ce0aebc445638c5f1097763e0da36fc40beb85fb256d980af440&scene=18#wechat_redirect",NULL);
            }else{
                execlp("am","am","start","--user","0","-a","android.intent.action.VIEW","-d", "https://mp.weixin.qq.com/s?__biz=MzI3OTU0MzI4MQ==&mid=2247484366&idx=2&sn=a015497277d2a6380a80fdc9031ca51c&chksm=eb476f50dc30e64620fbb8a7ce0aebc445638c5f1097763e0da36fc40beb85fb256d980af440&scene=18#wechat_redirect",NULL);
            }
        }
        env->ReleaseStringUTFChars(path,path_str);
    }
```

最后卸载监听的是我技术号里的一篇文章。

### 总结
- 6.0之后的就不能再进行监听卸载了。
- 凡是360手机助手能支持的该方式也基本支持
- 由于部分厂家修改底层源码导致部分手机无法监听下载
- 通过实验360手机助手也不能适配所有的6.0之前的手机，如小米红木手机



如果对ndk不了的同学们可以一下参考博客：

NDK探究之旅:

[NDK和JNI初探及其联系和区别](http://mp.weixin.qq.com/s?__biz=MzI3OTU0MzI4MQ==&mid=2247484266&idx=1&sn=b4f9390e0c45a04c331365b7faf8ef99&chksm=eb476ff4dc30e6e2b80e431c89bec941907bf5b308431097bb725ece700771b73faff581ac5d&scene=21#wechat_redirect)

[NDK探究之旅《一》——对jni和NDK的认识](http://blog.csdn.net/androidstarjack/article/details/71375042)

[NDK探究之旅《二》——C语言的基本认识](http://blog.csdn.net/androidstarjack/article/details/71156474)

[NDK探究之旅《三》—C语言的输入输出函数](http://blog.csdn.net/androidstarjack/article/details/71514125)

[NDK探究之旅《四》——指针的强化理解](http://blog.csdn.net/androidstarjack/article/details/71514125)

[NDK探究之旅《五》——指针和数组之间的关系](http://blog.csdn.net/androidstarjack/article/details/71699350)

[NDK探究之旅《六》—函数的指针、结构体、枚举、宏定义](http://blog.csdn.net/androidstarjack/article/details/72455218)

[NDK探究之旅《七》—函数指针,宏定义的优缺点及应用场景](http://blog.csdn.net/androidstarjack/article/details/72455218)

[NDK探究之旅《八》——jni的开发流程规范及环境配置](http://blog.csdn.net/androidstarjack/article/details/72615926)

[NDK探究之旅《九》——jni开发中常见的错误及其注意事项](http://blog.csdn.net/androidstarjack/article/details/72758094)

[NDK探究之旅《十》——ndk项目实战之Androidstudio开发经验总结](http://blog.csdn.net/androidstarjack/article/details/72813995)

[NDK探究之旅《十一》—C代码调用Java代码之项目实战](http://blog.csdn.net/androidstarjack/article/details/72827991)



>GitHub下载链接:
>
>https://github.com/androidstarjack/360InstallToListener
>
>下载慢？
>
>http://download.csdn.net/download/androidstarjack/9966557

### 相信自己，没有做不到的，只有想不到的
 如果你觉得此文对您有所帮助，欢迎入群 QQ交流群 ：232203809
微信公众号：终端研发部


![技术+职场](http://upload-images.jianshu.io/upload_images/4614633-977d06f49c7ba7be.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

（欢迎关注学习和交流）
