#!/bin/tcsh




set PWD = `pwd`

set old_file = $PWD/INFILE.mother
set new_file = $PWD/INFILE
cat /dev/null >! $new_file

cp $old_file $new_file
set bigdir = `cat $old_file|grep BIGDIR|awk '{print $2}'`

foreach NAME ( CURRENT_DIR  PLOTDIR C_DIR SHELL_DIR TAUP_DIR ED_CHECK_FILE T_star_lib )
	set full_name = "<${NAME}>"
	set name_content = `cat $old_file |grep $NAME |awk 'NR==1 {print $2}'`
	set new_name = "<${NAME}>\t\t\t\t\t\t$bigdir/$name_content"
	sed -i "/${full_name}/c ${new_name}" $new_file
end

set WORKDIR = `cat $PWD/INFILE |grep DATADIR |awk 'NR==1 {print $2}'`




set ID = $1
if($ID == "" || $ID == " ") then
exit 0
endif

csh $PWD/code_dir/run_work.sh $ID $WORKDIR $PWD> & /dev/null &

