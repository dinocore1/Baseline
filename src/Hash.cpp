/*
 * Copyright (C) 2018 Baseline
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
 */

#include <baseline/Baseline.h>
#include <baseline/Hash.h>
#include <baseline/SharedBuffer.h>
#include <baseline/Singleton.h>
#include <baseline/BaseEncoding.h>

namespace baseline {

static
void uint32_to_buf(uint8_t* buf, uint32_t value)
{
    buf[0] = ( value >> 24 ) & 0xFF;
    buf[1] = ( value >> 16 ) & 0xFF;
    buf[2] = ( value >>  8 ) & 0xFF;
    buf[3] = ( value >>  0 ) & 0xFF;
}

static
void buf_to_uint32(uint32_t& value, uint8_t* buf)
{
    value = buf[0] << 24 |
            buf[1] << 16 |
            buf[2] << 8  |
            buf[4] << 0;
}

HashCode::HashCode( void* buf, size_t len )
{
  mBuffer = SharedBuffer::alloc( len );
  memcpy( mBuffer->data(), buf, len );
}

HashCode::HashCode( const HashCode& c )
  : mBuffer( c.mBuffer )
{
  mBuffer->acquire();
}

HashCode::~HashCode()
{
  mBuffer->release();
  mBuffer = nullptr;
}

String8 HashCode::toHexString() const
{
  return hexEncoding().encode( mBuffer );
}

int HashCode::compare( const HashCode& rhs ) const
{
  int retval = mBuffer->size() - rhs.mBuffer->size();
  if( retval == 0 ) {
    retval = memcmp( mBuffer->data(), rhs.mBuffer->data(), mBuffer->size() );
  }
  return retval;
}

HashFunction::~HashFunction()
{}

////////////////// Crc32 //////////////////

static
const uint32_t crc32_lookup[256] = {
  0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA,
  0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
  0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
  0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
  0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
  0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
  0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
  0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
  0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
  0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
  0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,
  0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
  0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,
  0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
  0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
  0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
  0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,
  0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
  0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
  0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
  0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
  0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
  0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,
  0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
  0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
  0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
  0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
  0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
  0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,
  0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
  0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
  0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
  0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
  0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
  0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
  0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
  0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,
  0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
  0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
  0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
  0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
  0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
  0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,
  0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
  0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
  0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
  0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
  0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
  0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
  0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
  0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
  0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
  0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,
  0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
  0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
  0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
  0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
  0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
  0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,
  0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
  0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,
  0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
  0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
  0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
};


class Crc32 : public HashFunction
{
public:
  Crc32();

  void update( void* buf, size_t len ) override;
  HashCode finalize() override;

  uint32_t mHash;

};

Crc32::Crc32()
  : mHash( 0xFFFFFFFF )
{
}

void Crc32::update( void* buf, size_t len )
{
  uint8_t* data = ( uint8_t* ) buf;
  while( len-- > 0 ) {
    uint8_t lookup = mHash ^ *( data++ );
    mHash = crc32_lookup[lookup] ^ ( mHash >> 8 );
  }
}

HashCode Crc32::finalize()
{
  mHash = mHash ^ 0xFFFFFFFF;
  uint8_t values[4];
  uint32_to_buf(values, mHash);
  return HashCode( values, 4 );
}



up<HashFunction> createCRC32()
{
  return up<HashFunction>( new Crc32() );
}


///////////// SHA1 /////////////////////

/* Help macros */
#define SHA1_ROL(value, bits) (((value) << (bits)) | (((value) & 0xffffffff) >> (32 - (bits))))
#define SHA1_BLK(i) (block[i&15] = SHA1_ROL(block[(i+13)&15] ^ block[(i+8)&15] ^ block[(i+2)&15] ^ block[i&15],1))
 
/* (R0+R1), R2, R3, R4 are the different operations used in SHA1 */
#define SHA1_R0(v,w,x,y,z,i) z += ((w&(x^y))^y)     + block[i]    + 0x5a827999 + SHA1_ROL(v,5); w=SHA1_ROL(w,30);
#define SHA1_R1(v,w,x,y,z,i) z += ((w&(x^y))^y)     + SHA1_BLK(i) + 0x5a827999 + SHA1_ROL(v,5); w=SHA1_ROL(w,30);
#define SHA1_R2(v,w,x,y,z,i) z += (w^x^y)           + SHA1_BLK(i) + 0x6ed9eba1 + SHA1_ROL(v,5); w=SHA1_ROL(w,30);
#define SHA1_R3(v,w,x,y,z,i) z += (((w|x)&y)|(w&x)) + SHA1_BLK(i) + 0x8f1bbcdc + SHA1_ROL(v,5); w=SHA1_ROL(w,30);
#define SHA1_R4(v,w,x,y,z,i) z += (w^x^y)           + SHA1_BLK(i) + 0xca62c1d6 + SHA1_ROL(v,5); w=SHA1_ROL(w,30);

#define SHA1_BLOCKSIZE 64

class SHA1 : public HashFunction
{
public:
  SHA1();

  void update( void* buf, size_t len ) override;
  HashCode finalize() override;

protected:
  void addByteDontCountBits(uint8_t);
  void processBlock();

  uint32_t mDigest[5];
  uint8_t mBlock[SHA1_BLOCKSIZE];
  size_t mBlockPtr;
  uint64_t mNumBits;

};

SHA1::SHA1()
{
    /* SHA1 initialization constants */
    mDigest[0] = 0x67452301;
    mDigest[1] = 0xefcdab89;
    mDigest[2] = 0x98badcfe;
    mDigest[3] = 0x10325476;
    mDigest[4] = 0xc3d2e1f0;
 
    mBlockPtr = 0;
    mNumBits = 0;
}

void SHA1::update(void* buf, size_t len)
{
    int numBytes;
    int i = 0;
    uint8_t* input = reinterpret_cast<uint8_t*>(buf);

    mNumBits += len * 8;

    while(len > 0) {
        if(mBlockPtr < SHA1_BLOCKSIZE) {
            numBytes = MIN(len, SHA1_BLOCKSIZE - mBlockPtr);
            memcpy(&mBlock[mBlockPtr], &input[i], numBytes);
            i += numBytes;
            mBlockPtr += numBytes;
            len -= numBytes;
        } else {
            processBlock();
            mBlockPtr = 0;
        }
    }

    
}

void SHA1::addByteDontCountBits(uint8_t x) {
    mBlock[mBlockPtr++] = x;
    if(mBlockPtr >= SHA1_BLOCKSIZE) {
        processBlock();
        mBlockPtr = 0;
    }
}

HashCode SHA1::finalize()
{
  //hashed text ends with 0x80, some padding 0x00 and the lenth in bits
  addByteDontCountBits(0x80);
  while(mBlockPtr % SHA1_BLOCKSIZE != 56) {
      addByteDontCountBits(0x00);
  }
  for(int j=7;j>=0;j--) {
      addByteDontCountBits(mNumBits >> j * 8);
  }

  uint8_t values[20];
  uint32_to_buf(&values[0 * 4], mDigest[0]);
  uint32_to_buf(&values[1 * 4], mDigest[1]);
  uint32_to_buf(&values[2 * 4], mDigest[2]);
  uint32_to_buf(&values[3 * 4], mDigest[3]);
  uint32_to_buf(&values[4 * 4], mDigest[4]);

  return HashCode( values, 20 );

}

void SHA1::processBlock()
{
    /* Copy digest[] to working vars */
    uint32_t a = mDigest[0];
    uint32_t b = mDigest[1];
    uint32_t c = mDigest[2];
    uint32_t d = mDigest[3];
    uint32_t e = mDigest[4];
 
    uint32_t block[16];
    for(int i=0;i<16;i++){
        buf_to_uint32(block[i], &mBlock[i*4]);
    }
 
    /* 4 rounds of 20 operations each. Loop unrolled. */
    SHA1_R0(a,b,c,d,e, 0);
    SHA1_R0(e,a,b,c,d, 1);
    SHA1_R0(d,e,a,b,c, 2);
    SHA1_R0(c,d,e,a,b, 3);
    SHA1_R0(b,c,d,e,a, 4);
    SHA1_R0(a,b,c,d,e, 5);
    SHA1_R0(e,a,b,c,d, 6);
    SHA1_R0(d,e,a,b,c, 7);
    SHA1_R0(c,d,e,a,b, 8);
    SHA1_R0(b,c,d,e,a, 9);
    SHA1_R0(a,b,c,d,e,10);
    SHA1_R0(e,a,b,c,d,11);
    SHA1_R0(d,e,a,b,c,12);
    SHA1_R0(c,d,e,a,b,13);
    SHA1_R0(b,c,d,e,a,14);
    SHA1_R0(a,b,c,d,e,15);
    SHA1_R1(e,a,b,c,d,16);
    SHA1_R1(d,e,a,b,c,17);
    SHA1_R1(c,d,e,a,b,18);
    SHA1_R1(b,c,d,e,a,19);
    SHA1_R2(a,b,c,d,e,20);
    SHA1_R2(e,a,b,c,d,21);
    SHA1_R2(d,e,a,b,c,22);
    SHA1_R2(c,d,e,a,b,23);
    SHA1_R2(b,c,d,e,a,24);
    SHA1_R2(a,b,c,d,e,25);
    SHA1_R2(e,a,b,c,d,26);
    SHA1_R2(d,e,a,b,c,27);
    SHA1_R2(c,d,e,a,b,28);
    SHA1_R2(b,c,d,e,a,29);
    SHA1_R2(a,b,c,d,e,30);
    SHA1_R2(e,a,b,c,d,31);
    SHA1_R2(d,e,a,b,c,32);
    SHA1_R2(c,d,e,a,b,33);
    SHA1_R2(b,c,d,e,a,34);
    SHA1_R2(a,b,c,d,e,35);
    SHA1_R2(e,a,b,c,d,36);
    SHA1_R2(d,e,a,b,c,37);
    SHA1_R2(c,d,e,a,b,38);
    SHA1_R2(b,c,d,e,a,39);
    SHA1_R3(a,b,c,d,e,40);
    SHA1_R3(e,a,b,c,d,41);
    SHA1_R3(d,e,a,b,c,42);
    SHA1_R3(c,d,e,a,b,43);
    SHA1_R3(b,c,d,e,a,44);
    SHA1_R3(a,b,c,d,e,45);
    SHA1_R3(e,a,b,c,d,46);
    SHA1_R3(d,e,a,b,c,47);
    SHA1_R3(c,d,e,a,b,48);
    SHA1_R3(b,c,d,e,a,49);
    SHA1_R3(a,b,c,d,e,50);
    SHA1_R3(e,a,b,c,d,51);
    SHA1_R3(d,e,a,b,c,52);
    SHA1_R3(c,d,e,a,b,53);
    SHA1_R3(b,c,d,e,a,54);
    SHA1_R3(a,b,c,d,e,55);
    SHA1_R3(e,a,b,c,d,56);
    SHA1_R3(d,e,a,b,c,57);
    SHA1_R3(c,d,e,a,b,58);
    SHA1_R3(b,c,d,e,a,59);
    SHA1_R4(a,b,c,d,e,60);
    SHA1_R4(e,a,b,c,d,61);
    SHA1_R4(d,e,a,b,c,62);
    SHA1_R4(c,d,e,a,b,63);
    SHA1_R4(b,c,d,e,a,64);
    SHA1_R4(a,b,c,d,e,65);
    SHA1_R4(e,a,b,c,d,66);
    SHA1_R4(d,e,a,b,c,67);
    SHA1_R4(c,d,e,a,b,68);
    SHA1_R4(b,c,d,e,a,69);
    SHA1_R4(a,b,c,d,e,70);
    SHA1_R4(e,a,b,c,d,71);
    SHA1_R4(d,e,a,b,c,72);
    SHA1_R4(c,d,e,a,b,73);
    SHA1_R4(b,c,d,e,a,74);
    SHA1_R4(a,b,c,d,e,75);
    SHA1_R4(e,a,b,c,d,76);
    SHA1_R4(d,e,a,b,c,77);
    SHA1_R4(c,d,e,a,b,78);
    SHA1_R4(b,c,d,e,a,79);
 
    /* Add the working vars back into mDigest[] */
    mDigest[0] += a;
    mDigest[1] += b;
    mDigest[2] += c;
    mDigest[3] += d;
    mDigest[4] += e;
}

up<HashFunction> createSHA1()
{
  return up<HashFunction>( new SHA1() );
}

}