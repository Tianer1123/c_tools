#!/bin/bash
#开机启动在/etc/rc.local中添加执行路径，也可以将其作为service，见service脚本。
#/root/tian/watchdog.sh &  #需要加地址符后台运行(不加会怎么样可以试试^_^)
PROC_NAME=/home/tianer/opt/test/a.out
PNum=2      #默认开启两个进程（父子进程）

#name=value
CONF_FILE=/root/test.conf
#读取配置中的PNum
#去掉空行和#开头的注释

#egrep这种写法，在循环体中修改PNum的值无效.
#egrep -v "(^$)|(^#)" $CONF_FILE | while read line; do
while read line; do

#过滤掉空行或者以#开头的注释行，if 判断中 = 或 == 两边要有空格.
if [ x"$line" = x ];then
        continue
else
	#如果#前面有空格，去掉空格再比较.
        str=`echo "$line" | sed -e "s/^[ ]*//g" | sed -e "s/[ ]*$//g"`
        if [ "#" = ${str:0:1} ];then
                continue
        fi
fi

name=`echo $line|awk -F '=' '{print $1}'`
value=`echo $line|awk -F '=' '{print $2}'`

#如果配置文件设置了process_num的值,取process_num(子进程个数)的值+1.
if [ $name = "process_num" ];then
        #$((0x01)) 16进制转10进制,$[$a + $b]计算两个数字的和
        PNum=$[$(($value))+1]
        echo "$PNum"
        break
else
        echo "$PNum"
fi
done < $CONF_FILE # done < $CONF_FILE 这种写法中<只允许读文件而不允许读字符串?

#grep -w 精确匹配, grep -v grep 过滤掉包含grep的行， wc -l显示文件的行数.

#根据进程名查找到所有的进程ID，并kill掉所有的进程。
PID=`ps -ef | grep -w $PROC_NAME | grep -v grep | awk '{print $2}'`
echo $PID
for id in $PID
do
	kill -9 $id
done

#进程最多10个
if [ $PNum -gt 11 ];then
	PNum=11
fi

#开机后台运行$PROC_NAME进程.
$PROC_NAME &
#用20s的时间起服务,如果起不来在下面的循环里导致一直起不来.
sleep 20

#查找该进程的个数，如果与预设的不一致，killall掉所有进程并重启。
while :
do
	ProcNumber=`ps -ef | grep -w $PROC_NAME | grep -v grep | wc -l`
	#如果进程有异常，确保每次都能把所有进程起来.否则会死循环中一直在杀进程起进程.
	#如果进程无异常，只休眠2秒继续判断.
	if [ $ProcNumber -eq 0 ];then
		$PROC_NAME &
		#超过5个进程就多休眠一会儿，没超过就少休眠一会.
		if [ $ProcNumber -ge 5 ];then
			sleep 20
		else
			sleep 10
		fi
		continue
	elif [ $ProcNumber -ne $PNum ];then
		killall -9 $PROC_NAME
		$PROC_NAME &
		if [ $ProcNumber -ge 5 ];then
			sleep 20
		else
			sleep 10
		fi
		continue
	fi
	sleep 2
done
