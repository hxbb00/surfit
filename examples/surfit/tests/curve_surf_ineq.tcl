#
# This script was generated with funner v2.1beta (www.gridding.info)

# 
# load plugins
# 
load libtclsurfit[info sharedlibextension]

# remove all previous data and gridding rules
clear_data 

# set name of surface
set map_name "map_curv_surf_ineq" 

# set solver
set_solver "cg" 

# set tolerance for solver
set tol 1e-004 

##
## load initial data 
##
 
# load curve from text file 
curv_read "data/spiral2.xy" "spiral2"  
 
# load curve from text file 
curv_read "data/spiral.xy" "spiral"  
 
# load surface from surfit datafile 
surf_load "data/func.func" "test_func"  
 
##
## construct grid 
##
grid 
 
##
## create gridding rules
##

# resulting surface at curve <= surface values... 
curve_surf_leq "test_func" "spiral"  

# resulting surface at curve >= surface values... 
curve_surf_geq "test_func" "spiral2"  

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
surf_save "curve_surf_ineq.dat" "map_curv_surf_ineq" 

# plot resulting surface in PostScript
surf_plot "curve_surf_ineq.ps" $map_name