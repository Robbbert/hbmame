// license:BSD-3-Clause
// copyright-holders:S. Smith,David Haywood


#pragma once

#include "banked_cart.h"

#ifndef __NEOGEO_PROT__
#define __NEOGEO_PROT__

DECLARE_DEVICE_TYPE(NGBOOTLEG_PROT, ngbootleg_prot_device)


class ngbootleg_prot_device :  public device_t
{
public:
	// construction/destruction
	ngbootleg_prot_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock = 0);

	void neogeo_bootleg_cx_decrypt(u8*sprrom, u32 sprrom_size);
	void neogeo_bootleg_sx_decrypt(u8* fixed, u32 fixed_size, int value);
	void kof97oro_px_decode(u8* cpurom, u32 cpurom_size);
	void kof10thBankswitch(u16 nBank);
	u16 kof10th_RAM2_r(offs_t offset);
	u16 kof10th_RAMB_r(offs_t offset);
	void kof10th_custom_w(offs_t offset, u16 data, u16 mem_mask = ~0);
	void kof10th_bankswitch_w(offs_t offset, u16 data, u16 mem_mask = ~0);
	void install_kof10th_protection(cpu_device* maincpu, neogeo_banked_cart_device* bankdev, u8* cpurom, u32 cpurom_size, u8* fixedrom, u32 fixedrom_size);
	void decrypt_kof10th(u8* cpurom, u32 cpurom_size);
	void kf10thep_px_decrypt(u8* cpurom, u32 cpurom_size);
	void kf2k5uni_px_decrypt(u8* cpurom, u32 cpurom_size);
	void kf2k5uni_sx_decrypt(u8* fixedrom, u32 fixedrom_size);
	void kf2k5uni_mx_decrypt(u8* audiorom, u32 audiorom_size);
	void decrypt_kf2k5uni(u8* cpurom, u32 cpurom_size, u8* audiorom, u32 audiorom_size, u8* fixedrom, u32 fixedrom_size);
	void kof2002b_gfx_decrypt(u8 *src, int size);
	void kf2k2mp_decrypt(u8* cpurom, u32 cpurom_size);
	void kf2k2mp2_px_decrypt(u8* cpurom, u32 cpurom_size);
	void cthd2003_neogeo_gfx_address_fix_do(u8* sprrom, u32 sprrom_size, int start, int end, int bit3shift, int bit2shift, int bit1shift, int bit0shift);
	void cthd2003_neogeo_gfx_address_fix(u8* sprrom, u32 sprrom_size, int start, int end);
	void cthd2003_c(u8* sprrom, u32 sprrom_size, int pow);
	void decrypt_cthd2003(u8* sprrom, u32 sprrom_size, u8* audiorom, u32 audiorom_size, u8* fixedrom, u32 fixedrom_size);
	void cthd2003_bankswitch_w(offs_t offset, u16 data);
	void patch_cthd2003(cpu_device* maincpu, neogeo_banked_cart_device* bankdev, u8* cpurom, u32 cpurom_size);
	void ct2k3sp_sx_decrypt(u8* fixedrom, u32 fixedrom_size);
	void decrypt_ct2k3sp(u8* sprrom, u32 sprrom_size, u8* audiorom, u32 audiorom_size, u8* fixedrom, u32 fixedrom_size);
	void decrypt_ct2k3sa(u8* sprrom, u32 sprrom_size, u8* audiorom, u32 audiorom_size);
	void patch_ct2k3sa(u8* cpurom, u32 cpurom_size);
	void decrypt_kof2k4se_68k(u8* cpurom, u32 cpurom_size);
	void lans2004_vx_decrypt(u8* ymsndrom, u32 ymsndrom_size);
	void lans2004_decrypt_68k(u8* cpurom, u32 cpurom_size);
	u16 mslug5_prot_r();
	void ms5plus_bankswitch_w(offs_t offset, u16 data);
	void install_ms5plus_protection(cpu_device* maincpu, neogeo_banked_cart_device* bankdev);
	void svcboot_px_decrypt(u8* cpurom, u32 cpurom_size);
	void svcboot_cx_decrypt(u8*sprrom, u32 sprrom_size);
	void svcplus_px_decrypt(u8* cpurom, u32 cpurom_size);
	void svcplus_px_hack(u8* cpurom, u32 cpurom_size);
	void svcplusa_px_decrypt(u8* cpurom, u32 cpurom_size);
	void svcsplus_px_decrypt(u8* cpurom, u32 cpurom_size);
	void svcsplus_px_hack(u8* cpurom, u32 cpurom_size);
	u16 kof2003_r(offs_t offset);
	void kof2003_w(offs_t offset, u16 data, u16 mem_mask = ~0);
	void kof2003p_w(offs_t offset, u16 data, u16 mem_mask = ~0);
	u16 kof2003_overlay_r();
	void kf2k3bl_px_decrypt(u8* cpurom, u32 cpurom_size);
	void kf2k3bl_install_protection(cpu_device* maincpu, neogeo_banked_cart_device* bankdev, u8* cpurom, u32 cpurom_size);
	void kf2k3pl_px_decrypt(u8* cpurom, u32 cpurom_size);
	void kf2k3pl_install_protection(cpu_device* maincpu, neogeo_banked_cart_device* bankdev, u8* cpurom, u32 cpurom_size);
	u16 kof2k3_overlay;

	void kf2k3upl_px_decrypt(u8* cpurom, u32 cpurom_size);
	void samsho5b_px_decrypt(u8* cpurom, u32 cpurom_size);
	void samsho5b_vx_decrypt(u8* ymsndrom, u32 ymsndrom_size);
	void matrimbl_decrypt(u8* sprrom, u32 sprrom_size, u8* audiorom, u32 audiorom_size);

	u16 m_cartridge_ram[0x1000]; // bootlegs

	// for kof10th
	u8* m_mainrom;
	u8* m_fixedrom;
	neogeo_banked_cart_device* m_bankdev;
	u16 m_cartridge_ram2[0x10000];

protected:
	virtual void device_start() override;
	virtual void device_reset() override;
};


DECLARE_DEVICE_TYPE(KOG_PROT, kog_prot_device)


class kog_prot_device :  public device_t
{
public:
	// construction/destruction
	kog_prot_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock = 0);

	void kog_install_protection(cpu_device* maincpu);
	void kog_px_decrypt(u8* cpurom, u32 cpurom_size);
	u16 read_jumper();

	required_ioport m_jumper;

protected:
	virtual void device_start() override;
	virtual void device_reset() override;
	virtual ioport_constructor device_input_ports() const override;
};


DECLARE_DEVICE_TYPE(CMC_PROT, cmc_prot_device)

// cmc42
#define KOF99_GFX_KEY  (0x00)
#define GAROU_GFX_KEY (0x06)
#define MSLUG3_GFX_KEY (0xad)
#define ZUPAPA_GFX_KEY (0xbd)
#define GANRYU_GFX_KEY (0x07)
#define S1945P_GFX_KEY (0x05)
#define PREISLE2_GFX_KEY (0x9f)
#define BANGBEAD_GFX_KEY (0xf8)
#define NITD_GFX_KEY (0xff)
#define SENGOKU3_GFX_KEY (0xfe)

// cmc50
#define KOF2000_GFX_KEY (0x00)
#define KOF2001_GFX_KEY (0x1e)
#define MSLUG4_GFX_KEY (0x31)
#define ROTD_GFX_KEY (0x3f)
#define PNYAA_GFX_KEY (0x2e)
#define KOF2002_GFX_KEY (0xec)
#define MATRIM_GFX_KEY (0x6a)
#define SAMSHO5_GFX_KEY (0x0f)
#define SAMSHO5SP_GFX_KEY (0x0d)
#define MSLUG5_GFX_KEY (0x19)
#define SVC_GFX_KEY (0x57)
#define KOF2003_GFX_KEY (0x9d)
#define JOCKEYGP_GFX_KEY (0xac)


class cmc_prot_device :  public device_t
{
public:
	// construction/destruction
	cmc_prot_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock = 0);

	void decrypt(u8 *r0, u8 *r1,
		u8 c0, u8 c1,
		const u8 *table0hi,
		const u8 *table0lo,
		const u8 *table1,
		int base,
		int invert);

	void neogeo_gfx_decrypt(u8* rom, u32 rom_size, int extra_xor);
	void neogeo_sfix_decrypt(u8* rom, u32 rom_size, u8* fixed, u32 fixed_size);
	void cmc42_neogeo_gfx_decrypt(u8* rom, u32 rom_size, int extra_xor);
	void cmc50_neogeo_gfx_decrypt(u8* rom, u32 rom_size, int extra_xor);

	u16 generate_cs16(u8 *rom, int size);
	int m1_address_scramble(int address, u16 key);
	void neogeo_cmc50_m1_decrypt(u8* romcrypt, u32 romcrypt_size, u8* romaudio, u32 romaudio_size);

protected:
	virtual void device_start() override;
	virtual void device_reset() override;

	const u8 *type0_t03;
	const u8 *type0_t12;
	const u8 *type1_t03;
	const u8 *type1_t12;
	const u8 *address_8_15_xor1;
	const u8 *address_8_15_xor2;
	const u8 *address_16_23_xor1;
	const u8 *address_16_23_xor2;
	const u8 *address_0_7_xor;
};


DECLARE_DEVICE_TYPE(FATFURY2_PROT, fatfury2_prot_device)


class fatfury2_prot_device :  public device_t
{
public:
	// construction/destruction
	fatfury2_prot_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock = 0);

	u16 fatfury2_protection_16_r(offs_t offset);
	void fatfury2_protection_16_w(offs_t offset, u16 data);
	void fatfury2_install_protection(cpu_device* maincpu, neogeo_banked_cart_device* bankdev);


	neogeo_banked_cart_device* m_bankdev;
	u32     m_fatfury2_prot_data;

protected:
	virtual void device_start() override;
	virtual void device_reset() override;
};


DECLARE_DEVICE_TYPE(KOF2002_PROT, kof2002_prot_device)


class kof2002_prot_device :  public device_t
{
public:
	// construction/destruction
	kof2002_prot_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock = 0);

	void kof2002_decrypt_68k(u8* cpurom, u32 cpurom_size);
	void matrim_decrypt_68k(u8* cpurom, u32 cpurom_size);
	void samsho5_decrypt_68k(u8* cpurom, u32 cpurom_size);
	void samsh5sp_decrypt_68k(u8* cpurom, u32 cpurom_size);

protected:
	virtual void device_start() override;
	virtual void device_reset() override;
};


DECLARE_DEVICE_TYPE(KOF98_PROT, kof98_prot_device)


class kof98_prot_device :  public device_t
{
public:
	// construction/destruction
	kof98_prot_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock = 0);

	void kof98_decrypt_68k(u8* cpurom, u32 cpurom_size);
	void install_kof98_protection(cpu_device* maincpu);
	void kof98_prot_w(u16 data);
	u16 kof98_prot_r(offs_t offset);
	int kof98_prot_state;
	u16 m_default_rom[2];

protected:
	virtual void device_start() override;
	virtual void device_reset() override;
};


DECLARE_DEVICE_TYPE(MSLUGX_PROT, mslugx_prot_device)


class mslugx_prot_device :  public device_t
{
public:
	// construction/destruction
	mslugx_prot_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock = 0);


	void mslugx_protection_16_w(offs_t offset, u16 data);
	u16 mslugx_protection_16_r(address_space &space, offs_t offset);
	void mslugx_install_protection(cpu_device* maincpu);

	u16     m_mslugx_counter;
	u16     m_mslugx_command;

protected:
	virtual void device_start() override;
	virtual void device_reset() override;
};


DECLARE_DEVICE_TYPE(PCM2_PROT, pcm2_prot_device)


class pcm2_prot_device :  public device_t
{
public:
	// construction/destruction
	pcm2_prot_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock = 0);

	void neo_pcm2_snk_1999(u8* ymrom, u32 ymsize, int value);
	void neo_pcm2_swap(u8* ymrom, u32 ymsize, int value);

protected:
	virtual void device_start() override;
	virtual void device_reset() override;
};


DECLARE_DEVICE_TYPE(PVC_PROT, pvc_prot_device)


class pvc_prot_device :  public device_t
{
public:
	// construction/destruction
	pvc_prot_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock = 0);

	void pvc_write_unpack_color();
	void pvc_write_pack_color();
	void pvc_write_bankswitch();
	u16 pvc_prot_r(offs_t offset);
	void pvc_prot_w(offs_t offset, u16 data, u16 mem_mask = ~0);
	void install_pvc_protection(cpu_device* maincpu, neogeo_banked_cart_device* bankdev);
	neogeo_banked_cart_device* m_bankdev;

	u16 m_cartridge_ram[0x1000];

	void mslug5_decrypt_68k(u8* rom, u32 size);
	void svc_px_decrypt(u8* rom, u32 size);
	void kf2k3pcb_decrypt_68k(u8* rom, u32 size);
	void kof2003_decrypt_68k(u8* rom, u32 size);
	void kof2003h_decrypt_68k(u8* rom, u32 size);

protected:
	virtual void device_start() override;
	virtual void device_reset() override;
};


DECLARE_DEVICE_TYPE(SBP_PROT, sbp_prot_device)


class sbp_prot_device :  public device_t
{
public:
	// construction/destruction
	sbp_prot_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock = 0);


	u8* m_mainrom;

	void sbp_install_protection(cpu_device* maincpu, u8* cpurom, u32 cpurom_size);
	void sbp_lowerrom_w(offs_t offset, u16 data);
	u16 sbp_lowerrom_r(offs_t offset);

protected:
	virtual void device_start() override;
	virtual void device_reset() override;
};

DECLARE_DEVICE_TYPE(SMA_PROT, sma_prot_device)


class sma_prot_device :  public device_t
{
public:
	// construction/destruction
	sma_prot_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock = 0);


	void kof99_bankswitch_w(u16 data);
	void garou_bankswitch_w(u16 data);
	void garouh_bankswitch_w(u16 data);
	void mslug3_bankswitch_w(u16 data);
	void kof2000_bankswitch_w(u16 data);
	u16 prot_9a37_r();
	u16 sma_random_r();
	void reset_sma_rng();
	void sma_install_random_read_handler(cpu_device* maincpu, int addr1, int addr2 );
	void kof99_install_protection(cpu_device* maincpu, neogeo_banked_cart_device* bankdev);
	void garou_install_protection(cpu_device* maincpu, neogeo_banked_cart_device* bankdev);
	void garouh_install_protection(cpu_device* maincpu, neogeo_banked_cart_device* bankdev);
	void mslug3_install_protection(cpu_device* maincpu, neogeo_banked_cart_device* bankdev);
	void kof2000_install_protection(cpu_device* maincpu, neogeo_banked_cart_device* bankdev);
	neogeo_banked_cart_device* m_bankdev;
	void kof99_decrypt_68k(u8* base);
	void garou_decrypt_68k(u8* base);
	void garouh_decrypt_68k(u8* base);
	void mslug3_decrypt_68k(u8* base);
	void kof2000_decrypt_68k(u8* base);

	// PCB decrypts (not sma)
	void svcpcb_gfx_decrypt(u8* rom, u32 rom_size);
	void svcpcb_s1data_decrypt(u8* rom, u32 rom_size);
	void kf2k3pcb_gfx_decrypt(u8* rom, u32 rom_size);
	void kf2k3pcb_decrypt_s1data(u8* rom, u32 rom_size, u8* fixed, u32 fixed_size);
	void kf2k3pcb_sp1_decrypt(u16* rom);

	u16     m_sma_rng;
protected:
	virtual void device_start() override;
	virtual void device_reset() override;
};

#endif
