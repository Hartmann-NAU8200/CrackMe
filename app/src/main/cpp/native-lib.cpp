#include <jni.h>
#include <string>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <android/log.h>

#define TAG "jnitest" // 这个是自定义的LOG的标识
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型

static char * k = "15pb";
#define MAX 128
#define CHECK_TIME 10
JNIEXPORT jint JNICALL Java_com_example_jnitest_JNIClass_getValue(JNIEnv *,
                                                                  jclass) {
    int value;
    value = getpid();
    return value;
}
char * generateKey(const char* source, char* pass);

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_shush_crackme_MainActivity_resultFromJNI(JNIEnv *env, jobject instance,
                                                          jstring sn_) {
    const char *sn = env->GetStringUTFChars(sn_, 0);

    const char *ret = generateKey(sn,k);
    if(!strcmp(ret,"YP\x18\a")){
        return env->NewStringUTF("恭喜！破解成功！"); // flag:hehe
    }
    env->ReleaseStringUTFChars(sn_, sn);
    return env->NewStringUTF("失败！");
}

JNIEXPORT void JNICALL Java_com_example_jnitest_JNIClass_AntiDebug
        (JNIEnv *, jclass)
{
    return;
}

void AntiDebug() {

    int pid;
    FILE *fd;
    char filename[MAX];
    char line[MAX];
    pid = getpid();
    sprintf(filename,"/proc/%d/status",pid);// 读取proc/pid/status中的TracerPid
    if(fork()==0)
    {
        int pt;
        pt = ptrace(PTRACE_TRACEME, 0, 0, 0); //子进程反调试
        while(true)
        {
            fd = fopen(filename,"r");
            while(fgets(line,MAX,fd))
            {
                if(strncmp(line,"TracerPid",9) == 0)
                {
                    int statue = atoi(&line[10]);
                    fclose(fd);
                    if(statue != 0)
                    {
                        //int ret = kill( pid,SIGKILL);
                        k ="haha";
                        exit(1);
                        return ;
                    }
                    break;
                }
            }
            sleep(CHECK_TIME);
        }
    }
}

void antiDebug(){
    int pid = getpid();
    struct timeval t1;
    struct timeval t2;
    struct timezone tz;
    gettimeofday(&t1,&tz);
    gettimeofday(&t2,&tz);

    int timeoff = (t2.tv_sec)-(t1.tv_sec);
    if(timeoff>1){
        k = "haha";
        exit(1);
    }

}
/*
 * Set some test stuff up.
 *
 * Returns the JNI version on success, -1 on failure.
 */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    AntiDebug();
    int result = JNI_VERSION_1_6;
    antiDebug();
    return result;
}

char * generateKey(const char* source, char* pass){
    int source_length = strlen(source);
    int pass_length = strlen(pass);

    char* tmp_str = (char*)malloc((source_length + 1) * sizeof(char));
    memset(tmp_str, 0, source_length + 1);

    for(int i = 0; i < source_length; ++i)
    {
        tmp_str[i] = source[i]^pass[i%pass_length];
        if(tmp_str[i] == 0)              // 要考虑到XOR等于0的情况，如果等于0，就相当
        {                                // 于字符串就提前结束了， 这是不可以的。
            tmp_str[i] = source[i];      // 因此tmp_str[i]等于0的时候，保持原文不变
        }
    }
    tmp_str[source_length] = 0;
    printf("%s",tmp_str);
    return tmp_str;
}

