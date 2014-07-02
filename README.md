db
==

database project with our group

源文件orders.tbl customer.tbl的路径默认与makefile在同一目录
编译的.o文件以及可执行文件db默认在/bin目录下

本系统可以处理1G以上的数据
externalSort对于超过一页的数据是没有问题的，但是经过测试，小于一页的数据会出现错误
