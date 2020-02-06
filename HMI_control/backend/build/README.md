# On setting

**You should install 2017-09-07-raspbian-stretch.img on to the SD card!**

# How to install on sd?

You can get zip of img at [here](https://downloads.raspberrypi.org/raspbian/images/raspbian-2017-09-08/)
```language=shell
$ unzip 2017-09-07-raspbian-stretch.zip
$ sudo dd bs=1M if=2017-09-07-raspbian-stretch.img of=/dev/sd*
# /dev/sd* is based on your env.
```

and insert to raspi

and boot it!

# How to set WIFI?

After boot and setup your eth0
Then run this `setWIFI.sh`

```language=shell
$ chmod +x setWIFI.sh
$ ./setWIFI.sh
```
