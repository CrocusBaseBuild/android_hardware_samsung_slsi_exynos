/*
 *
 * Copyright 2012 Samsung Electronics S.LSI Co. LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*!
 * \file      exynos_gscaler.h
 * \brief     header file for Gscaler HAL
 * \author    ShinWon Lee (shinwon.lee@samsung.com)
 * \date      2012/01/09
 *
 * <b>Revision History: </b>
 * - 2012/01/09 : ShinWon Lee(shinwon.lee@samsung.com) \n
 *   Create
 *
 * - 2012/02/07 : ShinWon Lee(shinwon.lee@samsung.com) \n
 *   Change file name to exynos_gscaler.h
 *
 * - 2012/02/09 : Sangwoo, Parkk(sw5771.park@samsung.com) \n
 *   Use Multiple Gscaler by Multiple Process
 *
 * - 2012/02/20 : Sangwoo, Park(sw5771.park@samsung.com) \n
 *   Add exynos_gsc_set_rotation() API
 *
 * - 2012/02/20 : ShinWon Lee(shinwon.lee@samsung.com) \n
 *   Add size constrain
 *
 */

/*!
 * \defgroup exynos_gscaler
 * \brief API for gscaler
 * \addtogroup Exynos
 */
#include "Exynos_log.h"

#ifndef EXYNOS_GSCALER_H_
#define EXYNOS_GSCALER_H_

#ifdef __cplusplus
extern "C" {
#endif

//#define EXYNOS_GSC_TRACE 1
#ifdef EXYNOS_GSC_TRACE
#define EXYNOS_GSC_LOG_TAG "Exynos_gscaler"
#define Exynos_gsc_In() Exynos_Log(EXYNOS_DEV_LOG_DEBUG, EXYNOS_GSC_LOG_TAG, "%s In , Line: %d", __FUNCTION__, __LINE__)
#define Exynos_gsc_Out() Exynos_Log(EXYNOS_DEV_LOG_DEBUG, EXYNOS_GSC_LOG_TAG, "%s Out , Line: %d", __FUNCTION__, __LINE__)
#else
#define Exynos_gsc_In() ((void *)0)
#define Exynos_gsc_Out() ((void *)0)
#endif

typedef struct {
    uint32_t x;
    uint32_t y;
    uint32_t w;
    uint32_t h;
    uint32_t fw;
    uint32_t fh;
    uint32_t format;
    unsigned long yaddr;
    unsigned long uaddr;
    unsigned long vaddr;
    uint32_t rot;
    uint32_t cacheable;
    uint32_t drmMode;
    uint32_t narrowRgb;
    int      acquireFenceFd;
    int      releaseFenceFd;
    int      mem_type;
    uint32_t pre_multi;
#ifdef USES_VR_FRONT_BUFFER
    uint32_t use_sync;
#endif
} exynos_mpp_img;

enum SRC_BL_OP {
	/* [0, 0] */
	SRC_BL_OP_CLR = 1,
	/* [Sa, Sc] */
	SRC_BL_OP_SRC,
	/* [Da, Dc] */
	SRC_BL_OP_DST,
	/* [Sa + (1 - Sa)*Da, Rc = Sc + (1 - Sa)*Dc] */
	SRC_BL_OP_SRC_OVER,
	/* [Sa + (1 - Sa)*Da, Rc = Dc + (1 - Da)*Sc] */
	SRC_BL_OP_DST_OVER,
	/* [Sa * Da, Sc * Da] */
	SRC_BL_OP_SRC_IN,
	/* [Sa * Da, Sa * Dc] */
	SRC_BL_OP_DST_IN,
	/* [Sa * (1 - Da), Sc * (1 - Da)] */
	SRC_BL_OP_SRC_OUT,
	/* [Da * (1 - Sa), Dc * (1 - Sa)] */
	SRC_BL_OP_DST_OUT,
	/* [Da, Sc * Da + (1 - Sa) * Dc] */
	SRC_BL_OP_SRC_ATOP,
	/* [Sa, Sc * (1 - Da) + Sa * Dc ] */
	SRC_BL_OP_DST_ATOP,
	/* [-(Sa * Da), Sc * (1 - Da) + (1 - Sa) * Dc] */
	SRC_BL_OP_XOR,
	/* [Sa + Da - Sa*Da, Sc*(1 - Da) + Dc*(1 - Sa) + min(Sc, Dc)] */
	SRC_BL_OP_DARKEN,
	/* [Sa + Da - Sa*Da, Sc*(1 - Da) + Dc*(1 - Sa) + max(Sc, Dc)] */
	SRC_BL_OP_LIGHTEN,
	/** [Sa * Da, Sc * Dc] */
	SRC_BL_OP_MULTIPLY,
	/* [Sa + Da - Sa * Da, Sc + Dc - Sc * Dc] */
	SRC_BL_OP_SCREEN,
	/* Saturate(S + D) */
	SRC_BL_OP_ADD
};

enum colorspace {
    COLORSPACE_SMPTE170M,
    COLORSPACE_SMPTE240M,
    COLORSPACE_REC709,
    COLORSPACE_BT878,
    COLORSPACE_470_SYSTEM_M,
    COLORSPACE_470_SYSTEM_BG,
    COLORSPACE_JPEG,
    COLORSPACE_SRGB,
};

struct SrcGlobalAlpha {
	uint32_t enable;
	unsigned int val;
};

struct CSC_Spec{
	uint32_t enable;    // set 'true' for user-defined
	enum colorspace space;
	uint32_t wide;
};

struct SrcBlendInfo {
	enum SRC_BL_OP blop;
	unsigned int srcblendfmt;
	unsigned int srcblendhpos;
	unsigned int srcblendvpos;
	unsigned int srcblendpremulti;
	unsigned int srcblendstride;
	unsigned int srcblendwidth;
	unsigned int srcblendheight;
	struct SrcGlobalAlpha globalalpha;
	struct CSC_Spec cscspec;
};

/*
 * Create libgscaler handle.
 * Gscaler dev_num is dynamically changed.
 *
 * \ingroup exynos_gscaler
 *
 * \return
 *   libgscaler handle
 */
void *exynos_gsc_create(
    void);

/*!
 * Create exclusive libgscaler handle.
 * Other module can't use dev_num of Gscaler.
 *
 * \ingroup exynos_gscaler
 *
 * \param dev_num
 *   gscaler dev_num[in]
 * \param gsc_mode
 *It should be set to GSC_M2M_MODE or GSC_OUTPUT_MODE.
 *
 *\param out_mode
 *It should be set to GSC_OUT_FIMD or GSC_OUT_TV.
 *
 * \return
 *   libgscaler handle
 */
void *exynos_gsc_create_exclusive(
    int dev_num,
    int gsc_mode,
    int out_mode,
    int allow_drm);

/*!
 * Destroy libgscaler handle
 *
 * \ingroup exynos_gscaler
 *
 * \param handle
 *   libgscaler handle[in]
 */
void exynos_gsc_destroy(
    void *handle);

/*!
 * Set csc equation property
 *
 * \ingroup exynos_gscaler
 *
 * \param handle
 *   libgscaler handle[in]
 *
 * \param eq_auto
 *   csc mode (0: user, 1: auto)[in]
 *
 * \param range_full
 *   csc range (0: narrow, 1: full)[in]
 *
 * \param v4l2_colorspace
 *   ITU_R v4l2 colorspace(1: 601, 3: 709)[in]
 */
int exynos_gsc_set_csc_property(
    void        *handle,
    unsigned int eq_auto,
    unsigned int range_full,
    unsigned int v4l2_colorspace);

/*!
 * Set source format.
 *
 * \ingroup exynos_gscaler
 *
 * \param handle
 *   libgscaler handle[in]
 *
 * \param width
 *   image width[in]
 *
 * \param height
 *   image height[in]
 *
 * \param crop_left
 *   image left crop size[in]
 *
 * \param crop_top
 *   image top crop size[in]
 *
 * \param crop_width
 *   cropped image width[in]
 *
 * \param crop_height
 *   cropped image height[in]
 *
 * \param v4l2_colorformat
 *   color format[in]
 *
 * \param cacheable
 *   ccacheable[in]
 *
 * \param mode_drm
 *   mode_drm[in]
 *
 * \return
 *   error code
 */
int exynos_gsc_set_src_format(
    void        *handle,
    unsigned int width,
    unsigned int height,
    unsigned int crop_left,
    unsigned int crop_top,
    unsigned int crop_width,
    unsigned int crop_height,
    unsigned int v4l2_colorformat,
    unsigned int cacheable,
    unsigned int mode_drm);

/*!
 * Set destination format.
 *
 * \ingroup exynos_gscaler
 *
 * \param handle
 *   libgscaler handle[in]
 *
 * \param width
 *   image width[in]
 *
 * \param height
 *   image height[in]
 *
 * \param crop_left
 *   image left crop size[in]
 *
 * \param crop_top
 *   image top crop size[in]
 *
 * \param crop_width
 *   cropped image width[in]
 *
 * \param crop_height
 *   cropped image height[in]
 *
 * \param v4l2_colorformat
 *   color format[in]
 *
 * \param cacheable
 *   ccacheable[in]
 *
 * \param mode_drm
 *   mode_drm[in]
 *
 * \return
 *   error code
 */
int exynos_gsc_set_dst_format(
    void        *handle,
    unsigned int width,
    unsigned int height,
    unsigned int crop_left,
    unsigned int crop_top,
    unsigned int crop_width,
    unsigned int crop_height,
    unsigned int v4l2_colorformat,
    unsigned int cacheable,
    unsigned int mode_drm);

/*!
 * Set rotation.
 *
 * \ingroup exynos_gscaler
 *
 * \param handle
 *   libgscaler handle[in]
 *
 * \param rotation
 *   image rotation. It should be multiple of 90[in]
 *
 * \param flip_horizontal
 *   image flip_horizontal[in]
 *
 * \param flip_vertical
 *   image flip_vertical[in]
 *
 * \return
 *   error code
 */
int exynos_gsc_set_rotation(
    void *handle,
    int   rotation,
    int   flip_horizontal,
    int   flip_vertical);

/*!
 * Set source buffer
 *
 * \ingroup exynos_gscaler
 *
 * \param handle
 *   libgscaler handle[in]
 *
 * \param addr
 *   buffer pointer array[in]
 *
 * \param acquireFenceFd
 *   acquire fence fd for the buffer or -1[in]
 *
 * \return
 *   error code
 */
int exynos_gsc_set_src_addr(
    void *handle,
    void *addr[3],
    int mem_type,
    int acquireFenceFd);

/*!
 * Set destination buffer
 *
 * \param handle
 *   libgscaler handle[in]
 *
 * \param addr
 *   buffer pointer array[in]
 *
 * \param acquireFenceFd
 *   acquire fence fd for the buffer or -1[in]
 *
 * \return
 *   error code
 */
int exynos_gsc_set_dst_addr(
    void *handle,
    void *addr[3],
    int mem_type,
    int acquireFenceFd);

/*!
 * Convert color space with presetup color format
 *
 * \ingroup exynos_gscaler
 *
 * \param handle
 *   libgscaler handle[in]
 *
 * \return
 *   error code
 */
int exynos_gsc_convert(
    void *handle);

/*
 * API for setting GSC subdev crop
 * Used in OTF mode
 */
int exynos_gsc_subdev_s_crop(
        void *handle,
        exynos_mpp_img *src_img,
        exynos_mpp_img *dst_img);

/*
*api for setting the GSC config.
It configures the GSC for given config
*/
int exynos_gsc_config_exclusive(
    void *handle,
    exynos_mpp_img *src_img,
    exynos_mpp_img *dst_img);

/*
*api for GSC-OUT run.
It queues the srcBuf to GSC and deques a buf from driver.
It should be called after configuring the GSC.
*/
int exynos_gsc_run_exclusive(
    void *handle,
    exynos_mpp_img *src_img,
    exynos_mpp_img *dst_img);

/*!
 * Create exclusive libgscaler blend handle.
 * Other module can't use dev_num of Gscaler.
 *
 * \ingroup exynos_gscaler
 *
 * \param dev_num
 *   gscaler dev_num[in]
 * \param gsc_mode
 * \return
 *   libgscaler handle
 */
void *exynos_gsc_create_blend_exclusive(
    int dev_num,
    int gsc_mode,
    int out_mode,
    int allow_drm);

/*
*api for setting the GSC blend config.
It configures the GSC for given config
*/
int exynos_gsc_config_blend_exclusive(
    void *handle,
    exynos_mpp_img *src_img,
    exynos_mpp_img *dst_img,
    struct SrcBlendInfo  *srcblendinfo);

/*
 * Blocks until the current frame is done processing.
 */
int exynos_gsc_wait_frame_done_exclusive
(void *handle);

/*
*api for GSC stop.
It stops the GSC OUT streaming.
*/
int exynos_gsc_stop_exclusive
(void *handle);

/*
*api for GSC free_and_close.
*/
int exynos_gsc_free_and_close
(void *handle);

enum {
    GSC_M2M_MODE = 0,
    GSC_OUTPUT_MODE,
    GSC_CAPTURE_MODE,
    GSC_RESERVED_MODE,
};

/*flag info */
enum {
    GSC_DUMMY = 0,
    GSC_OUT_FIMD,
    GSC_OUT_TV,
    GSC_RESERVED,
};

enum {
    GSC_DONE_CNG_CFG = 0,
    GSC_NEED_CNG_CFG,
};

enum {
    GSC_MEM_MMAP = 1,
    GSC_MEM_USERPTR,
    GSC_MEM_OVERLAY,
    GSC_MEM_DMABUF,
};

#ifdef __cplusplus
}
#endif

#endif /*EXYNOS_GSCALER_H_*/
