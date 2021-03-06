#
# This script was generated with funner for surfit v3.0

# 
# load plugins
# 
load libtclsurfit[info sharedlibextension]
load libsurfit_io[info sharedlibextension]

# remove all previous data and gridding rules
clear_data 

# set name of surface
set map_name "top_tvd_simple" 

# set solver
set_solver "cg" 

# set tolerance for solver
set tol 1e-007 

##
## load initial data 
##
 
# load points from text file 
pnts_read "top_tvd.xyz" "top_tvd"  
 
##
## construct grid 
##
grid 
 
##
## create gridding rules
##

# resulting surface at points = points values 
points "top_tvd" 

# resulting surface should tend to be constant or plane 
completer 1 6

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
surf_save "top_tvd_simple.dat" "top_tvd_simple" 
