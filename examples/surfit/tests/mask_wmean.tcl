#
# This script was generated with funner v2.1beta (www.gridding.info)

# 
# load plugins
# 
load libtclsurfit[info sharedlibextension]

# remove all previous data and gridding rules
clear_data 

# set name of surface
set map_name "map_mask_wmean" 

# set solver
set_solver "cg" 

# set tolerance for solver
set tol 1e-005 

##
## load initial data 
##
 
# load points from text file 
pnts_read "data/points.txt" "points"  
 
# load surface from surfit datafile 
surf_load "data/func.func" "test_func"  
 
# load mask from surfit datafile 
mask_load "data/mask_not.mask" "test_mask"  
 
##
## construct grid 
##
grid_get -10 10 0.2 -10 10 0.4 
 
##
## create gridding rules
##

# resulting surface at points = points values 
points "points" 

# resulting surface weighted mean value = value where mask is true... 
mask_wmean 2 "test_mask" "test_func"  

# resulting surface should tend to be constant or plane 
completer 

##
## run gridding algorithm
##
surfit 

##
## save results 
##

# save surface to surfit datafile 
surf_save "mask_wmean.dat" "map_mask_wmean" 

puts [concat "Surface weighted mean value for test_mask : " [surf_mask_wmean "test_mask" "test_func" "map_mask_wmean"]]

# plot resulting surface in PostScript
surf_plot "mask_wmean.ps" $map_name