#
# This script was generated with funner v2.0beta

# 
# load plugins
# 
load libsurfit[info sharedlibextension]

# remove all previous data and gridding rules
clear_data 

# set name of surface
set map_name "map_surface_add" 

# set solver
set_solver "cg" 

# set tolerance for solver
set tol 1e-005 

##
## load initial data 
##
 
# load surface from surfit datafile 
surf_load "func.func" "test_func"  
 
##
## construct grid 
##
grid_get -10 10 0.2 -10 10 0.2 
 
##
## create gridding rules
##

# resulting surface should tend to be constant or plane 
completer 

# add "resulting surface = surface values" with weight 
surface_add 0.1 "test_func" 

##
## run gridding algorithm
##
surfit 

##
## save results 
##

# unload grid from memory
grid_unload 

# save surface to surfit datafile 
surf_save "surface_add.dat" 
