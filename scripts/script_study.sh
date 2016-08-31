#!/bin/bash
#变量赋值=两边不能有空格,有空格是判断
val=value   #赋值
val = value #判断是否相等

############################################

#获取字符串长度
length=${#value}

############################################

#检查是否为root用户,root用户的UID=0
if [ $UID -ne 0 ]; then
	echo Non root user.
else
	echo Root user
fi

############################################

#数学运算
#let,(()),[]基本运算.expr bc高级运算
no1=4
no2=5

let result=no1+no2 #let 变量前不加$符
let no1++
let no1--

result=$[ no1 + no2 ] #[]中可以不加$也可以加
result=$[ $no1 + 5 ]

result=$(( $no1 + 5 )) #(())变量前需要加$

#以上只能用于整数，不能用于浮点数,bc工具可以进行高级运算
echo "scale=2;3/8" | bc #0.37 精度为2
no=100
echo "obase=2;$no" | bc #1100100  二进制
no=1100100
echo "obase=10;ibase=2;$no" | bc #100 10进制
echo "sqrt(100)" | bc #平方根
echo "10^10" | bc #平方


############################################

#文件描述符和重定向
#0 ------- stdin
#1 ------- stdout
#2 ------- stderr

# > tmp.txt 重定向到文件会覆盖
# >> tmp.txt 追加文本
cmd 2>&1 output.txt #将stderr转成stdout 都输出到output.txt

############################################

#数组和关联数组

#定义数组
array_vars=(1 2 3 4 5 6) #存储在以0为起始索引的连续位置上
#或者
array_var[0]="test1"
array_var[1]="test2"
array_var[2]="test3"

echo ${array_var[0]} #打印0位置的变量的值 test1

index=2
echo ${array_var[$index]} #打印index位置上的值 test3

echo ${array_var[*]} #打印清单
echo ${array_var[@]} #打印清单

echo ${#array_var[*]} #打印长度

############################################

#别名
alias new_command=`command sequence`
#例如
alias install=`sudo apt-get install` #只是暂时的

echo `alias cmd="command seq"` >> ~/.bashrc #追加到配置文件中

############################################

#获取终端信息
tput cols #获取终端行数
tput lines #获取终端列数
