#pragma once
#include "..\cryptopp563\aes.h"
#include "..\cryptopp563\modes.h"

enum class AesMode
{
	CBC,
	CFB
};

/**
* 封装AES的加密解密操作
*/
class HBAes
{
public:
	HBAes(const char* key, const char* iv, const short keyLength, bool bPadding,const AesMode& aesMode);
	HBAes(const std::vector<char>& key, const std::vector<char>& iv, bool bPadding, const AesMode& aesMode);
	/**
	* 加密
	*/
	static std::string Encrypt(const short keyLength, const char *key, const char* iv, const std::string& plain, const AesMode& aesMode, bool padding);
	std::string encrypt(const std::string& plain) const;

	/**
	* 解密
	*/
	static std::string Decrypt(const short keyLength, const char *key, const char* iv, const std::string& cipher, const AesMode& aesMode, bool padding);
	std::string decrypt(const std::string& cipher);

private:
	const short m_keyLength;		//密钥长度
	bool m_bPadding;				//是否对明文进行填充
	const std::vector<char> m_key;	//密钥
	const std::vector<char> m_iv;	//iv向量
	const AesMode m_aesMode;		//aes加密模式
};

using HBAesPtr = std::shared_ptr<HBAes>;
