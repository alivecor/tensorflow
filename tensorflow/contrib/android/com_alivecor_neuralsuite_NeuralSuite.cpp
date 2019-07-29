
#include <jni.h>
#include <android/log.h>

#include <string>
#include <unistd.h>
#include <vector>
#include "tensorflow/contrib/android/inversion_model.h"
#include "tensorflow/contrib/android//identity_model.h"
#include "tensorflow/contrib/android/com_alivecor_neuralsuite_NeuralSuite.h"

#define SIG_BOOLEAN "Z" //boolean
#define SIG_BYTE 	"B" //byte
#define SIG_CHAR 	"C" //char
#define SIG_SHORT 	"S" //short
#define SIG_INT 	"I" //int
#define SIG_LONG 	"J" //long
#define SIG_FLOAT 	"F" //float
#define SIG_DOUBLE 	"D" //double
#define SIG_STRING 	"Ljava/lang/String;" //String
#define SIG_ARR_INT     "[I"
#define SIG_ARR_FLOAT   "[F"
#define SIG_ARR_DOUBLE  "[D"
#define SIG_ARR_STRING  "[Ljava/lang/String;"

#define DEBUG 0

#define LOG_TAG "com.alivecor.neuralsuite.android"

#define LOG_I(...)      __android_log_print(ANDROID_LOG_INFO,   LOG_TAG,__VA_ARGS__)
#define LOG_E(...)      __android_log_print(ANDROID_LOG_ERROR,  LOG_TAG,__VA_ARGS__)

void loge(char* buffer, char* formatStr, ...) {
    va_list args;
    va_start(args, formatStr);
    int ret = vsprintf(buffer, formatStr, args);
    va_end(args);

    fprintf(stderr, "%s\n", buffer);
    LOG_E(buffer);
}

void logi(char* buffer, char* formatStr, ...) {
    if (!DEBUG) return;
    va_list args;
    va_start(args, formatStr);
    int ret = vsprintf(buffer, formatStr, args);
    va_end(args);

    fprintf(stderr, "%s\n", buffer);
    LOG_I(buffer);
}

int jniCheck() {
    char buffer[10000];
    return 47;
}
//--------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

static JavaVM *java_vm;

struct tst_struct {
    int i;
    float f;
    char* text;
};

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    LOG_I("JNI_OnLoad");
    /*
    //java_vm = vm;

    // Get JNI Env for all function calls
    JNIEnv* env;
    if (vm->GetEnv(vm, (void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        LOG_E("GetEnv failed.");
        return -1;
    }

    // Find the class calling native function
    jclass classToCheck = (*env).FindClass(env, "com/alivecor/neuralsuite/NeuralSuite");
    if (class_home == NULL) {
        LOG_E("FindClass failed : No class found.");
        return -1;
    } else {
        LOG_I("Class found");
    }

    // Register native method for getUsbPermission
    JNINativeMethod nativeMethod[1] = {
            { "runInversionCheck", "([FFLjava/lang/String;)I", runInversionCheckPrivate}
    };

    if (env->RegisterNatives(env, classToCheck, nativeMethod , 1)) {
        LOG_E ("RegisterNatives Failed.");
        return -1;
    }

    */
    tst_struct a  = { .i = 42, .f = 0.17f};

    return JNI_VERSION_1_6;
}

using Status = ac::InversionModel::Status;

int runInversionCheckPrivate(jfloatArray samplesMV, jfloat confidenceThreshold, jstring inversionModelFile)
{
    //LOG_I (runInversionCheckPrivate);

}

//enum class Status { kSuccess = 0, kFailedSession = -1, kFailedReadGraph = -2, kFailedRun = -3, kInvalidInput = -4 };
int toInt(Status status) {
    switch (status) {
        case Status::kSuccess           : return com_alivecor_neuralsuite_NeuralSuite_STATUS_SUCCESS;
        case Status::kFailedSession     : return com_alivecor_neuralsuite_NeuralSuite_STATUS_FAILED_SESSION;
        case Status::kFailedReadGraph   : return com_alivecor_neuralsuite_NeuralSuite_STATUS_FAILED_READ_GRAPH;
        case Status::kFailedRun         : return com_alivecor_neuralsuite_NeuralSuite_STATUS_FAILED_RUN;
        case Status::kInvalidInput      : return com_alivecor_neuralsuite_NeuralSuite_STATUS_INVALID_INPUT;
    }
}

std::vector<float> toFloatVector(jfloatArray jFloats, JNIEnv *env) {

    jfloat* values  = env->GetFloatArrayElements(jFloats, 0);
    jsize   len     = env->GetArrayLength(jFloats);

    std::vector<float> result(static_cast<unsigned long>(len));
    for (jsize i = 0; i < len; i++) {
        result[i] = values[i];
    }

    env->ReleaseFloatArrayElements(jFloats, values, 0);
    return result;
}


JNIEXPORT void JNICALL
Java_com_alivecor_neuralsuite_NeuralSuite_jniCheck (JNIEnv *env, jclass type) {
    char logbuf[10000];
    int result = jniCheck();
    logi(logbuf, "JNI_CHECK: Java_com_alivecor_neuralsuite_NeuralSuite_jniCheck() result: %d", result);
}

JNIEXPORT jint JNICALL
Java_com_alivecor_neuralsuite_NeuralSuite_runInversionCheck(JNIEnv *env, jclass type, jfloatArray samplesMV, jfloat confidenceThreshold, jstring inversionModelFile, jobject outResult) {
    if (env == NULL)  {
        printf("Java_com_alivecor_neuralsuite_NeuralSuite_runInversionCheck() NULL ENVIRONMENT -> CHECK METHOD EXISTS: TRUE\n");
        return 100;
    }
    char logbuf[10000];

    const char *modelPath       = env->GetStringUTFChars(inversionModelFile, 0);
    jfloat*  samplesArr         = env->GetFloatArrayElements(samplesMV, 0);
    jsize    samplesLen         = env->GetArrayLength(samplesMV);

    jclass   resultClass        = env->GetObjectClass(outResult);
    jfieldID fidErrorMsg        = env->GetFieldID(resultClass,  "errorMsg",     SIG_STRING);
    jfieldID fidInverted        = env->GetFieldID(resultClass,  "inverted",     SIG_BOOLEAN);
    jfieldID fidProbability     = env->GetFieldID(resultClass,  "probability",  SIG_FLOAT);

    std::vector<float> ecg_data = toFloatVector(samplesMV, env);
    logi(logbuf, "runInversionCheck() samples: %d", ecg_data.size());
    for (int i = 0; i < ecg_data.size(); i++) {
        logi(logbuf, "%f", ecg_data[i]);
        if (i > 10) break;
    }

    logi(logbuf, "runInversionCheck() model is ready to initialize. modelPath: %s", modelPath);
    ac::InversionModel model;
    Status status = model.InitializeFromGraph(modelPath);

    loge(logbuf, "runInversionCheck() model init status: %d: %s\n", toInt(status), model.error_string().c_str());
    if (status != Status::kSuccess) {
        loge(logbuf, "runInversionCheck() Failed to initialize model:\n%s", model.error_string().c_str());
        return toInt(status);
    }

    ac::InversionResult result;
    status = model.CalculateInvertedProbability(ecg_data, &result);

    if (status != Status::kSuccess) {
        loge(logbuf, "Failed to run model\n%s", model.error_string().c_str());
        jstring jErrorMsg = env->NewStringUTF(model.error_string().c_str());
        env->SetObjectField(outResult, fidErrorMsg, jErrorMsg);
        return toInt(status);
    }

    logi(logbuf, "runInversionCheck() Done Calculate Inverted. Status: %d: %s", toInt(status), model.error_string().c_str());
    logi(logbuf, "runInversionCheck() Inversion probability: %g", result.raw_inverted_probability);
    logi(logbuf, "runInversionCheck() Was inverted: %d", result.was_inverted ? 1 : 0);
    logi(logbuf, "runInversionCheck() Confident: %d", result.confident ? 1 : 0);

    env->SetBooleanField(outResult, fidInverted,    result.was_inverted);
    env->SetFloatField  (outResult, fidProbability, result.raw_inverted_probability);

    // cleanup
    env->ReleaseStringUTFChars(inversionModelFile, modelPath);
    env->ReleaseFloatArrayElements(samplesMV, samplesArr, 0);

    return toInt(status);
}

JNIEXPORT jint JNICALL
Java_com_alivecor_neuralsuite_NeuralSuite_runIdentityCheck(JNIEnv *env, jclass type, jfloatArray samplesMV, jfloat confidenceThreshold, jstring identityModelFile, jobject outResult) {
    char logbuf[10000];

    const char *modelPath       = env->GetStringUTFChars(identityModelFile, 0);
    std::vector<float> ecg_data = toFloatVector(samplesMV, env);
    logi(logbuf, "runIdentityCheck() samples: %d", ecg_data.size());

    jclass   resultClass        = env->GetObjectClass(outResult);
    jfieldID fidErrorMsg        = env->GetFieldID(resultClass,  "errorMsg",       SIG_STRING);
    jfieldID fidIdentityMatched = env->GetFieldID(resultClass,  "identityMatched",SIG_BOOLEAN);
    jfieldID fidProbability     = env->GetFieldID(resultClass,  "probability",    SIG_FLOAT);


    for (int i = 0; i < ecg_data.size(); i++) {
        logi(logbuf, "%f", ecg_data[i]);
        if (i > 10) break;
    }

    logi(logbuf, "runIdentityCheck() model is ready to initialize. modelPath: %s", modelPath);
    ac::IdentityModel   model;
    Status status = model.InitializeFromGraph(modelPath);
    logi(logbuf, "runIdentityCheck() model init status: %d: %s", toInt(status), model.error_string().c_str());

    if (status != Status::kSuccess) {
        loge(logbuf, "runIdentityCheck() Failed to initialize model: %s", model.error_string().c_str());
        jstring jErrorMsg = env->NewStringUTF(model.error_string().c_str());
        env->SetObjectField(outResult, fidErrorMsg, jErrorMsg);
        return toInt(status);
    }

    logi(logbuf, "runIdentityCheck() Running model on %d samples ", ecg_data.size());

    ac::IdentityResult  result;
    status = model.CalculateIdentityProbability(ecg_data, &result);

    logi(logbuf, "runIdentityCheck() Running complete. status: %d", status);

    if (status != Status::kSuccess) {
        loge(logbuf, "Failed to run model: %s", model.error_string().c_str());
        jstring jErrorMsg = env->NewStringUTF(model.error_string().c_str());
        env->SetObjectField(outResult, fidErrorMsg, jErrorMsg);
        return toInt(status);
    }

    logi(logbuf, "runIdentityCheck() Done Calculate Identity. Status: %d: %s", toInt(status), model.error_string().c_str());
    logi(logbuf, "runIdentityCheck() Identity Matched: %d", result.identity_matched);
    logi(logbuf, "runIdentityCheck() Probability: %f", result.identity_probability);

    env->SetBooleanField(outResult, fidIdentityMatched, result.identity_matched);
    env->SetFloatField  (outResult, fidProbability,     result.identity_probability);

    // cleanup
    env->ReleaseStringUTFChars(identityModelFile, modelPath);

    return toInt(status);
}

#ifdef __cplusplus
}
#endif






