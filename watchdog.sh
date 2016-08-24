#!/bin/bash
PROC_NAME=/home/tianer/opt/test/a.out
PNum=2      #开启两个进程（父子进程）

#name=value
CONF_FILE=/root/test.conf
#读取配置中的PNum
#去掉空行和#开头的注释

#egrep这种写法，在循环体中修改PNum的值无效.
#egrep -v "(^$)|(^#)" $CONF_FILE | while read line; do
while read line; do

#过滤掉空行或者以#开头的注释行，if 判断中 = 或 == 两边要有空格.
if [ x"$line" = x ] || [ "#" = ${line:0:1} ];then
        continue
fi

name=`echo $line|awk -F '=' '{print $1}'`
value=`echo $line|awk -F '=' '{print $2}'`

if [ $name = "process_num" ];then
        #$((0x01)) 16进制转10进制,$[$a + $b]计算两个数字的和
        PNum=$[$(($value))+1]
        echo "$PNum"
        break
else
        echo "$PNum"
fi
done < $CONF_FILE # done < $CONF_FILE 这种写法中<只允许读文件而不允许读字符串?

#grep -w 精确匹配, grep -v grep 过滤掉包含grep的行， wc -l显示文件的行数

#根据进程名查找到所有的进程ID，并kill掉所有的进程。
PID=`ps -ef | grep -w $PROC_NAME | grep -v grep | awk '{print $2}'`
echo $PID
for id in $PID
do
	kill -9 $id
done

#查找该进程的个数，如果与预设的不一致，killall掉所有进程并重启。
while :
do
	ProcNumber=`ps -ef | grep -w $PROC_NAME | grep -v grep | wc -l`
	if [ $ProcNumber -lt $PNum ];then
		killall -9 $PROC_NAME
		$PROC_NAME &
	fi
	sleep 1
done
