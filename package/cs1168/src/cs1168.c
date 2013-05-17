
/*
 *  PORAY X5 and X6 kernel driver for battery voltage sampling from CS1168 ADC.
 * 
 *  Source code provide by Shenzhen Poray Communications CO. LTD
 *  ported to OpenWrt by Luis Soltero <lsoltero@globalmarinenet.com>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */



#include <linux/module.h>
#include <linux/version.h>

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <asm/delay.h>
#include <asm/errno.h>
#include <linux/proc_fs.h>

#include <asm/io.h>  
#include "ralink_gpio.h"

#define CS1168_SCLK		20
#define CS1168_DDAT 	21

#include <linux/delay.h>

static inline void Delay ( void )
{
	int i;
	for ( i=0; i<20000; i++ );
}

static inline write_bit(unsigned long idx, unsigned long arg)
{
		unsigned long tmp, a;
		
		tmp =le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIODATA));
		if (arg & 1L)
			tmp |= (1L << idx);
		else
			tmp &= ~(1L << idx);
		*(volatile u32 *)(RALINK_REG_PIODATA)= cpu_to_le32(tmp);
}

static inline unsigned long read_bit(unsigned long idx)
{
	unsigned long tmp=0, i, d, k, arg;                                                               
		
	tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIODATA));
	tmp = (tmp >> idx) & 1L;

	return tmp;  
}

#define LOW			0
#define HIGH		1

union LongData { 
	unsigned long word; 
	unsigned char byte[4];
};

void CS1168_Clk(void)
{
	write_bit(CS1168_SCLK, LOW);
	Delay(); 
	write_bit(CS1168_SCLK, HIGH);
	Delay();
} 

unsigned char CS1168_ReceiveByte(void) 
{
	unsigned char i; 
	unsigned char rdata = 0; 
	for(i = 0; i < 8; i++) 
	{
		rdata <<= 1; 
		CS1168_Clk(); 
		if( read_bit(CS1168_DDAT) ) 
		{
			rdata += 1;
		}
	}
	return(rdata);
} 


unsigned long CS1168_ReadAD(void) 
{
	unsigned char i; 
	union LongData gdata; 
	
	gdata.word = 0; 
	i = 0;
	while( read_bit(CS1168_DDAT) && i < 20000 ) i++; 
	
	CS1168_Clk(); 
	Delay(); 
	for(i = 1; i < 4; i++) 
	{
			gdata.byte[4-i] = CS1168_ReceiveByte();
	} 
	
	write_bit(CS1168_SCLK, LOW);	

	return(gdata.word);
} 

void CS1168_Init(void) 
{
		int a;
#if 1		
		write_bit(CS1168_SCLK, HIGH); 

		a = 0;
		while( !read_bit(CS1168_DDAT) && a < 20000) a++; 
		a = 0;
		while( read_bit(CS1168_DDAT) && a < 20000) a++; 
		write_bit(CS1168_SCLK, LOW);
#endif		 
} 


static int proc_write_sec(struct file* file, const char* buffer, unsigned long count, void* data)
{
		return count;
}

static int proc_read_sec(struct file *filp,
                                 char *buf,size_t count , loff_t *offp)
{
    char line[80];
  	int len = 0;
  	unsigned long n;
  	unsigned long flags, tmp;

  	if( *offp != 0 )
  		return 0;
  
    local_irq_save(flags);

    *(volatile u32 *)(RALINK_REG_INTDIS) = cpu_to_le32(RALINK_INTCTL_PIO);
		
	tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIODIR));

	tmp |=  (1L<<CS1168_SCLK);

	tmp &= ~(1L<<CS1168_DDAT);
	*(volatile u32 *)(RALINK_REG_PIODIR) = tmp;
		
	CS1168_Init( ); 	
  	n = CS1168_ReadAD();

  	*(volatile u32 *)(RALINK_REG_INTENA) = cpu_to_le32(RALINK_INTCTL_PIO);
  	local_irq_restore(flags);
  	
	len = sprintf(line,"%x\n", ((n>>24)&0xFF) );
	
		copy_to_user(buf,line,len );
	*offp = len;
	return len;	
}


struct file_operations ad_fops = {
	read : proc_read_sec,
	write : proc_write_sec,
};

#define BATTERY_FILE	"driver/cs1168"

static struct proc_dir_entry *battery_file = NULL;

static __init int cs1168_init(void)
{     
	unsigned long tmp, arg, gpiomode;
			
  	battery_file = create_proc_entry(BATTERY_FILE, 0777, NULL);
  	if (battery_file == NULL)
			return -EIO;
	
//  	battery_file->owner = THIS_MODULE;
  	battery_file->proc_fops = &ad_fops;
      
	gpiomode = le32_to_cpu(*(volatile u32 *)(RALINK_REG_GPIOMODE));
						
#if defined (CONFIG_RALINK_RT3052) || defined (CONFIG_RALINK_RT2883)
		gpiomode &= ~0x1C;
#endif
		gpiomode |= (RALINK_GPIOMODE_DFT|RALINK_GPIOMODE_JTAG);
		*(volatile u32 *)(RALINK_REG_GPIOMODE) = cpu_to_le32(gpiomode); 
		
		tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIODIR));
		tmp |=  (1L<<CS1168_SCLK);
		tmp &= ~(1L<<CS1168_DDAT);
		*(volatile u32 *)(RALINK_REG_PIODIR) = tmp;
		write_bit(CS1168_SCLK, LOW);

		printk("CS1168 ADC Linux Driver V1.0\n");
		printk("Bococom Co.,LTD. Hex 2011.5\n");
      return 0;
}

static __exit void cs1168_exit(void)
{
     if ( battery_file )	
 			remove_proc_entry(BATTERY_FILE, 0);
}

module_init(cs1168_init);
module_exit(cs1168_exit);

MODULE_AUTHOR("Shenzhen Bococom Technology Co.,Ltd.");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("CS1168 ADC Battery Voltage Interface");


