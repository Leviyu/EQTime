#\!/bin/expect -f
set timeout 10
spawn scp -r  -P 9022 hongyu@eq1.la.asu.edu:/mnt/data2/hongyu/git_lib/tstar_lib ./
expect "password"
send "leviyu.32\r"
interact
