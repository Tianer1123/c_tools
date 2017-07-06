#!/bin/sh

#Time 23:59:00开始执行
#startTime=235900
startTime=090000

#每天执行1次
isNewDay=1

#记录每天的日期
perDate=$(date "+%Y%m%d")

TEST_PATH=/root/test

#升级程序版本号
TEST_VERSION_FILE=version_file
TEST_VERSION_PATH_FILE=version_file

#当前版本号,第一次装程序必须存在,升级包不必存在
TEST_CURRENT_VERSION=/root/test/current_version

#version文件存放目录
TEST_VERSION_URL=192.168.1.105:8080/version

#设置默认的升级包版本
TEST_VERSION=`cat $TEST_CURRENT_VERSION`
new_version=$TEST_VERSION

TEST_TEST=/root/test/test
TEST_DOG=/root/test/watch_dog.sh

while true ; do

    curTime=$(date "+%H%M%S")  #当前时间
    curDate=$(date "+%Y%m%d")  #当前日期

    #新的一天或者第一次运行
    if [ "$isNewDay" -eq "1" ];then
        #到了当天定时下载版本号时间
        if [ "$curTime" -gt "$startTime" ];then

            url_version_file="$TEST_VERSION_URL/$TEST_VERSION_FILE"

            if [ -f $TEST_VERSION_PATH_FILE ];then
                rm -rf $TEST_VERSION_PATH_FILE
            fi
            #下载版本号文件
            `wget $url_version_file`

            #如果文件下载失败，等待下次升级
            if [ ! -f $TEST_VERSION_PATH_FILE ];then
                isNewDay=0
                sleep 2
                continue
            fi

            #读取版本号文件并获取服务器版本号
            while read line; do
                new_version=`echo $line | awk -F '=' '{print $2}'`
            done < $TEST_VERSION_PATH_FILE

            #如果升级文件为空值，等待第二天下载升级版本号
            if [ -z $new_version ];then
                isNewDay=0
                sleep 2
                continue
            fi

            #如果服务器版本号与本地版本号不一致
            if [[ $new_version != $TEST_VERSION ]];then

                #停止当前的程序
                for dogs in `ps | grep $TEST_DOG | grep -v grep | awk '{print $1}'`;
                do
                    kill -9 $dogs
                done

                for audit in `ps | grep $TEST_AUDIT | grep -v grep | awk '{print $1}'`;
                do
                    kill -9 $audit
                done

                #如果之前下载过这个升级包，并且没被删除掉，就不再重新下载
                need_download=1
                for ver in `ls $TEST_PATH/*.tar.gz`;
                do
                    if [ $ver == TEST_PATH/$new_version ];then
                        need_download=0
                    fi
                done

                #下载新的升级包文件
                if [ $need_download -eq 1 ];then
                    url_version="$TEST_VERSION_URL/$new_version"
                    `wget $url_version`
                fi

                if [ -f TEST_PATH/$new_version ];then

                    #解压升级包
                    tar zxvf TEST_PATH/$new_version
                    #运行TEST程序
                    $TEST_TEST &

                    sleep 5
                fi

                #如果运行失败,回滚
                test_num=`ps | grep -w $TEST_TEST | grep -v grep | wc -l`
                if [ $test_num -eq 0 ];then

                    echo "rolling back..."

                    #解压上一个升级包(升级失败前的升级包)
                    tar zxvf TEST_PATH/$TEST_VERSION

                    #保留原来的版本号
                    new_version=$TEST_VERSION
                fi

                #运行watch_dog
                watch_dog=`ps | grep -w $TEST_DOG | grep -v grep | wc -l`
                if [ $watch_dog -eq 0 ];then
                    $TEST_DOG &
                fi

                #记录正在运行的版本号
                TEST_VERSION=$new_version
                echo $TEST_VERSION > $TEST_CURRENT_VERSION

                #删除多余的升级包,保留当前运行的升级包
                for ver in `ls TEST_PATH/*.tar.gz`;
                do
                    if [ $ver == TEST_PATH/$TEST_VERSION ];then
                        continue
                    else
                        rm -rf TEST_PATH/$ver
                    fi
                done
            fi
            isNewDay=0
        fi
    else
        #如果是新的一天
        if [ "$curDate" -gt "$perDate" ];then
            isNewDay=1
            perDate=$curDate
        fi
    fi

    sleep 2
done
