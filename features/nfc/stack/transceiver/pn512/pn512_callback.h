/**
 * \file pn512_callback.h
 * \copyright Copyright (c) ARM Ltd 2015
 * \author Donatien Garnier
 */

#ifndef PN512_CALLBACK_H_
#define PN512_CALLBACK_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __pn512 pn512_t;
typedef void (*pn512_cb_t)(pn512_t* pPN512, nfc_err_t ret);

#ifdef __cplusplus
}
#endif

#endif /* PN512_CALLBACK_H_ */
