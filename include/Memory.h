#pragma once
#include <cstdint>
#include <array>
#include <algorithm>

class Memory
{
public:
  Memory();
  void clean();
  uint8_t readByte(uint16_t address) const;
  void writeByte(uint16_t address, uint8_t byte);

  //special IO registers which writing to or reading from does stuff
  enum IOregisters: uint16_t {P1 = 0xff00};

private:
  static constexpr uint16_t memorySize = 0xffff;
  std::array<uint8_t, memorySize> memoryArray{};

  void P1Call(uint16_t);
  struct IOreg
  {
    using callbackT = void(Memory::*)(uint16_t);

    IOreg(uint16_t d, callbackT cal): addr(d), callback(cal){};
    bool operator==(const int nr) {return addr == nr;};
    uint16_t addr;
    callbackT callback;
  };

  std::array<IOreg, 1> IOregs = {IOreg(P1, &Memory::P1Call)};
};

/*
  0000-3FFF   16KB ROM Bank 00     (in cartridge, fixed at bank 00)
  4000-7FFF   16KB ROM Bank 01..NN (in cartridge, switchable bank number)
  8000-9FFF   8KB Video RAM (VRAM) (switchable bank 0-1 in CGB Mode)
  A000-BFFF   8KB External RAM     (in cartridge, switchable bank, if any)
  C000-CFFF   4KB Work RAM Bank 0 (WRAM)
  D000-DFFF   4KB Work RAM Bank 1 (WRAM)  (switchable bank 1-7 in CGB Mode)
  E000-FDFF   Same as C000-DDFF (ECHO)    (typically not used)
  FE00-FE9F   Sprite Attribute Table (OAM)
  FEA0-FEFF   Not Usable
  FF00-FF7F   I/O Ports
  FF80-FFFE   High RAM (HRAM)
  FFFF        Interrupt Enable Register

 */
