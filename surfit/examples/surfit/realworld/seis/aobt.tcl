#
# This script was generated with funner for surfit v3.0

# 
# load plugins
# 
load libsurfit[info sharedlibextension]
load libsurfit_io[info sharedlibextension]

# remove all previous data and gridding rules
clear_data 

# set name of surface
set map_name "aobt" 

# set solver
set_solver "cg" 

# set tolerance for solver
set tol 1e-007 

##
## load initial data 
##
 
# load points from text file 
pnts_read "aobt.xyz" "aobt"  
 
# load surface from surfit datafile 
surf_load "aoup.dat" "aoup"  
 
##
## construct grid 
##
grid 25 25
 
##
## create gridding rules
##

# resulting surface at points = points values 
points "aobt" 

# resulting surface looks like trend surface 
trend 1 3 "aoup" 

# resulting surface should tend to be constant or plane 
completer 

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
surf_save "aobt.dat" "aobt" 
