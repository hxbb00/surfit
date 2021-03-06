#
# This script was generated with funner v2.1beta (www.gridding.info)

# 
# load plugins
# 
load libtclsurfit[info sharedlibextension]

# remove all previous data and gridding rules
clear_data 

# set name of surface
set map_name "map_surface_ineq" 

# set solver
set_solver "cg" 

# set tolerance for solver
set tol 1e-005 

##
## load initial data 
##
 
# load surface from surfit datafile 
surf_load "data/func.func" "test_func"  
surf_plus_value 80 "test_func"
surf_setName "test_func_geq" "test_func"

# load surface from surfit datafile 
surf_load "data/func.func" "test_func"  
surf_plus_value 90 "test_func"
surf_setName "test_func_leq" "test_func"
 
# load points from text file 
pnts_read "data/7points.txt" "7points"  
 
##
## construct grid 
##
grid_get -10 10 0.2 -10 10 0.2 
 
##
## create gridding rules
##

# resulting surface >= surface values 
surface_geq "test_func_geq" 

# resulting surface at points = points values 
points "7points" 

# resulting surface should tend to be constant or plane 
completer 2 1  

##
## run gridding algorithm
##
surfit 

##
## save results 
##

# save surface to surfit datafile 
surf_save "surface_ineq.dat" "map_surface_ineq" 

# plot resulting surface in PostScript
surf_plot "surface_ineq.ps" $map_name