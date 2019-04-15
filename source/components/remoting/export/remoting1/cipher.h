/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2014, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifndef REMOTING1_CIPHER_H
#define REMOTING1_CIPHER_H

#include <cstdint>
#include <cstdlib>

namespace Remoting1 {

/*! Cipher base class. An implementation shall provide encryption and
	decryption.
 */
class Cipher
{
public:
	Cipher();
	virtual ~Cipher();

	/*! Shall return a buffer size that is large enough to encrypt data to with
		the specified size.
		\param size		The size of the unencrypted data.
	 */
	virtual size_t decryptBufferSize(size_t size) const = 0;

	/*! Shall return a buffer size that is large enough to encrypt data to with
		the specified size.
		\param size		The size of the unencrypted data.
	 */
	virtual size_t encryptBufferSize(size_t size) const = 0;

	/*! Shall decrypt data from a buffer and appends to the destination buffer.
		\param src		Input buffer.
		\param srcLen	Length of the input buffer.
		\param dst		Destination buffer.
		\param dstLen	In: Length of the destination buffer. 
						Out: The number of bytes written to the destination buffer.
		\returns		The number of bytes read from the input buffer.
		*/
	virtual size_t decrypt(const char *src, size_t srcLen, char *dst, size_t &dstLen) const = 0;

	/*! Shall encrypt data from a buffer and appends to the destination buffer.
		\param src		Input buffer.
		\param srcLen	Length of the input buffer.
		\param dst		Destination buffer
		\param dstLen	In: Length of the destination buffer. 
						Out: The number of bytes written to the destination buffer.
		\returns		The number of bytes read from the input buffer.
		*/
	virtual size_t encrypt(const char *src, size_t srcLen, char *dst, size_t &dstLen) const = 0;

private:
	Cipher(const Cipher &c);
	Cipher & operator=(const Cipher &);
};

/*! Null cipher, does not encrypt at all.
 */
class CipherNull final : public Cipher
{
public:
	CipherNull();
	virtual ~CipherNull();

	virtual size_t decryptBufferSize(size_t size) const override final;
	virtual size_t encryptBufferSize(size_t size) const override final;
    virtual size_t decrypt(const char *src, size_t srcLen, char *dst, size_t &dstLen) const override final;
    virtual size_t encrypt(const char *src, size_t srcLen, char *dst, size_t &dstLen) const override final;
};

/*! Blowfish cipher, provides Blowfish encryption and decryption. Blowfish is a
	symmetric-key block cipher, designed in 1993 by Bruce Schneier and included
	in a large number of cipher suites and encryption products. Blowfish
	provides a good encryption rate in software and no effective cryptanalysis
	of it has been found to date.
 */
class CipherBlowfish final : public Cipher
{
public:
	template <size_t _size> 
	CipherBlowfish(const uint32_t (&key)[_size]) { importKey(key, _size); }
	CipherBlowfish(const uint32_t *key, size_t len);
	virtual ~CipherBlowfish();

	virtual size_t decryptBufferSize(size_t size) const override final;
	virtual size_t encryptBufferSize(size_t size) const override final;
    virtual size_t decrypt(const char *src, size_t srcLen, char *dst, size_t &dstLen) const override final;
    virtual size_t encrypt(const char *src, size_t srcLen, char *dst, size_t &dstLen) const override final;

private:
	void importKey(const uint32_t *key, size_t len);
	uint32_t f(uint32_t x) const;
	static void swap(uint32_t &, uint32_t &);

	union Block { char c[8]; uint32_t l[2]; };
	void decrypt(Block &) const;
	void encrypt(Block &) const;

private:
	static const uint32_t pdefault[18];
	static const uint32_t sdefault[4][256];

	uint32_t p[18];
	uint32_t s[4][256];
};

} // End of namespaces

#endif
