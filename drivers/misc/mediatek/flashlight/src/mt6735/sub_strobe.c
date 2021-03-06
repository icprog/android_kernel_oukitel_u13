
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/wait.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/poll.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/time.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include "kd_camera_typedef.h"
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/version.h>
#ifdef CONFIG_COMPAT
#include <linux/fs.h>
#include <linux/compat.h>
#endif
#include "kd_flashlight.h"
#include <linux/hct_board_config.h>
/******************************************************************************
 * Debug configuration
******************************************************************************/
/* availible parameter */
/* ANDROID_LOG_ASSERT */
/* ANDROID_LOG_ERROR */
/* ANDROID_LOG_WARNING */
/* ANDROID_LOG_INFO */
/* ANDROID_LOG_DEBUG */
/* ANDROID_LOG_VERBOSE */
#define TAG_NAME "[sub_strobe.c]"
#define PK_DBG_NONE(fmt, arg...)    do {} while (0)
#define PK_DBG_FUNC(fmt, arg...)    pr_debug(TAG_NAME "%s: " fmt, __func__ , ##arg)
#define PK_WARN(fmt, arg...)        pr_warn(TAG_NAME "%s: " fmt, __func__ , ##arg)
#define PK_NOTICE(fmt, arg...)      pr_notice(TAG_NAME "%s: " fmt, __func__ , ##arg)
#define PK_INFO(fmt, arg...)        pr_info(TAG_NAME "%s: " fmt, __func__ , ##arg)
#define PK_TRC_FUNC(f)              pr_debug(TAG_NAME "<%s>\n", __func__)
#define PK_TRC_VERBOSE(fmt, arg...) pr_debug(TAG_NAME fmt, ##arg)
#define PK_ERROR(fmt, arg...)       pr_err(TAG_NAME "%s: " fmt, __func__ , ##arg)

#define DEBUG_LEDS_STROBE
#ifdef DEBUG_LEDS_STROBE
#define PK_DBG PK_DBG_FUNC
#define PK_VER PK_TRC_VERBOSE
#define PK_ERR PK_ERROR
#else
#define PK_DBG(a, ...)
#define PK_VER(a, ...)
#define PK_ERR(a, ...)
#endif

/******************************************************************************
 * local variables
******************************************************************************/

#if __HCT_SUB_FLASHLIGHT_SUPPORT__ || __HCT_SUB_FLASHLIGHT_PWM_SUPPORT__

static DEFINE_SPINLOCK(g_strobeSMPLock);	/* cotta-- SMP proection */


static u32 strobe_Res = 0;
static u32 strobe_Timeus = 0;
static BOOL g_strobe_On = 0;

static int g_duty = -1;
static int g_timeOutTimeMs=0;


static struct work_struct workTimeOut;
extern void sub_FL_as_pwm_Enable(int duty);
extern void sub_FL_as_pwm_Disable(void);

/*****************************************************************************
Functions
*****************************************************************************/
static void sub_led_work_timeOutFunc(struct work_struct *data);

int sub_FL_Enable(void)
{
#if __HCT_SUB_FLASHLIGHT_AW3641_SUPPORT__
	int i;
	if(g_duty)
		flashlight_gpio_output(FL_SUB_MODE_PIN, 1);
	else
		flashlight_gpio_output(FL_SUB_MODE_PIN, 0);
	for(i=0; i<9; i++)
	{
		flashlight_gpio_output(FL_SUB_EN_PIN, 0);
		udelay(2);
		flashlight_gpio_output(FL_SUB_EN_PIN, 1);
		udelay(2);
	}
#elif __HCT_SUB_FLASHLIGHT_AW3141_SUPPORT__
      flashlight_gpio_output(FL_SUB_EN_PIN, 1);
      udelay(2);

      if(g_duty)
          flashlight_gpio_output(FL_SUB_MODE_PIN, 1);
      else
          flashlight_gpio_output(FL_SUB_MODE_PIN, 0);
#elif __HCT_SUB_FLASHLIGHT_PWM_SUPPORT__
	sub_FL_as_pwm_Enable(g_duty);
#elif __HCT_SUB_FLASHLIGHT_OCP8132_SUPPORT__
		if(g_duty)
			flashlight_gpio_output(FL_SUB_MODE_PIN, 1);
		else
			flashlight_gpio_output(FL_SUB_EN_PIN, 1);

#else
      flashlight_gpio_output(FL_SUB_EN_PIN, 1);
#endif

    	PK_DBG(" FL_Enable line=%d g_duty=%d\n",__LINE__, g_duty);
	return 0;
}



int sub_FL_Disable(void)
{
#if __HCT_SUB_FLASHLIGHT_AW3641_SUPPORT__||__HCT_SUB_FLASHLIGHT_OCP8132_SUPPORT__
	flashlight_gpio_output(FL_SUB_EN_PIN, 0);
	flashlight_gpio_output(FL_SUB_MODE_PIN, 0);
	mdelay(1);
#elif __HCT_SUB_FLASHLIGHT_PWM_SUPPORT__
	sub_FL_as_pwm_Disable();
#else
     flashlight_gpio_output(FL_SUB_EN_PIN, 0);
#endif

	PK_DBG(" FL_Disable line=%d\n",__LINE__);
	return 0;
}

int sub_FL_dim_duty(kal_uint32 duty)
{
	PK_DBG(" FL_dim_duty line=%d\n", __LINE__);
	g_duty = duty;
	return 0;
}




int sub_FL_Init(void)
{
	flashlight_gpio_output(FL_SUB_EN_PIN, 0);
	PK_DBG(" FL_Init line=%d\n",__LINE__);
	return 0;
}


int sub_FL_Uninit(void)
{
	sub_FL_Disable();
	return 0;
}

/*****************************************************************************
User interface
*****************************************************************************/

static void sub_led_work_timeOutFunc(struct work_struct *data)
{
	sub_FL_Disable();
	PK_DBG("ledTimeOut_callback\n");
}



enum hrtimer_restart subledTimeOutCallback(struct hrtimer *timer)
{
	schedule_work(&workTimeOut);
	return HRTIMER_NORESTART;
}

static struct hrtimer g_timeOutTimer;
static void sub_timerInit(void)
{
	static int init_flag;
	if (init_flag==0)
	{
		init_flag=1;
	 	INIT_WORK(&workTimeOut, sub_led_work_timeOutFunc);
		g_timeOutTimeMs=1000; //1s
		hrtimer_init( &g_timeOutTimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
		g_timeOutTimer.function=subledTimeOutCallback;
	}
}



static int sub_strobe_ioctl(unsigned int cmd, unsigned long arg)
{
	int i4RetValue = 0;
	int ior_shift;
	int iow_shift;
	int iowr_shift;

	ior_shift = cmd - (_IOR(FLASHLIGHT_MAGIC, 0, int));
	iow_shift = cmd - (_IOW(FLASHLIGHT_MAGIC, 0, int));
	iowr_shift = cmd - (_IOWR(FLASHLIGHT_MAGIC, 0, int));
	PK_DBG("constant_flashlight_ioctl() line=%d ior_shift=%d, iow_shift=%d iowr_shift=%d arg=%d\n",__LINE__, ior_shift, iow_shift, iowr_shift, (int)arg);
	switch (cmd) {

	case FLASH_IOC_SET_TIME_OUT_TIME_MS:
		PK_DBG("FLASH_IOC_SET_TIME_OUT_TIME_MS: %d\n", (int)arg);
		g_timeOutTimeMs = arg;
		break;


	case FLASH_IOC_SET_DUTY:
		PK_DBG("FLASHLIGHT_DUTY: %d\n", (int)arg);
		sub_FL_dim_duty(arg);
		break;


	case FLASH_IOC_SET_STEP:
		PK_DBG("FLASH_IOC_SET_STEP: %d\n", (int)arg);

		break;

	case FLASH_IOC_SET_ONOFF:
		PK_DBG("FLASHLIGHT_ONOFF: %d\n", (int)arg);
		if (arg == 1) {

			int s;
			int ms;

			if (g_timeOutTimeMs > 1000) {
				s = g_timeOutTimeMs / 1000;
				ms = g_timeOutTimeMs - s * 1000;
			} else {
				s = 0;
				ms = g_timeOutTimeMs;
			}

			if (g_timeOutTimeMs != 0) {
				ktime_t ktime;

				ktime = ktime_set(s, ms * 1000000);
				hrtimer_start(&g_timeOutTimer, ktime, HRTIMER_MODE_REL);
			}
			sub_FL_Enable();
		} else {
			sub_FL_Disable();
			hrtimer_cancel(&g_timeOutTimer);
		}
		break;
	default:
		PK_DBG(" No such command\n");
		i4RetValue = -EPERM;
		break;
	}
	return i4RetValue;
}

static int sub_strobe_open(void *pArg)
{
	int i4RetValue = 0;

	PK_DBG("sub_strobe_open line=%d\n", __LINE__);

	if (0 == strobe_Res) {
		sub_FL_Init();
		sub_timerInit();
	}
	PK_DBG("sub_strobe_open line=%d\n", __LINE__);
	spin_lock_irq(&g_strobeSMPLock);


	if (strobe_Res) {
		PK_DBG(" busy!\n");
		i4RetValue = -EBUSY;
	} else {
		strobe_Res += 1;
	}


	spin_unlock_irq(&g_strobeSMPLock);
	PK_DBG("sub_strobe_open line=%d\n", __LINE__);

	return i4RetValue;

}



static int sub_strobe_release(void *pArg)
{
	PK_DBG(" sub_strobe_release\n");

	if (strobe_Res) {
		spin_lock_irq(&g_strobeSMPLock);

		strobe_Res = 0;
		strobe_Timeus = 0;

		/* LED On Status */
		g_strobe_On = FALSE;

		spin_unlock_irq(&g_strobeSMPLock);

		sub_FL_Uninit();
	}

	PK_DBG(" Done\n");

	return 0;

}



FLASHLIGHT_FUNCTION_STRUCT subStrobeFunc = {
	sub_strobe_open,
	sub_strobe_release,
	sub_strobe_ioctl
};

#else
static int dummy_sub_strobe_ioctl(unsigned int cmd, unsigned long arg)
{
	PK_DBG("sub dummy ioctl");
	return 0;
}

static int dummy_sub_strobe_open(void *pArg)
{
	PK_DBG("sub dummy open");
	return 0;

}

static int dummy_sub_strobe_release(void *pArg)
{
	PK_DBG("sub dummy release");
	return 0;

}

FLASHLIGHT_FUNCTION_STRUCT subStrobeFunc = {
	dummy_sub_strobe_open,
	dummy_sub_strobe_release,
	dummy_sub_strobe_ioctl
};
#endif

MUINT32 subStrobeInit(PFLASHLIGHT_FUNCTION_STRUCT *pfFunc)
{
	if (pfFunc != NULL)
		*pfFunc = &subStrobeFunc;
	return 0;
}
