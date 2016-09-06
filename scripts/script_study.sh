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
tput longname #打印终端名
tput cup 100 100 #光标移动到坐标（100， 100）
tput setb n #设置终端背景 n数字
tput setf n #设置文本前景色 n数字
tput bold #设置文本为粗体
tput smul #下划线起始
tput rmul #下划线终止
tputed #删除光标到行尾的所有内容

############################################

#日期

date  #读取日期

date +%s #时间戳

date "+%d %B %Y" #date +按指定格式输出日期

date -s "格式化日期字符串" #设置日期和时间

#-------------------------
#检查一组命令锁话费的时间
#!/bin/bash
#time_take.sh
start=$(date +%s)
commands;
statements;
end=$(date +%s)
difference=$((end - start))
echo Time taken to execute commands is $difference seconds.
#-------------------------

############################################

#脚本调试

sh -x script  #-x 启动调试功能.
set -x
command
set +x  
#只对 -x 和 +x中间的部分调试

# set -x: 在执行时显示参数和命令.
# set +x: 禁止调试.
# set -v: 当命令进行读取时显示输入.
# set +v: 禁止打印输入.

#!/bin/bash -xv 启用调试功能

############################################

#函数和参数
function fname()
{
	statements;
}
#或者
fname()
{
	statements;
}

#执行函数
fname ;
#传递参数
fname arg1 arg2;


fname() {
	echo $1, $2; #访问参数1，参数2
	echo "$@"; #以列表方式一次性打印所有参数 常用
	echo "$*"; #类似$@，把参数作为整体,但不常用
	return 0; #返回值
}

#导出函数
export -f fname


#返回值
cmd;
echo $? # $?会给出cmd的返回值 0 -- 成功，非0 -- 不成功

#例：
#!/bin/bash
#success_test.sh
CMD="command" #要执行的命令
$CMD
if [ $? -eq 0 ];
then
	echo "$CMD executed successfully"
else
	echo "$CMD terminated unsuccessfully"
fi


############################################

#过滤器|管道

cmd1 | cmd2 | cmd3  # | 管道符cmd1输出传递给cmd2，cmd2输出传递给cmd3


cmd_output=$(ls | cat -n) #子shell
echo $cmd_output

cmd_output=`ls | cat -n` #反引用


############################################

#read ：用于从键盘或标准输入读取文本

#read -n number_of_chars variable_name
read -n 2 var   #读取2个字符

read -s var     #以无回显方式读取密码

read -p "Enter input:" var #显示提示信息

#read -t timeout var
read -t 2 var    #在2s内将键入的字符读入变量var

#read -d delim_char var  #用特定的定界符作为输入行的结束
read -d ":" var #hello:     var被设置为hello

