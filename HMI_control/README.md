This is the new control method to control the arduino.

In my thought i want to build a cental control pannel to handle all the endpoint.

And make it smarter, more usefull...

On the other hand, i want to make my parents can use this HMI to grasp all the environment argument, and give it human decision.

## Data/goal.csv format
|temperature(C)|Humidity(%)|UV index|RaspberryPi Height(cm)|isAutoNet|check|
|-|-|-|-|-|-|
|25|50.0|6|100|1|55|

check = (((temperature%127 + int(Humidity)%127)%127 + (UV index)%127)%127 + (RaspberryPi Height)%127)%127 + isAutoNet
> For any two numbers got mod by 127, form left to right.

In LaTeX expression: $check = {({T \mod 127} + {Hu \mod 127} + {UV \mod 127} + {Hi \mod 127}) \mod 127} + isAutoNet$

So, $check \le 127, \in \mathabb{N}$

## Data/history.csv
|Time stamp(sec)|machine ID[0-9]|temperature(C)|Humidity(%)|UV index|check|
|-|-|-|-|-|-|
|1581358631|0|27|70.0|5|102|

# 一些中文簡易說明：

**我這邊用雙引號 "" 引起來的東西，就是對於這個 README.md 來說的相對路徑（就是會放在 git 上啦）。**

## 一些架構說明：
### IO
從這邊資料夾("./")可以看到，Arduino 端，以及後端。

Arduino 透過 GPIO 跟樹莓派傳輸資料，
Arduino 的 "./arduino/main_lib/main_lib.ino" 的 `void serialInput()` 接收 RaspberryPi 傳過來的資料。

RaspberryPi 是透過 "./backend/corntab/IO.py" 跟 Arduino 做溝通。
（印象中，我是在 RaspberryPi 的 /etc/corntab 寫某一固定週期會去執行這個 IO.py ）

### Engine
Web server engine 是用 `Nginx`，他的 root 目錄好像是在 /var/www/ 底下
/var/www/ 底下放的都是 "./backend/website/*" 的東西。（印象中是）
然後 `Nginx` 的 config 放在 /etc/nginx

### Data
這邊 web 控制沒有弄 `SQL` 因為我不熟，所以一開始用 csv 格式東西硬爆（搭配 PHP 的 fcntl 做 sync.）。

這些檔案在 "./backend/website/Data/*" 有，我好像有寫一個 sh 幫我 pull from GitHub and `cp` to /var/www/ ，那個 `sh` 好像在 /home/pi 底下（或是他的子目錄， `$ find . -name "*.sh"` 一下就會看到）。

應該有不少 bug ，不確定，因為沒有測。就只有寫大概（學弟拿走樹莓派，又沒動作，我也只能沒動做r，然後我又很忙zzz）

### Wi-Fi
這台 RaspberryPi 還有用 `hostapd`, `dhcpcpd`, `iptable`, `dnsmsq` 來控制 Wi-Fi 子網路的運作。
全部都是 default path, 沒有特別改，所以應該可以 Google 的到放在哪邊。

**這功能挺重要的**，不可以讓 Wi-Fi 炸掉，因為我這網站阿，就是打算讓手機去控制。

讓手機連到這台 RaspberryPi 的 Wi-Fi (SSID: farm_ap，如果你沒有看到，就是炸了)，然後 http://192.168.2.1 就可以看到 Web home page("./backend/website/index.php")
透過這個 "./backend/website/index.php" 來對 RaspberryPi 操作，進而對 Arduino 控制。
（當然，備援機制就是接螢幕讓 RaspberryPi 瀏覽 localhost）

## 待解決問題
1. 寫個 py 去控制超音波（問學弟（威年他們），他們知道）來控制高度。（這邊整個還沒有寫）

2. Synchronize 當前高度到 "./backend/website/Data/goal.csv" 的 "RaspberryPi Height" 欄（檔案格式在上面英文的地方）。
（印象中，這邊只要把 py 讀 "./backend/website/Data/goal.csv" 的 "RaspberryPi Height" 就可以知道前端傳過來的目標高度是多少了）
這邊看 "./backend/website/thisHMI.php" 的 `readGoals()` 跟 `writeHeigh($_value)` 就會知道問題了。對，那邊寫法很髒。就很髒。

3. 那個 "./backend/website/history.php" 是吃 "./backend/website/Data/history" 化成圖的，印象中功能好像偶爾會爛掉，就是顯示不出來吧（可能要去 git log 看）。
（因為這 Git 也被我當作 sync 的工具，所以 log 看起來也很髒.... 我應該開一個 branch 去做啦，可是當初就懶（壞習慣））。

4. 如果時間允許，可以把 "./backend/website/Data/*" 換成 `SQL` 這樣應該會省掉很多麻煩

大概就這樣