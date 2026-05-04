#include <LMS7002M/LMS7002M.h>

void LMS7002M_CORE_LDO_ENABLE (LMS7002M_t *lms, const bool enable);
void LMS7002M_RXEN (LMS7002M_t *lms, const bool enable);
void LMS7002M_TXEN (LMS7002M_t *lms, const bool enable);
void LMS7002M_TXNRX1 (LMS7002M_t *lms, const bool enable);
void LMS7002M_TXNRX2 (LMS7002M_t *lms, const bool enable);
void LMS7002M_RESET (LMS7002M_t *lms);