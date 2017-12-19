#!/bin/tcsh

set work_dir = $1


if($work_dir == "" ) then
exit 0
endif



set OUT_PDF = $work_dir/master.pdf
set OUT = $work_dir/master.ps
cat /dev/null >! $OUT
cd $work_dir

set EQ_LIST = list.EQ
ls 1* 2* -d|awk -F"_" '{print $1}'>$EQ_LIST


set total_num = `cat  $EQ_LIST |wc -l`
set current_page = 1
set per_page = 10

set total_page = `python -c "from math import floor; print int(floor($total_num/$per_page+1))"`
echo "total page is $total_page"

while ($current_page <= $total_page) 

set min = `echo "($current_page-1)* $per_page +1"|bc`
set max = `echo "($current_page)* $per_page "|bc`
echo $min $max
set current_page_EQ = `cat $EQ_LIST|awk '{if(NR>= '$min' && NR <= '$max') print $1}'`
echo $current_page_EQ


psxy -JX6i/1i -R0/10/0/10 -Y10i  -K -P  << EOF>> $OUT
EOF


cat << EOF >>$OUT
[ /_objdef {ZaDb} /type /dict /OBJ pdfmark
[ {ZaDb} <<
    /Type /Font
    /Subtype /Type1
    /Name /ZaDb
    /BaseFont /ZapfDingbats
>> /PUT pdfmark
[ /_objdef {Helv} /type /dict /OBJ pdfmark
[ {Helv} <<
    /Type /Font
    /Subtype /Type1
    /Name /Helv
    /BaseFont /Helvetica
>> /PUT pdfmark
[ /_objdef {aform} /type /dict /OBJ pdfmark
[ /_objdef {afields} /type /array /OBJ pdfmark
[ {aform} <<
    /Fields {afields}
    /DR << /Font << /ZaDb {ZaDb} /Helv {Helv} >> >>
    /DA (/Helv 0 Tf 0 g)
    /NeedAppearances true
>> /PUT pdfmark
[ {Catalog} << /AcroForm {aform} >> /PUT pdfmark
EOF

set current_OUT = $OUT





pstext -Yi -JX -R -O -K -N << EOF >>$OUT
0 10 15 0 0 LB Master PDF for Catalog Under Current Directory
EOF

foreach EQ (`echo $current_page_EQ`)
psxy -JX -R -O -K -Y-0.6i << EOF >>$OUT
EOF
	## add EQ checking flag
cat  << EOF >> $OUT
[ 
	/T ( ${EQ}_checking )
	/FT /Btn
	/Rect [-150 0 -30 100] % — position
	/F 4 /H /O
	/BS << /W 1 /S /S >>
	/MK << /CA (8) /BC [ 0 ] /BG [ 2 ] >>  % Colors
	/DA (/ZaDb 0 Tf 1 0 0 rg) % — size and colors
	/AP << /N << /${EQ}_checking /null >> >> % — checkbox value
	/Subtype /Widget
	/ANN pdfmark
EOF

	# add EQ name
pstext -N -JX -R -O -K -W <<EOF>>$OUT
0 1.5 10 0 0 LB $EQ
EOF

	# add phase
	set x_pos = 3
	set xx_pos = 500
	foreach PHASE (S ScS Sdiff SS SSS ScSScS)
		ls $work_dir/${EQ}*/*_${PHASE}_*pdf > & /dev/null
		if( $? != 0) then
			echo "$EQ $PHASE does not exist"
		continue
		endif

		#//set pdf_file = `ls $work_dir/${EQ}*/*_${PHASE}_*pdf`
		#if( $pdf_file == "") then
			#continue
			#endif
		#if(! -e $pdf_file ) then
			#continue
			#endif

		set pdf_file = `ls ./${EQ}*/*_${PHASE}_*pdf`
		set pdf_file1 = ""
		if($PHASE == "all") then
			set pdf_file =  ./200708041424_update/200708041424_SSS_T_bp_catalog_all_record.pdf 
			set pdf_file1 = ./200801060514_update/200801060514_Sdiff_T_bp_catalog_all_record.pdf
		endif
		echo $pdf_file $pdf_file1


		# add phase name
pstext -N -JX -R -O -K -W <<EOF>>$OUT
$x_pos 3.5 10 0 0 CB $PHASE
EOF
		set x_pos = `echo "$x_pos + 1.15"|bc -l`

		#add EQ-PHSE checking flag
		set xx_end = `echo "$xx_pos + 120"|bc`
cat  << EOF >> $OUT
[ 
	/T ( ${EQ}_${PHASE}_checking )
	/FT /Btn
	/Rect [$xx_pos  0 $xx_end 100] % — position
	/F 4 /H /O
	/BS << /W 1 /S /S >>
	/MK << /CA (8) /BC [ 0 ] /BG [ 2 ] >>  % Colors
	/DA (/ZaDb 0 Tf 1 0 0 rg) % — size and colors
	/AP << /N << /${EQ}_${PHASE}_checking /null >> >> % — checkbox value
	/Subtype /Widget
	/ANN pdfmark
EOF
		# add link
if($PHASE == "all") then
cat <<EOF >> $OUT
[ /Rect [$xx_pos 100 $xx_end 200] 
	/Action /Launch 
	/Border [16 16 1]
	/Color [1 0 0]
	/File ($pdf_file)
	/Subtype /Link
	/ANN pdfmark
EOF
else
cat <<EOF >> $OUT
[ /Rect [$xx_pos 100 $xx_end 200] 
	/Action /Launch 
	/Border [16 16 1]
	/Color [1 0 0]
	/File ($pdf_file) 
	/Subtype /Link
	/ANN pdfmark
EOF
endif

		set xx_pos = `echo "$xx_pos + 200"|bc`


	end # PHASE

end # EQ

psxy -JX -R -O << EOF>>$OUT
EOF

@ current_page ++
end


ps2pdf $OUT $OUT_PDF
rm $OUT
rm $EQ_LIST
exit 0
