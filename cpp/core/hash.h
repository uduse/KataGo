/*
 * hash.h
 * Author: David Wu
 */

#ifndef CORE_HASH_H_
#define CORE_HASH_H_

#include <iostream>

#include "../core/global.h"

namespace Hash
{
  //Splitting of uint64s
  uint32_t highBits(uint64_t x);
  uint32_t lowBits(uint64_t x);
  uint64_t combine(uint32_t hi, uint32_t lo);

  uint64_t basicLCong(uint64_t x);
  uint64_t basicLCong2(uint64_t x);
  uint64_t murmurMix(uint64_t x);
  uint64_t splitMix64(uint64_t x);

  void jenkinsMix(uint32_t& a, uint32_t& b, uint32_t& c);
  uint32_t jenkinsMixSingle(uint32_t a, uint32_t b, uint32_t c);

  uint64_t simpleHash(const char* str);
  uint64_t simpleHash(const int* input, int len);
}

//Hash is "little endian" in the sense that if you printed out hash1, then hash0, you would
//get the standard 128 bit hash string as would be output by something like MD5, as well as
//for comparison.
struct Hash128
{
  uint64_t hash0;
  uint64_t hash1;

  Hash128();
  Hash128(uint64_t hash0, uint64_t hash1);

  bool operator<(const Hash128 other) const;
  bool operator>(const Hash128 other) const;
  bool operator<=(const Hash128 other) const;
  bool operator>=(const Hash128 other) const;
  bool operator==(const Hash128 other) const;
  bool operator!=(const Hash128 other) const;

  Hash128 operator^(const Hash128 other) const;
  Hash128 operator|(const Hash128 other) const;
  Hash128 operator&(const Hash128 other) const;
  Hash128& operator^=(const Hash128 other);
  Hash128& operator|=(const Hash128 other);
  Hash128& operator&=(const Hash128 other);

  friend std::ostream& operator<<(std::ostream& out, const Hash128 other);

  std::string toString() const;
  static Hash128 fromString(const std::string &string);
};

inline Hash128::Hash128()
:hash0(0),hash1(0)
{}

inline Hash128::Hash128(uint64_t h0, uint64_t h1)
:hash0(h0),hash1(h1)
{}



inline bool Hash128::operator==(const Hash128 other) const
{return hash0 == other.hash0 && hash1 == other.hash1;}

inline bool Hash128::operator!=(const Hash128 other) const
{return hash0 != other.hash0 || hash1 != other.hash1;}

inline bool Hash128::operator>(const Hash128 other) const
{
  if(hash1 > other.hash1) return true;
  if(hash1 < other.hash1) return false;
  return hash0 > other.hash0;
}
inline bool Hash128::operator>=(const Hash128 other) const
{
  if(hash1 > other.hash1) return true;
  if(hash1 < other.hash1) return false;
  return hash0 >= other.hash0;
}
inline bool Hash128::operator<(const Hash128 other) const
{
  if(hash1 < other.hash1) return true;
  if(hash1 > other.hash1) return false;
  return hash0 < other.hash0;
}
inline bool Hash128::operator<=(const Hash128 other) const
{
  if(hash1 < other.hash1) return true;
  if(hash1 > other.hash1) return false;
  return hash0 <= other.hash0;
}

inline Hash128 Hash128::operator^(const Hash128 other) const {
  return Hash128(hash0 ^ other.hash0, hash1 ^ other.hash1);
}
inline Hash128 Hash128::operator|(const Hash128 other) const {
  return Hash128(hash0 | other.hash0, hash1 | other.hash1);
}
inline Hash128 Hash128::operator&(const Hash128 other) const {
  return Hash128(hash0 & other.hash0, hash1 & other.hash1);
}
inline Hash128& Hash128::operator^=(const Hash128 other) {
  hash0 ^= other.hash0;
  hash1 ^= other.hash1;
  return *this;
}
inline Hash128& Hash128::operator|=(const Hash128 other) {
  hash0 |= other.hash0;
  hash1 |= other.hash1;
  return *this;
}
inline Hash128& Hash128::operator&=(const Hash128 other) {
  hash0 &= other.hash0;
  hash1 &= other.hash1;
  return *this;
}

inline std::string Hash128::toString() const {
  return std::to_string(hash0) + ',' + std::to_string(hash1);
}

inline Hash128 Hash128::fromString(const std::string &string) {
  auto pos = string.find(',');
  auto first = string.substr(0, pos);
  auto second = string.substr(pos + 1);
  return Hash128(std::stoull(first), std::stoull(second));
}

inline std::size_t hash_value(const Hash128 &hash) {
  return hash.hash0 ^ hash.hash1;
}

#endif  // CORE_HASH_H_
