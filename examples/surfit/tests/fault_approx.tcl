#
# This script was generated with funner v2.1beta (www.gridding.info)

# 
# load plugins
# 
load libtclsurfit[info sharedlibextension]

# remove all previous data and gridding rules
clear_data 

# set name of surface
set map_name "map_faults_approx" 

# set solver
set_solver "cg" 

# set tolerance for solver
set tol 1e-005 

##
## load initial data 
##
 
# load points from text file 
pnts_read "data/11points.txt" "11points"  
 
# load curve from text file 
curv_read "data/flt_line1.txt" "flt_line1"  
 
# load curve from text file 
curv_read "data/flt_line2.txt" "flt_line2"  
 
# load curve from text file 
curv_read "data/flt_line3.txt" "flt_line3"  
 
# load curve from text file 
curv_read "data/flt_line4.txt" "flt_line4"  
 
##
## construct grid 
##
grid_get 1 99 1 -50 50 1 
 
##
## create gridding rules
##

# fault line 
fault "flt_line1"  

# fault line 
fault "flt_line2"  

# fault line 
fault "flt_line3"  

# fault line 
fault "flt_line4"  

# resulting surface should tend to be constant or plane 
completer 0.5 2  

# add "resulting surface at points = points values" with weight 
points_add 1 "11points" 

##
## run gridding algorithm
##
surfit 

##
## save results 
##

# save surface to surfit datafile 
surf_save "fault_approx.dat" "map_faults_approx" 

# plot resulting surface in PostScript
surf_plot "fault_approx.ps" $map_name