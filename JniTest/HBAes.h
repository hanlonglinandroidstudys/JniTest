#pragma once
#include "..\cryptopp563\aes.h"
#include "..\cryptopp563\modes.h"

enum class AesMode
{
	CBC,
	CFB
};

/**
* ��װAES�ļ��ܽ��ܲ���
*/
class HBAes
{
public:
	HBAes(const char* key, const char* iv, const short keyLength, bool bPadding,const AesMode& aesMode);
	HBAes(const std::vector<char>& key, const std::vector<char>& iv, bool bPadding, const AesMode& aesMode);
	/**
	* ����
	*/
	static std::string Encrypt(const short keyLength, const char *key, const char* iv, const std::string& plain, const AesMode& aesMode, bool padding);
	std::string encrypt(const std::string& plain) const;

	/**
	* ����
	*/
	static std::string Decrypt(const short keyLength, const char *key, const char* iv, const std::string& cipher, const AesMode& aesMode, bool padding);
	std::string decrypt(const std::string& cipher);

private:
	const short m_keyLength;		//��Կ����
	bool m_bPadding;				//�Ƿ�����Ľ������
	const std::vector<char> m_key;	//��Կ
	const std::vector<char> m_iv;	//iv����
	const AesMode m_aesMode;		//aes����ģʽ
};

using HBAesPtr = std::shared_ptr<HBAes>;
