#define _CRT_SECURE_NO_WARNINGS
#include "com_jni_test_HelloWorld.h"

#include <Windows.h>
#include <iostream>
#include <stdlib.h>

JNIEXPORT jstring JNICALL Java_com_jni_test_HelloWorld_getStringFromC
(JNIEnv * env, jobject obj, jstring jstr) {
	//return env->NewStringUTF(env,"Hello ,I am hanlonglin from CCCCC+++"); //c中写法   env是一个二级指针

	//WinExec("E://software_install//qq9.0.4//Bin//QQScLauncher.exe", 0);  //调用qq.exe
	return env->NewStringUTF("Hello ,I am hanlonglin from CCCCC+++");   //c++中写法  env是一个一级指针
}

JNIEXPORT jint JNICALL Java_com_jni_test_HelloWorld_calculate
(JNIEnv * env, jobject obj, jint a, jint b) {
	//return env->NewStringUTF(env,"Hello ,I am hanlonglin from CCCCC+++"); //c中写法   env是一个二级指针
	std::cout << "c++ 中正在进行计算..." << std::endl;
	return a + b;   //c++中写法  env是一个一级指针
}

//访问java属性 
//访问成员属性
JNIEXPORT jstring JNICALL Java_com_jni_test_HelloWorld_accessField
(JNIEnv * env, jobject obj) {
	//获取jclass
	jclass clazz = env->GetObjectClass(obj);

	//属性名称，属性签名
	jfieldID  fid = env->GetFieldID(clazz, "key", "Ljava/lang/String;");

	//获取属性key的值
	jstring jstr = (jstring)env->GetObjectField(obj, fid);

	//jstring >> C的字符串
	//is_Copy
	const char * c_str = env->GetStringUTFChars(jstr, NULL);

	//拼接得到新的字符串
	char text[30] = "good man is ";
	strcat_s(text, c_str);

	//C的字符串 >> jstring
	jstring new_jstr = env->NewStringUTF(text);

	//修改key
	env->SetObjectField(obj, fid, new_jstr);

	return new_jstr;
}

//访问静态属性
JNIEXPORT jint JNICALL Java_com_jni_test_HelloWorld_accessStaticField
(JNIEnv * env, jobject obj) {

	//获取jclass
	jclass clazz = env->GetObjectClass(obj);

	//获取field
	jfieldID fid = env->GetStaticFieldID(clazz, "count", "I");

	//获取key的值
	jint count = (jint)env->GetStaticIntField(clazz, fid);

	count++;

	//修改count
	env->SetStaticIntField(clazz, fid, count);

	return count;

}

//访问java方法
//访问成员方法
JNIEXPORT jint JNICALL Java_com_jni_test_HelloWorld_accessMethod
(JNIEnv * env, jobject obj) {

	//获取class
	jclass clazz = env->GetObjectClass(obj);

	//获取methodID
	jmethodID mid = env->GetMethodID(clazz, "generateRandom", "()I");

	//调用method
	jint re_int = env->CallIntMethod(obj, mid);

	return re_int;
}

//访问静态方法
JNIEXPORT jstring JNICALL Java_com_jni_test_HelloWorld_accessStaticMethod
(JNIEnv * env, jobject obj) {

	//获取类
	jclass clazz = env->GetObjectClass(obj);

	//获取methodID
	jmethodID mid = env->GetStaticMethodID(clazz, "getUUID", "()Ljava/lang/String;");

	//调用method
	jstring jstr = (jstring)env->CallStaticObjectMethod(clazz, mid);

	//随机文件名称
	const char * uuid = env->GetStringUTFChars(jstr, NULL);

	//拼接
	char filename[100];
	sprintf_s(filename, "D://%s.txt", uuid);

	FILE *fp = fopen(filename, "w");
	fputs("I like it ", fp);
	fclose(fp);

	return jstr;
}

//访问构造方法 
//使用java.util.Data产生一个当前的时间戳
JNIEXPORT jobject JNICALL Java_com_jni_test_HelloWorld_accessConstructor
(JNIEnv * env, jobject obj) {

	//获取class
	jclass clazz = env->FindClass("java/util/Date");

	//methodID
	jmethodID constuctor_mid = env->GetMethodID(clazz, "<init>", "()V");   //构造方法 className需要使用 <init> ()V是无参构造方法的签名

	//实例化对象
	jobject date_obj = env->NewObject(clazz, constuctor_mid);

	//调用getTime方法
	jmethodID getTime_mid = env->GetMethodID(clazz, "getTime", "()J");

	jlong time = env->CallLongMethod(date_obj, getTime_mid);

	printf("time:%ld\n", time);
	//std::cout << "c++ 中获取时间戳：" << time << std::endl;

	return date_obj;
}

//调用父类的方法
JNIEXPORT void JNICALL Java_com_jni_test_HelloWorld_accessNonvirtualMethod
(JNIEnv * env, jobject obj) {

	//获取类
	jclass clazz = env->GetObjectClass(obj);

	//获取human属性
	jfieldID fid = env->GetFieldID(clazz, "humen", "Lcom/jni/test/Humen;");

	//获取obj
	jobject humen_obj = env->GetObjectField(obj, fid);

	//执行sayHi方法
	jclass humen_class = env->FindClass("com/jni/test/Humen");

	//获取methodID
	jmethodID sayhi_mid = env->GetMethodID(humen_class, "sayHi", "()V");

	//调用sayHi()
	env->CallNonvirtualVoidMethod(humen_obj, humen_class, sayhi_mid);
}

//传入传出中文
JNIEXPORT jobject JNICALL Java_com_jni_test_HelloWorld_chineseChars
(JNIEnv * env, jobject obj, jstring jstr) {

	const char* j_str = env->GetStringUTFChars(jstr, NULL);
	std::cout << "c++中接收来自java的字符串：" << j_str << std::endl;

	char *c_str = "马荣与松江";
	//const char c_str[] = "马荣与松江";

	//jstring jstr = env->NewStringUTF("我是来自c++的中文字符串"); //这样有乱码
	//通过调用 java中String 的 String(byte[] bytes,String charsetName) 来指定编码
	//获取String 类
	jclass str_class = env->FindClass("java/lang/String");

	//获取String构造函数
	jmethodID str_mid = env->GetMethodID(str_class, "<init>", "([BLjava/lang/String;)V");

	//jbyte -> char
	//jbyteArray -> char[]
	jbyteArray bytes = env->NewByteArray(strlen(c_str));

	//byte数组赋值
	//0 -> strlen 从头到尾部
	env->SetByteArrayRegion(bytes, 0, strlen(c_str), (jbyte*)c_str);

	//字符编码
	jstring charsetName = env->NewStringUTF("GB2312");

	return env->NewObject(str_class, str_mid, bytes, charsetName);

}

int compare(const void *a, const void *b) {
	int *pa = (int*)a;
	int *pb = (int*)b;
	return (*pa) - (*pb);
}

//传入数组 并进行排序
JNIEXPORT void JNICALL Java_com_jni_test_HelloWorld_giveArray
(JNIEnv * env, jobject obj, jintArray intArray) {

	//获取数组指针
	//jintArray >> jint指针 >> c int 数组
	jint *elems = env->GetIntArrayElements(intArray, NULL);

	//数组长度
	int len = env->GetArrayLength(intArray);

	//排序
	qsort(elems, len, sizeof(jint), compare);

	//同步
	//0             java数组进行更新 并且释放C/C++数组
	//JNI_ABORT     java数组不进行更新  但是释放C/C++数组
	//JNI_COMMIT    java数组进行更新  不是放C/C++数组（函数执行完，数组还是会被释放）
	env->ReleaseIntArrayElements(intArray, elems, JNI_COMMIT);
}

//传出数组
JNIEXPORT jintArray JNICALL Java_com_jni_test_HelloWorld_getArray
(JNIEnv * env, jobject obj,jint len) {
	
	//创建一个jint数组
	jintArray jint_arr = env->NewIntArray(len);

	jint *elems = env->GetIntArrayElements(jint_arr, NULL);

	//赋值
	int i = 0;
	for (; i < len; i++) {
		elems[i] = i;
	}

	//同步
	env->ReleaseIntArrayElements(jint_arr, elems, JNI_COMMIT);

	return jint_arr; 
}

//JNI引用变量
//引用类型：局部引用和全局引用
//作用：在JNI中告知虚拟机何时回收一个JNI变量

//局部引用，通过调用DeleteLocalRef函数手动释放对象
//1.访问一个很大的java对象，使用完之后，还要进行复杂的耗时操作
//2.占用了大量的局部引用，占用了太多的内存，而这些局部引用跟后面的操作没有关联性
JNIEXPORT void JNICALL Java_com_jni_test_HelloWorld_localRef
(JNIEnv * env, jobject obj) {
	int i = 0;
	for (; i < 20; i++) {
		//创建Date对象
		jclass clazz = env->FindClass("java/util/Date");
		jmethodID constructor_mid = env->GetMethodID(clazz, "<init>", "()V");
		jobject date_obj = env->NewObject(clazz, constructor_mid);

		//此处省略一百行代码...

		//不在使用Date对象 
		//通知垃圾回收器回收这些对象
		env->DeleteLocalRef(date_obj);

		//此处省略一百行代码...

	}
}


//全局引用
//1.共享（跨多个线程），手动控制内存使用
jstring global_str;

//创建
JNIEXPORT void JNICALL Java_com_jni_test_HelloWorld_createGlobalRef
(JNIEnv * env, jobject obj) {
	jstring jstr = env->NewStringUTF("hanlonglin is a good man");
	global_str = (jstring)env->NewGlobalRef(jstr);
}

//获得
JNIEXPORT jstring JNICALL Java_com_jni_test_HelloWorld_getGlobalRef
(JNIEnv * env, jobject obj) {
	return global_str;
}

//释放
JNIEXPORT void JNICALL Java_com_jni_test_HelloWorld_deleteGlobalRef
(JNIEnv * env, jobject obj) {
	env->DeleteGlobalRef(global_str);
}

//异常处理
//1.保证java代码可以运行
//2.补救措施保证C代码继续运行

//JNI自己抛出的异常，在java层无法被捕捉，只能在C层清空
//用户通过ThrowNew抛出的异常，可以在java层捕捉

JNIEXPORT void JNICALL Java_com_jni_test_HelloWorld_exception
(JNIEnv * env, jobject obj) {
	jclass clazz = env->GetObjectClass(obj);
	jfieldID fid = env->GetFieldID(clazz, "key2", "Ljava/lang/String;");  //没有key2属性

	jthrowable exception = env->ExceptionOccurred();
	if (exception != NULL) {
		//让java代码可一个继续运行
		//清空异常信息
		env->ExceptionClear();

		std::cout << "c++中捕捉到异常信息，清空异常...." << std::endl;

		//补救措施
		fid = env->GetFieldID(clazz, "key", "Ljava/lang/String;");
	}
	//获取属性的值
	jstring jstr = (jstring)env->GetObjectField(obj, fid);
	const char *str = env->GetStringUTFChars(jstr, NULL);

	//对比属性的值是否相等
	if (strcmp(str, "hanlonglin") != 0) {
		//人为抛出异常，给java层处理
		jclass excep_class = env->FindClass("java/lang/IllegalArgumentException");
		env->ThrowNew(excep_class, "key's value is invalid!");
		std::cout << "keys value:" << str << std::endl;
	}
}

//缓存策略
JNIEXPORT void JNICALL Java_com_jni_test_HelloWorld_cached
(JNIEnv * env, jobject obj) {
	std::cout << "invoke cached()" << std::endl;
	jclass clazz = env->GetObjectClass(obj);
	
	//获取jfieldID之获取一次
	//局部静态变量
	static jfieldID key_id = NULL;
	if (key_id == NULL) {
		key_id = env->GetFieldID(clazz, "key", "Ljava/lang/String;");
		std::cout << "cached() getFieldID()" << key_id << std::endl;
	}
}

//初始化全局变量
//动态库加载完成之后，立刻缓存起来
jfieldID key_id;
jmethodID random_id;

JNIEXPORT void JNICALL Java_com_jni_test_HelloWorld_initIDs
(JNIEnv * env, jclass clazz) {
	std::cout << "c++中初始化全局变量" << std::endl;
	
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
	
	std::cout << "加密之后：" << after_encrypt << std::endl;

	std::string after_dencrypt = PHBEAS->decrypt(after_encrypt);

	std::cout << "解密之后:" << after_dencrypt << std::endl;
	system("pause");
}

//加密
JNIEXPORT jstring JNICALL Java_com_jni_test_HelloWorld_encrypt
(JNIEnv * env, jobject obj, jstring jstr) {
	HBAes *PHBEAS = new HBAes(AES_KEY_WITH_LOG, AES_IV_WITH_LOG, 16, false, AesMode::CFB);

	const char* text = env->GetStringUTFChars(jstr, NULL);
	std::cout << "加密传入字符串：" << text << std::endl;
	std::string re = PHBEAS->encrypt(text);
	std::cout << "c++加密后字符串：" << re << std::endl;
	return env->NewString((jchar*)re.c_str(), strlen(re.c_str()));
}

//解密
JNIEXPORT jstring JNICALL Java_com_jni_test_HelloWorld_decrypt
(JNIEnv * env, jobject obj, jstring jstr) {
	HBAes *PHBEAS = new HBAes(AES_KEY_WITH_LOG, AES_IV_WITH_LOG, 16, false, AesMode::CFB);

	const char* text = env->GetStringUTFChars(jstr, NULL);
	std::cout << "解密传入字符串：" << text << std::endl;
	std::string re = PHBEAS->decrypt(text);
	std::cout << "c++解密后字符串：" << re << std::endl;
	return env->NewString((jchar*)re.c_str(), strlen(re.c_str()));
}