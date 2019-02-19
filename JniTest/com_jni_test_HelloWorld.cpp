#define _CRT_SECURE_NO_WARNINGS
#include "com_jni_test_HelloWorld.h"

#include <Windows.h>
#include <iostream>
#include <stdlib.h>

JNIEXPORT jstring JNICALL Java_com_jni_test_HelloWorld_getStringFromC
(JNIEnv * env, jobject obj, jstring jstr) {
	//return env->NewStringUTF(env,"Hello ,I am hanlonglin from CCCCC+++"); //c��д��   env��һ������ָ��

	//WinExec("E://software_install//qq9.0.4//Bin//QQScLauncher.exe", 0);  //����qq.exe
	return env->NewStringUTF("Hello ,I am hanlonglin from CCCCC+++");   //c++��д��  env��һ��һ��ָ��
}

JNIEXPORT jint JNICALL Java_com_jni_test_HelloWorld_calculate
(JNIEnv * env, jobject obj, jint a, jint b) {
	//return env->NewStringUTF(env,"Hello ,I am hanlonglin from CCCCC+++"); //c��д��   env��һ������ָ��
	std::cout << "c++ �����ڽ��м���..." << std::endl;
	return a + b;   //c++��д��  env��һ��һ��ָ��
}

//����java���� 
//���ʳ�Ա����
JNIEXPORT jstring JNICALL Java_com_jni_test_HelloWorld_accessField
(JNIEnv * env, jobject obj) {
	//��ȡjclass
	jclass clazz = env->GetObjectClass(obj);

	//�������ƣ�����ǩ��
	jfieldID  fid = env->GetFieldID(clazz, "key", "Ljava/lang/String;");

	//��ȡ����key��ֵ
	jstring jstr = (jstring)env->GetObjectField(obj, fid);

	//jstring >> C���ַ���
	//is_Copy
	const char * c_str = env->GetStringUTFChars(jstr, NULL);

	//ƴ�ӵõ��µ��ַ���
	char text[30] = "good man is ";
	strcat_s(text, c_str);

	//C���ַ��� >> jstring
	jstring new_jstr = env->NewStringUTF(text);

	//�޸�key
	env->SetObjectField(obj, fid, new_jstr);

	return new_jstr;
}

//���ʾ�̬����
JNIEXPORT jint JNICALL Java_com_jni_test_HelloWorld_accessStaticField
(JNIEnv * env, jobject obj) {

	//��ȡjclass
	jclass clazz = env->GetObjectClass(obj);

	//��ȡfield
	jfieldID fid = env->GetStaticFieldID(clazz, "count", "I");

	//��ȡkey��ֵ
	jint count = (jint)env->GetStaticIntField(clazz, fid);

	count++;

	//�޸�count
	env->SetStaticIntField(clazz, fid, count);

	return count;

}

//����java����
//���ʳ�Ա����
JNIEXPORT jint JNICALL Java_com_jni_test_HelloWorld_accessMethod
(JNIEnv * env, jobject obj) {

	//��ȡclass
	jclass clazz = env->GetObjectClass(obj);

	//��ȡmethodID
	jmethodID mid = env->GetMethodID(clazz, "generateRandom", "()I");

	//����method
	jint re_int = env->CallIntMethod(obj, mid);

	return re_int;
}

//���ʾ�̬����
JNIEXPORT jstring JNICALL Java_com_jni_test_HelloWorld_accessStaticMethod
(JNIEnv * env, jobject obj) {

	//��ȡ��
	jclass clazz = env->GetObjectClass(obj);

	//��ȡmethodID
	jmethodID mid = env->GetStaticMethodID(clazz, "getUUID", "()Ljava/lang/String;");

	//����method
	jstring jstr = (jstring)env->CallStaticObjectMethod(clazz, mid);

	//����ļ�����
	const char * uuid = env->GetStringUTFChars(jstr, NULL);

	//ƴ��
	char filename[100];
	sprintf_s(filename, "D://%s.txt", uuid);

	FILE *fp = fopen(filename, "w");
	fputs("I like it ", fp);
	fclose(fp);

	return jstr;
}

//���ʹ��췽�� 
//ʹ��java.util.Data����һ����ǰ��ʱ���
JNIEXPORT jobject JNICALL Java_com_jni_test_HelloWorld_accessConstructor
(JNIEnv * env, jobject obj) {

	//��ȡclass
	jclass clazz = env->FindClass("java/util/Date");

	//methodID
	jmethodID constuctor_mid = env->GetMethodID(clazz, "<init>", "()V");   //���췽�� className��Ҫʹ�� <init> ()V���޲ι��췽����ǩ��

	//ʵ��������
	jobject date_obj = env->NewObject(clazz, constuctor_mid);

	//����getTime����
	jmethodID getTime_mid = env->GetMethodID(clazz, "getTime", "()J");

	jlong time = env->CallLongMethod(date_obj, getTime_mid);

	printf("time:%ld\n", time);
	//std::cout << "c++ �л�ȡʱ�����" << time << std::endl;

	return date_obj;
}

//���ø���ķ���
JNIEXPORT void JNICALL Java_com_jni_test_HelloWorld_accessNonvirtualMethod
(JNIEnv * env, jobject obj) {

	//��ȡ��
	jclass clazz = env->GetObjectClass(obj);

	//��ȡhuman����
	jfieldID fid = env->GetFieldID(clazz, "humen", "Lcom/jni/test/Humen;");

	//��ȡobj
	jobject humen_obj = env->GetObjectField(obj, fid);

	//ִ��sayHi����
	jclass humen_class = env->FindClass("com/jni/test/Humen");

	//��ȡmethodID
	jmethodID sayhi_mid = env->GetMethodID(humen_class, "sayHi", "()V");

	//����sayHi()
	env->CallNonvirtualVoidMethod(humen_obj, humen_class, sayhi_mid);
}

//���봫������
JNIEXPORT jobject JNICALL Java_com_jni_test_HelloWorld_chineseChars
(JNIEnv * env, jobject obj, jstring jstr) {

	const char* j_str = env->GetStringUTFChars(jstr, NULL);
	std::cout << "c++�н�������java���ַ�����" << j_str << std::endl;

	char *c_str = "�������ɽ�";
	//const char c_str[] = "�������ɽ�";

	//jstring jstr = env->NewStringUTF("��������c++�������ַ���"); //����������
	//ͨ������ java��String �� String(byte[] bytes,String charsetName) ��ָ������
	//��ȡString ��
	jclass str_class = env->FindClass("java/lang/String");

	//��ȡString���캯��
	jmethodID str_mid = env->GetMethodID(str_class, "<init>", "([BLjava/lang/String;)V");

	//jbyte -> char
	//jbyteArray -> char[]
	jbyteArray bytes = env->NewByteArray(strlen(c_str));

	//byte���鸳ֵ
	//0 -> strlen ��ͷ��β��
	env->SetByteArrayRegion(bytes, 0, strlen(c_str), (jbyte*)c_str);

	//�ַ�����
	jstring charsetName = env->NewStringUTF("GB2312");

	return env->NewObject(str_class, str_mid, bytes, charsetName);

}

int compare(const void *a, const void *b) {
	int *pa = (int*)a;
	int *pb = (int*)b;
	return (*pa) - (*pb);
}

//�������� ����������
JNIEXPORT void JNICALL Java_com_jni_test_HelloWorld_giveArray
(JNIEnv * env, jobject obj, jintArray intArray) {

	//��ȡ����ָ��
	//jintArray >> jintָ�� >> c int ����
	jint *elems = env->GetIntArrayElements(intArray, NULL);

	//���鳤��
	int len = env->GetArrayLength(intArray);

	//����
	qsort(elems, len, sizeof(jint), compare);

	//ͬ��
	//0             java������и��� �����ͷ�C/C++����
	//JNI_ABORT     java���鲻���и���  �����ͷ�C/C++����
	//JNI_COMMIT    java������и���  ���Ƿ�C/C++���飨����ִ���꣬���黹�ǻᱻ�ͷţ�
	env->ReleaseIntArrayElements(intArray, elems, JNI_COMMIT);
}

//��������
JNIEXPORT jintArray JNICALL Java_com_jni_test_HelloWorld_getArray
(JNIEnv * env, jobject obj,jint len) {
	
	//����һ��jint����
	jintArray jint_arr = env->NewIntArray(len);

	jint *elems = env->GetIntArrayElements(jint_arr, NULL);

	//��ֵ
	int i = 0;
	for (; i < len; i++) {
		elems[i] = i;
	}

	//ͬ��
	env->ReleaseIntArrayElements(jint_arr, elems, JNI_COMMIT);

	return jint_arr; 
}

//JNI���ñ���
//�������ͣ��ֲ����ú�ȫ������
//���ã���JNI�и�֪�������ʱ����һ��JNI����

//�ֲ����ã�ͨ������DeleteLocalRef�����ֶ��ͷŶ���
//1.����һ���ܴ��java����ʹ����֮�󣬻�Ҫ���и��ӵĺ�ʱ����
//2.ռ���˴����ľֲ����ã�ռ����̫����ڴ棬����Щ�ֲ����ø�����Ĳ���û�й�����
JNIEXPORT void JNICALL Java_com_jni_test_HelloWorld_localRef
(JNIEnv * env, jobject obj) {
	int i = 0;
	for (; i < 20; i++) {
		//����Date����
		jclass clazz = env->FindClass("java/util/Date");
		jmethodID constructor_mid = env->GetMethodID(clazz, "<init>", "()V");
		jobject date_obj = env->NewObject(clazz, constructor_mid);

		//�˴�ʡ��һ���д���...

		//����ʹ��Date���� 
		//֪ͨ����������������Щ����
		env->DeleteLocalRef(date_obj);

		//�˴�ʡ��һ���д���...

	}
}


//ȫ������
//1.���������̣߳����ֶ������ڴ�ʹ��
jstring global_str;

//����
JNIEXPORT void JNICALL Java_com_jni_test_HelloWorld_createGlobalRef
(JNIEnv * env, jobject obj) {
	jstring jstr = env->NewStringUTF("hanlonglin is a good man");
	global_str = (jstring)env->NewGlobalRef(jstr);
}

//���
JNIEXPORT jstring JNICALL Java_com_jni_test_HelloWorld_getGlobalRef
(JNIEnv * env, jobject obj) {
	return global_str;
}

//�ͷ�
JNIEXPORT void JNICALL Java_com_jni_test_HelloWorld_deleteGlobalRef
(JNIEnv * env, jobject obj) {
	env->DeleteGlobalRef(global_str);
}

//�쳣����
//1.��֤java�����������
//2.���ȴ�ʩ��֤C�����������

//JNI�Լ��׳����쳣����java���޷�����׽��ֻ����C�����
//�û�ͨ��ThrowNew�׳����쳣��������java�㲶׽

JNIEXPORT void JNICALL Java_com_jni_test_HelloWorld_exception
(JNIEnv * env, jobject obj) {
	jclass clazz = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(clazz, "key2", "Ljava/lang/String;");  //û��key2����

	jthrowable exception = env->ExceptionOccurred();
	if (exception != NULL) {
		//��java�����һ����������
		//����쳣��Ϣ
		env->ExceptionClear();

		std::cout << "c++�в�׽���쳣��Ϣ������쳣...." << std::endl;

		//���ȴ�ʩ
		fid = env->GetFieldID(clazz, "key", "Ljava/lang/String;");
	}
	//��ȡ���Ե�ֵ
	jstring jstr = (jstring)env->GetObjectField(obj, fid);
	const char *str = env->GetStringUTFChars(jstr, NULL);

	//�Ա����Ե�ֵ�Ƿ����
	if (strcmp(str, "hanlonglin") != 0) {
		//��Ϊ�׳��쳣����java�㴦��
		jclass excep_class = env->FindClass("java/lang/IllegalArgumentException");
		env->ThrowNew(excep_class, "key's value is invalid!");
		std::cout << "keys value:" << str << std::endl;
	}
}

//�������
JNIEXPORT void JNICALL Java_com_jni_test_HelloWorld_cached
(JNIEnv * env, jobject obj) {
	std::cout << "invoke cached()" << std::endl;
	jclass clazz = env->GetObjectClass(obj);
	
	//��ȡjfieldID֮��ȡһ��
	//�ֲ���̬����
	static jfieldID key_id = NULL;
	if (key_id == NULL) {
		key_id = env->GetFieldID(clazz, "key", "Ljava/lang/String;");
		std::cout << "cached() getFieldID()" << key_id << std::endl;
	}
}

//��ʼ��ȫ�ֱ���
//��̬��������֮�����̻�������
jfieldID key_id;
jmethodID random_id;

JNIEXPORT void JNICALL Java_com_jni_test_HelloWorld_initIDs
(JNIEnv * env, jclass clazz) {
	std::cout << "c++�г�ʼ��ȫ�ֱ���" << std::endl;
	
	//jclass clazz = env->GetObjectClass(obj);
	key_id = env->GetFieldID(clazz, "key", "Ljava/lang/String;");
	random_id = env->GetMethodID(clazz, "generateRandom", "()I");
}

#include "HBAes.h"

const char AES_IV_WITH_LOG[] = { 0x7D,0xE,0xD3,0xB9,0x57,0x88,0xDE,0x92,0xE3,0x47,0x9C,0x83,0xFB,0x28,0xBE,0xD6 };
const char AES_KEY_WITH_LOG[] = { 0xD0,0xD0,0x31,0x7C,0xB8,0x38,0xF4,0x4F,0x2B,0xA7,0x7,0xE3,0xDF,0x6E,0xCB,0x7D };

int main() {
	HBAes *PHBEAS = new HBAes(AES_KEY_WITH_LOG, AES_IV_WITH_LOG, 16, false, AesMode::CFB);

	std::string str = "hanlonglin";
	std::string after_encrypt = PHBEAS->encrypt(str);
	
	std::cout << "����֮��" << after_encrypt << std::endl;

	std::string after_dencrypt = PHBEAS->decrypt(after_encrypt);

	std::cout << "����֮��:" << after_dencrypt << std::endl;
	system("pause");
}

//����
JNIEXPORT jstring JNICALL Java_com_jni_test_HelloWorld_encrypt
(JNIEnv * env, jobject obj, jstring jstr) {
	HBAes *PHBEAS = new HBAes(AES_KEY_WITH_LOG, AES_IV_WITH_LOG, 16, false, AesMode::CFB);

	const char* text = env->GetStringUTFChars(jstr, NULL);
	std::cout << "���ܴ����ַ�����" << text << std::endl;
	std::string re = PHBEAS->encrypt(text);
	std::cout << "c++���ܺ��ַ�����" << re << std::endl;
	return env->NewString((jchar*)re.c_str(), strlen(re.c_str()));
}

//����
JNIEXPORT jstring JNICALL Java_com_jni_test_HelloWorld_decrypt
(JNIEnv * env, jobject obj, jstring jstr) {
	HBAes *PHBEAS = new HBAes(AES_KEY_WITH_LOG, AES_IV_WITH_LOG, 16, false, AesMode::CFB);

	const char* text = env->GetStringUTFChars(jstr, NULL);
	std::cout << "���ܴ����ַ�����" << text << std::endl;
	std::string re = PHBEAS->decrypt(text);
	std::cout << "c++���ܺ��ַ�����" << re << std::endl;
	return env->NewString((jchar*)re.c_str(), strlen(re.c_str()));
}