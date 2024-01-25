## System boot log with modified kernel with
## rkwifi/BCMDHD_WIFI6 overall bug fixes

```
 _____      _____    ____          ___ ___  ____  
| ____|___ |  ___|  / ___|__ _ _ _|_ _/ _ \/ ___| 
|  _| / _ \| |_    | |   / _` | '__| | | | \___ \ 
| |__| (_) |  _|   | |__| (_| | |  | | |_| |___) |
|_____\___/|_|      \____\__,_|_| |___\___/|____/ 
                                                  

Welcome to Ubuntu 18.04.6 LTS 4.4.179-eof

System load:   0.37            	Up time:       18 sec		
Memory usage:  5 % of 1961Mb 	IP:            192.168.181.211,192.168.200.1
CPU temp:      38°C           	
Usage of /:    68% of 15G    	

Linux SOM-RK3399 4.4.179-eof #3 SMP Fri Jan 19 10:02:39 UTC 2024 aarch64 aarch64 aarch64 GNU/Linux

Last login: Tue Jan 23 15:22:42 2024 from 192.168.181.4

*******************************************************************************
* NOTES TO THIS LOG:
*
* Here the WiFi client devices in the logs
*
* Android Car Radio		MAC: VV:VV:VV:VV:VV:VV mcu-px6
* iPhone 6 IOS 12.x  	MAC: ZZ:ZZ:ZZ:ZZ:ZZ:ZZ iPhone
* iPhone 6 SE IOS 17.x	MAC: YY:YY:YY:YY:YY:YY iPhone
* SOM-RK3399v2 Board	MAC: XX:XX:XX:XX:XX:XX (same Linux kernel as WiFi AP)
*
* Firmware BIN: cypress-BCM4356A2_V1.2.20230117
* Firmware CLM: Cypress.CYW4356 (2021-04-09 00:38:31)
*
* fw_bcm4356a2_ag.bin -> cypress/cyfmac4356-sdio.bin
* clm_bcm4356a2_ag.blob -> cypress/cyfmac4356-sdio.clm_blob
*
* Kernel changes:
* https://github.com/britus/kernel-rk3399.git
*
*******************************************************************************

Jan 25 10:21:07 SOM-RK3399 kernel: [    1.187288] [WLAN_RFKILL]: Enter rfkill_wlan_init
Jan 25 10:21:07 SOM-RK3399 kernel: [    1.188052] [WLAN_RFKILL]: Enter rfkill_wlan_probe
Jan 25 10:21:07 SOM-RK3399 kernel: [    1.188528] [WLAN_RFKILL]: wlan_platdata_parse_dt: wifi_chip_type = ap6354
Jan 25 10:21:07 SOM-RK3399 kernel: [    1.189136] [WLAN_RFKILL]: wlan_platdata_parse_dt: enable wifi power control.
Jan 25 10:21:07 SOM-RK3399 kernel: [    1.189774] [WLAN_RFKILL]: wlan_platdata_parse_dt: wifi power controled by gpio.
Jan 25 10:21:07 SOM-RK3399 kernel: [    1.190432] of_get_named_gpiod_flags: can't parse 'WIFI,poweren_gpio' property of node '/wireless-wlan[0]'
Jan 25 10:21:07 SOM-RK3399 kernel: [    1.190442] of_get_named_gpiod_flags: can't parse 'WIFI,vbat_gpio' property of node '/wireless-wlan[0]'
Jan 25 10:21:07 SOM-RK3399 kernel: [    1.190450] of_get_named_gpiod_flags: can't parse 'WIFI,reset_gpio' property of node '/wireless-wlan[0]'
Jan 25 10:21:07 SOM-RK3399 kernel: [    1.190490] of_get_named_gpiod_flags: parsed 'WIFI,host_wake_irq' property of node '/wireless-wlan[0]' - status (0)
Jan 25 10:21:07 SOM-RK3399 kernel: [    1.190498] [WLAN_RFKILL]: wlan_platdata_parse_dt: get property: WIFI,host_wake_irq = 3, flags = 0.
Jan 25 10:21:07 SOM-RK3399 kernel: [    1.191304] [WLAN_RFKILL]: wlan_platdata_parse_dt: The ref_wifi_clk not found !
Jan 25 10:21:07 SOM-RK3399 kernel: [    1.191955] [WLAN_RFKILL]: rfkill_wlan_probe: init gpio
Jan 25 10:21:07 SOM-RK3399 kernel: [    1.192424] [WLAN_RFKILL]: Exit rfkill_wlan_probe
Jan 25 10:21:07 SOM-RK3399 kernel: [    1.220386] Key type dns_resolver registered
Jan 25 10:21:07 SOM-RK3399 kernel: [    6.009910] [dhd] dhd_module_init: in Dongle Host Driver, version 100.10.545.19 (r826445-20210310-3)
Jan 25 10:21:07 SOM-RK3399 kernel: [    6.009925] [dhd] ======== dhd_wlan_init_plat_data ========
Jan 25 10:21:07 SOM-RK3399 kernel: [    6.010438] [dhd] dhd_wlan_init_gpio: WL_HOST_WAKE=-1, oob_irq=74, oob_irq_flags=0x414
Jan 25 10:21:07 SOM-RK3399 kernel: [    6.010442] [dhd] dhd_wlan_init_gpio: WL_REG_ON=-1
Jan 25 10:21:07 SOM-RK3399 kernel: [    6.010447] [dhd] dhd_wifi_platform_load: Enter
Jan 25 10:21:07 SOM-RK3399 kernel: [    6.010452] [dhd] Power-up adapter 'DHD generic adapter'
Jan 25 10:21:07 SOM-RK3399 kernel: [    6.010457] [dhd]  - irq 74 [flags 4], firmware: (null), nvram: (null)
Jan 25 10:21:07 SOM-RK3399 kernel: [    6.010461] [dhd]  - bus type -1, bus num -1, slot num -1
Jan 25 10:21:08 SOM-RK3399 kernel: [    7.960498] [dhd] wifi_platform_set_power = 1, delay: 200 msec
Jan 25 10:21:08 SOM-RK3399 kernel: [    7.960568] [dhd] ======== PULL WL_REG_ON(-1) HIGH! ========
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.261991] [dhd] wifi_platform_bus_enumerate device present 1
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.262035] [dhd] ======== Card detection to detect SDIO card! ========
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.278999] [dhd] bcmsdh_register: register client driver
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.279485] [dhd] bus num (host idx)=2, slot num (rca)=1
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.279747] [dhd] found adapter info 'DHD generic adapter'
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.279829] [dhd] sdioh_attach: set sd_f2_blocksize 256
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.282886] [dhd] sdioh_attach: sd clock rate = 0
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.283228] [dhd] dhdsdio_probe : no mutex held. set lock
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.283375] [dhd] F1 signature read @0x18000000=0x17224356
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.288507] [dhd] F1 signature OK, socitype:0x1 chip:0x4354 rev:0x2 pkg:0x2
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.289237] [dhd] DHD: dongle ram size is set to 786432(orig 786432) at 0x180000
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.289386] [dhd] dhd_conf_set_chiprev : devid=0x4356, chip=0x4354, chiprev=2
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.290435] [dhd] CFG80211[I]: wl_setup_wiphy : Registering Vendor80211
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.291108] [dhd] CFG80211[I]: wl_cfgvendor_attach : Vendor: Register BRCM cfg80211 vendor cmd(0x67) interface 
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.295098] [dhd] dhd_log_dump_init: kernel log buf size = 256KB; logdump_prsrv_tailsize = 80KB; limit prsrv tail size to = 38KB
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.297407] [dhd] dhd_attach(): thread:dhd_watchdog_thread:29f started
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.297999] [dhd] dhd_attach(): thread:dhd_dpc:2a0 started
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.298407] [dhd] dhd_attach(): thread:dhd_rxf:2a1 started
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.298444] [dhd] dhd_deferred_work_init: work queue initialized
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.298477] [dhd] dhd_tcpack_suppress_set: TCP ACK Suppress mode 0 -> mode 2
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.300824] [dhd] dhd_get_memdump_info: MEMDUMP ENABLED = 0
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.300881] [dhd] sdioh_cis_read: func_cis_ptr[0]=0x10ac
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.308143] [dhd] dhdsdio_probe_init: making DHD_BUS_DOWN
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.308257] [dhd] Dongle Host Driver, version 100.10.545.19 (r826445-20210310-3)
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.310566] [dhd] Register interface [wlan0]  MAC: UU:UU:UU:UU:UU:UU
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.310876] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=3 DOWN buf=NULL
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.310889] [dhd] dhd_dbg_detach_pkt_monitor, 2099
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.310905] [dhd] dhd_bus_devreset: == Power OFF ==
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.311108] [dhd] dhd_bus_stop: making DHD_BUS_DOWN
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.311140] [dhd] bcmsdh_oob_intr_unregister: Enter
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.311146] [dhd] bcmsdh_oob_intr_unregister: irq is not registered
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.311162] [dhd] dhd_bus_devreset: making dhdpub up FALSE
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.311172] [dhd] dhd_txglom_enable: enable 0
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.311179] [dhd] dhd_bus_devreset: making DHD_BUS_DOWN
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.311188] [dhd] dhd_bus_devreset:  WLAN OFF DONE
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.311217] [dhd] wifi_platform_set_power = 0, delay: 0 msec
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.311228] [dhd] ======== PULL WL_REG_ON(-1) LOW! ========
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.313914] [dhd] dhdsdio_probe : the lock is released.
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.316565] [dhd] nl_to socket created successfully...
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.316591] [dhd] dhd_module_init: Exit err=0
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.486121] [dhd] dhd_ioctl_entry: Interface is down 
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.486162] [dhd] dhd_ioctl_entry: Interface is down 
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.486188] [dhd] dhd_ioctl_entry: Interface is down 
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.486211] [dhd] dhd_ioctl_entry: Interface is down 
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.486232] [dhd] dhd_ioctl_entry: Interface is down 
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.486250] [dhd] WL_IW: wl_iw_get_essid : Error getting the SSID -22
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.486948] [dhd] dhd_ioctl_entry: Interface is down 
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.486981] [dhd] dhd_ioctl_entry: Interface is down 
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.487005] [dhd] dhd_ioctl_entry: Interface is down 
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.487024] [dhd] dhd_ioctl_entry: Interface is down 
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.487042] [dhd] dhd_ioctl_entry: Interface is down 
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.487562] [dhd] dhd_ioctl_entry: Interface is down 
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.487590] [dhd] dhd_ioctl_entry: Interface is down 
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.487612] [dhd] dhd_ioctl_entry: Interface is down 
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.491236] [dhd] dhd_ioctl_entry: Interface is down 
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.496412] [dhd] dhd_open: Enter wlan0
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.496433] [dhd] dhd_open : no mutex held. set lock
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.496449] [dhd] 
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.496477] [dhd] wifi_platform_set_power = 1, delay: 200 msec
Jan 25 10:21:09 SOM-RK3399 kernel: [    8.496493] [dhd] ======== PULL WL_REG_ON(-1) HIGH! ========
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.285282] [dhd] sdioh_start: set sd_f2_blocksize 256
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.286083] [dhd] dhd_bus_devreset: == Power ON ==
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.286231] [dhd] F1 signature read @0x18000000=0x17224356
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.291493] [dhd] F1 signature OK, socitype:0x1 chip:0x4354 rev:0x2 pkg:0x2
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.292292] [dhd] DHD: dongle ram size is set to 786432(orig 786432) at 0x180000
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.292402] [dhd] dhd_bus_devreset: making DHD_BUS_DOWN
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.292475] [dhd] dhdsdio_probe_init: making DHD_BUS_DOWN
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.295495] [dhd] dhd_os_open_image1: /system/etc/firmware/config.txt (199 bytes) open success
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.296011] [dhd] dhd_conf_read_country : ccode = DE
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.296473] [dhd] dhd_conf_read_country : regrev = 1
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.297066] [dhd] dhd_conf_read_sdio_params : dhd_doflow = 1
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.297633] [dhd] dhd_conf_read_others : srl = 15
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.298079] [dhd] dhd_conf_read_others : lrl = 15
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.298575] [dhd] dhd_conf_set_path_params : Final fw_path=/system/etc/firmware/fw_bcm4356a2_ag.bin
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.299384] [dhd] dhd_conf_set_path_params : Final nv_path=/system/etc/firmware/nvram_ap6356.txt
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.300243] [dhd] dhd_conf_set_path_params : Final clm_path=/system/etc/firmware/clm_bcm4356a2_ag.blob
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.301141] [dhd] dhd_conf_set_path_params : Final conf_path=/system/etc/firmware/config.txt
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.307792] [dhd] dhd_os_open_image1: /system/etc/firmware/fw_bcm4356a2_ag.bin (580258 bytes) open success
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.345255] [dhd] dhd_os_open_image1: /system/etc/firmware/nvram_ap6356.txt (4264 bytes) open success
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.345842] [dhd] NVRAM version: AP6356_NVRAM_V1.1_20141015
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.346409] [dhd] dhdsdio_write_vars: Download, Upload and compare of NVRAM succeeded.
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.413418] [dhd] dhd_bus_init: enable 0x06, ready 0x06 (waited 0us)
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.413765] [dhd] bcmsdh_oob_intr_register: HW_OOB irq=74 flags=0x4
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.414004] [dhd] dhd_get_memdump_info: MEMDUMP ENABLED = 0
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.415313] [dhd] dhd_tcpack_suppress_set: TCP ACK Suppress mode 2 -> mode 1
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.415883] [dhd] dhd_preinit_ioctls : Set IOCTL response time.
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.416744] [dhd] dhd_os_open_image1: /system/etc/firmware/clm_bcm4356a2_ag.blob (5750 bytes) open success
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.417851] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=clmload
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.417872] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x58000021002
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.418705] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=clmload
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.418720] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x58000021000
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.419375] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=clmload
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.419391] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x58000021000
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.420168] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=clmload
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.420186] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x58000021000
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.420958] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=clmload
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.420977] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x7600021004
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.422546] [dhd] dhd_apply_default_clm: CLM download succeeded 
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.424103] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=apsta
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.424125] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x1
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.424693] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=p2p_da_override
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.424709] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x954312d6
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.425202] [dhd] dhd_preinit_ioctls: p2p_da_override succeeded
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.425218] [dhd] Firmware up: op_mode=0x0005, MAC=UU:UU:UU:UU:UU:UU
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.425240] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=assoc_listen
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.425247] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0xa
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.426049] [dhd] Disable tdls_auto_op failed. -1
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.426077] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=86 WLC_SET_PM value=0x2
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.426375] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=bus:txglomalign
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.426384] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x20
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.426981] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=bcn_timeout
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.426999] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x8
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.427320] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=assoc_retry_max
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.427331] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x3
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.427739] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=mkeep_alive
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.427754] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x7530000b0001
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.428052] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=scancache
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.428063] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x1
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.428345] [dhd] dhd_preinit_ioctls Set scancache failed -23
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.428360] [dhd] dhd_iovar: GET 'event_log_max_sets' -> res_len(4) < input_len(19)
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.428927] [dhd] dhd_preinit_ioctls: event_log_max_sets: 26 ret: -23
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.428953] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=219 WLC_SET_FAKEFRAG value=0x0
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.429174] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=buf_key_b4_m4
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.429184] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x1
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.429942] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=event_msgs
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.429956] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x2c06012008b1fff
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.430488] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=event_msgs_ext
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.430496] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x8b1fff18180301
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.430842] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=185 WLC_SET_SCAN_CHANNEL_TIME value=0x28
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.431114] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=187 WLC_SET_SCAN_UNASSOC_TIME value=0x50
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.431386] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=258 WLC_SET_SCAN_PASSIVE_TIME value=0x82
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.431657] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=arpoe
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.431668] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x1
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.432343] [dhd] dhd_arp_offload_enable: ARP Version= 20120910
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.432357] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=arp_ol
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.432364] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x19
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.432765] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=pkt_filter_add
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.432779] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x65
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.433095] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=pkt_filter_add
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.433105] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x66
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.433405] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=pkt_filter_add
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.433414] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x67
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.433747] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=pkt_filter_add
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.433759] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x6e
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.434079] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=pkt_filter_add
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.434090] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x6f
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.434393] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=pkt_filter_add
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.434402] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x70
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.434763] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=pkt_filter_add
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.434777] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x79
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.435422] [dhd] CLM version = Cypress.CYW4356 (2021-04-09 00:38:31)
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.435432] [dhd] CLM version = Cypress.CYW4356 (2021-04-09 00:38:31)
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.435442] [dhd]   Driver: 100.10.545.19 (r826445-20210310-3)
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.435864] [dhd] dhd_preinit_ioctls wlc_ver failed -23
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.435892] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=bus:rxglom
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.435900] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x1
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.436129] [dhd] dhd_txglom_enable: enable 1
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.436140] [dhd] dhd_conf_set_txglom_params : txglom_mode=multi-desc
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.436788] [dhd] dhd_conf_set_txglom_params : txglomsize=36, deferred_tx_len=0
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.437435] [dhd] dhd_conf_set_txglom_params : txinrx_thres=128, dhd_txminmax=-1
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.438134] [dhd] dhd_conf_set_txglom_params : tx_max_offset=0, txctl_tmo_fix=300
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.438850] [dhd] dhd_conf_get_disable_proptx : fw_proptx=1, disable_proptx=-1
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.439491] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=3 DOWN value=0x1
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.439858] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=ampdu_hostreorder
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.439876] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x1
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.440132] [dhd] dhd_preinit_ioctls wl ampdu_hostreorder failed -23
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.440151] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=ap_isolate
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.440158] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x1
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.440740] [dhd] dhd_pno_init: Support Android Location Service
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.440764] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=2 UP value=0x1
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.454280] [dhd] rtt_do_get_ioctl: failed to send getbuf proxd iovar (CMD ID : 1), status=-23
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.454304] [dhd] dhd_rtt_init : FTM is not supported
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.454324] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=proxd
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.454332] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x4000400140300
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.454749] [dhd] dhd_rtt_ftm_config : failed to set config
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.455151] [dhd] dhd_ndo_get_version: failed, retcode = -23
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.455659] [dhd] dhd_wl_ioctl_get_intiovar: get int iovar wnm_bsstrans_resp failed, ERR -23
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.455675] [dhd] failed to get wnm_bsstrans_resp
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.455694] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wnm
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.455702] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.456059] [dhd] failed to set WNM capabilities
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.456084] [dhd] dhd_iovar: GET 'event_log_set_type' -> res_len(8) < input_len(27)
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.456463] [dhd] dhd_iovar: GET 'event_log_set_type' -> res_len(8) < input_len(27)
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.456956] [dhd] dhd_iovar: GET 'event_log_set_type' -> res_len(8) < input_len(27)
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.457356] [dhd] dhd_iovar: GET 'event_log_set_type' -> res_len(8) < input_len(27)
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.457998] [dhd] dhd_iovar: GET 'event_log_set_type' -> res_len(8) < input_len(27)
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.458394] [dhd] dhd_iovar: GET 'event_log_set_type' -> res_len(8) < input_len(27)
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.458895] [dhd] dhd_iovar: GET 'event_log_set_type' -> res_len(8) < input_len(27)
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.459296] [dhd] dhd_iovar: GET 'event_log_set_type' -> res_len(8) < input_len(27)
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.459756] [dhd] dhd_iovar: GET 'event_log_set_type' -> res_len(8) < input_len(27)
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.460151] [dhd] dhd_iovar: GET 'event_log_set_type' -> res_len(8) < input_len(27)
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.460584] [dhd] dhd_iovar: GET 'event_log_set_type' -> res_len(8) < input_len(27)
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.461000] [dhd] dhd_iovar: GET 'event_log_set_type' -> res_len(8) < input_len(27)
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.461392] [dhd] dhd_iovar: GET 'event_log_set_type' -> res_len(8) < input_len(27)
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.461946] [dhd] dhd_iovar: GET 'event_log_set_type' -> res_len(8) < input_len(27)
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.462336] [dhd] dhd_iovar: GET 'event_log_set_type' -> res_len(8) < input_len(27)
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.462853] [dhd] dhd_iovar: GET 'event_log_set_type' -> res_len(8) < input_len(27)
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.463248] [dhd] dhd_iovar: GET 'event_log_set_type' -> res_len(8) < input_len(27)
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.463771] [dhd] dhd_iovar: GET 'event_log_set_type' -> res_len(8) < input_len(27)
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.464239] [dhd] dhd_iovar: GET 'event_log_set_type' -> res_len(8) < input_len(27)
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.464787] [dhd] dhd_iovar: GET 'event_log_set_type' -> res_len(8) < input_len(27)
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.465239] [dhd] dhd_iovar: GET 'event_log_set_type' -> res_len(8) < input_len(27)
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.465970] [dhd] dhd_iovar: GET 'event_log_set_type' -> res_len(8) < input_len(27)
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.466427] [dhd] dhd_iovar: GET 'event_log_set_type' -> res_len(8) < input_len(27)
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.466922] [dhd] dhd_iovar: GET 'event_log_set_type' -> res_len(8) < input_len(27)
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.467364] [dhd] dhd_iovar: GET 'event_log_set_type' -> res_len(8) < input_len(27)
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.467893] [dhd] dhd_iovar: GET 'event_log_set_type' -> res_len(8) < input_len(27)
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.468340] [dhd] dhd_preinit_ioctls: Failed to get preserve log # !
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.468365] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=2 UP value=0x0
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.468815] [dhd] dhd_conf_map_country_list : DE/7
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.469259] [dhd] dhd_conf_set_country : set country DE, revision 7
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.469918] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=country
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.469931] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x700004544
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.470504] [dhd] CONFIG: dhd_conf_set_bufiovar : country setting failed -2, len=12
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.471792] [dhd] dhd_conf_set_country : Country code: DE (DE/0)
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.473381] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=bcn_timeout
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.473399] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x10
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.473850] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=32 WLC_SET_SRL value=0xf
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.474116] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=34 WLC_SET_LRL value=0xf
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.474367] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=roam_off
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.474380] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x1
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.474848] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=219 WLC_SET_FAKEFRAG value=0x1
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.475132] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=3 DOWN buf=NULL
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.475981] [dhd] dhd_wl_ioctl: cmd: 3, buf is NULL
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.476012] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=assoc_retry_max
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.476023] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x14
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.476382] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=3 DOWN buf=NULL
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.476697] [dhd] dhd_wl_ioctl: cmd: 3, buf is NULL
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.476719] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=ampdu_mpdu
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.476730] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x10
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.477166] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=txbf
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.477181] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x1
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.478493] [dhd] WL-CFGP2P: wl_cfgp2p_supported : p2p is supported
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.479690] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=2 UP value=0x0
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.492296] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=20 SET_INFRA value=0x1
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.509709] [dhd] CFG80211[I]: __wl_cfg80211_up: scan_ver, UNSUPPORTED
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.511666] [dhd] dhd_open : the lock is released.
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.511691] [dhd] dhd_open: Exit wlan0 ret=0
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.511701] [dhd] [wlan0] tx queue started
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.511793] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=mcast_list
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.511806] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x333300000002
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.512376] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=allmulti
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.512392] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.513041] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=10 SET_PROMISC value=0x0
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.513048] [dhd] CFG80211[I]: wl_cfg80211_add_if : if name: wlan1, wl_iftype:0 
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.514154] [dhd] dhd_set_mcast_list_handler: set multicast list for if 0
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.514167] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=50 SCAN value=0x0
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.514487] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=mcast_list
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.514501] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x333300000004
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.515279] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=tlv
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.515301] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x4f
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.515854] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=allmulti
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.515873] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.516732] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=10 SET_PROMISC value=0x0
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.517012] [dhd] dhd_set_mcast_list_handler: set multicast list for if 0
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.517023] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wlfc_mode
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.517029] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x1c
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.517357] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=mcast_list
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.517369] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x333300000004
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.518741] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=2 UP value=0xa9bd7bfd
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.519186] [dhd] dhd_wlfc_enable: ret=0
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.519360] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=allmulti
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.519371] [dhd] CFG80211[I]: wl_get_vif_macaddr : Get virtual I/F mac addr: d6:12:43:95:28:32
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.520188] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.522076] [dhd] CFG80211[I]: wl_cfg80211_notify_ifadd : IF_ADD ifidx:1 bssidx:1 role:0
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.522884] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=10 SET_PROMISC value=0x0
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.523247] [dhd] dhd_set_mcast_list_handler: set multicast list for if 0
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.523256] [dhd] dhd_wl_ioctl:iovar: ifidx=1 cmd=263 WLC_SET_VAR name=cur_etheraddr
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.523264] [dhd] dhd_wl_ioctl:iovar: ifidx=1 cmd=263 WLC_SET_VAR value=0x954312d6
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.523800] [dhd] dhd_register_if: MACID is overwritten
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.524913] [dhd] Register interface [wlan1]  MAC: d6:12:43:95:28:32
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.524961] [dhd] dhd_wl_ioctl:iovar: ifidx=1 cmd=20 SET_INFRA value=0x1
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.525638] [dhd] CFG80211[I]: wl_cfg80211_post_ifcreate : Network Interface (wlan1) registered with host. cfg_iftype:2 wl_role:0 d6:12:43:95:28:32
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.526894] [dhd] dhd_wl_ioctl:iovar: ifidx=1 cmd=263 WLC_SET_VAR name=buf_key_b4_m4
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.526909] [dhd] dhd_wl_ioctl:iovar: ifidx=1 cmd=263 WLC_SET_VAR value=0x1
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.527377] [dhd] CFG80211[I]: wl_cfg80211_add_if : Vif created. dev->ifindex:5 cfg_iftype:2, vif_count:1
Jan 25 10:21:10 SOM-RK3399 avahi-daemon[586]: Joining mDNS multicast group on interface wlan0.IPv4 with address 192.168.200.1.
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.563858] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=arp_hostip_clear
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.563892] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x726165
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.564401] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=arp_hostip
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.564422] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x1c8a8c0
Jan 25 10:21:10 SOM-RK3399 avahi-daemon[586]: New relevant interface wlan0.IPv4 for mDNS.
Jan 25 10:21:10 SOM-RK3399 avahi-daemon[586]: Registering new address record for 192.168.200.1 on wlan0.IPv4.
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.565555] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=mcast_list
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.565581] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x333300000005
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.566191] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=allmulti
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.566212] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.566850] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=10 SET_PROMISC value=0x0
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.567134] [dhd] dhd_set_mcast_list_handler: set multicast list for if 0
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.574577] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=mcast_list
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.574610] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x333300000005
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.575153] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=allmulti
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.575175] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.575698] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=10 SET_PROMISC value=0x0
Jan 25 10:21:10 SOM-RK3399 kernel: [    9.576107] [dhd] dhd_set_mcast_list_handler: set multicast list for if 0
Jan 25 10:21:10 SOM-RK3399 sh[700]: ifup: interface wlan0 already configured
Jan 25 10:21:11 SOM-RK3399 kernel: [   10.532456] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=mcast_list
Jan 25 10:21:11 SOM-RK3399 kernel: [   10.532500] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x333300000006
Jan 25 10:21:11 SOM-RK3399 avahi-daemon[586]: Joining mDNS multicast group on interface wlan0.IPv6 with address fe80::d612:43ff:fe95:6832.
Jan 25 10:21:11 SOM-RK3399 avahi-daemon[586]: New relevant interface wlan0.IPv6 for mDNS.
Jan 25 10:21:11 SOM-RK3399 avahi-daemon[586]: Registering new address record for fe80::d612:43ff:fe95:6832 on wlan0.*.
Jan 25 10:21:11 SOM-RK3399 kernel: [   10.538495] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=allmulti
Jan 25 10:21:11 SOM-RK3399 kernel: [   10.538565] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:21:11 SOM-RK3399 kernel: [   10.539294] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=10 SET_PROMISC value=0x0
Jan 25 10:21:11 SOM-RK3399 kernel: [   10.539985] [dhd] dhd_set_mcast_list_handler: set multicast list for if 0
Jan 25 10:21:11 SOM-RK3399 kernel: [   10.540041] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=mcast_list
Jan 25 10:21:11 SOM-RK3399 kernel: [   10.540063] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x333300000007
Jan 25 10:21:11 SOM-RK3399 kernel: [   10.540740] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=allmulti
Jan 25 10:21:11 SOM-RK3399 kernel: [   10.540776] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:21:11 SOM-RK3399 kernel: [   10.541334] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=10 SET_PROMISC value=0x0
Jan 25 10:21:11 SOM-RK3399 kernel: [   10.541874] [dhd] dhd_set_mcast_list_handler: set multicast list for if 0
Jan 25 10:21:12 SOM-RK3399 hostapd[875]: 1706178072.564749: Configuration file: /etc/hostapd/hostapd.conf
Jan 25 10:21:12 SOM-RK3399 dnsmasq[874]: dnsmasq: syntax check OK.
Jan 25 10:21:12 SOM-RK3399 kernel: [   11.758468] [dhd] CFG80211[I]: wl_cfg80211_change_virtual_iface : [wlan0] Enter. current cfg_iftype:2 new cfg_iftype:3 
Jan 25 10:21:12 SOM-RK3399 kernel: [   11.759574] [dhd] CFG80211[I]: wl_cfg80211_set_ap_role : [wlan0] Bringup SoftAP on bssidx:0 
Jan 25 10:21:12 SOM-RK3399 kernel: [   11.763083] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=3 DOWN value=0x1
Jan 25 10:21:12 SOM-RK3399 kernel: [   11.764121] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=apsta
Jan 25 10:21:12 SOM-RK3399 kernel: [   11.764146] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:21:12 SOM-RK3399 kernel: [   11.765076] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=118 WLC_SET_AP value=0x1
Jan 25 10:21:12 SOM-RK3399 kernel: [   11.765557] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=86 WLC_SET_PM value=0x0
Jan 25 10:21:12 SOM-RK3399 kernel: [   11.765951] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=20 SET_INFRA value=0x1
Jan 25 10:21:12 SOM-RK3399 kernel: [   11.766222] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=20 SET_INFRA value=0x1
Jan 25 10:21:12 SOM-RK3399 hostapd[875]: 1706178072.600042: wlan0: interface state UNINITIALIZED->COUNTRY_UPDATE
Jan 25 10:21:12 SOM-RK3399 rc.local[879]: Try Bluetooth hciattach...
Jan 25 10:21:12 SOM-RK3399 kernel: [   11.814209] [dhd][wlan0]AEXT[I] wl_ext_in4way_sync_ap: Reconnect not waiting for STA status=7, ap_recon_sta=NO => 0
Jan 25 10:21:12 SOM-RK3399 kernel: [   11.816243] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=201 WLC_SCB_DEAUTHENTICATE_FOR_REASON value=0xffffffff00000003
Jan 25 10:21:12 SOM-RK3399 kernel: [   11.817047] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:21:12 SOM-RK3399 kernel: [   11.817073] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:21:12 SOM-RK3399 kernel: [   11.817709] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:21:12 SOM-RK3399 kernel: [   11.817732] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x1
Jan 25 10:21:12 SOM-RK3399 kernel: [   11.818271] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:21:12 SOM-RK3399 kernel: [   11.818303] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x2
Jan 25 10:21:12 SOM-RK3399 kernel: [   11.818968] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:21:12 SOM-RK3399 kernel: [   11.818997] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x3
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.846595] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=ndoe
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.846627] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.847634] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=arp_ol
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.847655] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.848065] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=arpoe
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.848081] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.848730] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=chanspec
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.848743] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x1006
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.849375] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=76 WLC_SET_BCNPRD value=0x64
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.850236] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=78 WLC_SET_DTIMPRD value=0x2
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.850476] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wme_bss_disable
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.850486] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.851139] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=auth
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.851159] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.851467] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.851479] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x44
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.851803] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wpa_auth
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.851818] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x80
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.852257] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=20 SET_INFRA value=0x1
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.852468] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=50 SCAN value=0x0
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.853929] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=3 DOWN value=0x1
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.854135] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=apsta
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.854144] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.854458] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=118 WLC_SET_AP value=0x1
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.854750] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=86 WLC_SET_PM value=0x0
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.854971] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=20 SET_INFRA value=0x1
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.856471] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=2 UP value=0x1
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.869479] [dhd] _dhd_wlfc_mac_entry_update():1867, entry(32)
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.872363] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=26 SET_SSID value=0x2d4d4f530000000c
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.944769] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=scb_probe
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.944807] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x500000014
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.945938] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=pspretend_retry_limit
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.945970] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x1
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.947103] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=pspretend_threshold
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.947134] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x2
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.948452] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=event_msgs
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.948481] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x2c06013008b1fff
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.949107] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=86 WLC_SET_PM value=0x0
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.949980] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=ap_isolate
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.950008] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.953827] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.953863] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x1000000001
Jan 25 10:21:11 SOM-RK3399 dnsmasq[904]: started, version 2.79 cachesize 150
Jan 25 10:21:11 SOM-RK3399 dnsmasq[904]: compile time options: IPv6 GNU-getopt DBus i18n IDN DHCP DHCPv6 no-Lua TFTP conntrack ipset auth nettlehash DNSSEC loop-detect inotify
Jan 25 10:21:11 SOM-RK3399 dnsmasq-dhcp[904]: DHCP, IP range 192.168.200.50 -- 192.168.200.150, lease time infinite
Jan 25 10:21:11 SOM-RK3399 dnsmasq-dhcp[904]: DHCP, sockets bound exclusively to interface wlan0
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.959587] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.959601] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x44
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.960280] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_info
Jan 25 10:21:11 SOM-RK3399 kernel: [   11.960291] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x8010601000001
Jan 25 10:21:11 SOM-RK3399 dnsmasq[904]: using nameserver 1.1.1.1#53
Jan 25 10:21:11 SOM-RK3399 dnsmasq[904]: using local addresses only for domain localnet
Jan 25 10:21:11 SOM-RK3399 dnsmasq[904]: read /etc/hosts - 3 addresses
Jan 25 10:21:12 SOM-RK3399 hciattach[910]: bcm43xx_init
Jan 25 10:21:12 SOM-RK3399 hciattach[910]: Found firmware: /etc/firmware/BCM4354A2.hcd
Jan 25 10:21:12 SOM-RK3399 hciattach[910]: Set Controller UART speed to 1500000 bit/s
Jan 25 10:21:12 SOM-RK3399 hciattach[910]: Flash firmware: /etc/firmware/BCM4354A2.hcd
Jan 25 10:21:12 SOM-RK3399 hciattach[910]: Set Controller UART speed to 1500000 bit/s
Jan 25 10:21:12 SOM-RK3399 hciattach[910]: Device setup complete
Jan 25 10:21:12 SOM-RK3399 hostapd[924]: Selected interface 'wlan0'
Jan 25 10:21:12 SOM-RK3399 hostapd[924]: bssid=UU:UU:UU:UU:UU:UU
Jan 25 10:21:12 SOM-RK3399 hostapd[924]: ssid=SOM-RK3399v1
Jan 25 10:21:12 SOM-RK3399 hostapd[924]: wpa=2
Jan 25 10:21:12 SOM-RK3399 hostapd[924]: key_mgmt=WPA-PSK
Jan 25 10:21:12 SOM-RK3399 hostapd[924]: group_cipher=CCMP
Jan 25 10:21:12 SOM-RK3399 hostapd[924]: rsn_pairwise_cipher=CCMP
Jan 25 10:21:12 SOM-RK3399 hostapd[926]: Selected interface 'wlan0'
Jan 25 10:21:18 SOM-RK3399 dnsmasq[904]: read /etc/hosts - 3 addresses
Jan 25 10:21:18 SOM-RK3399 dnsmasq[904]: time 1706178078
Jan 25 10:21:18 SOM-RK3399 dnsmasq[904]: cache size 150, 0/0 cache insertions re-used unexpired cache entries.
Jan 25 10:21:18 SOM-RK3399 dnsmasq[904]: queries forwarded 0, queries answered locally 0
Jan 25 10:21:18 SOM-RK3399 dnsmasq[904]: queries for authoritative zones 0
Jan 25 10:21:18 SOM-RK3399 dnsmasq[904]: server 1.1.1.1#53: queries sent 0, retried or failed 0
Jan 25 10:21:20 SOM-RK3399 kernel: [   20.895085] [dhd][wlan0]AEXT[I] wl_ext_in4way_sync_ap: Connected STA VV:VV:VV:VV:VV:VV status=9, ap_recon_sta=NO
Jan 25 10:21:20 SOM-RK3399 kernel: [   20.897885] [dhd] CFG80211[I]: wl_cfg80211_change_station: [wlan0] VV:VV:VV:VV:VV:VV flags_set=0x0 flags_mask=0x1c flags_sta=0xc
Jan 25 10:21:20 SOM-RK3399 kernel: [   20.899165] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:21:20 SOM-RK3399 kernel: [   20.899174] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:21:20 SOM-RK3399 kernel: [   20.900318] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:21:20 SOM-RK3399 kernel: [   20.900334] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:21:20 SOM-RK3399 kernel: [   20.900708] [dhd] CFG80211[I]: wl_cfg80211_change_station: [wlan0] VV:VV:VV:VV:VV:VV flags_set=0x0 flags_mask=0x2 flags_sta=0xc
Jan 25 10:21:20 SOM-RK3399 kernel: [   20.904888] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:21:20 SOM-RK3399 kernel: [   20.904911] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x1000000001
Jan 25 10:21:20 SOM-RK3399 kernel: [   20.906542] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec
Jan 25 10:21:20 SOM-RK3399 kernel: [   20.906561] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x44
Jan 25 10:21:20 SOM-RK3399 kernel: [   20.906958] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_info
Jan 25 10:21:20 SOM-RK3399 kernel: [   20.906966] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x8010601000001
Jan 25 10:21:20 SOM-RK3399 kernel: [   21.299379] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:21:20 SOM-RK3399 kernel: [   21.299404] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x1000000000
Jan 25 10:21:20 SOM-RK3399 kernel: [   21.300133] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec
Jan 25 10:21:20 SOM-RK3399 kernel: [   21.300143] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x44
Jan 25 10:21:20 SOM-RK3399 kernel: [   21.301633] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_info
Jan 25 10:21:20 SOM-RK3399 kernel: [   21.301650] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x8010601000001
Jan 25 10:21:20 SOM-RK3399 kernel: [   21.302487] [dhd] CFG80211[I]: wl_cfg80211_change_station: [wlan0] VV:VV:VV:VV:VV:VV flags_set=0x2 flags_mask=0x2 flags_sta=0xc
Jan 25 10:21:20 SOM-RK3399 kernel: [   21.303518] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=121 WLC_SCB_AUTHORIZE value=0x98bfef44
Jan 25 10:21:20 SOM-RK3399 dnsmasq-dhcp[904]: DHCPDISCOVER(wlan0) VV:VV:VV:VV:VV:VV
Jan 25 10:21:20 SOM-RK3399 dnsmasq-dhcp[904]: DHCPOFFER(wlan0) 192.168.200.11 VV:VV:VV:VV:VV:VV
Jan 25 10:21:21 SOM-RK3399 dnsmasq-dhcp[904]: DHCPREQUEST(wlan0) 192.168.200.11 VV:VV:VV:VV:VV:VV
Jan 25 10:21:21 SOM-RK3399 dnsmasq-dhcp[904]: DHCPACK(wlan0) 192.168.200.11 VV:VV:VV:VV:VV:VV mcu-px6
Jan 25 10:21:55 SOM-RK3399 kernel: [   55.897363] [dhd][wlan0]AEXT[I] wl_ext_in4way_sync_ap: Connected STA ZZ:ZZ:ZZ:ZZ:ZZ:ZZ status=9, ap_recon_sta=NO
Jan 25 10:21:55 SOM-RK3399 kernel: [   55.899979] [dhd] CFG80211[I]: wl_cfg80211_change_station: [wlan0] ZZ:ZZ:ZZ:ZZ:ZZ:ZZ flags_set=0x0 flags_mask=0x1c flags_sta=0xc
Jan 25 10:21:55 SOM-RK3399 kernel: [   55.901207] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:21:55 SOM-RK3399 kernel: [   55.901217] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:21:55 SOM-RK3399 kernel: [   55.902274] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:21:55 SOM-RK3399 kernel: [   55.902297] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:21:55 SOM-RK3399 kernel: [   55.903284] [dhd] CFG80211[I]: wl_cfg80211_change_station: [wlan0] ZZ:ZZ:ZZ:ZZ:ZZ:ZZ flags_set=0x0 flags_mask=0x2 flags_sta=0xc
Jan 25 10:21:55 SOM-RK3399 kernel: [   56.362446] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:21:55 SOM-RK3399 kernel: [   56.362471] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x1000000000
Jan 25 10:21:55 SOM-RK3399 kernel: [   56.364044] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec
Jan 25 10:21:55 SOM-RK3399 kernel: [   56.364070] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x44
Jan 25 10:21:55 SOM-RK3399 kernel: [   56.364370] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_info
Jan 25 10:21:55 SOM-RK3399 kernel: [   56.364379] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x8010601000001
Jan 25 10:21:55 SOM-RK3399 kernel: [   56.365915] [dhd] CFG80211[I]: wl_cfg80211_change_station: [wlan0] ZZ:ZZ:ZZ:ZZ:ZZ:ZZ flags_set=0x2 flags_mask=0x2 flags_sta=0xc
Jan 25 10:21:55 SOM-RK3399 kernel: [   56.366980] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=121 WLC_SCB_AUTHORIZE value=0x4701fc9c
Jan 25 10:22:07 SOM-RK3399 CarIOS[1160]: [I] NETIPT: LAP-IFC -> wlan0
Jan 25 10:22:15 SOM-RK3399 dnsmasq[904]: time 1706178135
Jan 25 10:22:15 SOM-RK3399 dnsmasq[904]: cache size 150, 0/38 cache insertions re-used unexpired cache entries.
Jan 25 10:22:15 SOM-RK3399 dnsmasq[904]: queries forwarded 5, queries answered locally 0
Jan 25 10:22:15 SOM-RK3399 dnsmasq[904]: queries for authoritative zones 0
Jan 25 10:22:15 SOM-RK3399 dnsmasq[904]: server 1.1.1.1#53: queries sent 5, retried or failed 0
Jan 25 10:22:15 SOM-RK3399 dnsmasq[904]: read /etc/hosts - 3 addresses
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.630624] [dhd][wlan0]AEXT[I] wl_ext_in4way_sync_ap: Connected STA YY:YY:YY:YY:YY:YY status=9, ap_recon_sta=NO
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.632599] [dhd] CFG80211[I]: wl_cfg80211_change_station: [wlan0] YY:YY:YY:YY:YY:YY flags_set=0x0 flags_mask=0x1c flags_sta=0xc
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.633833] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.633843] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.635947] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.635973] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.636359] [dhd] CFG80211[I]: wl_cfg80211_change_station: [wlan0] YY:YY:YY:YY:YY:YY flags_set=0x0 flags_mask=0x2 flags_sta=0xc
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.655449] [dhd][wlan0]AEXT[I] wl_ext_in4way_sync_ap: Latest disconnect STA 00:00:00:00:00:00 ap_recon_sta=0
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.657990] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.658015] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.658763] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.658777] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.660470] [dhd] CFG80211[I]: wl_cfg80211_change_station: [wlan0] YY:YY:YY:YY:YY:YY flags_set=0x0 flags_mask=0x2 flags_sta=0xc
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.930433] [dhd][wlan0]AEXT[I] wl_ext_in4way_sync_ap: Connected STA YY:YY:YY:YY:YY:YY status=9, ap_recon_sta=YES
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.932525] [dhd] CFG80211[I]: wl_cfg80211_change_station: [wlan0] YY:YY:YY:YY:YY:YY flags_set=0x0 flags_mask=0x1c flags_sta=0xc
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.933792] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.933802] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.935637] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.935717] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.936150] [dhd] CFG80211[I]: wl_cfg80211_change_station: [wlan0] YY:YY:YY:YY:YY:YY flags_set=0x0 flags_mask=0x2 flags_sta=0xc
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.945812] [dhd][wlan0]AEXT[I] wl_ext_in4way_sync_ap: Latest disconnect STA YY:YY:YY:YY:YY:YY ap_recon_sta=1
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.947533] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.947553] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.949979] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.950002] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.950313] [dhd] CFG80211[I]: wl_cfg80211_change_station: [wlan0] YY:YY:YY:YY:YY:YY flags_set=0x0 flags_mask=0x2 flags_sta=0xc
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.967216] [dhd][wlan0]AEXT[I] wl_ext_in4way_sync_ap: Connected STA YY:YY:YY:YY:YY:YY status=9, ap_recon_sta=YES
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.969265] [dhd] CFG80211[I]: wl_cfg80211_change_station: [wlan0] YY:YY:YY:YY:YY:YY flags_set=0x0 flags_mask=0x1c flags_sta=0xc
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.970531] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.970542] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.971166] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.971182] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:22:37 SOM-RK3399 kernel: [   97.971499] [dhd] CFG80211[I]: wl_cfg80211_change_station: [wlan0] YY:YY:YY:YY:YY:YY flags_set=0x0 flags_mask=0x2 flags_sta=0xc
Jan 25 10:22:37 SOM-RK3399 kernel: [   98.198483] [dhd] _dhd_wlfc_deque_afq: can't find matching seq(0)
Jan 25 10:22:43 SOM-RK3399 kernel: [  104.244292] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:22:43 SOM-RK3399 kernel: [  104.244318] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:22:43 SOM-RK3399 kernel: [  104.244895] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:22:43 SOM-RK3399 kernel: [  104.244906] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:22:43 SOM-RK3399 kernel: [  104.245196] [dhd] CFG80211[I]: wl_cfg80211_change_station: [wlan0] YY:YY:YY:YY:YY:YY flags_set=0x0 flags_mask=0x2 flags_sta=0xc
Jan 25 10:22:43 SOM-RK3399 kernel: [  104.246601] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:22:43 SOM-RK3399 kernel: [  104.246616] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:22:47 SOM-RK3399 kernel: [  107.802324] [dhd][wlan0]AEXT[I] wl_ext_in4way_sync_ap: Latest disconnect STA YY:YY:YY:YY:YY:YY ap_recon_sta=1
Jan 25 10:22:47 SOM-RK3399 kernel: [  107.803526] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:22:47 SOM-RK3399 kernel: [  107.803532] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:22:47 SOM-RK3399 kernel: [  107.804865] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:22:47 SOM-RK3399 kernel: [  107.804881] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:22:47 SOM-RK3399 kernel: [  107.805181] [dhd] CFG80211[I]: wl_cfg80211_change_station: [wlan0] YY:YY:YY:YY:YY:YY flags_set=0x0 flags_mask=0x2 flags_sta=0xc
Jan 25 10:22:47 SOM-RK3399 kernel: [  107.867346] [dhd][wlan0]AEXT[I] wl_ext_in4way_sync_ap: Connected STA YY:YY:YY:YY:YY:YY status=9, ap_recon_sta=YES
Jan 25 10:22:47 SOM-RK3399 kernel: [  107.868818] [dhd] CFG80211[I]: wl_cfg80211_change_station: [wlan0] YY:YY:YY:YY:YY:YY flags_set=0x0 flags_mask=0x1c flags_sta=0xc
Jan 25 10:22:47 SOM-RK3399 kernel: [  107.869911] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:22:47 SOM-RK3399 kernel: [  107.869916] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:22:47 SOM-RK3399 kernel: [  107.870572] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:22:47 SOM-RK3399 kernel: [  107.870588] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:22:47 SOM-RK3399 kernel: [  107.870912] [dhd] CFG80211[I]: wl_cfg80211_change_station: [wlan0] YY:YY:YY:YY:YY:YY flags_set=0x0 flags_mask=0x2 flags_sta=0xc
Jan 25 10:22:47 SOM-RK3399 kernel: [  108.191321] [dhd][wlan0]AEXT[I] wl_ext_in4way_sync_ap: Latest disconnect STA YY:YY:YY:YY:YY:YY ap_recon_sta=1
Jan 25 10:22:47 SOM-RK3399 kernel: [  108.192746] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:22:47 SOM-RK3399 kernel: [  108.192764] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:22:47 SOM-RK3399 kernel: [  108.193443] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:22:47 SOM-RK3399 kernel: [  108.193463] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:22:47 SOM-RK3399 kernel: [  108.194260] [dhd] CFG80211[I]: wl_cfg80211_change_station: [wlan0] YY:YY:YY:YY:YY:YY flags_set=0x0 flags_mask=0x2 flags_sta=0xc
Jan 25 10:22:48 SOM-RK3399 kernel: [  108.467222] [dhd][wlan0]AEXT[I] wl_ext_in4way_sync_ap: Connected STA YY:YY:YY:YY:YY:YY status=9, ap_recon_sta=YES
Jan 25 10:22:48 SOM-RK3399 kernel: [  108.469243] [dhd] CFG80211[I]: wl_cfg80211_change_station: [wlan0] YY:YY:YY:YY:YY:YY flags_set=0x0 flags_mask=0x1c flags_sta=0xc
Jan 25 10:22:48 SOM-RK3399 kernel: [  108.470519] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:22:48 SOM-RK3399 kernel: [  108.470529] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:22:48 SOM-RK3399 kernel: [  108.471465] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:22:48 SOM-RK3399 kernel: [  108.471486] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:22:48 SOM-RK3399 kernel: [  108.472327] [dhd] CFG80211[I]: wl_cfg80211_change_station: [wlan0] YY:YY:YY:YY:YY:YY flags_set=0x0 flags_mask=0x2 flags_sta=0xc
Jan 25 10:22:52 SOM-RK3399 kernel: [  112.480505] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:22:52 SOM-RK3399 kernel: [  112.480531] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:22:52 SOM-RK3399 kernel: [  112.484585] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:22:52 SOM-RK3399 kernel: [  112.484609] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:22:52 SOM-RK3399 kernel: [  112.484957] [dhd] CFG80211[I]: wl_cfg80211_change_station: [wlan0] YY:YY:YY:YY:YY:YY flags_set=0x0 flags_mask=0x2 flags_sta=0xc
Jan 25 10:22:52 SOM-RK3399 kernel: [  112.486341] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:22:52 SOM-RK3399 kernel: [  112.486356] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:22:52 SOM-RK3399 kernel: [  112.617279] [dhd] _dhd_wlfc_deque_afq: can't find matching seq(10)
Jan 25 10:22:56 SOM-RK3399 kernel: [  116.745283] [dhd][wlan0]AEXT[I] wl_ext_in4way_sync_ap: Latest disconnect STA YY:YY:YY:YY:YY:YY ap_recon_sta=1
Jan 25 10:22:56 SOM-RK3399 kernel: [  116.746742] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:22:56 SOM-RK3399 kernel: [  116.746762] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:22:56 SOM-RK3399 kernel: [  116.747662] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:22:56 SOM-RK3399 kernel: [  116.747684] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:22:56 SOM-RK3399 kernel: [  116.748197] [dhd] CFG80211[I]: wl_cfg80211_change_station: [wlan0] ZZ:ZZ:ZZ:ZZ:ZZ:ZZ flags_set=0x0 flags_mask=0x2 flags_sta=0x0
Jan 25 10:22:56 SOM-RK3399 kernel: [  116.749273] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=122 SCB_DEAUTHORIZE value=0x4701fc9c
Jan 25 10:22:56 SOM-RK3399 kernel: [  116.749845] [dhd][wlan0]AEXT[I] wl_ext_in4way_sync_ap: Reconnect wait STA status=7, ap_recon_sta=NO, waiting 300ms ...
Jan 25 10:22:56 SOM-RK3399 kernel: [  117.051246] [dhd][wlan0]AEXT[I] wl_ext_in4way_sync_ap: Reconnect wait STA status=7, ap_recon_sta=NO, timeout=0
Jan 25 10:22:56 SOM-RK3399 kernel: [  117.057375] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=201 WLC_SCB_DEAUTHENTICATE_FOR_REASON value=0x4701fc9c00000003
Jan 25 10:22:56 SOM-RK3399 kernel: [  117.325643] [dhd][wlan0]AEXT[I] wl_ext_in4way_sync_ap: Connected STA ZZ:ZZ:ZZ:ZZ:ZZ:ZZ status=9, ap_recon_sta=NO
Jan 25 10:22:56 SOM-RK3399 kernel: [  117.327669] [dhd] CFG80211[I]: wl_cfg80211_change_station: [wlan0] ZZ:ZZ:ZZ:ZZ:ZZ:ZZ flags_set=0x0 flags_mask=0x1c flags_sta=0xc
Jan 25 10:22:56 SOM-RK3399 kernel: [  117.328935] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:22:56 SOM-RK3399 kernel: [  117.328946] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:22:56 SOM-RK3399 kernel: [  117.329654] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:22:56 SOM-RK3399 kernel: [  117.329674] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:22:56 SOM-RK3399 kernel: [  117.330176] [dhd] CFG80211[I]: wl_cfg80211_change_station: [wlan0] ZZ:ZZ:ZZ:ZZ:ZZ:ZZ flags_set=0x0 flags_mask=0x2 flags_sta=0xc
Jan 25 10:22:56 SOM-RK3399 kernel: [  117.362460] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:22:56 SOM-RK3399 kernel: [  117.362485] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x1000000000
Jan 25 10:22:56 SOM-RK3399 kernel: [  117.363016] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec
Jan 25 10:22:56 SOM-RK3399 kernel: [  117.363026] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x44
Jan 25 10:22:56 SOM-RK3399 kernel: [  117.363329] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_info
Jan 25 10:22:56 SOM-RK3399 kernel: [  117.363337] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x8010601000001
Jan 25 10:22:56 SOM-RK3399 kernel: [  117.364115] [dhd] CFG80211[I]: wl_cfg80211_change_station: [wlan0] ZZ:ZZ:ZZ:ZZ:ZZ:ZZ flags_set=0x2 flags_mask=0x2 flags_sta=0xc
Jan 25 10:22:56 SOM-RK3399 kernel: [  117.365182] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=121 WLC_SCB_AUTHORIZE value=0x4701fc9c
Jan 25 10:22:56 SOM-RK3399 dnsmasq-dhcp[904]: DHCPREQUEST(wlan0) 192.168.200.13 ZZ:ZZ:ZZ:ZZ:ZZ:ZZ
Jan 25 10:22:56 SOM-RK3399 dnsmasq-dhcp[904]: DHCPACK(wlan0) 192.168.200.13 ZZ:ZZ:ZZ:ZZ:ZZ:ZZ PattPhone
Jan 25 10:22:57 SOM-RK3399 kernel: [  117.485102] [dhd][wlan0]AEXT[I] wl_ext_in4way_sync_ap: Reconnect not waiting for STA status=7, ap_recon_sta=NO => 0
Jan 25 10:22:57 SOM-RK3399 kernel: [  117.486494] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=201 WLC_SCB_DEAUTHENTICATE_FOR_REASON value=0x48c0e24600000003
Jan 25 10:22:57 SOM-RK3399 kernel: [  117.499284] [dhd][wlan0]AEXT[I] wl_ext_in4way_sync_ap: Latest disconnect STA ZZ:ZZ:ZZ:ZZ:ZZ:ZZ ap_recon_sta=0
Jan 25 10:22:57 SOM-RK3399 kernel: [  117.932987] [dhd][wlan0]AEXT[I] wl_ext_in4way_sync_ap: Connected STA YY:YY:YY:YY:YY:YY status=9, ap_recon_sta=NO
Jan 25 10:22:57 SOM-RK3399 kernel: [  117.934866] [dhd] CFG80211[I]: wl_cfg80211_change_station: [wlan0] YY:YY:YY:YY:YY:YY flags_set=0x0 flags_mask=0x1c flags_sta=0xc
Jan 25 10:22:57 SOM-RK3399 kernel: [  117.936208] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:22:57 SOM-RK3399 kernel: [  117.936220] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:22:57 SOM-RK3399 kernel: [  117.936980] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:22:57 SOM-RK3399 kernel: [  117.937000] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:22:57 SOM-RK3399 kernel: [  117.937469] [dhd] CFG80211[I]: wl_cfg80211_change_station: [wlan0] YY:YY:YY:YY:YY:YY flags_set=0x0 flags_mask=0x2 flags_sta=0xc
Jan 25 10:22:57 SOM-RK3399 kernel: [  117.949777] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:22:57 SOM-RK3399 kernel: [  117.949804] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x1000000000
Jan 25 10:22:57 SOM-RK3399 kernel: [  117.950382] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec
Jan 25 10:22:57 SOM-RK3399 kernel: [  117.950397] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x44
Jan 25 10:22:57 SOM-RK3399 kernel: [  117.950810] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_info
Jan 25 10:22:57 SOM-RK3399 kernel: [  117.950822] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x8010601000001
Jan 25 10:22:57 SOM-RK3399 kernel: [  117.951471] [dhd] CFG80211[I]: wl_cfg80211_change_station: [wlan0] YY:YY:YY:YY:YY:YY flags_set=0x2 flags_mask=0x2 flags_sta=0xc
Jan 25 10:22:57 SOM-RK3399 kernel: [  117.952534] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=121 WLC_SCB_AUTHORIZE value=0x48c0e246
Jan 25 10:22:57 SOM-RK3399 dnsmasq-dhcp[904]: DHCPREQUEST(wlan0) 192.168.200.66 YY:YY:YY:YY:YY:YY
Jan 25 10:22:57 SOM-RK3399 dnsmasq-dhcp[904]: DHCPACK(wlan0) 192.168.200.66 YY:YY:YY:YY:YY:YY
Jan 25 10:24:00 SOM-RK3399 kernel: [  181.397269] [dhd][wlan0]AEXT[I] wl_ext_in4way_sync_ap: Connected STA XX:XX:XX:XX:XX:XX status=9, ap_recon_sta=NO
Jan 25 10:24:00 SOM-RK3399 kernel: [  181.399527] [dhd] CFG80211[I]: wl_cfg80211_change_station: [wlan0] XX:XX:XX:XX:XX:XX flags_set=0x0 flags_mask=0x1c flags_sta=0xc
Jan 25 10:24:00 SOM-RK3399 kernel: [  181.400746] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:24:00 SOM-RK3399 kernel: [  181.400756] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:24:00 SOM-RK3399 kernel: [  181.403047] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:24:00 SOM-RK3399 kernel: [  181.403071] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x0
Jan 25 10:24:00 SOM-RK3399 kernel: [  181.403436] [dhd] CFG80211[I]: wl_cfg80211_change_station: [wlan0] XX:XX:XX:XX:XX:XX flags_set=0x0 flags_mask=0x2 flags_sta=0xc
Jan 25 10:24:00 SOM-RK3399 kernel: [  181.420543] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_key
Jan 25 10:24:00 SOM-RK3399 kernel: [  181.420586] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x1000000000
Jan 25 10:24:00 SOM-RK3399 kernel: [  181.421117] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec
Jan 25 10:24:00 SOM-RK3399 kernel: [  181.421126] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x44
Jan 25 10:24:00 SOM-RK3399 kernel: [  181.421402] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR name=wsec_info
Jan 25 10:24:00 SOM-RK3399 kernel: [  181.421409] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=263 WLC_SET_VAR value=0x8010601000001
Jan 25 10:24:00 SOM-RK3399 kernel: [  181.421941] [dhd] CFG80211[I]: wl_cfg80211_change_station: [wlan0] XX:XX:XX:XX:XX:XX flags_set=0x2 flags_mask=0x2 flags_sta=0xc
Jan 25 10:24:00 SOM-RK3399 kernel: [  181.423000] [dhd] dhd_wl_ioctl:iovar: ifidx=0 cmd=121 WLC_SCB_AUTHORIZE value=0x9f4312d4
Jan 25 10:24:01 SOM-RK3399 dnsmasq-dhcp[904]: DHCPDISCOVER(wlan0) 192.168.200.142 XX:XX:XX:XX:XX:XX
Jan 25 10:24:01 SOM-RK3399 dnsmasq-dhcp[904]: DHCPOFFER(wlan0) 192.168.200.142 XX:XX:XX:XX:XX:XX
Jan 25 10:24:01 SOM-RK3399 dnsmasq-dhcp[904]: DHCPREQUEST(wlan0) 192.168.200.142 XX:XX:XX:XX:XX:XX
Jan 25 10:24:01 SOM-RK3399 dnsmasq-dhcp[904]: DHCPACK(wlan0) 192.168.200.142 XX:XX:XX:XX:XX:XX SOM-RK3399v2
```
