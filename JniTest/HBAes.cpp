//#include "stdafx.h"
#include "..\cryptopp563\base64.h"
#include "..\cryptopp563\filters.h"
#include "HBAes.h"
#include <assert.h>
#include <iostream>

using namespace CryptoPP;

//Ӧ��֤iv��key�ĳ�����һ�µ�
HBAes::HBAes(const char* key, const char* iv, const short keyLength, bool bPadding, const AesMode& aesMode)
	: m_iv(iv,iv+keyLength)
	, m_key(key,key+keyLength)
	, m_keyLength(keyLength)
	, m_bPadding(bPadding)
	, m_aesMode(aesMode)
{
}

//iv��key�ĳ�����vector�ĳ���ȷ��
HBAes::HBAes(const std::vector<char>& key, const std::vector<char>& iv, bool bPadding, const AesMode & aesMode)
	:m_key(key)
	,m_iv(iv)
	,m_bPadding(bPadding)
	,m_aesMode(aesMode)
	,m_keyLength(static_cast<short>(key.size()))
{
}

//************************************
// ��������: Encrypt
// ����˵��: ������plain����aes����
// ����:     const short keyLength
// ����:     const char * key
// ����:     const char * iv
// ����:     const std::string & plain
// ����:     const AesMode & aesMode
// ����:     bool padding
// ����ֵ:   std::string
// ��������: 2016/05/07
//************************************
std::string HBAes::Encrypt(const short keyLength, const char* key, const char* iv, const std::string& plain, const AesMode& aesMode, bool padding)
{
	if (plain.empty())
		return plain;

	std::string outstr;
	AES::Encryption aesEncryption((byte*)key, keyLength);

	CryptoPP::BlockPaddingSchemeDef::BlockPaddingScheme paddMode = CryptoPP::BlockPaddingSchemeDef::BlockPaddingScheme::NO_PADDING;
	if (padding)
		paddMode = CryptoPP::BlockPaddingSchemeDef::BlockPaddingScheme::PKCS_PADDING;

	try
	{
		switch (aesMode)
		{
		case AesMode::CBC:
		{
			CBC_Mode_ExternalCipher::Encryption encryption(aesEncryption, (byte*)iv);
			StreamTransformationFilter encryptor(encryption, new Base64Encoder(new StringSink(outstr),false), paddMode);
			encryptor.Put((byte *)plain.data(), plain.size());
			encryptor.MessageEnd();
			break;
		}

		case AesMode::CFB:
		{
			CFB_Mode_ExternalCipher::Encryption encryption(aesEncryption, (byte*)iv);
			StreamTransformationFilter encryptor(encryption, new Base64Encoder(new StringSink(outstr),false), paddMode);
			encryptor.Put((byte *)plain.data(), plain.size());
			encryptor.MessageEnd();
			break;
		}
		}
	}
	catch (std::exception& e)
	{
		//BOOST_LOG_TRIVIAL(error) << e.what();
		return nullptr;
	}

	return outstr;
}

std::string HBAes::encrypt(const std::string& plain) const
{
	return HBAes::Encrypt(m_keyLength, m_key.data(), m_iv.data(), plain, m_aesMode, m_bPadding);
}

//************************************
// ��������: Decrypt
// ����˵��: ������cipher����aes����
// ����:     const short keyLength
// ����:     const char * key
// ����:     const char * iv
// ����:     const std::string & cipher
// ����:     const AesMode & aesMode
// ����:     bool padding
// ����ֵ:   std::string ���ʧ�ܷ��ؿ��ַ���
// ��������: 2016/05/07
//************************************
std::string HBAes::Decrypt(const short keyLength, const char *key, const char* iv, const std::string& cipher, const AesMode& aesMode, bool padding)
{
	if (cipher.empty())
		return cipher;

	std::string outstr;
	CryptoPP::BlockPaddingSchemeDef::BlockPaddingScheme paddMode = CryptoPP::BlockPaddingSchemeDef::BlockPaddingScheme::NO_PADDING;
	if (padding)
		paddMode = CryptoPP::BlockPaddingSchemeDef::BlockPaddingScheme::PKCS_PADDING;

	try
	{
		switch (aesMode)
		{
		case AesMode::CBC:
		{
			CBC_Mode<AES>::Decryption cfbDecryption((byte*)key, keyLength, (byte*)iv);
			Base64Decoder decryptor(new StreamTransformationFilter(cfbDecryption, new StringSink(outstr), paddMode));
			decryptor.Put((byte *)cipher.c_str(), cipher.size());
			decryptor.MessageEnd();
			break;
		}
		case AesMode::CFB:
		{
			CFB_Mode<AES>::Decryption cfbDecryption((byte*)key, keyLength, (byte*)iv);
			Base64Decoder decryptor(new StreamTransformationFilter(cfbDecryption, new StringSink(outstr), paddMode));
			decryptor.Put((byte *)cipher.c_str(), cipher.size());
			decryptor.MessageEnd();
			break;
		}
		}
	}
	catch (std::exception& e)
	{
		//BOOST_LOG_TRIVIAL(error) << e.what();
		return "";
	}
	return outstr;
}

std::string HBAes::decrypt(const std::string& cipher)
{
	return HBAes::Decrypt(m_keyLength, m_key.data(), m_iv.data(), cipher, m_aesMode, m_bPadding);
}

