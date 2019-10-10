
/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * NOTICE:���c�ļ���������Ҫ�Ķ����ֱ��ʶ�� TODO1 �� TODO2
 */
#include<jni.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/inotify.h>
#include<sys/stat.h>

#include<android/log.h>

/* �궨��begin */
//��0��
#define MEM_ZERO(pDest, destSize) memset(pDest, 0, destSize)

//LOG�궨��
#define LOG_INFO(tag, msg) __android_log_write(ANDROID_LOG_INFO, tag, msg)
#define LOG_DEBUG(tag, msg) __android_log_write(ANDROID_LOG_DEBUG, tag, msg)
#define LOG_WARN(tag, msg) __android_log_write(ANDROID_LOG_WARN, tag, msg)
#define LOG_ERROR(tag, msg) __android_log_write(ANDROID_LOG_ERROR, tag, msg)


/* ��ȫ�ֱ���begin */
static char TAG[] = "UninstalledObserverActivity.init";
static jboolean isCopy = JNI_TRUE;

//--TODO1--------------------������Ҫ�ĳ��Լ�apk�İ���------------------------------------
static const char APP_DIR[] = "/data/data/com.lzyblog.uninstalldemo";
static const char APP_FILES_DIR[] = "/data/data/com.lzyblog.uninstalldemo/files";
static const char APP_OBSERVED_FILE[] = "/data/data/com.lzyblog.uninstalldemo/files/observedFile";
static const char APP_LOCK_FILE[] = "/data/data/com.lzyblog.uninstalldemo/files/lockFile";
/* ��ȫ�ֱ��� */

/*-------TODO2-----------------------
 * Class:     ��Ҫ�ĳ��Լ�app������
 * Method:    init
 * Signature: ()V
 * return: �ӽ���pid
 */
JNIEXPORT int JNICALL Java_com_lzyblog_uninstalldemo_UninstalledObserverActivity_init(JNIEnv *env, jobject obj, jstring userSerial, jstring website)
{
    jstring tag = (*env)->NewStringUTF(env, TAG);
	char* websiteStr=(char*) (*env)->GetStringUTFChars(env, website, NULL);

    LOG_DEBUG((*env)->GetStringUTFChars(env, tag, &isCopy)
            , (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "init observer"), &isCopy));

    // fork�ӽ��̣���ִ����ѯ����
    pid_t pid = fork();
    if (pid < 0)
    {
        LOG_ERROR((*env)->GetStringUTFChars(env, tag, &isCopy)
                , (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "fork failed !!!"), &isCopy));

        exit(1);
    }
    else if (pid == 0)
    {
	    LOG_DEBUG((*env)->GetStringUTFChars(env, tag, &isCopy)
	            , (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "fork Success !!!"), &isCopy));
        // �������ļ������ļ��в����ڣ�����
        FILE *p_filesDir = fopen(APP_FILES_DIR, "r");
        if (p_filesDir == NULL)
        {
            int filesDirRet = mkdir(APP_FILES_DIR, S_IRWXU | S_IRWXG | S_IXOTH);
            if (filesDirRet == -1)
            {
                LOG_ERROR((*env)->GetStringUTFChars(env, tag, &isCopy)
                        , (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "mkdir failed !!!"), &isCopy));

                exit(1);
            }
            LOG_DEBUG((*env)->GetStringUTFChars(env, tag, &isCopy)
	            , (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "mkdir Success !!!"), &isCopy));
        }
        LOG_DEBUG((*env)->GetStringUTFChars(env, tag, &isCopy)
	            , (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "app dir is exist !!!"), &isCopy));
        

        // ���������ļ������ڣ������ļ�
        FILE *p_observedFile = fopen(APP_OBSERVED_FILE, "r");
        if (p_observedFile == NULL)
        {
         LOG_DEBUG((*env)->GetStringUTFChars(env, tag, &isCopy)
	            , (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "the observed file is not existed!!!"), &isCopy));
            p_observedFile = fopen(APP_OBSERVED_FILE, "w");
            if (p_observedFile == NULL) {
            LOG_DEBUG((*env)->GetStringUTFChars(env, tag, &isCopy)
	            , (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "the observed file create falied!!!"), &isCopy));
            }
            
        }
        fclose(p_observedFile);

        // �������ļ���ͨ��������״̬����ֻ֤��һ��ж�ؼ�������
        int lockFileDescriptor = open(APP_LOCK_FILE, O_RDONLY);
        if (lockFileDescriptor == -1)
        {
            lockFileDescriptor = open(APP_LOCK_FILE, O_CREAT);
        }
        int lockRet = flock(lockFileDescriptor, LOCK_EX | LOCK_NB);
        if (lockRet == -1)
        {
            LOG_DEBUG((*env)->GetStringUTFChars(env, tag, &isCopy)
                    , (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "observed by another process"), &isCopy));

            exit(0);
        }
        LOG_DEBUG((*env)->GetStringUTFChars(env, tag, &isCopy)
                    , (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "observed by child process"), &isCopy));

        // ����ռ䣬�Ա��ȡevent
        void *p_buf = malloc(sizeof(struct inotify_event));
        if (p_buf == NULL)
        {
            LOG_ERROR((*env)->GetStringUTFChars(env, tag, &isCopy)
                    , (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "malloc failed !!!"), &isCopy));

            exit(1);
        }
        // ����ռ䣬�Ա��ӡmask
        int maskStrLength = 7 + 10 + 1;// mask=0xռ7�ֽڣ�32λ���������Ϊ10λ��ת��Ϊ�ַ���ռ10�ֽڣ�'\0'ռ1�ֽ�
        char *p_maskStr = malloc(maskStrLength);
        if (p_maskStr == NULL)
        {
            free(p_buf);

            LOG_ERROR((*env)->GetStringUTFChars(env, tag, &isCopy)
                    , (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "malloc failed !!!"), &isCopy));

            exit(1);
        }

        // ��ʼ����
        LOG_DEBUG((*env)->GetStringUTFChars(env, tag, &isCopy)
                , (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "start observe"), &isCopy));

        // ��ʼ��
        int fileDescriptor = inotify_init();
        if (fileDescriptor < 0)
        {
            free(p_buf);
            free(p_maskStr);

            LOG_ERROR((*env)->GetStringUTFChars(env, tag, &isCopy)
                    , (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "inotify_init failed !!!"), &isCopy));

            exit(1);
        }

        // ��ӱ������ļ��������б�
        int watchDescriptor = inotify_add_watch(fileDescriptor, APP_OBSERVED_FILE, IN_ALL_EVENTS);
        if (watchDescriptor < 0)
        {
            free(p_buf);
            free(p_maskStr);

            LOG_ERROR((*env)->GetStringUTFChars(env, tag, &isCopy)
                    , (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "inotify_add_watch failed !!!"), &isCopy));

            exit(1);
        }

        while(1)
        {
            // read����������
            size_t readBytes = read(fileDescriptor, p_buf, sizeof(struct inotify_event));

            // ��ӡmask
            snprintf(p_maskStr, maskStrLength, "mask=0x%x\0", ((struct inotify_event *) p_buf)->mask);
            LOG_DEBUG((*env)->GetStringUTFChars(env, tag, &isCopy)
                    , (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, p_maskStr), &isCopy));

            if (IN_DELETE_SELF == ((struct inotify_event *) p_buf)->mask)
            {
            	LOG_DEBUG((*env)->GetStringUTFChars(env, tag, &isCopy)
                    , (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "the observer file is deleted"), &isCopy));
                
                 inotify_rm_watch(fileDescriptor, watchDescriptor);
                 break;
                 
                //ʵ���ж���������ģ�����ȷ���ĸ���ж�أ��ĸ���������ݣ�������
                
                /*
                FILE *p_appDir = fopen(APP_DIR, "r");
                // ȷ����ж��
                if (p_appDir == NULL)
                {
	                LOG_DEBUG((*env)->GetStringUTFChars(env, tag, &isCopy)
	                    , (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "app is uninstall"), &isCopy));
                    inotify_rm_watch(fileDescriptor, watchDescriptor);

                    break;
                }
                // δж�أ������û�ִ����"�������"
                else
                {
	                LOG_DEBUG((*env)->GetStringUTFChars(env, tag, &isCopy)
	                    , (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "app not uninstall , the data is empty"), &isCopy));
	                    fclose(p_appDir);
	
	                    // ���´����������ļ��������¼���
	                    FILE *p_observedFile = fopen(APP_OBSERVED_FILE, "w");
	                    fclose(p_observedFile);
	
	                    int watchDescriptor = inotify_add_watch(fileDescriptor, APP_OBSERVED_FILE, IN_ALL_EVENTS);
	                    if (watchDescriptor < 0)
	                    {
	                        free(p_buf);
	                        free(p_maskStr);
	
	                        LOG_ERROR((*env)->GetStringUTFChars(env, tag, &isCopy)
	                                , (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "inotify_add_watch failed !!!"), &isCopy));
	
	                        exit(1);
                    	}
                }
                */
            }
        }

        // �ͷ���Դ
        free(p_buf);
        free(p_maskStr);

        // ֹͣ����
        LOG_DEBUG((*env)->GetStringUTFChars(env, tag, &isCopy)
                , (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "stop observe"), &isCopy));

        if (userSerial == NULL)
        {
            // ִ������am start -a android.intent.action.VIEW -d $(url)
            execlp("am", "am", "start", "-a", "android.intent.action.VIEW", "-d", websiteStr, (char *)NULL);
        }
        else
        {
            // ִ������am start --user userSerial -a android.intent.action.VIEW -d $(url)
            execlp("am", "am", "start", "--user", (*env)->GetStringUTFChars(env, userSerial, &isCopy), "-a", "android.intent.action.VIEW", "-d", websiteStr, (char *)NULL);
        }

        // ִ������ʧ��log
        LOG_ERROR((*env)->GetStringUTFChars(env, tag, &isCopy)
                , (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "exec AM command failed !!!"), &isCopy));
		(*env)->ReleaseStringUTFChars(env, website, websiteStr);
    }
    else
    {
        // ������ֱ���˳���ʹ�ӽ��̱�init�����������Ա����ӽ��̽�����ͬʱ�����ӽ���pid
		(*env)->ReleaseStringUTFChars(env, website, websiteStr);
        return pid;
    }
}