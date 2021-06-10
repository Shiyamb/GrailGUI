#pragma once

#include <memory>
class BlockLoader {
 public:
  std::unique_ptr<uint64_t> mem;
  struct GeneralHeader {
    uint32_t magic;    // magic number for all block loaders
    uint32_t type;     // type of block loader
    uint32_t version;  // version
    uint32_t unused;
  };
  struct SecurityHeaderV0 {};
#if 0
	struct SecurityHeaderV1 {
		uint8_t hash[32];  // SHA256 hash signed by author
		uint8_t hash2[32]; // SHA256 hash of alternate region in data signed by author
		uint8_t id[32]; // id of author
	};
#endif

  GeneralHeader* generalHeader;
  SecurityHeaderV0* securityHeader;
  enum class Type {
    gismap,
    hashmap
  };
  // get the size of the SecurityHeader for any BlockLoader
  uint32_t getAuthHeaderSize() const { return sizeof(SecurityHeaderV0); }
  uint32_t getHeaderSize() const { return sizeof(GeneralHeader) + sizeof(SecurityHeaderV0); }
  bool authenticate() const;
  GeneralHeader* generalHeader;
	BlockLoader(uint64_t bytes, Type t, uint32_t version);
};

class BlockMapLoader : public BlockLoader, public ESRIShape {
 private:
  SpecificHeader* specificHeader;
  const float* points;
  static constexpr uint32_t version = 0x00010000;

 public:
  struct SpecificHeader {
    uint32_t numLists;
    uint32_t unused;
  };

  struct Segment {
    uint32_t numPoints;
    uint32_t type : 8;
    uint32_t unused : 24;
    double baseLocX, baseLocY;
  };
  BlockMapLoader(uint32_t numLists, uint32_t numPoints);
  BlastMapLoader(const char filename[]);
