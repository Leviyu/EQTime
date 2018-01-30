#!/bin/csh

# ==================================================
#	This Script is the main script for ESF
#	
#	
#	Hongyu DATE: 
#	Key words: 
# ==================================================
set EQ = $1
set DATADIR = $2
set PLOTDIR = $3
set C_DIR = $4
set SHELL_DIR = $5
set DIR = $6
set PHASE = $7

set SRCDIR = $SHELL_DIR
set INFILE = $DATADIR/$EQ/INFILE_${PHASE}

set INPUT = ( $EQ $DATADIR $PLOTDIR $C_DIR $SHELL_DIR )

set c01_INPUT = ($INPUT $PHASE  )
csh $SRCDIR/c01.preprocess_for_ESF.sh					$c01_INPUT
#csh $SRCDIR/c03.catalog_plot_of_ESF.sh					$INPUT $PHASE  $DIR 


exit 0
