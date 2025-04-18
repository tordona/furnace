/**
 * Furnace Tracker - multi-system chiptune tracker
 * Copyright (C) 2021-2025 tildearrow and contributors
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef _GENESIS_H
#define _GENESIS_H

#include "fmshared_OPN.h"
#include "sound/ymfm/ymfm_opn.h"
extern "C" {
#include "../../../extern/YMF276-LLE/fmopn2.h"
}

class DivPlatformGenesis: public DivPlatformOPN {
  protected:
    const unsigned short chanOffs[6]={
      0x00, 0x01, 0x02, 0x100, 0x101, 0x102
    };

    const unsigned char konOffs[6]={
      0, 1, 2, 4, 5, 6
    };

    struct Channel: public FMChannelStereo {
      bool furnaceDac;
      bool dacMode;
      int dacPeriod;
      int dacRate;
      unsigned int dacPos;
      int dacSample;
      int dacDelay;
      bool dacDirection;
      bool setPos;
      unsigned char sampleBank;
      signed char dacOutput;
      Channel():
        FMChannelStereo(),
        furnaceDac(false),
        dacMode(false),
        dacPeriod(0),
        dacRate(0),
        dacPos(0),
        dacSample(-1),
        dacDelay(0),
        dacDirection(false),
        setPos(false),
        sampleBank(0),
        dacOutput(0) {}
    };
    Channel chan[10];
    DivDispatchOscBuffer* oscBuf[10];
    bool isMuted[10];
    ym3438_t fm;
    fmopn2_t fm_276;

    ymfm::ym2612* fm_ymfm;
    ymfm::ym2612::output_data out_ymfm;
    DivOPNInterface iface;

    int softPCMTimer;

    bool extMode, softPCM, noExtMacros, canWriteDAC, msw;
    unsigned char useYMFM;
    unsigned char chipType;
    short dacWrite;

    int lleCycle;
    int llePrevCycle;
    int lleOscData[6];
    int dacShifter, o_lro, o_bco;

    int interruptSim;
    int interruptSimCycles;
  
    unsigned char dacVolTable[128];
  
    friend void putDispatchChip(void*,int);
    friend void putDispatchChan(void*,int,int);

    inline void processDAC(int iRate);
    inline void commitState(int ch, DivInstrument* ins);
    inline void acquire276OscSub(int h);
    void acquire_nuked(short** buf, size_t len);
    void acquire_nuked276(short** buf, size_t len);
    void acquire_ymfm(short** buf, size_t len);
  
    friend void putDispatchChip(void*,int);
    friend void putDispatchChan(void*,int,int);
  public:
    void acquire(short** buf, size_t len);
    void fillStream(std::vector<DivDelayedWrite>& stream, int sRate, size_t len);
    int dispatch(DivCommand c);
    void* getChanState(int chan);
    DivMacroInt* getChanMacroInt(int ch);
    virtual unsigned short getPan(int chan);
    DivSamplePos getSamplePos(int ch);
    DivDispatchOscBuffer* getOscBuffer(int chan);
    virtual int mapVelocity(int ch, float vel);
    unsigned char* getRegisterPool();
    int getRegisterPoolSize();
    void reset();
    void forceIns();
    void tick(bool sysTick=true);
    void muteChannel(int ch, bool mute);
    int getOutputCount();
    void setYMFM(unsigned char use);
    bool keyOffAffectsArp(int ch);
    bool keyOffAffectsPorta(int ch);
    float getPostAmp();
    void toggleRegisterDump(bool enable);
    void setFlags(const DivConfig& flags);
    void notifyInsChange(int ins);
    virtual void notifyInsDeletion(void* ins);
    void setSoftPCM(bool value);
    int getPortaFloor(int ch);
    void poke(unsigned int addr, unsigned short val);
    void poke(std::vector<DivRegWrite>& wlist);
    int init(DivEngine* parent, int channels, int sugRate, const DivConfig& flags);
    void quit();
    DivPlatformGenesis():
      DivPlatformOPN(2, 6, 6, 6, 6, 9440540.0, 72, 32, false, 7) {}
    ~DivPlatformGenesis();
};
#endif
