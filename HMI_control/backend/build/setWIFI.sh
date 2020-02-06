#!/bin/bash

sudo ip link set wlan0 down
sudo echo "auto eth0
iface eth0 inet static
  address 192.168.0.200
  network 255.255.255.0
  broadcast 192.168.0.255
  gateway 192.168.0.1
  dns-nameservers 192.168.0.1 168.95.192.1 168.95.1.1

auto wlan0
  iface wlan0 inet static
  address 192.168.2.1
  netmask 255.255.255.0" >> /etc/network/interfaces
sudo ip link set wlan0 up

# install and set dhcpd conf
sudo apt-get install isc-dhcp-server -y
sudo mv /etc/dhcp/dhcpd.conf /etc/dhcp/dhcpd.conf.old
sudo echo "default-lease-time 600;
max-lease-time 7200;

ddns-update-style none;

authoritative;

subnet 192.168.2.0 netmask 255.255.255.0 {
  range 192.168.2.10 192.168.2.50;
  option broadcast-address 192.168.2.255;
  option routers 192.168.2.1;
  default-lease-time 600;
  max-lease-time 7200;
  option domain-name "local";
  option domain-name-servers 1.1.1.1;
}" > /etc/dhcp/dhcpd.conf

# set dhcp server interface
sudo sed -i 's/INTERFACESv4=\"\"/INTERFACESv4=\"wlan0\"/g' /etc/default/isc-dhcp-server
service isc-dhcp-server restart

# set iptable
sudo echo "net.ipv4.ip_forward=1" >> /etc/sysctl.conf
sudo sh -c "echo 1 > /proc/sys/net/ipv4/ip_forward"
sudo iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
sudo iptables -A FORWARD -i eth0 -o wlan0 -m state --state RELATED,ESTABLISHED -j ACCEPT
sudo iptables -A FORWARD -i wlan0 -o eth0 -j ACCEPT
sudo iptables -t nat -S
sudo iptables -S
sudo sh -c "iptables-save > /etc/iptables.ipv4.nat"
sudo echo "up iptables-restore < /etc/iptables.ipv4.nat" >> /etc/network/interfaces

# install hostapd
sudo apt-get install hostapd -y
sudo cp hostapd.conf /etc/hostapd/
sudo echo "DAEMON_CONF=\"/etc/hostapd/hostapd.conf\"" >> /etc/default/hostapd
sudo service hostapd restart

sudo reboot -h now





