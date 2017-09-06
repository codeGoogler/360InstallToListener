#include <jni.h>
#include <string>
#include <android/log.h>
#include <sys/inotify.h>
#define LOG_TAG "yuyahao1"
#include <unistd.h>
#include <malloc.h>

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGF(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)



extern "C"{
JNIEXPORT jstring JNICALL
Java_unstall_yyh_com_a360installtolistener_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "欢迎关注终端研发部:<br><br><br>"
            "ndk项目实战—高仿360手机助手之卸载监听";
    LOGD(LOG_TAG,"我进入到这个减小的局面了");
    return env->NewStringUTF(hello.c_str());
}
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



}


