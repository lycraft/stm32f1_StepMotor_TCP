# stm32f1_pwm_esp8266
## 按键
* 按键向上是开启PB5的PWM输出，PWM输出为开漏输出。
* 按键向下是关闭PB5的PWM输出
* 按键←是DIR输出为低电平。方向反向
* 按键→是DIR输出为高电平。方向正向
## 引脚
### OLED & key：
stm32|oled|stm32|key
-|-|-|-
PA8|SCL|PB9|key1
PB15|SDA|PB8|key2
PB14|RES|PB7|key3
PB13|DC|PA0|key4
PA3|CS|PA1|key5

### ESP8266
stm32|ESP8266|stm32|PWM
-|-|-|-
PA4|RST|
PA5|EA|