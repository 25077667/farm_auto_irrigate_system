# 目的: 保證 arduino 傳回 raspberry pi 的資料是對的
# 作法：hash

# 探討：
## Arduino 預設 int 是一個 Byte
所以給他 unsigned 的最大值會是 255，那 255 / 2 = 127.5
我可以保證"任兩正整數對 127 的同餘"相加，肯定小於 255。
所以肯定可以在 1 個 Byte 裡面，不會出現未定義行為。

## 選擇 127
因為 127 是在 [0, 127] 中最大的質數
> 為啥選最大?
> 因為這樣發生碰撞，的機率最低。(機率是 1/127)

# 實作
$Temp \equiv a_1 \pmod 127$
$Hermidity \equiv a_2 \pmod 127$
$uvIndex \equiv a_3 \pmod 127$

$a_1 + a_2 \pmod b_1 127$
$b_1 + a_3 \eq b_2$

$b_2$就是 hash 值