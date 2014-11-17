/*
 * android-specific implementation of the s3eApkExpansionFile extension.
 * Add any platform-specific functionality here.
 */
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
#include "s3eApkExpansionFile_internal.h"

#include "s3eEdk.h"
#include "s3eEdk_android.h"
#include <jni.h>
#include "IwDebug.h"

static jobject g_Obj;
static jmethodID g_s3eApkExpansionFileInitialize;
static jmethodID g_s3eApkExpansionFileGetPathToMainObb;
static jmethodID g_s3eApkExpansionFileGetPathToPatchObb;
static jmethodID g_s3eApkExpansionFileNeedDownloadMainObb;
static jmethodID g_s3eApkExpansionFileNeedDownloadPatchObb;
static jmethodID g_s3eApkExpansionFileStartDownloading;

void JNICALL s3eApkExpansionFileDownloadingCompletedCallback(JNIEnv* env, jobject obj, jobject param);
void JNICALL s3eApkExpansionFileDownloadingFailedCallback(JNIEnv* env, jobject obj, jobject param);
void JNICALL s3eApkExpansionFileDownloadingUpdateCallback(JNIEnv* env, jobject obj, jobject param);

s3eResult s3eApkExpansionFileInit_platform()
{
    // Get the environment from the pointer
    JNIEnv* env = s3eEdkJNIGetEnv();
    jobject obj = NULL;
    jmethodID cons = NULL;

    // Get the extension class
    jclass cls = s3eEdkAndroidFindClass("com/ideaworks3d/marmalade/s3eApkExpansionFile/s3eApkExpansionFile");
    if (!cls)
        goto fail;

    // Get its constructor
    cons = env->GetMethodID(cls, "<init>", "()V");
    if (!cons)
        goto fail;

    // Construct the java class
    obj = env->NewObject(cls, cons);
    if (!obj)
        goto fail;

    // Get all the extension methods
    g_s3eApkExpansionFileInitialize = env->GetMethodID(cls, "s3eApkExpansionFileInitialize", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I");
    if (!g_s3eApkExpansionFileInitialize)
        goto fail;

    g_s3eApkExpansionFileGetPathToMainObb = env->GetMethodID(cls, "s3eApkExpansionFileGetPathToMainObb", "()Ljava/lang/String;");
    if (!g_s3eApkExpansionFileGetPathToMainObb)
        goto fail;

    g_s3eApkExpansionFileGetPathToPatchObb = env->GetMethodID(cls, "s3eApkExpansionFileGetPathToPatchObb", "()Ljava/lang/String;");
    if (!g_s3eApkExpansionFileGetPathToPatchObb)
        goto fail;

    g_s3eApkExpansionFileNeedDownloadMainObb = env->GetMethodID(cls, "s3eApkExpansionFileNeedDownloadMainObb", "()Z");
    if (!g_s3eApkExpansionFileNeedDownloadMainObb)
        goto fail;

    g_s3eApkExpansionFileNeedDownloadPatchObb = env->GetMethodID(cls, "s3eApkExpansionFileNeedDownloadPatchObb", "()Z");
    if (!g_s3eApkExpansionFileNeedDownloadPatchObb)
        goto fail;

    g_s3eApkExpansionFileStartDownloading = env->GetMethodID(cls, "s3eApkExpansionFileStartDownloading", "()V");
    if (!g_s3eApkExpansionFileStartDownloading)
        goto fail;

	static const JNINativeMethod methods[]=
	{
		{ "native_DownloadingComplete", "()V", 	(void*)&s3eApkExpansionFileDownloadingCompletedCallback },
		{ "native_DownloadingFailed", "(I)V",  	(void*)&s3eApkExpansionFileDownloadingFailedCallback },
		{ "native_DownloadingUpdate", "(Lcom/ideaworks3d/marmalade/s3eApkExpansionFile/s3eApkExpansionFile$DownloadingInfo;)V",
												(void*)&s3eApkExpansionFileDownloadingUpdateCallback }
	};
	
	// Register the native hooks
	if(env->RegisterNatives(cls, methods, sizeof(methods) / sizeof(methods[0])) )
		goto fail;

    IwTrace(APKEXPANSIONFILE, ("APKEXPANSIONFILE init success"));
    g_Obj = env->NewGlobalRef(obj);
    env->DeleteLocalRef(obj);
    env->DeleteGlobalRef(cls);

    // Add any platform-specific initialisation code here
    return S3E_RESULT_SUCCESS;

fail:
    jthrowable exc = env->ExceptionOccurred();
    if (exc)
    {
        env->ExceptionDescribe();
        env->ExceptionClear();
        IwTrace(s3eApkExpansionFile, ("One or more java methods could not be found"));
    }

    env->DeleteLocalRef(obj);
    env->DeleteGlobalRef(cls);
    return S3E_RESULT_ERROR;

}

void s3eApkExpansionFileTerminate_platform()
{ 
    // Add any platform-specific termination code here
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->DeleteGlobalRef(g_Obj);
    g_Obj = NULL;
}

static char* javaStringToNative(jstring javastr)
{
    if (!javastr) return NULL;
    JNIEnv* env = s3eEdkJNIGetEnv();
    const char* utf = env->GetStringUTFChars(javastr, NULL);
    jsize utfLen = env->GetStringUTFLength(javastr) + 1;
    char* native = new char[utfLen];
    memcpy(native, utf, utfLen);
    env->ReleaseStringUTFChars(javastr, utf);
    return native;
}

s3eResult s3eApkExpansionFileInitialize_platform(const char* apiKey, const char* mainFileVersion, const char* patchFileVersion)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring apiKey_jstr = env->NewStringUTF(apiKey);
    jstring mainFileVersion_jstr = (mainFileVersion == NULL) ? NULL : env->NewStringUTF(mainFileVersion);
    jstring patchFileVersion_jstr = (patchFileVersion == NULL) ? NULL : env->NewStringUTF(patchFileVersion);
    return (s3eResult)env->CallIntMethod(g_Obj, g_s3eApkExpansionFileInitialize, apiKey_jstr, mainFileVersion_jstr, patchFileVersion_jstr);
}

const char* s3eApkExpansionFileGetPathToMainObb_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring javastr = (jstring)env->CallObjectMethod(g_Obj, g_s3eApkExpansionFileGetPathToMainObb);	
    jsize utfLen = env->GetStringUTFLength(javastr);
	if (utfLen > 1023) return NULL;
	static char buf[1024];
    const char* utf = env->GetStringUTFChars(javastr, NULL);
    memcpy(buf, utf, utfLen);
	buf[utfLen] = 0;
	env->ReleaseStringUTFChars(javastr, utf);
    return buf;		
}

const char* s3eApkExpansionFileGetPathToPatchObb_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring javastr = (jstring)env->CallObjectMethod(g_Obj, g_s3eApkExpansionFileGetPathToPatchObb);	
    jsize utfLen = env->GetStringUTFLength(javastr);
	if (utfLen > 1023) return NULL;
	static char buf[1024];
    const char* utf = env->GetStringUTFChars(javastr, NULL);
    memcpy(buf, utf, utfLen);
	buf[utfLen] = 0;
	env->ReleaseStringUTFChars(javastr, utf);
    return buf;		
}

bool s3eApkExpansionFileNeedDownloadMainObb_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    return (bool)env->CallBooleanMethod(g_Obj, g_s3eApkExpansionFileNeedDownloadMainObb);
}

bool s3eApkExpansionFileNeedDownloadPatchObb_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    return (bool)env->CallBooleanMethod(g_Obj, g_s3eApkExpansionFileNeedDownloadPatchObb);
}

void s3eApkExpansionFileStartDownloading_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_s3eApkExpansionFileStartDownloading);
}

void JNICALL s3eApkExpansionFileDownloadingCompletedCallback(JNIEnv* env, jobject obj, jobject param)
{
	s3eEdkCallbacksEnqueue(S3E_EXT_APKEXPANSIONFILE_HASH, S3E_APKEXPANSIONFILE_DOWNLOADING_COMPLETED, NULL, 0 , NULL, false, NULL, NULL );
}

void JNICALL s3eApkExpansionFileDownloadingFailedCallback(JNIEnv* env, jobject obj, jobject param)
{
	int err = ((jint)param);
	s3eEdkCallbacksEnqueue(S3E_EXT_APKEXPANSIONFILE_HASH, S3E_APKEXPANSIONFILE_DOWNLOADING_FAILED, (void*)err, 0 , NULL, false, NULL, NULL );
	
}

static void s3eApkExpansionFileReleaseUpdateCallback(uint32, int32, void*, void*, int32, void* pData )
{
	s3eApkExpansionFileDownloadingInfo* pInfo = (s3eApkExpansionFileDownloadingInfo*) pData;	
    delete pInfo;
}

void JNICALL s3eApkExpansionFileDownloadingUpdateCallback(JNIEnv* env, jobject obj, jobject param)
{
	s3eApkExpansionFileDownloadingInfo* pInfo = new s3eApkExpansionFileDownloadingInfo();
	jclass cls = env->FindClass("com/ideaworks3d/marmalade/s3eApkExpansionFile/s3eApkExpansionFile$DownloadingInfo");
	pInfo->mOverallTotal 	= env->GetLongField( param, env->GetFieldID(cls, "mOverallTotal", "J"));
	pInfo->mOverallProgress = env->GetLongField( param, env->GetFieldID(cls, "mOverallProgress", "J"));	
	pInfo->mTimeRemaining 	= env->GetLongField( param, env->GetFieldID(cls, "mTimeRemaining", "J"));		
	pInfo->mCurrentSpeed 	= env->GetFloatField(param, env->GetFieldID(cls, "mCurrentSpeed", "F"));			
	if (s3eEdkCallbacksEnqueue(S3E_EXT_APKEXPANSIONFILE_HASH, S3E_APKEXPANSIONFILE_DOWNLOADING_UPDATE,
		(void*)pInfo, 0, 
		NULL, false,
		&s3eApkExpansionFileReleaseUpdateCallback, (void*)pInfo) != S3E_RESULT_SUCCESS)
	{
		s3eDebugOutputString("s3eApkExpansionFileDownloadingUpdateCallback: s3eEdkCallbacksEnqueue - some error!");
	}
}