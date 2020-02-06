#On setting

**You should install 2017-09-07-raspbian-stretch.img on to the SD card!**

# How to install on micro SD?

You can get zip of img at [here](https://downloads.raspberrypi.org/raspbian/images/raspbian-2017-09-08/)
```language=shell
$ unzip 2017-09-07-raspbian-stretch.zip
$ sudo dd bs=1M if=2017-09-07-raspbian-stretch.img of=/dev/sd*
# /dev/sd* is based on your environment.
```

then insert the micro SD to raspi, and boot it!

# How to set Wi-Fi?

After boot and setup your eth0
Then run this `setWIFI.sh`

```language=shell
# Maybe you need to clone this repo. to get these files.
$ git clone git@github.com:25077667/farm_auto_irrigate_system.git
$ cd HMI_control/backend/build/

#
$ chmod +x setWIFI.sh
$ ./setWIFI.sh
```

# Set environment
```language=shell
$ sudo apt update
$ sudo apt upgrade
$ sudo apt intall vim build-essential nginx php7.0-fpm -y
```

# php setup
[reference](https://blog.gtwang.org/iot/raspberry-pi-install-nginx-lightweight-web-server/2/)
