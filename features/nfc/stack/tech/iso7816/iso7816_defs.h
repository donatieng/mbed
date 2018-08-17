/**
 * \file iso7816_defs.h
 * \copyright Copyright (c) ARM Ltd 2013
 * \author Donatien Garnier
 */

#ifndef ISO7816_DEFS_H_
#define ISO7816_DEFS_H_

#define ISO7816_INS_SELECT          0xA4
#define ISO7816_INS_READ_BINARY     0xB0
#define ISO7816_INS_UPDATE_BINARY   0xD6
#define ISO7816_INS_ENVELOPE        0xC2


#define ISO7816_SW_OK               0x9000
#define ISO7816_SW_INVALID_CLASS    0x6E00
#define ISO7816_SW_INVALID_INS      0x6D00
#define ISO7816_SW_NOT_FOUND        0x6A82
#define ISO7816_SW_WRONG_LENGTH     0x6700

#define ISO7816_PUT_SW(buf, sw) do{ *(buf)=(sw>>8) & 0xFF; *(buf+1)=(sw>>0) & 0xFF; } while(0);



#endif /* ISO7816_DEFS_H_ */
