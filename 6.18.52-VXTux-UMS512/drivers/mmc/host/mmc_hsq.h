/* SPDX-License-Identifier: GPL-2.0 */
/* V18: stub for VXTux 6.18 - mmc_hsq not in upstream */
#ifndef _MMC_HSQ_H
#define _MMC_HSQ_H

struct mmc_hsq {
    int dummy;
};

static inline int mmc_hsq_init(struct mmc_hsq *hsq, struct mmc_host *mmc)
{
    (void)hsq; (void)mmc;
    return 0;
}

static inline void mmc_hcq_suspend(struct mmc_host *mmc) { (void)mmc; }
static inline int mmc_hsq_finalize_request(struct mmc_host *mmc, struct mmc_request *mrq)
{
    (void)mmc; (void)mrq;
    return 0;
}
static inline void mmc_hsq_suspend(struct mmc_host *mmc) { (void)mmc; }
static inline void mmc_hsq_resume(struct mmc_host *mmc) { (void)mmc; }

#endif
