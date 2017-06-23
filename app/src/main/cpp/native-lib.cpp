#include <jni.h>
#include <string>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char * generateKey(const char* source, char* pass);

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_shush_crackme_MainActivity_resultFromJNI(JNIEnv *env, jobject instance,
                                                          jstring sn_) {
    const char *sn = env->GetStringUTFChars(sn_, 0);

    const char *ret = generateKey(sn,"15pb");
    if(!strcmp(ret,"YP\x18\a")){
        return env->NewStringUTF("恭喜！破解成功！");
    }
    env->ReleaseStringUTFChars(sn_, sn);
    return env->NewStringUTF("失败！");
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