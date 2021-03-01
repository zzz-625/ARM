#ifndef __DOWNLOAD_FLAG_H__
#define __DOWNLOAD_FLAG_H__

#define DATA_FLASH_DL_FLAG_BASE        0xFA00
#define DATA_FLASH_DL_FLAG_END         0xFBFF

extern void dl_flag_init(void);
extern void set_boot_flags(void);
extern int get_dl_flag(void);
extern void set_dl_flag(void);

#endif
